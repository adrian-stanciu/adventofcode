#include <bits/stdc++.h>

using namespace std;

using Edges = map<string, pair<string, string>>;

auto solve1(const string& dirs, const Edges& edges)
{
    auto cnt = 0;

    string node{"AAA"};
    while (node != "ZZZ") {
        const auto& dst = edges.at(node);
        node = dirs[cnt % ssize(dirs)] == 'L' ? dst.first : dst.second;
        ++cnt;
    }

    return cnt;
}

auto solve2(const string& dirs, const Edges& edges)
{
    vector<string> nodes;
    for (const auto& [src, _] : edges)
        if (src.back() == 'A')
            nodes.push_back(src);

    auto res = 1l;

    for (auto node : nodes) {
        map<pair<string, int>, int> uniq;
        uniq[make_pair(node, 0)] = 0;

        auto cnt = 0;

        while (true) {
            const auto& dst = edges.at(node);
            node = dirs[cnt % ssize(dirs)] == 'L' ? dst.first : dst.second;
            ++cnt;

            auto [_, inserted] = uniq.emplace(make_pair(node, cnt % ssize(dirs)), cnt);
            if (!inserted)
                break;
        }

        assert(count_if(begin(uniq), end(uniq), [](const auto& elem) {
            return elem.first.first.back() == 'Z';
        }) == 1);

        auto it = find_if(begin(uniq), end(uniq), [](const auto& elem) {
            return elem.first.first.back() == 'Z';
        });

        assert(uniq[make_pair(node, cnt % ssize(dirs))] == cnt - it->second);

        res = lcm(res, it->second);
    }

    return res;
}

int main()
{
    string dirs;
    cin >> dirs;

    auto edges = []() {
        static const regex re{R"(([A-Z0-9]{3}) = \(([A-Z0-9]{3}), ([A-Z0-9]{3})\))"};

        Edges edges;

        string s;
        while (getline(cin, s)) {
            smatch matched;
            if (!regex_match(s, matched, re))
                continue;

            edges.try_emplace(matched[1].str(), matched[2].str(), matched[3].str());
        }

        return edges;
    }();

    cout << solve1(dirs, edges) << '\n';
    cout << solve2(dirs, edges) << '\n';

    return 0;
}

