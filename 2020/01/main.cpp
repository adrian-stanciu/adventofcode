#include <bits/stdc++.h>

using namespace std;

auto prod2(const vector<int>& v, int sum)
{
    unordered_set<int> hs;

    for (auto x : v) {
        if (hs.count(sum - x) > 0)
            return x * (sum - x);

        hs.insert(x);
    }

    return -1;
}

auto prod3(const vector<int>& v, int sum)
{
    auto n = v.size();

    unordered_set<int> hs;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            auto sum2 = v[i] + v[j];

            if (hs.count(sum - sum2) > 0)
                return v[i] * v[j] * (sum - sum2);
        }

        hs.insert(v[i]);
    }

    return -1;
}

int main()
{
    vector<int> v;

    int x;
    while (cin >> x)
        v.push_back(x);

    cout << prod2(v, 2020) << '\n';
    cout << prod3(v, 2020) << '\n';

    return 0;
}

