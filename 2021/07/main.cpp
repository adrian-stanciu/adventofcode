#include <bits/stdc++.h>

using namespace std;

auto solve1(const vector<long>& v)
{
    auto mid = v[v.size() / 2];

    auto cost = 0l;

    for (auto x : v)
        cost += abs(mid - x);

    return cost;
}

auto solve2(const vector<long>& v)
{
    auto sum_n = [](auto n) {
        return n * (n + 1) / 2;
    };

    auto min_cost = numeric_limits<long>::max();

    for (auto pos = v.front(); pos <= v.back(); ++pos) {
        auto cost = 0l;

        for (auto x : v)
            cost += sum_n(abs(pos - x));

        min_cost = min(min_cost, cost);
    }

    return min_cost;
}

int main()
{
    vector<long> v;

    string x;
    while (getline(cin, x, ','))
        v.push_back(strtol(x.data(), nullptr, 10));

    sort(begin(v), end(v));

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

