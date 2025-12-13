#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

using Graph = unordered_map<string, vector<string>>;

auto dfs(const Graph& g, const string& node, const string& dst,
    const vector<string>& via, vector<bool>& found, unordered_map<string, long>& seen)
{
    string state{node};
    for (auto i = 0; i < ssize(via); ++i)
        if (found[i]) {
            state.push_back('-');
            state.append(via[i]);
        }

    if (auto it = seen.find(state); it != end(seen))
        return it->second;

    if (node == dst) {
        if (all_of(begin(found), end(found), [](auto elem) { return elem; }))
            return 1l;
        else
            return 0l;
    }

    auto cnt = 0l;

    for (auto i = 0; i < ssize(via); ++i)
        if (node == via[i])
            found[i] = true;

    for (const auto& next : g.at(node))
        cnt += dfs(g, next, dst, via, found, seen);

    for (auto i = 0; i < ssize(via); ++i)
        if (node == via[i])
            found[i] = false;

    seen.emplace(state, cnt);

    return cnt;
}

auto solve(const Graph& g, const string& src, const string& dst,
    const vector<string>& via)
{
    vector<bool> found(ssize(via));
    unordered_map<string, long> seen;
    return dfs(g, src, dst, via, found, seen);
}

int main()
{
    Graph g;

    string s;
    while (getline(cin, s)) {
        const string_view sv{s};
        auto colon_pos = sv.find(':');

        vector<string> adj_vec;
        for (auto next : split(sv.substr(colon_pos + 1), ' '))
            adj_vec.emplace_back(next);

        g.emplace(sv.substr(0, colon_pos), move(adj_vec));
    }

    cout << solve(g, "you", "out", vector<string>{}) << '\n';
    cout << solve(g, "svr", "out", vector<string>{"dac", "fft"}) << '\n';

    return 0;
}

