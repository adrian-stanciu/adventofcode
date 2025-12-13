#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto topo_sort(unordered_map<int, vector<int>>& g_fwd, unordered_map<int, int>& in_deg)
{
    vector<int> sorted;

    queue<int> q;
    for (auto [u, d] : in_deg)
        if (d == 0)
            q.push(u);

    while (!q.empty()) {
        auto u = q.front();
        q.pop();

        sorted.push_back(u);

        for (auto v : g_fwd[u]) {
            --in_deg[v];
            if (in_deg[v] == 0)
                q.push(v);
        }
    }

    return sorted;
}

int main()
{
    set<pair<int, int>> deps;

    string s;
    while (getline(cin, s)) {
        if (s.empty())
            break;

        auto dep = split(s, '|');
        deps.emplace(str2num(dep[0]), str2num(dep[1]));
    }

    auto sort = [&](const auto& v) {
        unordered_map<int, vector<int>> g_fwd;
        unordered_map<int, int> in_deg;

        for (auto x : v)
            in_deg[x] = 0;

        for (auto i = 0; i < ssize(v); ++i)
            for (auto j = i + 1; j < ssize(v); ++j) {
                if (deps.contains(make_pair(v[i], v[j]))) {
                    g_fwd[v[i]].push_back(v[j]);
                    ++in_deg[v[j]];
                }
                if (deps.contains(make_pair(v[j], v[i]))) {
                    g_fwd[v[j]].push_back(v[i]);
                    ++in_deg[v[i]];
                }
            }

        return topo_sort(g_fwd, in_deg);
    };

    auto sum1 = 0l;
    auto sum2 = 0l;

    while (getline(cin, s)) {
        vector<int> v;
        for (auto str : split(s, ','))
            v.push_back(str2num(str));

        auto w = sort(v);

        if (w == v)
            sum1 += w[ssize(w) / 2];
        else
            sum2 += w[ssize(w) / 2];
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

