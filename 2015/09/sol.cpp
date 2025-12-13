#include <algorithm>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

using DistMap = std::unordered_map<std::string,
                                   std::unordered_map<std::string, int>>;

auto parse(const std::string& line)
{
    static const std::regex dist_re{"([a-zA-Z]+) to ([a-zA-Z]+) = ([1-9][0-9]*)"};

    std::smatch matched;
    regex_match(line, matched, dist_re);

    auto p1 = matched[1].str();
    auto p2 = matched[2].str();
    auto dist = strtol(matched[3].str().data(), nullptr, 10);

    return std::make_tuple(p1, p2, dist);
}

auto dist(const std::vector<std::string>& points, const DistMap& distances)
{
    if (points.empty())
        return 0;

    auto dist = 0;

    for (auto i = 0U; i < points.size() - 1; ++i)
        dist += distances.at(points[i]).at(points[i + 1]);

    return dist;
}

auto route_dist(std::vector<std::string> points, const DistMap& distances)
{
    sort(points.begin(), points.end());

    auto min_dist = std::numeric_limits<int>::max();
    auto max_dist = std::numeric_limits<int>::min();

    do {
        auto dist = ::dist(points, distances);
        min_dist = std::min(min_dist, dist);
        max_dist = std::max(max_dist, dist);
    } while (std::next_permutation(points.begin(), points.end()));

    return std::make_pair(min_dist, max_dist);
}

int main()
{
    DistMap distances;

    std::string line;
    while (getline(std::cin, line)) {
        auto [p1, p2, dist] = parse(line);
        distances[p1][p2] = dist;
        distances[std::move(p2)][std::move(p1)] = dist;
    }

    std::vector<std::string> points;

    for (const auto& [p, _] : distances)
        points.push_back(p);

    auto [min_dist, max_dist] = route_dist(points, distances);

    std::cout << min_dist << "\n";
    std::cout << max_dist << "\n";

    return 0;
}

