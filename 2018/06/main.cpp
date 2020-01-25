#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

auto max_finite_area(const std::vector<std::pair<int, int>>& points, int max_x, int max_y)
{
    int p_map[max_x + 1][max_y + 1];

    for (auto i = 0; i <= max_x; ++i)
        for (auto j = 0; j <= max_y; ++j) {
            std::vector<std::pair<int, size_t>> d_vec; // pair of distance and point's index

            for (auto k = 0U; k < points.size(); ++k) {
                auto d = abs(i - points[k].first) + abs(j - points[k].second);
                d_vec.push_back(std::make_pair(d, k));
            }

            auto min_p = *min_element(d_vec.begin(), d_vec.end());
            auto min_d_cnt = count_if(d_vec.begin(), d_vec.end(),
                [min_p](auto p) {
                    return p.first == min_p.first;
                });

            if (min_d_cnt == 1)
                p_map[i][j] = min_p.second;
            else
                p_map[i][j] = -1; // multiple points at min distance
        }

    std::set<int> infinite_areas;
    for (auto i = 0; i <= max_x; ++i) {
        infinite_areas.emplace(p_map[i][0]);
        infinite_areas.emplace(p_map[i][max_y]);
    }
    for (auto j = 0; j <= max_y; ++j) {
        infinite_areas.emplace(p_map[0][j]);
        infinite_areas.emplace(p_map[max_x][j]);
    }

    std::unordered_map<size_t, size_t> finite_areas;
    for (auto i = 0; i <= max_x; ++i)
        for (auto j = 0; j <= max_y; ++j) {
            if (p_map[i][j] == -1)
                continue;
            if (infinite_areas.count(p_map[i][j]))
                continue;
            ++finite_areas[p_map[i][j]];
        }

    size_t max_area = 0;
    for (const auto& a : finite_areas)
        max_area = std::max(max_area, a.second);

    return max_area;
}

auto compute_safe_area(const std::vector<std::pair<int, int>>& points,
    int max_x, int max_y, int limit)
{
    auto count = 0;

    for (auto i = 0; i <= max_x; ++i)
        for (auto j = 0; j <= max_y; ++j) {
            auto d_sum = 0;

            for (const auto& [x, y] : points) {
                d_sum += abs(i - x) + abs(j - y);
                if (d_sum >= limit)
                    break;
            }

            if (d_sum < limit)
                ++count;
        }

    return count;
}

int main()
{
    auto min_x = std::numeric_limits<int>::max();
    auto min_y = std::numeric_limits<int>::max();
    auto max_x = std::numeric_limits<int>::min();
    auto max_y = std::numeric_limits<int>::min();

    std::vector<std::pair<int, int>> points;

    std::string line;
    while (getline(std::cin, line)) {
        int x, y;
        sscanf(line.data(), "%d, %d", &x, &y);

        min_x = std::min(min_x, x);
        min_y = std::min(min_y, y);
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
        points.push_back(std::make_pair(x, y));
    }

    max_x -= min_x;
    max_y -= min_y;
    for (auto& [x, y] : points) {
        x -= min_x;
        y -= min_y;
    }

    std::cout << max_finite_area(points, max_x, max_y) << "\n";
    std::cout << compute_safe_area(points, max_x, max_y, 10000) << "\n";

    return 0;
}

