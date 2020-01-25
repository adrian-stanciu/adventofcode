#include <array>
#include <bitset>
#include <iostream>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Map = std::vector<std::string>;
using KeySet = std::bitset<10>;

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

struct State {
    Point point;
    KeySet key_set;
};

bool operator==(const State& s1, const State& s2)
{
    return s1.point == s2.point && s1.key_set == s2.key_set;
}

struct StateHasher {
    auto operator()(const State& s) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= PointHasher{}(s.point) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<KeySet>{}(s.key_set) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto find_reachable_keys(const Map& map, const State& state, long num_keys)
{
    static const std::array<int, 4> dr{0, 0, -1, 1};
    static const std::array<int, 4> dc{-1, 1, 0, 0};

    std::queue<std::pair<Point, long>> q;
    q.emplace(state.point, 0);

    std::unordered_set<Point, PointHasher> visited;
    visited.emplace(state.point);

    std::vector<std::pair<char, long>> keys;

    while (!q.empty()) {
        const auto& [p, d] = q.front();

        for (auto i = 0; i < 4; ++i) {
            Point next_p{p.r + dr[i], p.c + dc[i]};

            if (visited.count(next_p) != 0)
                continue;

            auto k = map[next_p.r][next_p.c];

            if (k == '#')
                continue;

            // consider '0' as a key only when all other keys are found
            if ((k >= '1' && k <= '9' && !state.key_set[k - '0']) ||
                (k == '0' && static_cast<long>(state.key_set.count()) == num_keys - 1)) {
                keys.emplace_back(k, d + 1);
                continue;
            }

            q.emplace(next_p, d + 1);
            visited.emplace(std::move(next_p));
        }

        q.pop();
    }

    return keys;
}

auto sp_dijkstra(const Map& map, const Point& point, const KeyMap& key_map, const KeySet& key_set)
{
    State s{point, key_set};

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

    std::optional<long> sp_dist;

    while (!states.empty()) {
        auto s = *states.begin();
        states.erase(states.begin());

        if (s.key_set.count() == key_map.size() - 1 && !sp_dist.has_value())
            sp_dist = d_map[s];
        else if (s.key_set.count() == key_map.size())
            return std::make_pair(*sp_dist, d_map[s]);

        auto keys = find_reachable_keys(map, s, key_map.size());
        if (keys.empty())
            continue;

        for (const auto& [key, d2key] : keys) {
            auto new_key_set{s.key_set};
            new_key_set.set(key - '0');

            auto d = d_map[s] + d2key;
            State next_s{key_map.at(key), new_key_set};

            if (d_map.find(next_s) == d_map.end() || d_map[next_s] > d) {
                auto it = states.find(next_s);
                if (it != states.end())
                    states.erase(it);

                d_map[next_s] = d;
                states.emplace(std::move(next_s));
            }
        }
    }

    return std::make_pair(-1L, -1L);
}

int main()
{
    Map map;

    std::string line;
    while (getline(std::cin, line))
        map.push_back(std::move(line));

    Point start;
    KeyMap key_map;

    for (auto i = 0U; i < map.size(); ++i)
        for (auto j = 0U; j < map[i].size(); ++j)
            if (map[i][j] >= '0' && map[i][j] <= '9') {
                key_map.try_emplace(map[i][j], i, j);

                if (map[i][j] == '0')
                    start = {i, j};
            }

    auto [sp_dist, sp_dist_cycle] = sp_dijkstra(map, start, key_map, {});

    std::cout << sp_dist << "\n";
    std::cout << sp_dist_cycle << "\n";

    return 0;
}

