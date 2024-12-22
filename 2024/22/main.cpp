#include <bits/stdc++.h>

using namespace std;

constexpr auto power(long n, int p)
{
    auto r = 1l;
    while (p) {
        if (p & 1)
            r = r * n;
        n *= n;
        p /= 2;
    }
    return r;
}

auto solve(const vector<long>& v, int iters)
{
    static constexpr auto Mod{16777216};
    static constexpr auto Base{20};
    static constexpr auto Size{power(Base, 4)};

    auto index = [](const array<int, 4>& seq) {
        auto idx = 0;
        for (auto x : seq) {
            idx *= Base;
            idx += x + 9;
        }
        return idx;
    };

    auto sum = 0l;
    vector<int> counters(Size);

    for (auto n : v) {
        vector<int> prices;
        prices.reserve(iters + 1);
        prices.push_back(n % 10);
        for (auto i = 0; i < iters; ++i) {
            n ^= n * 64;
            n %= Mod;
            n ^= n / 32;
            n %= Mod;
            n ^= n * 2048;
            n %= Mod;
            prices.push_back(n % 10);
        }

        sum += n;

        vector<int> diffs;
        diffs.reserve(iters);
        for (auto i = 1; i < iters + 1; ++i)
            diffs.push_back(prices[i] - prices[i - 1]);

        bitset<Size> seen;
        for (auto i = 0; i < iters - 3; ++i)
            if (auto idx = index({diffs[i], diffs[i + 1], diffs[i + 2], diffs[i + 3]}); !seen[idx]) {
                counters[idx] += prices[i + 4];
                seen.set(idx);
            }
    }

    cout << sum << '\n';
    cout << *max_element(begin(counters), end(counters)) << '\n';
}

int main()
{
    vector<long> v;

    long n;
    while (cin >> n)
        v.push_back(n);

    solve(v, 2000);

    return 0;
}

