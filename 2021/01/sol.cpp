#include <bits/stdc++.h>

using namespace std;

auto solve1(const vector<int>& v)
{
    auto cnt = 0;

    auto prev = numeric_limits<int>::max();

    for (auto x : v) {
        cnt += x > prev;
        prev = x;
    }

    return cnt;
}

auto solve2(const vector<int>& v)
{
    auto n = static_cast<int>(v.size());

    auto cnt = 0;

    auto prev = accumulate(begin(v), begin(v) + 3, 0);

    for (auto i = 3; i < n; ++i) {
        auto sum = prev + v[i] - v[i - 3];
        cnt += sum > prev;
        prev = sum;
    }

    return cnt;
}

int main()
{
    vector<int> v;

    int x;
    while (cin >> x)
        v.push_back(x);

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

