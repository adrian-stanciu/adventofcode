#include <bits/stdc++.h>

using namespace std;

auto read(const string& line)
{
    static const regex r{"([a-zA-Z]* [a-zA-Z]*) bags contain (.*)."};
    smatch matched;
    regex_match(line, matched, r);

    auto color = matched[1].str();
    vector<pair<string, int>> children;

    auto children_s = matched[2].str();
    if (children_s != "no other bags") {
        stringstream children_ss{children_s};

        string child_s;
        while (getline(children_ss, child_s, ',')) {
            stringstream child_ss{child_s};

            int n;
            string c1, c2;
            child_ss >> n >> c1 >> c2;

            children.emplace_back(move(c1) + " " + move(c2), n);
        }
    }

    return make_pair(color, children);
}

using Graph1 = unordered_map<string, vector<string>>;
using Graph2 = unordered_map<string, vector<pair<string, int>>>;

auto count(const Graph1& g, string target)
{
    queue<string> q;
    q.emplace(target);

    unordered_set<string> visited;
    visited.emplace(move(target));

    while (!q.empty()) {
        auto color = q.front();
        q.pop();

        for (const auto& child : g.at(color))
            if (visited.count(child) == 0) {
                q.emplace(child);
                visited.emplace(child);
            }
    }

    return visited.size() - 1; // do not count target
}

auto count(const Graph2& g, string target)
{
    auto cnt = 0;

    queue<pair<string, int>> q;
    q.emplace(move(target), 1);

    while (!q.empty()) {
        auto [color, n] = q.front();
        q.pop();

        cnt += n;

        for (const auto& [child, m] : g.at(color))
            q.emplace(child, n * m);
    }

    return cnt - 1; // do not count target
}

int main()
{
    Graph1 g1;
    Graph2 g2;

    string line;
    while (getline(cin, line)) {
        auto [color, children] = read(line);

        g1[color]; // just to create the node if it does not exist
        for (const auto& [child, n] : children)
            g1[child].push_back(color);

        g2[move(color)] = move(children);
    }

    auto target = "shiny gold";
    cout << count(g1, target) << '\n';
    cout << count(g2, target) << '\n';

    return 0;
}

