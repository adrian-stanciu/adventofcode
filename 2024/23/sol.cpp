#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

using Graph = vector<vector<int>>;

auto are_connected(const Graph& g, int u, int v)
{
    auto it = lower_bound(begin(g[u]), end(g[u]), v);
    return it != end(g[u]) && *it == v;
}

auto solve1(const Graph& g, int n, const vector<string>& names, char ch)
{
    auto cnt = 0;

    for (auto u = 0; u < n; ++u)
        for (auto v : g[u])
            if (u < v)
                for (auto w : g[v])
                    if (v < w && are_connected(g, w, u) &&
                        (names[u][0] == ch || names[v][0] == ch || names[w][0] == ch))
                        ++cnt;

    return cnt;
}

auto extends_clique(const Graph& g, int u, const vector<int>& clique)
{
    for (auto v : clique)
        if (!are_connected(g, u, v))
            return false;
    return true;
}

void solve2_rec(const Graph& g, int u, vector<int>& clique, vector<int>& max_clique)
{
    clique.push_back(u);

    for (auto v : g[u])
        if (u < v && extends_clique(g, v, clique))
            solve2_rec(g, v, clique, max_clique);

    if (ssize(clique) > ssize(max_clique))
        max_clique = clique;

    clique.pop_back();
}

auto solve2(const Graph& g, int n)
{
    vector<int> max_clique;

    for (auto u = 0; u < n; ++u)
        for (auto v : g[u])
            if (u < v) {
                vector<int> clique;
                clique.push_back(u);
                solve2_rec(g, v, clique, max_clique);
            }

    return max_clique;
}

int main()
{
    unordered_map<string, int> name2id;
    vector<string> names;

    auto id = [&](string_view name_sv) -> int {
        string name{name_sv};

        if (auto it = name2id.find(name); it != end(name2id))
            return it->second;

        auto id = ssize(names);
        name2id.emplace(name, id);
        names.push_back(move(name));
        return id;
    };

    vector<pair<int, int>> edges;
    string s;
    while (getline(cin, s)) {
        auto parts = split(s, '-');
        auto u = id(parts[0]);
        auto v = id(parts[1]);
        edges.emplace_back(u, v);
    }

    const auto n = ssize(names);
    Graph g(n);
    for (const auto& [u, v] : edges) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (auto& adj : g)
        sort(begin(adj), end(adj));

    cout << solve1(g, n, names, 't') << '\n';

    vector<string> max_clique_names;
    for (auto id : solve2(g, n))
        max_clique_names.push_back(names[id]);
    sort(begin(max_clique_names), end(max_clique_names));
    for (auto i = 0; i < ssize(max_clique_names) - 1; ++i)
        cout << max_clique_names[i] << ',';
    cout << max_clique_names.back() << '\n';

    return 0;
}

