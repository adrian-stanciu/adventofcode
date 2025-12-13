#include <bits/stdc++.h>

using namespace std;

auto solve1(const vector<int>& v)
{
    array<int, 4> diff{};

    for (size_t i = 1; i < v.size(); ++i)
        ++diff[v[i] - v[i - 1]];

    return diff[1] * diff[3];
}

auto solve2(const vector<int>& v)
{
    vector<long> dp(v.size(), 0);
    dp[0] = 1;

    for (size_t i = 0; i < v.size(); ++i)
        for (size_t j = i + 1; j < v.size() && v[j] - v[i] <= 3; ++j)
            dp[j] += dp[i];

    return dp[v.size() - 1];
}

int main()
{
    vector<int> v;

    int x;
    while (cin >> x)
        v.push_back(x);

    v.push_back(0);
    v.push_back(*max_element(begin(v), end(v)) + 3);

    sort(begin(v), end(v));

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

