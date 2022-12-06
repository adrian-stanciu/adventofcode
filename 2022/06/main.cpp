#include <bits/stdc++.h>

using namespace std;

auto solve(const string& s, int n)
{
    map<char, int> freq;

    for (auto i = 0; i < ssize(s); ++i) {
        ++freq[s[i]];

        if (i >= n && --freq[s[i - n]] == 0)
            freq.erase(s[i - n]);

        if (ssize(freq) == n)
            return i + 1;
    }

    assert(false);
}

int main()
{
    string s;
    cin >> s;

    cout << solve(s, 4) << '\n';
    cout << solve(s, 14) << '\n';

    return 0;
}

