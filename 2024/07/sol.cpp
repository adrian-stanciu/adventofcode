#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

using Func = optional<long>(*)(long, long);

optional<long> sub(long a, long b)
{
    return a - b;
}

optional<long> div(long a, long b)
{
    if (b == 0)
        return nullopt;

    if (a % b != 0)
        return nullopt;

    return a / b;
}

optional<long> dissociate(long a, long b)
{
    auto p = 1l;
    for (auto tmp = b; tmp; tmp /= 10)
        p *= 10;

    if (a % p != b)
        return nullopt;

    return a / p;
}

auto is_possible_rec(long r, const vector<long>& v, int i,
    const vector<Func>& f_vec, const Func& f)
{
    if (i == 0)
        return r == v[0];

    r = f(r, v[i]).value_or(-1);
    if (r < 0)
        return false;

    --i;

    for (const auto& f : f_vec)
        if (is_possible_rec(r, v, i, f_vec, f))
            return true;

    return false;
}

auto is_possible(long r, const vector<long>& v, const vector<Func>& f_vec)
{
    auto n = ssize(v);

    for (const auto& f : f_vec)
        if (is_possible_rec(r, v, n - 1, f_vec, f))
            return true;

    return false;
}

int main()
{
    const vector<Func> f_vec1{sub, div};
    const vector<Func> f_vec2{sub, div, dissociate};

    auto sum1 = 0l;
    auto sum2 = 0l;

    string s;
    while (getline(cin, s)) {
        auto parts = split(s, ':');

        auto r = str2num(parts[0]);

        vector<long> v;
        for (auto str : split(parts[1], ' '))
            v.push_back(str2num(str));

        if (is_possible(r, v, f_vec1))
            sum1 += r;
        if (is_possible(r, v, f_vec2))
            sum2 += r;
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

