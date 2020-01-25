#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "hasher.h"

using TypeMap = std::vector<std::vector<int>>;

constexpr auto Inf = std::numeric_limits<long>::max();
constexpr auto ChangeCost = 7;

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

enum Region {
    Rocky,
    Wet,
    Narrow,
};

struct Dist {
    long to;
    long cg;
    long no;

    long score;
};

bool operator==(const Dist& d1, const Dist& d2)
{
    return d1.to == d2.to && d1.cg == d2.cg && d1.no == d2.no &&
        d1.score == d2.score;
}

void init_dist(Dist& next_d, int next_pos_type,
    const Dist& curr_d, int curr_pos_type)
{
    if (curr_pos_type == Region::Rocky) {
        if (next_pos_type == Region::Rocky) {
            next_d.to = curr_d.to + 1;
            next_d.cg = curr_d.cg + 1;
            next_d.no = Inf;
        } else if (next_pos_type == Region::Wet) {
            next_d.to = Inf;
            next_d.cg = curr_d.cg + 1;
            next_d.no = curr_d.cg + 1 + ChangeCost;
        } else if (next_pos_type == Region::Narrow) {
            next_d.to = curr_d.to + 1;
            next_d.cg = Inf;
            next_d.no = curr_d.to + 1 + ChangeCost;
        }
    } else if (curr_pos_type == Region::Wet) {
        if (next_pos_type == Region::Rocky) {
            next_d.to = curr_d.cg + 1 + ChangeCost;
            next_d.cg = curr_d.cg + 1;
            next_d.no = Inf;
        } else if (next_pos_type == Region::Wet) {
            next_d.to = Inf;
            next_d.cg = curr_d.cg + 1;
            next_d.no = curr_d.no + 1;
        } else if (next_pos_type == Region::Narrow) {
            next_d.to = curr_d.no + 1 + ChangeCost;
            next_d.cg = Inf;
            next_d.no = curr_d.no + 1;
        }
    } else if (curr_pos_type == Region::Narrow) {
        if (next_pos_type == Region::Rocky) {
            next_d.to = curr_d.to + 1;
            next_d.cg = curr_d.to + 1 + ChangeCost;
            next_d.no = Inf;
        } else if (next_pos_type == Region::Wet) {
            next_d.to = Inf;
            next_d.cg = curr_d.no + 1 + ChangeCost;
            next_d.no = curr_d.no + 1;
        } else if (next_pos_type == Region::Narrow) {
            next_d.to = curr_d.to + 1;
            next_d.cg = Inf;
            next_d.no = curr_d.no + 1;
        }
    }
}

void update_dist(Dist& next_d, int next_pos_type,
    const Dist& curr_d, int curr_pos_type)
{
    auto to = next_d.to;
    auto cg = next_d.cg;
    auto no = next_d.no;

    if (curr_pos_type == Region::Rocky) {
        if (next_pos_type == Region::Rocky) {
            to = std::min(to, curr_d.to + 1);
            cg = std::min(cg, curr_d.cg + 1);
        } else if (next_pos_type == Region::Wet) {
            cg = std::min(cg, curr_d.cg + 1);
            no = std::min(no, cg + ChangeCost);
        } else if (next_pos_type == Region::Narrow) {
            to = std::min(to, curr_d.to + 1);
            no = std::min(no, to + ChangeCost);
        }
    } else if (curr_pos_type == Region::Wet) {
        if (next_pos_type == Region::Rocky) {
            cg = std::min(cg, curr_d.cg + 1);
            to = std::min(to, cg + ChangeCost);
        } else if (next_pos_type == Region::Wet) {
            cg = std::min(cg, curr_d.cg + 1);
            no = std::min(no, curr_d.no + 1);
        } else if (next_pos_type == Region::Narrow) {
            no = std::min(no, curr_d.no + 1);
            to = std::min(to, no + ChangeCost);
        }
    } else if (curr_pos_type == Region::Narrow) {
        if (next_pos_type == Region::Rocky) {
            to = std::min(to, curr_d.to + 1);
            cg = std::min(cg, to + ChangeCost);
        } else if (next_pos_type == Region::Wet) {
            no = std::min(no, curr_d.no + 1);
            cg = std::min(cg, no + ChangeCost);
        } else if (next_pos_type == Region::Narrow) {
            to = std::min(to, curr_d.to + 1);
            no = std::min(no, curr_d.no + 1);
        }
    }

    next_d.to = to;
    next_d.cg = cg;
    next_d.no = no;
}

auto compute_dist(const TypeMap& t_map, int x_t, int y_t, int limit)
{
    const std::pair<int, int> neighbours[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    auto dist = [x_t, y_t] (int x, int y) {
        return abs(x_t - x) + abs(y_t -y);
    };

    auto update_score = [&dist] (auto& d, auto x, auto y) {
        if (d.to == Inf)
            d.score = std::min(d.cg, d.no) + dist(x, y) + ChangeCost;
        else
            d.score = d.to + dist(x, y);
    };

    std::unordered_map<std::pair<int, int>, Dist, PairHasher<int>> d_map;

    auto point_cmp = [&d_map] (const auto& p1, const auto& p2) {
        auto dist1 = d_map[p1];
        auto dist2 = d_map[p2];

        if (dist1.score == dist2.score)
            return p1 < p2;
        else
            return dist1.score < dist2.score;
    };

    std::set<std::pair<int, int>, decltype(point_cmp)> open_set(point_cmp);
    std::unordered_set<std::pair<int, int>, PairHasher<int>> closed_set;

    d_map[std::make_pair(0, 0)] = {0, ChangeCost, Inf, dist(0, 0)};
    open_set.emplace(0, 0);

    while (!open_set.empty()) {
        auto curr = *open_set.begin();
        open_set.erase(open_set.begin());

        if (curr.first == x_t && curr.second == y_t)
            break;

        closed_set.emplace(curr.first, curr.second);

        const auto& curr_d = d_map[curr];
        auto curr_pos_type = t_map[curr.second][curr.first];

        for (const auto& [neigh_x, neigh_y] : neighbours) {
            auto next_x = curr.first + neigh_x;
            auto next_y = curr.second + neigh_y;

            if (next_x < 0 || next_y < 0)
                continue;
            if (next_x > limit || next_y > limit)
                continue;

            auto next = std::make_pair(next_x, next_y);
            auto next_pos_type = t_map[next_y][next_x];

            if (closed_set.find(next) == closed_set.end()) {
                auto it = open_set.find(next);
                if (it == open_set.end()) {
                    auto& next_d = d_map[next];

                    init_dist(next_d, next_pos_type, curr_d, curr_pos_type);
                    update_score(next_d, next_x, next_y);

                    open_set.emplace(next_x, next_y);
                } else {
                    open_set.erase(it);

                    auto& next_d = d_map[next];

                    update_dist(next_d, next_pos_type, curr_d, curr_pos_type);
                    update_score(next_d, next_x, next_y);

                    open_set.emplace(std::move(next));
                }
            } else {
                const auto& old_next_d = d_map[next];
                auto new_next_d = d_map[next];

                update_dist(new_next_d, next_pos_type, curr_d, curr_pos_type);
                update_score(new_next_d, next_x, next_y);

                if ((new_next_d.score > old_next_d.score) ||
                    (new_next_d == old_next_d))
                    continue;

                d_map[next] = std::move(new_next_d);

                open_set.emplace(std::move(next));
            }
        }
    }

    return d_map[std::make_pair(x_t, y_t)].to;
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
    std::cout << compute_dist(t_map, x_target, y_target, limit) << "\n";

    return 0;
}

