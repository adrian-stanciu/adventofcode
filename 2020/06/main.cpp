#include <bits/stdc++.h>

using namespace std;

int main()
{
    auto cnt1 = 0;
    auto cnt2 = 0;

    auto n = 0;
    array<int, 26> freq{};

    string line;
    while (getline(cin, line)) {
        if (line.empty()) {
            if (n == 0)
                continue;

            cnt1 += accumulate(begin(freq), end(freq), 0, [&] (auto acc, auto x) {
                return acc + (x != 0);
            });
            cnt2 += accumulate(begin(freq), end(freq), 0, [&] (auto acc, auto x) {
                return acc + (x == n);
            });

            n = 0;
            freq.fill(0);
        } else {
            ++n;
            for (auto c : line)
                ++freq[c - 'a'];
        }
    }

    if (n > 0) {
        cnt1 += accumulate(begin(freq), end(freq), 0, [&] (auto acc, auto x) {
            return acc + (x != 0);
        });
        cnt2 += accumulate(begin(freq), end(freq), 0, [&] (auto acc, auto x) {
            return acc + (x == n);
        });
    }

    cout << cnt1 << '\n';
    cout << cnt2 << '\n';

    return 0;
}

