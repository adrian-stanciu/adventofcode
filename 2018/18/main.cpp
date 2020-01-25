#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Map = std::vector<std::string>;

struct MapHasher {
    size_t operator()(const Map& map) const
    {
        size_t h = 0;
        std::hash<std::string> hash_fn;

        for (auto const& s : map) {
            // based on boost's hash_combine()
            h ^= hash_fn(s) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }

        return h;
    }
};

auto count_adj(const Map& map, int i, int j, char c)
{
    auto count = 0;

    if (map[i-1][j-1] == c)
        ++count;
    if (map[i-1][j] == c)
        ++count;
    if (map[i-1][j+1] == c)
        ++count;
    if (map[i][j-1] == c)
        ++count;
    if (map[i][j+1] == c)
        ++count;
    if (map[i+1][j-1] == c)
        ++count;
    if (map[i+1][j] == c)
        ++count;
    if (map[i+1][j+1] == c)
        ++count;

    return count;
}

Map create_map(const Map& old_map)
{
    Map new_map = old_map;

    for (auto i = 1U; i < old_map.size() - 1; ++i)
        for (auto j = 1U; j < old_map[i].size() - 1; ++j)
            if (old_map[i][j] == '.') {
                if (count_adj(old_map, i, j, '|') >= 3)
                    new_map[i][j] = '|';
            } else if (old_map[i][j] == '|') {
                if (count_adj(old_map, i, j, '#') >= 3)
                    new_map[i][j] = '#';
            } else {
                if (count_adj(old_map, i, j, '|') == 0 ||
                    count_adj(old_map, i, j, '#') == 0)
                    new_map[i][j] = '.';
            }

    return new_map;
}

auto total_resource_value_after(Map map, int t)
{
    std::unordered_map<Map, int, MapHasher> maps;
    maps[map] = 0;

    auto extra_iters = 0;

    for (auto i = 1; i <= t; ++i) {
        map = create_map(map);

        auto it = maps.find(map);
        if (it != maps.end()) {
            extra_iters = (t - i) % (i - it->second);
            break;
        }

        maps[map] = i;
    }

    for (auto i = 1; i <= extra_iters; ++i)
        map = create_map(map);

    auto trees = 0;
    auto lumberyards = 0;

    for (const auto& row : map)
        for (auto c : row)
            if (c == '|')
                ++trees;
            else if (c == '#')
                ++lumberyards;

    return trees * lumberyards;
}

int main()
{
    Map map;

    auto row_size = 0;
    auto first_row = true;

    std::string row;
    while (getline(std::cin, row)) {
        row.insert(row.cbegin(), '.');
        row.push_back('.');

        if (first_row) {
            row_size = row.size();
            first_row = false;

            std::string border_row(row_size, '.');
            map.push_back(move(border_row));
        }

        map.push_back(move(row));
    }

    std::string border_row(row_size, '.');
    map.push_back(move(border_row));

    std::cout << total_resource_value_after(map, 10) << "\n";
    std::cout << total_resource_value_after(map, 1000000000) << "\n";

    return 0;
}

