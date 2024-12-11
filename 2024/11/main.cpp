#include <bits/stdc++.h>

using namespace std;

auto extract_digits(long x)
{
    vector<int> digits;

    if (x == 0) {
        digits.push_back(0);
        return digits;
    }

    while (x) {
        digits.push_back(x % 10);
        x /= 10;
    }

    reverse(begin(digits), end(digits));

    return digits;
}

auto build_num(const vector<int>& digits, int begin, int end)
{
    auto x = 0l;

    for (auto i = begin; i < end; ++i) {
        x *= 10;
        x += digits[i];
    }

    return x;
}

auto solve(unordered_map<long, long> freq_map, int iters)
{
    while (iters--) {
        unordered_map<long, long> next_freq_map;

        for (const auto& [x, freq] : freq_map)
            if (x == 0) {
                next_freq_map[1] += freq;
            } else {
                auto digits = extract_digits(x);

                if (auto n = ssize(digits); n % 2 == 0) {
                    next_freq_map[build_num(digits, 0, n / 2)] += freq;
                    next_freq_map[build_num(digits, n / 2, n)] += freq;
                } else {
                    next_freq_map[x * 2024] += freq;
                }
            }

        freq_map.swap(next_freq_map);
    }

    return accumulate(begin(freq_map), end(freq_map), 0l,
        [](auto acc, const auto& elem) { return acc + elem.second; });
}

int main()
{
    unordered_map<long, long> freq_map;

    long x;
    while (cin >> x)
        ++freq_map[x];

    cout << solve(freq_map, 25) << '\n';
    cout << solve(freq_map, 75) << '\n';

    return 0;
}

