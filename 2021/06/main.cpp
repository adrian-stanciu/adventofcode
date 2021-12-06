#include <bits/stdc++.h>

using namespace std;

auto solve(const vector<long>& v, int num_iters)
{
    static const auto Size = 9;

    array<long, Size> counters{};
    for (auto x : v)
        ++counters[x];

    while (num_iters--) {
        array<long, Size> new_counters{};

        new_counters[6] += counters[0];
        new_counters[8] += counters[0];

        for (auto i = 1; i < Size; ++i)
            new_counters[i - 1] += counters[i];

        counters.swap(new_counters);
    }

    return accumulate(begin(counters), end(counters), 0l);
}

int main()
{
    vector<long> v;

    string x;
    while (getline(cin, x, ','))
        v.push_back(strtol(x.data(), nullptr, 10));

    cout << solve(v, 80) << '\n';
    cout << solve(v, 256) << '\n';

    return 0;
}

