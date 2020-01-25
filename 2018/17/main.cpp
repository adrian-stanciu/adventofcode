#include <algorithm>
#include <iostream>
#include <limits>
#include <stack>
#include <string>
#include <vector>

struct Range2D {
    int b_x;
    int e_x;
    int b_y;
    int e_y;
};

auto check_bucket(const std::vector<std::vector<char>>& map, char type, int y, int x, int dir)
{
    while (map[y][x] == type && (map[y + 1][x] == '#' || map[y + 1][x] == '~'))
        x += dir;

    if (map[y][x] == '#' && (map[y + 1][x - dir] == '#' || map[y + 1][x - dir] == '~'))
        return std::make_pair(true, x);

    return std::make_pair(false, x);
}

void drop_water(std::vector<std::vector<char>>& map, int max_y, int start_y, int start_x)
{
    std::stack<std::pair<int, int>> sources;
    sources.push(std::make_pair(start_y, start_x));

    while (!sources.empty()) {
        auto p = sources.top();
        auto y = p.first;
        auto x = p.second;

        bool changed = false;

        while (y < max_y) {
            if (map[y + 1][x] != '#' && map[y + 1][x] != '~') {
                map[y + 1][x] = '|';
                ++y;
            } else {
                map[y][x] = '.';
                auto l_water = check_bucket(map, '.', y, x, -1);
                auto r_water = check_bucket(map, '.', y, x, 1);

                if (l_water.first && r_water.first) {
                    for (auto i = l_water.second + 1; i <= r_water.second - 1; ++i)
                        map[y][i] = '~';
                    changed = true;
                } else {
                    auto l_before = map[y][l_water.second];
                    auto r_before = map[y][r_water.second];

                    auto from = l_water.first ? l_water.second + 1 : l_water.second;
                    auto to = r_water.first ? r_water.second - 1: r_water.second;
                    for (auto i = from; i <= to; ++i)
                        map[y][i] = '|';

                    if (y == p.first) {
                        auto l_wet_sand = check_bucket(map, '|', y, x, -1);
                        auto r_wet_sand = check_bucket(map, '|', y, x, 1);

                        if (l_wet_sand.first && r_wet_sand.first)
                            for (auto i = l_wet_sand.second + 1;
                                i <= r_wet_sand.second - 1; ++i)
                                map[y][i] = '~';

                        sources.pop();
                        changed = true;
                    }

                    if (!l_water.first && l_before == '.') {
                        sources.push(std::make_pair(y, l_water.second));
                        changed = true;
                    }
                    if (!r_water.first && r_before == '.') {
                        sources.push(std::make_pair(y, r_water.second));
                        changed = true;
                    }
                }
                break;
            }
        }

        if (!changed)
            sources.pop();
    }
}

void count_tiles_with_water(const std::vector<Range2D>& clay_zones,
    int min_x, int max_x, int min_y, int max_y)
{
    std::vector<std::vector<char>> map (max_y + 1, std::vector<char> (max_x + 2, '.'));

    for (const auto& clay : clay_zones)
        for (auto i = clay.b_y; i <= clay.e_y; ++i)
            for (auto j = clay.b_x; j <= clay.e_x; ++j)
                map[i][j] = '#';

    drop_water(map, max_y, 0, 500);

    auto count_wet_sand = 0;
    auto count_water = 0;
    for (auto i = min_y; i <= max_y; ++i)
        for (auto j = min_x - 1; j <= max_x + 1; ++j)
            if (map[i][j] == '|')
                ++count_wet_sand;
            else if (map[i][j] == '~')
                ++count_water;

    std::cout << count_water + count_wet_sand << "\n";
    std::cout << count_water << "\n";
}

int main()
{
    std::vector<Range2D> clay_zones;

    auto min_x = std::numeric_limits<int>::max();
    auto min_y = std::numeric_limits<int>::max();
    auto max_x = std::numeric_limits<int>::min();
    auto max_y = std::numeric_limits<int>::min();

    std::string line;
    while (getline(std::cin, line)) {
        int b_x, e_x, b_y, e_y;

        char c1, c2;
        int val_c1, b_c2, e_c2;

        sscanf(line.data(), "%c=%d, %c=%d..%d", &c1, &val_c1, &c2, &b_c2, &e_c2);
        if (c1 == 'x') {
            b_x = e_x = val_c1;
            b_y = b_c2;
            e_y = e_c2;
        } else {
            b_y = e_y = val_c1;
            b_x = b_c2;
            e_x = e_c2;
        }

        min_x = std::min(min_x, b_x);
        min_y = std::min(min_y, b_y);
        max_x = std::max(max_x, e_x);
        max_y = std::max(max_y, e_y);
        clay_zones.push_back({b_x, e_x, b_y, e_y});
    }

    count_tiles_with_water(clay_zones, min_x, max_x, min_y, max_y);

    return 0;
}

