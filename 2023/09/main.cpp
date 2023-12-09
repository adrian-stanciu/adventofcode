#include <bits/stdc++.h>

using namespace std;

auto solve(vector<long> v)
{
    vector<long> last_vals;
    vector<long> first_vals;

    while (count(begin(v), end(v), 0l) != ssize(v)) {
        last_vals.push_back(v.back());
        first_vals.push_back(v.front());

        vector<long> w;
        w.reserve(ssize(v) - 1);
        for (auto i = 1; i < ssize(v); ++i)
            w.push_back(v[i] - v[i - 1]);

        v.swap(w);
    }

    reverse(begin(last_vals), end(last_vals));
    for (auto i = 1; i < ssize(last_vals); ++i)
        last_vals[i] += last_vals[i - 1];

    reverse(begin(first_vals), end(first_vals));
    for (auto i = 1; i < ssize(first_vals); ++i)
        first_vals[i] -= first_vals[i - 1];

    return make_pair(last_vals.back(), first_vals.back());
}

int main()
{
    auto sum1 = 0l;
    auto sum2 = 0l;

    string s;
    while (getline(cin, s)) {
        stringstream ss{s};

        vector<long> v;
        long x;
        while (ss >> x)
            v.push_back(x);

        auto [sol1, sol2] = solve(move(v));
        sum1 += sol1;
        sum2 += sol2;
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

