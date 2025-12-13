#include <bits/stdc++.h>

using namespace std;

auto solve(const vector<int>& v, int n)
{
    auto sz = static_cast<int>(ssize(v));

    vector<int> last_pos(n, 0);
    for (auto i = 0; i < sz - 1; ++i)
        last_pos[v[i]] = i + 1;

    auto prev = v.back();

    for (auto i = sz; i < n; ++i) {
        auto next = last_pos[prev] ? i - last_pos[prev] : 0;
        last_pos[prev] = i;
        prev = next;
    }

    return prev;
}

int main()
{
    vector<int> v;

    string line;
    while (getline(cin, line, ','))
        v.push_back(strtol(line.data(), nullptr, 10));

    cout << solve(v, 2020) << '\n';
    cout << solve(v, 30000000) << '\n';

    return 0;
}

