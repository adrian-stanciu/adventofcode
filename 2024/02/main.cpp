#include <bits/stdc++.h>

using namespace std;

auto is_sol1(const vector<int>& v)
{
    auto sign = [](auto n) {
        return n < 0 ? -1 : 1;
    };

    auto s = sign(v[1] - v[0]);
    for (auto i = 2; i < ssize(v); ++i)
        if (s != sign(v[i] - v[i - 1]))
            return false;

    for (auto i = 1; i < ssize(v); ++i) {
        auto d = abs(v[i] - v[i - 1]);
        if (d < 1 || d > 3)
            return false;
    }

    return true;
}

auto is_sol2(const vector<int>& v)
{
    if (is_sol1(v))
        return true;

    for (auto i = 0; i < ssize(v); ++i) {
        vector<int> w{v};
        w.erase(next(begin(w), i));
        if (is_sol1(w))
            return true;
    }

    return false;
}

int main()
{
    auto cnt1 = 0;
    auto cnt2 = 0;

    string s;
    while (getline(cin, s)) {
        stringstream ss{s};
        vector<int> v;
        int x;
        while (ss >> x)
            v.push_back(x);
        cnt1 += is_sol1(v);
        cnt2 += is_sol2(v);
    }

    cout << cnt1 << '\n';
    cout << cnt2 << '\n';

    return 0;
}

