#include <bits/stdc++.h>

using namespace std;

const auto Inf = numeric_limits<int>::max();

enum Res {
    Or = 0,
    Cl = 1,
    Ob = 2,
    Ge = 3,
};

const auto ResCnt{4};

struct Blueprint {
    int id;
    array<array<int, ResCnt>, ResCnt> cost{};
    array<int, ResCnt> max_needed{};
};

struct State {
    int t;
    array<int, ResCnt> res{};
    array<int, ResCnt> rob{};
};

auto new_robot_ready_after(const Blueprint& bp, const State& s, int i)
{
    auto ready_after = 0;

    for (auto j = 0; j < ResCnt; ++j)
        if (bp.cost[i][j] && bp.cost[i][j] > s.res[j]) {
            if (!s.rob[j])
                return Inf;
            auto needed = bp.cost[i][j] - s.res[j];
            ready_after = max(ready_after, needed / s.rob[j] + (needed % s.rob[j] ? 1 : 0));
        }

    return ready_after + 1;
}

void search(const Blueprint& bp, const State& s, int& max_sol)
{
    auto max_exp = s.res[Res::Ge] + s.rob[Res::Ge] * s.t + (s.t - 1) * s.t / 2;
    if (max_sol > max_exp)
        return;

    auto is_final = true;
    for (auto i = 0; i < ResCnt; ++i)
        if (s.rob[i] < bp.max_needed[i]) {
            auto ready_after = new_robot_ready_after(bp, s, i);
            if (ready_after < s.t) {
                auto ns{s};
                ns.t -= ready_after;
                for (auto j = 0; j < ResCnt; ++j) {
                    ns.res[j] -= bp.cost[i][j];
                    ns.res[j] += ns.rob[j] * ready_after;
                }
                ++ns.rob[i];
                search(bp, ns, max_sol);
                is_final = false;
            }
        }

    if (is_final)
        max_sol = max(max_sol, s.res[Res::Ge] + s.rob[Res::Ge] * s.t);
}

auto count_geodes(const Blueprint& bp, int t)
{
    State s{.t = t};
    s.rob[Res::Or] = 1;

    auto max_sol = 0;
    search(bp, s, max_sol);
    return max_sol;
}

auto solve1(const vector<Blueprint>& bp_vec)
{
    auto sum = 0;
    for (const auto& bp : bp_vec)
        sum += bp.id * count_geodes(bp, 24);
    return sum;
}

auto solve2(const vector<Blueprint>& bp_vec)
{
    auto prod = 1;
    for (auto i = 0; i < 3; ++i)
        prod *= count_geodes(bp_vec[i], 32);
    return prod;
}

int main()
{
    vector<Blueprint> bp_vec;

    string line;
    while (getline(cin, line)) {
        Blueprint bp;
        sscanf(line.data(),
            "Blueprint %d: "
            "Each ore robot costs %d ore. "
            "Each clay robot costs %d ore. "
            "Each obsidian robot costs %d ore and %d clay. "
            "Each geode robot costs %d ore and %d obsidian.",
            &bp.id,
            &bp.cost[Res::Or][Res::Or],
            &bp.cost[Res::Cl][Res::Or],
            &bp.cost[Res::Ob][Res::Or], &bp.cost[Res::Ob][Res::Cl],
            &bp.cost[Res::Ge][Res::Or], &bp.cost[Res::Ge][Res::Ob]);

        for (auto res = 0; res < ResCnt - 1; ++res)
            for (auto rob = 0; rob < ResCnt; ++rob)
                bp.max_needed[res] = max(bp.max_needed[res], bp.cost[rob][res]);
        bp.max_needed[Res::Ge] = Inf;

        bp_vec.push_back(bp);
    }

    cout << solve1(bp_vec) << '\n';
    cout << solve2(bp_vec) << '\n';

    return 0;
}

