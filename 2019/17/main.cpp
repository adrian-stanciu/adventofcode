#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "int_computer.h"

using Map = std::vector<std::vector<char>>;

auto read_map(const std::vector<long long>& prog)
{
    IntComputer ic(prog);

    Map map;

    std::vector<char> row;
    while (ic.run()) {
        auto out = *ic.get_last_output();

        if (out != '\n')
            row.push_back(out);
        else if (!row.empty()) {
            map.push_back(std::move(row));
            row.clear();
        }
    }

    return map;
}

auto compute_map_csum(const Map& map)
{
    auto s = 0;

    for (auto i = 1U; i < map.size() - 1; ++i)
        for (auto j = 1U; j < map[i].size() - 1; ++j)
            if (map[i][j] == '#') {
                if (map[i - 1][j] != '#')
                    continue;
                if (map[i + 1][j] != '#')
                    continue;
                if (map[i][j - 1] != '#')
                    continue;
                if (map[i][j + 1] != '#')
                    continue;

                s += i * j;
            }

    return s;
}

auto find_start(const Map& map)
{
    auto r = 0;
    auto c = 0;

    for (auto i = 0U; i < map.size(); ++i)
        for (auto j = 0U; j < map[i].size(); ++j)
            if (map[i][j] == '^' || map[i][j] == 'v' ||
                map[i][j] == '<' || map[i][j] == '>') {
                r = i;
                c = j;
            }

    auto dr = 0;
    auto dc = 0;

    switch (map[r][c]) {
    case '^':
        dr = -1;
        dc = 0;
        break;
    case 'v':
        dr = 1;
        dc = 0;
        break;
    case '<':
        dr = 0;
        dc = -1;
        break;
    case '>':
        dr = 0;
        dc = 1;
        break;
    }

    return std::make_tuple(r, c, dr, dc);
}

auto get_dir(int r, int c, int next_r, int next_c, int& dr, int& dc)
{
    auto new_dr = next_r - r;
    auto new_dc = next_c - c;

    char dir = '?';

    if (dr == -1) {
        if (new_dc == -1)
            dir = 'L';
        else
            dir = 'R';
    } else if (dr == 1) {
        if (new_dc == -1)
            dir = 'R';
        else
            dir = 'L';
    } else if (dc == -1) {
        if (new_dr == -1)
            dir = 'R';
        else
            dir = 'L';
    } else if (dc == 1) {
        if (new_dr == -1)
            dir = 'L';
        else
            dir = 'R';
    }

    dr = new_dr;
    dc = new_dc;

    return dir;
}

using Path = std::vector<std::pair<char, int>>;

auto compute_path(const Map& map)
{
    const auto map_sz = static_cast<int>(map.size());

    // direction is represented as deltas on rows and columns
    auto [r, c, dr, dc] = find_start(map);

    Path path;

    while (true) {
        const auto map_r_sz = static_cast<int>(map[r].size());

        auto next_r = r;
        auto next_c = c;

        if (r - 1 >= 0 && map[r - 1][c] == '#' && dr == 0)
            next_r = r - 1;
        else if (r + 1 < map_sz && map[r + 1][c] == '#' && dr == 0)
            next_r = r + 1;
        else if (c - 1 >= 0 && map[r][c - 1] == '#' && dc == 0)
            next_c = c - 1;
        else if (c + 1 < map_r_sz && map[r][c + 1] == '#' && dc == 0)
            next_c = c + 1;

        // check end of line
        if (next_r == r && next_c == c)
            break;

        auto dir = get_dir(r, c, next_r, next_c, dr, dc);

        r = next_r;
        c = next_c;

        auto len = 1;

        while (true) {
            if (r + dr < 0 || r + dr == map_sz)
                break;
            if (c + dc < 0 || c + dc == map_r_sz)
                break;

            if (map[r + dr][c + dc] != '#')
                break;

            r += dr;
            c += dc;

            ++len;
        }

        path.emplace_back(dir, len);
    }

    return path;
}

auto extract_instruction_block(const std::string& path,
    unsigned int num_instructions, const std::set<char>& skip_chars)
{
    auto i = 0UL;

    while (i < path.size())
        if (skip_chars.count(path[i]) > 0)
            i += 2; // skip that char and the following comma
        else
            break;

    std::string block;

    while (i < path.size()) {
        if (skip_chars.count(path[i]) > 0)
            break;

        block.push_back(path[i]);

        if (path[i] == ',') { // a new instruction was added
            --num_instructions;
            if (num_instructions == 0)
                return block;
        }

        ++i;
    }

    return std::string();
}

void replace(std::string& where, const std::string& what, const std::string& with)
{
    auto i = 0UL;

    while (true) {
        i = where.find(what, i);
        if (i == std::string::npos)
            break;

        where.replace(i, what.size(), with);

        i += with.size();
    }
}

auto is_valid_main(const std::string& main, unsigned int max_fct_len)
{
    for (auto c : main)
        if (c != 'A' && c != 'B' && c != 'C' && c != ',')
            return false;
    return main.size() <= max_fct_len;
}

auto split_path(const Path& path, unsigned int max_fct_len)
{
    std::string path_s;

    for (const auto& p : path) {
        path_s.push_back(p.first);
        path_s.push_back(',');

        path_s.append(std::to_string(p.second));
        path_s.push_back(',');
    }

    auto max_instr_cnt = max_fct_len / 2;

    for (auto a_len = 1U; a_len <= max_instr_cnt; ++a_len) {
        auto path_a = path_s;

        auto a = extract_instruction_block(path_a, a_len, {});
        if (a.empty() || a.size() > max_fct_len)
            continue;

        replace(path_a, a, "A,");

        for (auto b_len = 1U; b_len <= max_instr_cnt; ++b_len) {
            auto path_ab = path_a;

            auto b = extract_instruction_block(path_ab, b_len, {'A'});
            if (b.empty() || b.size() > max_fct_len)
                continue;

            replace(path_ab, b, "B,");

            for (auto c_len = 1U; c_len <= max_instr_cnt; ++c_len) {
                auto path_abc = path_ab;

                auto c = extract_instruction_block(path_abc, c_len, {'A', 'B'});
                if (c.empty() || c.size() > max_fct_len)
                    continue;

                replace(path_abc, c, "C,");

                if (is_valid_main(path_abc, max_fct_len)) {
                    path_abc.pop_back();
                    a.pop_back();
                    b.pop_back();
                    c.pop_back();
                    return make_tuple(path_abc, a, b, c);
                }
            }
        }
    }

    return make_tuple(std::string(), std::string(), std::string(), std::string());
}

void fill_input(IntComputer& ic, const Path& path, unsigned int max_fct_len)
{
    auto [main, a, b, c] = split_path(path, max_fct_len);

    auto encode_function = [&] (const auto& f) {
        for (auto i : f)
            ic.append_input(i);
        ic.append_input('\n');
    };

    encode_function(main);
    encode_function(a);
    encode_function(b);
    encode_function(c);

    ic.append_input('n');
    ic.append_input('\n');
}

auto traverse_path(const std::vector<long long>& prog, const Path& path,
    unsigned int max_fct_len)
{
    IntComputer ic(prog);
    ic.set_mem(0, 2);

    fill_input(ic, path, max_fct_len);

    while (ic.run());

    return *ic.get_last_output();
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    auto map = read_map(prog);

    std::cout << compute_map_csum(map) << "\n";

    auto path = compute_path(map);

    std::cout << traverse_path(prog, path, 20) << "\n";

    return 0;
}

