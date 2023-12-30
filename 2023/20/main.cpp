#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

enum class Type {
    Broadcaster,
    FlipFlop,
    Conjunction,
};

void move(const unordered_map<string, Type>& type_map,
    unordered_map<string, bool>& ff_state_map,
    unordered_map<string, vector<pair<string, bool>>>& in_map,
    const unordered_map<string, vector<string>>& out_map,
    const string& src, const string& dst, bool pulse,
    queue<tuple<string, string, bool>>& q)
{
    if (!type_map.contains(dst))
        return;

    switch (type_map.at(dst)) {
    case Type::Broadcaster:
        for (const auto& out : out_map.at(dst))
            q.emplace(dst, out, pulse);
        break;
    case Type::FlipFlop:
        if (!pulse) {
            auto& state = ff_state_map.at(dst);
            state = !state;
            for (const auto& out : out_map.at(dst))
                q.emplace(dst, out, state);
        }
        break;
    case Type::Conjunction:
        {
            auto& ins = in_map.at(dst);
            for (auto& [in, state] : ins)
                if (in == src) {
                    state = pulse;
                    break;
                }
            auto pulse = !all_of(begin(ins), end(ins), [](const auto& elem) {
                return elem.second;
            });
            for (const auto& out : out_map.at(dst))
                q.emplace(dst, out, pulse);
        }
        break;
    }
}

auto solve1(const unordered_map<string, Type>& type_map,
    unordered_map<string, bool> ff_state_map,
    unordered_map<string, vector<pair<string, bool>>> in_map,
    const unordered_map<string, vector<string>>& out_map,
    int iters)
{
    queue<tuple<string, string, bool>> q;

    array<long, 2> cnt{};

    while (iters--) {
        q.emplace("button", "broadcaster", false);

        while (!q.empty()) {
            auto [src, dst, pulse] = q.front();
            q.pop();

            ++cnt[pulse];

            move(type_map, ff_state_map, in_map, out_map, src, dst, pulse, q);
        }
    }

    return cnt[0] * cnt[1];
}

auto solve2(const unordered_map<string, Type>& type_map,
    unordered_map<string, bool> ff_state_map,
    unordered_map<string, vector<pair<string, bool>>> in_map,
    const unordered_map<string, vector<string>>& out_map,
    const string& sink)
{
    // assumptions based on input
    auto sink_ins = in_map.at(sink);
    assert(ssize(sink_ins) == 1);
    auto sink_in = sink_ins[0].first;
    assert(type_map.at(sink_in) == Type::Conjunction);

    unordered_map<string, int> significant_in2cycle_sz;
    for (const auto& [in, _] : in_map.at(sink_in))
        significant_in2cycle_sz.emplace(in, 0);

    auto iters = 0;
    while (true) {
        ++iters;

        queue<tuple<string, string, bool>> q;
        q.emplace("button", "broadcaster", false);

        while (!q.empty()) {
            auto [src, dst, pulse] = q.front();
            q.pop();

            if (dst == sink_in && pulse) {
                auto& cycle_sz = significant_in2cycle_sz.at(src);
                if (!cycle_sz) {
                    cycle_sz = iters;

                    if (all_of(begin(significant_in2cycle_sz), end(significant_in2cycle_sz),
                        [](const auto& elem) { return elem.second != 0; })) {
                        auto sol = 1l;
                        for (const auto& [_, cycle_sz] : significant_in2cycle_sz)
                            sol = lcm(sol, cycle_sz);
                        return sol;
                    }
                }
            }

            move(type_map, ff_state_map, in_map, out_map, src, dst, pulse, q);
        }
    }
}

int main()
{
    unordered_map<string, Type> type_map;
    unordered_map<string, bool> ff_state_map;
    unordered_map<string, vector<pair<string, bool>>> in_map;
    unordered_map<string, vector<string>> out_map;

    string s;
    while (getline(cin, s)) {
        string_view sv{s};

        const string_view delim{"->"};
        auto sep = sv.find(delim);
        auto src = sv.substr(0, sep - 1);
        vector<string> outs;
        for (auto out : split(sv.substr(sep + delim.size()), ','))
            outs.emplace_back(out.substr(1));

        if (src[0] == '%') {
            src.remove_prefix(1);
            type_map.emplace(src, Type::FlipFlop);
            ff_state_map.emplace(src, false);
        } else if (src[0] == '&') {
            src.remove_prefix(1);
            type_map.emplace(src, Type::Conjunction);
        } else {
            type_map.emplace(src, Type::Broadcaster);
        }

        for (const auto& out : outs)
            in_map[out].emplace_back(src, false);

        out_map.emplace(src, move(outs));
    }

    cout << solve1(type_map, ff_state_map, in_map, out_map, 1000) << '\n';
    cout << solve2(type_map, ff_state_map, in_map, out_map, "rx") << '\n';

    return 0;
}

