#include <bits/stdc++.h>

using namespace std;

auto solve(const vector<long>& v, int iters)
{
    const auto n = ssize(v);

    vector<int> idx(n);
    iota(begin(idx), end(idx), 0);

    for (auto iter = 0; iter < iters; ++iter)
        for (auto i = 0; i < n; ++i) {
            auto old_pos = distance(begin(idx), find(begin(idx), end(idx), i));

            if (v[i] == 0)
                continue;

            auto forward_moves = (v[i] % (n - 1) + n - 1) % (n - 1);
            auto new_pos = (old_pos + forward_moves) % (n - 1);

            if (new_pos > old_pos)
                rotate(begin(idx) + old_pos, begin(idx) + old_pos + 1, begin(idx) + new_pos + 1);
            else if (new_pos < old_pos)
                rotate(begin(idx) + new_pos, begin(idx) + old_pos, begin(idx) + old_pos + 1);
        }

    auto pos0 = distance(begin(idx), find_if(begin(idx), end(idx), [&](auto i) {
        return v[i] == 0;
    }));

    auto sum = 0l;
    for (auto i = 1; i <= 3; ++i)
        sum += v[idx[(pos0 + i * 1000) % n]];
    return sum;
}

int main()
{
    vector<long> v;

    long x;
    while (cin >> x)
        v.push_back(x);

    cout << solve(v, 1) << '\n';

    for (auto& x : v)
        x *= 811'589'153;

    cout << solve(v, 10) << '\n';

    return 0;
}

