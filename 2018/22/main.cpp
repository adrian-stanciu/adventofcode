#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "hasher.hpp"

using TypeMap = std::vector<std::vector<int>>;

static constexpr auto Inf = std::numeric_limits<long>::max();
static constexpr auto ChangeCost = 7;

auto build_type_map(int d, int x_t, int y_t, int limit)
{
    static constexpr auto mod = 20183L;
    static constexpr auto x_f = 16807L;
    static constexpr auto y_f = 48271L;

    std::vector<std::vector<long>> e_mat;
    TypeMap t_mat;

    for (auto y = 0L; y <= limit; ++y) {
        std::vector<long> e_row;
        std::vector<int> t_row;
        for (auto x = 0L; x <= limit; ++x) {
            if (y == 0 && x == 0) {
                auto e = d % mod;
                e_row.push_back(e);
                t_row.push_back(e % 3);
            } else if (y == 0) {
                auto e = (x * x_f + d) % mod;
                e_row.push_back(e);
                t_row.push_back(e % 3);
            } else if (x == 0) {
                auto e = (y * y_f + d) % mod;
                e_row.push_back(e);
                t_row.push_back(e % 3);
            } else if (x == x_t && y == y_t) {
                auto e = d % mod;
                e_row.push_back(e);
                t_row.push_back(e % 3);
            } else {
                auto e = (e_mat[y - 1][x] * e_row[x - 1] + d) % mod;
                e_row.push_back(e);
                t_row.push_back(e % 3);
            }
        }
        e_mat.push_back(std::move(e_row));
        t_mat.push_back(std::move(t_row));
    }

    return t_mat;
}

auto sum_all_types(const TypeMap& t_map, int x_t, int y_t)
{
    auto s = 0L;

    for (auto y = 0; y <= y_t; ++y)
        for (auto x = 0; x <= x_t; ++x)
            s += t_map[y][x];

    return s;
}

enum class Region {
    Rocky,
    Wet,
    Narrow,
};

enum class Tool {
    Torch,
    ClimbingGear,
    None,
};

bool is_compatible(Tool t, Region r)
{
    return (t == Tool::Torch && (r == Region::Rocky || r == Region::Narrow)) ||
           (t == Tool::ClimbingGear && (r == Region::Rocky || r == Region::Wet)) ||
           (t == Tool::None && (r == Region::Wet || r == Region::Narrow));
}

struct State {
    int x;
    int y;
    Tool t;
    long cost;
    long score;

    State(int x, int y, Tool t, long cost, long cost2target)
    : x(x)
    , y(y)
    , t(t)
    , cost(cost)
    , score(cost + cost2target)
    {}
};

bool operator==(const State& s1, const State& s2)
{
    return s1.x == s2.x && s1.y == s2.y && s1.t == s2.t;
}

bool operator<(const State& s1, const State& s2)
{
    if (s1.score == s2.score) {
        if (s1.cost == s2.cost)
            return std::tie(s1.x, s1.y, s1.t) < std::tie(s2.x, s2.y, s2.t);
        else
            return s1.cost < s2.cost;
    } else {
        return s1.score < s2.score;
    }
}

struct StateHasher {
    auto operator()(const State& s) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= std::hash<decltype(s.x)>{}(s.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<decltype(s.y)>{}(s.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<decltype(s.t)>{}(s.t) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto compute_cost(const TypeMap& t_map, int x_t, int y_t, int limit)
{
    auto cost2target = [&] (auto x, auto y, auto t) {
        return abs(x_t - x) + abs(y_t - y) + (t == Tool::Torch ? 0 : ChangeCost);
    };

    auto next_states = [&] (const auto& s) {
        static const std::pair<int, int> neigh[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        static const Tool tools[] = {Tool::Torch, Tool::ClimbingGear, Tool::None};

        std::vector<State> states;

        for (auto [dx, dy] : neigh) {
            auto next_x = s.x + dx;
            if (next_x < 0 || next_x > limit)
                continue;
            auto next_y = s.y + dy;
            if (next_y < 0 || next_y > limit)
                continue;

            if (is_compatible(s.t, static_cast<Region>(t_map[next_y][next_x])))
                states.emplace_back(next_x, next_y, s.t,
                    s.cost + 1, cost2target(next_x, next_y, s.t));
        }

        for (auto t : tools) {
            if (t == s.t)
                continue;

            if (is_compatible(t, static_cast<Region>(t_map[s.y][s.x])))
                states.emplace_back(s.x, s.y, t,
                    s.cost + ChangeCost, cost2target(s.x, s.y, t));
        }

        return states;
    };

    std::set<State> open_set;
    std::unordered_set<State, StateHasher> closed_set;

    open_set.emplace(0, 0, Tool::Torch, 0, cost2target(0, 0, Tool::Torch));

    while (!open_set.empty()) {
        auto s = *open_set.begin();
        open_set.erase(open_set.begin());

        if (s.x == x_t && s.y == y_t && s.t == Tool::Torch)
            return s.cost;

        closed_set.emplace(s);

        for (const auto& next : next_states(s))
            if (closed_set.find(next) == closed_set.end()) {
                auto it = open_set.find(next);
                if (it == open_set.end()) {
                    open_set.emplace(next);
                } else {
                    if (it->score > next.score ||
                        (it->score == next.score && it->cost > next.cost)) {
                        open_set.erase(it);
                        open_set.emplace(next);
                    }
                }
            }
    }

    return Inf;
}

int main()
{
    std::string line;

    getline(std::cin, line);
    int depth;
    sscanf(line.data(), "depth: %d", &depth);

    getline(std::cin, line);
    int x_target, y_target;
    sscanf(line.data(), "target: %d,%d", &x_target, &y_target);

    auto limit = (1 + ChangeCost) * (x_target + y_target);
    auto t_map = build_type_map(depth, x_target, y_target, limit);

    std::cout << sum_all_types(t_map, x_target, y_target) << "\n";
    std::cout << compute_cost(t_map, x_target, y_target, limit) << "\n";

    return 0;
}

