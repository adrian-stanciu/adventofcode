#include <bits/stdc++.h>

using namespace std;

auto solve1(const vector<int>& l, const vector<int>& r)
{
    auto s = 0l;

    for (auto i = 0; i < ssize(l); ++i)
        s += abs(l[i] - r[i]);

    return s;
}

auto solve2(const vector<int>& l, const vector<int>& r)
{
    auto s = 0l;

    for (auto x : l) {
        auto [b, e] = equal_range(begin(r), end(r), x);
        s += 1l * x * distance(b, e);
    }

    return s;
}

int main()
{
    vector<int> l;
    vector<int> r;

    string s;
    while (getline(cin, s)) {
        stringstream ss{s};
        int x, y;
        ss >> x >> y;
        l.push_back(x);
        r.push_back(y);
    }

    sort(begin(l), end(l));
    sort(begin(r), end(r));

    cout << solve1(l, r) << '\n';
    cout << solve2(l, r) << '\n';

    return 0;
}

