#include <bits/stdc++.h>

using namespace std;

auto solve(const vector<string>& v)
{
    auto n = ssize(v);
    auto m = ssize(v[0]);

    auto [r, c] = [&]() {
        for (auto i = 0; i < n; ++i)
            for (auto j = 0; j < m; ++j)
                if (v[i][j] == 'S')
                    return make_pair(i, j);
        assert(false);
    }();

    auto cnt = 0;

    vector<vector<long>> freq(n, vector<long>(m));
    freq[r][c] = 1l;

    for (auto i = r; i < n - 1; ++i)
        for (auto j = 0; j < m; ++j) {
            if (!freq[i][j])
                continue;

            if (v[i + 1][j] == '^') {
                ++cnt;

                if (j != 0)
                    freq[i + 1][j - 1] += freq[i][j];
                if (j != m - 1)
                    freq[i + 1][j + 1] += freq[i][j];
            } else {
                freq[i + 1][j] += freq[i][j];
            }
        }

    cout << cnt << '\n';

    cout << accumulate(begin(freq.back()), end(freq.back()), 0l) << '\n';
}

int main()
{
    vector<string> v;

    string s;
    while (getline(cin, s))
        v.push_back(move(s));

    solve(v);

    return 0;
}

