#include <bits/stdc++.h>

#include "hasher.hpp"

using namespace std;

using BlackMap = unordered_map<pair<int, int>, bool, PairHasher<int>>;

const array<pair<int, int>, 6> neigh{{
    {0, -1}, {0, 1}, {1, 0}, {1, 1}, {-1, -1}, {-1, 0}
}};

enum class Dir {
    W,
    E,
    NW,
    NE,
    SW,
    SE,
};

auto split_path(const string& path)
{
    vector<Dir> dirs;

    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == 'w') {
            dirs.push_back(Dir::W);
        } else if (path[i] == 'e') {
            dirs.push_back(Dir::E);
        } else if (path[i] == 'n') {
            ++i;
            dirs.push_back(path[i] == 'w' ? Dir::NW : Dir::NE);
        } else {
            ++i;
            dirs.push_back(path[i] == 'w' ? Dir::SW : Dir::SE);
        }
    }

    return dirs;
}

auto find_dst(const string& path)
{
    auto row = 0;
    auto dia = 0;

    for (auto dir : split_path(path))
        switch (dir) {
        case Dir::W:
            --dia;
            break;
        case Dir::E:
            ++dia;
            break;
        case Dir::NW:
            ++row;
            break;
        case Dir::NE:
            ++row;
            ++dia;
            break;
        case Dir::SW:
            --row;
            --dia;
            break;
        case Dir::SE:
            --row;
            break;
        }

    return make_pair(row, dia);
}

auto compute_black_map(const vector<string>& paths)
{
    BlackMap black_map;

    for (const auto& path : paths) {
        auto dst = find_dst(path);
        black_map[dst] = !black_map[dst];
    }

    return black_map;
}

void fill_border(BlackMap& black_map)
{
    BlackMap new_black_map{black_map};

    for (const auto& [pos, is_black] : black_map) {
        new_black_map[pos] = is_black;

        auto [row, dia] = pos;
        for (auto [d_row, d_dia] : neigh) {
            auto neigh_pos = make_pair(row + d_row, dia + d_dia);

            if (black_map.find(neigh_pos) == end(black_map))
                new_black_map.emplace(move(neigh_pos), false);
        }
    }

    black_map.swap(new_black_map);
}

auto count_blacks(const BlackMap& black_map, const pair<int, int>& pos, BlackMap& new_black_map)
{
    auto num_blacks = 0;

    auto [row, dia] = pos;
    for (auto [d_row, d_dia] : neigh) {
        auto neigh_pos = make_pair(row + d_row, dia + d_dia);

        auto it = black_map.find(neigh_pos);
        if (it == end(black_map))
            new_black_map.emplace(move(neigh_pos), false);
        else
            num_blacks += it->second;
    }

    return num_blacks;
}

void sim(BlackMap& black_map, int iters)
{
    fill_border(black_map);

    for (auto i = 0; i < iters; ++i) {
        BlackMap new_black_map{black_map};

        for (const auto& [pos, is_black] : black_map) {
            auto num_blacks = count_blacks(black_map, pos, new_black_map);
            if (is_black) {
                if (num_blacks == 0 || num_blacks > 2)
                    new_black_map[pos] = false;
            } else {
                if (num_blacks == 2)
                    new_black_map[pos] = true;
            }
        }

        black_map.swap(new_black_map);
    }
}

int main()
{
    vector<string> paths;

    string line;
    while (getline(cin, line))
        paths.push_back(move(line));

    auto count = [] (const auto& black_map) {
        return accumulate(begin(black_map), end(black_map), 0, [] (auto acc, const auto& kv) {
            return acc + kv.second;
        });
    };

    auto black_map = compute_black_map(paths);
    cout << count(black_map) << '\n';

    sim(black_map, 100);
    cout << count(black_map) << '\n';

    return 0;
}

