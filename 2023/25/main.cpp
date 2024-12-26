#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

void dfs(int n, const vector<vector<int>>& g, int u, int p_u,
    vector<bool>& visited, vector<int>& parents)
{
    visited[u] = true;
    parents[u] = p_u;

    for (auto v = 0; v < n; ++v)
        if (!visited[v] && g[u][v] > 0)
            dfs(n, g, v, u, visited, parents);
}

void dfs(int n, const vector<vector<int>>& g, int u, vector<bool>& reachable)
{
    reachable[u] = true;

    for (auto v = 0; v < n; ++v)
        if (!reachable[v] && g[u][v] != 0)
            dfs(n, g, v, reachable);
}

optional<int> edmond_karp(int n, vector<vector<int>> g, int required_flow, int dst)
{
    auto flow = 0;

    while (true) {
        vector<bool> visited(n, false);
        vector<int> parents(n, -1);
        dfs(n, g, 0, -1, visited, parents);

        if (!visited[dst])
            break;

        auto min_flow = numeric_limits<int>::max();

        auto u = dst;
        while (u != 0) {
            auto p_u = parents[u];
            min_flow = min(min_flow, g[p_u][u]);
            u = p_u;
        }

        flow += min_flow;

        if (flow > required_flow)
            return {};

        u = dst;
        while (u != 0) {
            auto p_u = parents[u];
            g[p_u][u] -= min_flow;
            g[u][p_u] += min_flow;
            u = p_u;
        }
    }

    if (flow != required_flow)
        return {};

    vector<bool> reachable(n, false);
    dfs(n, g, 0, reachable);
    auto num_reachable = count(begin(reachable), end(reachable), true);

    return num_reachable * (n - num_reachable);
}

auto solve(int n, const vector<vector<int>>& g, int required_flow)
{
    random_device rd;
    mt19937 gen{rd()};

    vector<int> order(n - 1);
    iota(begin(order), end(order), 1);
    shuffle(begin(order), end(order), gen);

    for (auto dst : order)
        if (auto sol = edmond_karp(n, g, required_flow, dst); sol)
            return *sol;

    assert(false);
}

int main()
{
    auto [n, g] = []() {
        vector<pair<int, int>> edges;

        auto n = 0;
        unordered_map<string, int> name2idx;

        string s;
        while (getline(cin, s)) {
            const string_view sv{s};

            auto sep = sv.find(':');

            const string src{sv.substr(0, sep)};
            if (!name2idx.contains(src))
                name2idx.emplace(src, n++);

            for (auto dst_sv : split(sv.substr(sep + 1), ' ')) {
                const string dst{dst_sv};
                if (!name2idx.contains(dst))
                    name2idx.emplace(dst, n++);

                edges.emplace_back(name2idx[src], name2idx[dst]);
            }
        }

        vector<vector<int>> g(n, vector<int>(n, 0));
        for (auto [u, v]: edges) {
            g[u][v] = 1;
            g[v][u] = 1;
        }

        return make_pair(n, g);
    }();

    cout << solve(n, g, 3) << '\n';

    return 0;
}

