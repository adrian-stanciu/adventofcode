#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

static const vector<string_view> NumTable{
    "789",
    "456",
    "123",
    " 0A",
};

static const auto NumTableRows{ssize(NumTable)};
static const auto NumTableCols{ssize(NumTable[0])};

auto get_num_table_pos(char ch)
{
    for (auto r = 0; r < NumTableRows; ++r)
        for (auto c = 0; c < NumTableCols; ++c)
            if (NumTable[r][c] == ch)
                return make_pair(r, c);
    assert(false);
}

static constexpr array<pair<int, int>, 4> Neigh{{
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
}};

auto comp_num_table_dist(int r, int c)
{
    static constexpr auto Inf{numeric_limits<int>::max()};

    queue<pair<int, int>> q;
    q.emplace(r, c);

    vector<vector<int>> dist(NumTableRows, vector<int>(NumTableCols, Inf));
    dist[r][c] = 0;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (const auto& [dr, dc] : Neigh) {
            auto nr = r + dr;
            if (nr < 0 || nr == NumTableRows)
                continue;
            auto nc = c + dc;
            if (nc < 0 || nc == NumTableCols)
                continue;

            if (NumTable[nr][nc] == ' ')
                continue;
            if (dist[nr][nc] != Inf)
                continue;

            q.emplace(nr, nc);
            dist[nr][nc] = dist[r][c] + 1;
        }
    }

    return dist;
}

auto find_num_table_paths(int sr, int sc, int er, int ec)
{
    auto dist = comp_num_table_dist(er, ec);

    auto dir = [](auto dr, auto dc) {
        if (dr) {
            if (dr < 0)
                return '^';
            else
                return 'v';
        } else {
            if (dc < 0)
                return '<';
            else
                return '>';
        }
    };

    queue<tuple<int, int, string>> q;
    q.emplace(sr, sc, "");

    vector<string> paths;

    while (!q.empty()) {
        auto [r, c, path] = q.front();
        q.pop();

        if (r == er && c == ec) {
            paths.push_back(move(path));
            continue;
        }

        for (const auto& [dr, dc] : Neigh) {
            auto nr = r + dr;
            if (nr < 0 || nr == NumTableRows)
                continue;
            auto nc = c + dc;
            if (nc < 0 || nc == NumTableCols)
                continue;

            if (dist[nr][nc] != dist[r][c] - 1)
                continue;

            path.push_back(dir(dr, dc));
            q.emplace(nr, nc, path);
            path.pop_back();
        }
    }

    return paths;
}

auto find_num_table_paths(const string& s)
{
    auto [r, c] = get_num_table_pos('A');

    vector<string> full_paths;
    full_paths.emplace_back("");

    for (auto ch : s) {
        auto [nr, nc] = get_num_table_pos(ch);
        auto paths = find_num_table_paths(r, c, nr, nc);
        r = nr;
        c = nc;

        vector<string> new_full_paths;

        for (const auto& full_path : full_paths)
            for (const auto& path : paths) {
                new_full_paths.push_back(full_path + path);
                new_full_paths.back().push_back('A');
            }

        full_paths.swap(new_full_paths);
    }

    return full_paths;
}

// unique best paths in directional table:
// " ^A"
// "<v>"
// rules:
// 1. group together the same directions ("<<" and ">>")
// 2. prioritize '<' over '^' or 'v'
// 3. prioritize '^' or 'v' over '>'
static const vector<tuple<char, char, string_view>> DirTable{
    {'^', '^', "A"},
    {'^', 'A', ">A"},
    {'^', '<', "v<A"},
    {'^', 'v', "vA"},
    {'^', '>', "v>A"},
    {'A', '^', "<A"},
    {'A', 'A', "A"},
    {'A', '<', "v<<A"},
    {'A', 'v', "<vA"},
    {'A', '>', "vA"},
    {'<', '^', ">^A"},
    {'<', 'A', ">>^A"},
    {'<', '<', "A"},
    {'<', 'v', ">A"},
    {'<', '>', ">>A"},
    {'v', '^', "^A"},
    {'v', 'A', "^>A"},
    {'v', '<', "<A"},
    {'v', 'v', "A"},
    {'v', '>', ">A"},
    {'>', '^', "<^A"},
    {'>', 'A', "^A"},
    {'>', '<', "<<A"},
    {'>', 'v', "<A"},
    {'>', '>', "A"},
};

auto get_dir_table_path(char from, char to)
{
    for (const auto& [src, dst, path] : DirTable)
        if (from == src && to == dst)
            return path;
    assert(false);
}

auto solve(const string& s, int levels)
{
    auto min_len = numeric_limits<long>::max();

    for (string_view sv : find_num_table_paths(s)) {
        map<string_view, long> freq_map;

        while (true) {
            auto pos = sv.find('A');
            if (pos == sv.npos)
                break;

            ++freq_map[sv.substr(0, pos + 1)];

            sv.remove_prefix(pos + 1);
        }

        for (auto i = 0; i < levels; ++i) {
            map<string_view, long> next_freq_map;

            for (const auto& [sv, freq] : freq_map)
                for (char prev_ch = 'A'; auto ch : sv) {
                    next_freq_map[get_dir_table_path(prev_ch, ch)] += freq;
                    prev_ch = ch;
                }

            freq_map.swap(next_freq_map);
        }

        auto len = 0l;
        for (const auto& [sv, freq] : freq_map)
            len += ssize(sv) * freq;

        min_len = min(min_len, len);
    }

    return min_len;
}

int main()
{
    auto sum1 = 0l;
    auto sum2 = 0l;

    string s;
    while (getline(cin, s)) {
        auto n = str2num(s);

        sum1 += n * solve(s, 2);
        sum2 += n * solve(s, 25);
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

