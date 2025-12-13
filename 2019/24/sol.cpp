#include <cmath>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using Map = std::vector<std::string>;

struct MapHasher {
    auto operator()(const Map& map) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        for (const auto& row : map)
            h ^= std::hash<std::string>{}(row) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto count_bugs(const Map& map, int i, int j)
{
    auto bugs = 0;

    if (i - 1 >= 0)
        bugs += map[i - 1][j] == '#';
    if (i + 1 < static_cast<int>(map.size()))
        bugs += map[i + 1][j] == '#';
    if (j - 1 >= 0)
        bugs += map[i][j - 1] == '#';
    if (j + 1 < static_cast<int>(map[0].size()))
        bugs += map[i][j + 1] == '#';

    return bugs;
}

auto count_biodiv_rating(Map map)
{
    std::unordered_set<Map, MapHasher> uniq;

    uniq.insert(map);

    while (true) {
        Map next_map;

        for (auto i = 0U; i < map.size(); ++i) {
            std::string row;

            for (auto j = 0U; j < map[0].size(); ++j) {
                auto bugs = count_bugs(map, i, j);

                if (map[i][j] == '#') {
                    if (bugs == 1)
                        row.push_back('#');
                    else
                        row.push_back('.');
                } else {
                    if (bugs == 1 || bugs == 2)
                        row.push_back('#');
                    else
                        row.push_back('.');
                }
            }

            next_map.push_back(std::move(row));
        }

        swap(map, next_map);

        if (!uniq.insert(map).second)
            break;
    }

    auto rating = 0;

    for (auto i = 0U; i < map.size(); ++i)
        for (auto j = 0U; j < map[0].size(); ++j)
            if (map[i][j] == '#')
                rating += pow(2, i * map[0].size() + j);

    return rating;
}

auto count_neigh_bugs(const std::vector<Map>& levels, unsigned int l,
    int i, int j, int sz, int neigh_i, int neigh_j)
{
    auto center = sz / 2;

    if (neigh_i == center && neigh_j == center) {
        if (l == levels.size() - 1)
            return 0;

        auto bugs = 0;

        if (i == center) {
            auto col = (j == center - 1) ? 0 : (sz - 1);
            for (auto k = 0; k < sz; ++k)
                bugs += levels[l + 1][k][col] == '#';
        } else {
            auto row = (i == center - 1) ? 0 : (sz - 1);
            for (auto k = 0; k < sz; ++k)
                bugs += levels[l + 1][row][k] == '#';
        }

        return bugs;
    } else if (neigh_i == -1 || neigh_i == sz) {
        if (l == 0)
            return 0;

        auto delta = neigh_i < 0 ? -1 : 1;
        return (levels[l - 1][center + delta][center] == '#') ? 1 : 0;
    } else if (neigh_j == -1 || neigh_j == sz) {
        if (l == 0)
            return 0;

        auto delta = neigh_j < 0 ? -1 : 1;
        return (levels[l - 1][center][center + delta] == '#') ? 1 : 0;
    } else
        return (levels[l][neigh_i][neigh_j] == '#') ? 1 : 0;
}

auto count_bugs(const std::vector<Map>& levels, unsigned int l, int i, int j, int sz)
{
    return count_neigh_bugs(levels, l, i, j, sz, i - 1, j) +
           count_neigh_bugs(levels, l, i, j, sz, i + 1, j) +
           count_neigh_bugs(levels, l, i, j, sz, i, j - 1) +
           count_neigh_bugs(levels, l, i, j, sz, i, j + 1);
}

bool has_inner_bugs(const Map& map)
{
    const auto center = map.size() / 2;

    return map[center - 1][center] == '#' ||
           map[center + 1][center] == '#' ||
           map[center][center - 1] == '#' ||
           map[center][center + 1] == '#';
}

bool has_outer_bugs(const Map& map)
{
    const auto sz = map.size();

    for (auto i = 0U; i < sz; ++i)
        if (map[i][0] == '#' || map[i][sz - 1] == '#')
            return true;

    for (auto j = 1U; j < sz - 1; ++j)
        if (map[0][j] == '#' || map[sz - 1][j] == '#')
            return true;

    return false;
}

auto count_bugs_recursively(Map map, int iters)
{
    const auto sz = static_cast<int>(map.size());
    map[sz / 2][sz / 2] = '?';

    Map empty_map(sz, std::string(sz, '.'));
    empty_map[sz / 2][sz / 2] = '?';

    // prev level (L-1) is the outer level, next level (L+1) is the inner level
    std::vector<Map> levels;

    levels.push_back(empty_map);
    levels.push_back(map);
    levels.push_back(empty_map);

    while (iters--) {
        std::vector<Map> next_levels{levels};

        for (auto l = 0U; l < next_levels.size(); ++l) {
            auto& next_map = next_levels[l];

            for (auto i = 0; i < sz; ++i)
                for (auto j = 0; j < sz; ++j)
                    if (next_map[i][j] != '?') {
                        auto bugs = count_bugs(levels, l, i, j, sz);

                        if (next_map[i][j] == '#') {
                            if (bugs == 1)
                                next_map[i][j] = '#';
                            else
                                next_map[i][j] = '.';
                        } else {
                            if (bugs == 1 || bugs == 2)
                                next_map[i][j] = '#';
                            else
                                next_map[i][j] = '.';
                        }
                    }
        }

        levels.swap(next_levels);

        if (has_outer_bugs(levels.front()))
            levels.insert(levels.begin(), empty_map);
        if (has_inner_bugs(levels.back()))
            levels.push_back(empty_map);
    }

    auto bugs = 0;

    for (const auto& map : levels)
        for (const auto& row : map)
            for (auto c : row)
                if (c == '#')
                    ++bugs;

    return bugs;
}

int main()
{
    Map map;

    std::string line;
    while (getline(std::cin, line))
        map.emplace_back(std::move(line));

    std::cout << count_biodiv_rating(map) << "\n";
    std::cout << count_bugs_recursively(map, 200) << "\n";

    return 0;
}

