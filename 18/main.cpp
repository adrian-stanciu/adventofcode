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

struct Point {
    long r;
    long c;

    Point() = default;

    Point(long r, long c)
    : r(r), c(c)
    {}
};

bool operator==(const Point& p1, const Point& p2)
{
    return p1.r == p2.r && p1.c == p2.c;
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

using KeyMap = std::unordered_map<char, Point>;

struct Key {
    char k;
    long d;
    int finder;

    Key(char k, long d, int finder)
    : k(k), d(d), finder(finder)
    {}
};

auto find_reachable_keys_by(const Map& map, const Point& p, const KeySet& key_set, int finder)
{
    static const std::array<int, 4> dr{0, 0, -1, 1};
    static const std::array<int, 4> dc{1, -1, 0, 0};

    std::queue<std::pair<Point, long>> q;
    q.emplace(p, 0);

    std::unordered_set<Point, PointHasher> visited;
    visited.emplace(std::move(p));

    std::vector<Key> keys;

    while (!q.empty()) {
        const auto& [p, d] = q.front();

        for (auto i = 0; i < 4; ++i) {
            Point next_p{p.r + dr[i], p.c + dc[i]};

            if (visited.count(next_p) != 0)
                continue;

            auto k = map[next_p.r][next_p.c];

            if (k == '#')
                continue;

            if (k >= 'A' && k <= 'Z' && !key_set[k - 'A'])
                continue;

            if (k >= 'a' && k <= 'z' && !key_set[k - 'a']) {
                keys.emplace_back(k, d + 1, finder);
                continue;
            }

            q.emplace(next_p, d + 1);
            visited.emplace(std::move(next_p));
        }

        q.pop();
    }

    return keys;
}

auto find_reachable_keys(const Map& map, const std::vector<Point>& points, const KeySet& key_set)
{
    std::vector<Key> all_keys;

    for (auto i = 0; i < points.size(); ++i) {
        auto keys = find_reachable_keys_by(map, points[i], key_set, i);
        move(keys.begin(), keys.end(), back_inserter(all_keys));
    }

    return all_keys;
}

struct State {
    std::vector<Point> points;
    KeySet key_set;
};

bool operator==(const State& s1, const State& s2)
{
    return s1.points == s2.points && s1.key_set == s2.key_set;
}

struct StateHasher {
    auto operator()(const State& s) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        for (const auto& p : s.points)
            h ^= PointHasher{}(p) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<KeySet>{}(s.key_set) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto sp_dijkstra(const Map& map, const std::vector<Point>& points,
    const KeyMap& key_map, const KeySet& key_set)
{
    State s{points, key_set};

    std::unordered_map<State, long, StateHasher> d_map;
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

        if (s.key_set.count() == key_map.size())
            return d_map[s];

        auto keys = find_reachable_keys(map, s.points, s.key_set);
        if (keys.empty())
            return -1L;

        for (const auto& key : keys) {
            auto new_key_set{s.key_set};
            new_key_set.set(key.k - 'a');

            std::vector<Point> new_points;
            for (auto i = 0; i < s.points.size(); ++i)
                if (i == key.finder)
                    new_points.push_back(key_map.at(key.k));
                else
                    new_points.push_back(s.points[i]);

            auto d = d_map[s] + key.d;
            State next_s{new_points, new_key_set};

            if (d_map.find(next_s) == d_map.end() || d_map[next_s] > d) {
                auto it = states.find(next_s);
                if (it != states.end())
                    states.erase(it);

                d_map[next_s] = d;
                states.emplace(std::move(next_s));
            }
        }
    }

    return -1L;
}

auto sp(const Map& map, const KeyMap& key_map)
{
    std::vector<Point> points;

    for (auto i = 0; i < map.size(); ++i)
        for (auto j = 0; j < map[i].size(); ++j)
            if (map[i][j] == '@')
                points.emplace_back(i, j);

    return sp_dijkstra(map, points, key_map, {});
}

int main()
{
    Map map;

    std::string line;
    while (getline(std::cin, line))
        map.push_back(std::move(line));

    KeyMap key_map;

    for (auto i = 0; i < map.size(); ++i)
        for (auto j = 0; j < map[i].size(); ++j)
            if (map[i][j] >= 'a' && map[i][j] <= 'z')
                key_map.try_emplace(map[i][j], i, j);

    std::cout << sp(map, key_map) << "\n";

    return 0;
}

