#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Map = std::vector<std::string>;
using KeySet = std::bitset<26>;
using Graph = std::vector<std::vector<std::pair<int, int>>>;

struct Point {
    int r;
    int c;

    Point(int r, int c)
    : r(r), c(c)
    {}
};

bool operator==(const Point& p1, const Point& p2)
{
    return p1.r == p2.r && p1.c == p2.c;
}

bool operator!=(const Point& p1, const Point& p2)
{
    return !(p1 == p2);
}

struct PointHasher {
    auto operator()(const Point& p) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= p.r + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= p.c + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto node_of(const std::vector<Point>& points, const Point& p)
{
    return std::distance(points.begin(), std::find(points.begin(), points.end(), p));
}

auto is_in_range(const Map& map, const Point& p, int lo, int hi)
{
    return lo <= map[p.r][p.c] && map[p.r][p.c] <= hi;
}

auto is_door(const Map& map, const Point& p)
{
    return is_in_range(map, p, 'A', 'Z');
}

auto is_key(const Map& map, const Point& p)
{
    return is_in_range(map, p, 'a', 'z');
}

auto bfs(const Map& map, const std::vector<Point>& points, const Point& start,
    Graph& g)
{
    static const std::array<int, 4> dr{0, 0, -1, 1};
    static const std::array<int, 4> dc{-1, 1, 0, 0};

    std::queue<std::pair<Point, int>> q;
    q.emplace(start, 0);

    std::unordered_set<Point, PointHasher> visited;
    visited.emplace(std::move(start));

    auto start_node = node_of(points, start);

    while (!q.empty()) {
        auto [p, d] = q.front();
        q.pop();

        if (p != start && (is_door(map, p) || is_key(map, p))) {
            g[start_node].emplace_back(node_of(points, p), d);
            continue;
        }

        for (auto i = 0; i < 4; ++i) {
            Point next_p{p.r + dr[i], p.c + dc[i]};

            if (visited.count(next_p) != 0)
                continue;

            if (map[next_p.r][next_p.c] == '#')
                continue;

            q.emplace(next_p, d + 1);
            visited.emplace(std::move(next_p));
        }
    }
}

struct Key {
    Point p;
    int d;
    unsigned int finder;

    Key(Point p, int d, unsigned int finder)
    : p(p), d(d), finder(finder)
    {}
};

auto find_reachable_keys_by(const Map& map, const std::vector<Point>& points,
    const Point& p, const Graph& g, const KeySet& key_set, unsigned int finder,
    std::vector<Key>& keys)
{
    auto u = node_of(points, p);

    std::queue<std::pair<int, int>> q;
    q.emplace(u, 0);

    std::vector<bool> visited(g.size(), false);
    visited[u] = true;

    while (!q.empty()) {
        auto [u, total_d] = q.front();
        q.pop();

        for (const auto& [v, d] : g[u]) {
            if (visited[v])
                continue;
            visited[v] = true;

            auto p = points[v];

            if (is_door(map, p) && !key_set[map[p.r][p.c] - 'A'])
                continue;

            auto dist = total_d + d;

            if (is_key(map, p) && !key_set[map[p.r][p.c] - 'a']) {
                keys.emplace_back(p, dist, finder);
                continue;
            }

            q.emplace(v, dist);
        }
    }
}

auto find_reachable_keys(const Map& map, const std::vector<Point>& points,
    const std::vector<Point>& robots, const Graph& g, const KeySet& key_set)
{
    std::vector<Key> keys;

    for (auto i = 0U; i < robots.size(); ++i)
        find_reachable_keys_by(map, points, robots[i], g, key_set, i, keys);

    return keys;
}

struct State {
    std::vector<Point> robots;
    KeySet key_set;
};

bool operator==(const State& s1, const State& s2)
{
    return s1.robots == s2.robots && s1.key_set == s2.key_set;
}

struct StateHasher {
    auto operator()(const State& s) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        for (const auto& p : s.robots)
            h ^= PointHasher{}(p) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<KeySet>{}(s.key_set) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto sp_dijkstra(const Map& map, const std::vector<Point>& points,
    const std::vector<Point>& robots, const Graph& g, const KeySet& key_set)
{
    State s{robots, key_set};

    std::unordered_map<State, int, StateHasher> d_map;
    d_map[s] = 0;

    auto state_cmp = [&] (const State& s1, const State& s2) {
        auto d1 = d_map[s1];
        auto d2 = d_map[s2];

        if (d1 == d2) {
            if (s1.key_set.count() == s2.key_set.count())
                return !(s1 == s2);
            else
                return s1.key_set.count() > s2.key_set.count();
        } else
            return d1 < d2;
    };

    std::set<State, decltype(state_cmp)> states(state_cmp);
    states.insert(s);

    while (!states.empty()) {
        auto s = *states.begin();
        states.erase(states.begin());

        if (s.key_set.count() == s.key_set.size())
            return d_map[s];

        auto keys = find_reachable_keys(map, points, s.robots, g, s.key_set);
        if (keys.empty())
            return -1;

        for (const auto& key : keys) {
            auto new_key_set{s.key_set};
            new_key_set.set(map[key.p.r][key.p.c] - 'a');

            std::vector<Point> new_robots;
            for (auto i = 0U; i < s.robots.size(); ++i)
                if (i == key.finder)
                    new_robots.push_back(key.p);
                else
                    new_robots.push_back(s.robots[i]);

            auto d = d_map[s] + key.d;
            State next_s{new_robots, new_key_set};

            if (d_map.find(next_s) == d_map.end() || d_map[next_s] > d) {
                auto it = states.find(next_s);
                if (it != states.end())
                    states.erase(it);

                d_map[next_s] = d;
                states.emplace(std::move(next_s));
            }
        }
    }

    return -1;
}

auto sp(const Map& map)
{
    std::vector<Point> points;
    std::vector<Point> robots;

    for (auto i = 0U; i < map.size(); ++i)
        for (auto j = 0U; j < map[i].size(); ++j)
            if (map[i][j] == '@') {
                points.emplace_back(i, j);
                robots.emplace_back(i, j);
            } else if ((map[i][j] >= 'a' && map[i][j] <= 'z') ||
                (map[i][j] >= 'A' && map[i][j] <= 'Z')) {
                points.emplace_back(i, j);
            }

    Graph g(points.size());
    for (const auto& start : points)
        bfs(map, points, start, g);

    return sp_dijkstra(map, points, robots, g, {});
}

int main()
{
    Map map;

    std::string line;
    while (getline(std::cin, line))
        map.push_back(std::move(line));

    std::cout << sp(map) << "\n";

    return 0;
}

