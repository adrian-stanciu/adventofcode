#include <bits/stdc++.h>

using namespace std;

const auto Floor = '.';
const auto Empty = 'L';
const auto Occupied = '#';

const array<pair<int, int>, 8> neigh{{
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
}};

using count_fn = function<int(const vector<string>&, size_t, size_t, size_t, size_t)>;

auto count_occupied1(const vector<string>& map, size_t n, size_t m, size_t r, size_t c)
{
    auto cnt = 0;

    for (auto [dr, dc] : neigh) {
        auto i = r + dr;
        if (~i == 0 || i == n)
            continue;
        auto j = c + dc;
        if (~j == 0 || j == m)
            continue;

        cnt += map[i][j] == Occupied;
    }

    return cnt;
}

auto count_occupied2(const vector<string>& map, size_t n, size_t m, size_t r, size_t c)
{
    auto cnt = 0;

    for (auto [dr, dc] : neigh) {
        auto i = r;
        auto j = c;

        while (true) {
            i += dr;
            if (~i == 0 || i == n)
                break;
            j += dc;
            if (~j == 0 || j == m)
                break;

            if (map[i][j] == Floor)
                continue;

            cnt += map[i][j] == Occupied;
            break;
        }
    }

    return cnt;
}

auto sim(vector<string> map, const count_fn& count_occupied, int abandon_threshold)
{
    auto n = map.size();
    auto m = map[0].size();

    while (true) {
        auto changed{false};
        auto new_map{map};

        for (size_t r = 0; r < n; ++r)
            for (size_t c = 0; c < m; ++c) {
                if (map[r][c] == Floor)
                    continue;

                auto num_occupied = count_occupied(map, n, m, r, c);
                if (map[r][c] == Empty) {
                    if (num_occupied == 0)
                        new_map[r][c] = Occupied;
                } else {
                    if (num_occupied >= abandon_threshold)
                        new_map[r][c] = Empty;
                }

                changed |= new_map[r][c] != map[r][c];
            }

        if (!changed)
            break;
        map.swap(new_map);
    }

    auto cnt = 0;
    for (const auto& row : map)
        for (auto elem : row)
            cnt += elem == Occupied;
    return cnt;
}

int main()
{
    vector<string> map;

    string line;
    while (getline(cin, line))
        map.push_back(move(line));

    cout << sim(map, count_occupied1, 4) << '\n';
    cout << sim(map, count_occupied2, 5) << '\n';

    return 0;
}

