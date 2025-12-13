#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto solve(long xa, long ya, long xb, long yb, long x, long y)
{
    // a * xa + b * xb = x
    // a * ya + b * yb = y
    // a * xa * yb - a * ya * xb = x * yb - y * xb
    // a = (x * yb - y * xb) / (xa * yb - ya * xb)
    // b = (x - a * xa) / xb

    auto a_den = xa * yb - ya * xb;
    if (a_den == 0)
        return 0l;

    auto a_num = x * yb - y * xb;

    if (a_num % a_den)
        return 0l;

    auto a = a_num / a_den;
    if (a < 0)
        return 0l;

    auto b_num = x - a * xa;

    if (b_num % xb)
        return 0l;

    auto b = b_num / xb;
    if (b < 0)
        return 0l;

    return 3 * a + b;
}

int main()
{
    auto parse = [](string_view sv) {
        auto parts = split(split(sv, ':')[1], ',');

        for (auto& p : parts)
            p.remove_prefix(3);

        return make_pair(str2num(parts[0]), str2num(parts[1]));
    };

    const auto Inc = 10000000000000l;

    auto sum1 = 0l;
    auto sum2 = 0l;

    string s;
    while (getline(cin, s)) {
        if (s.empty())
            continue;

        auto [xa, ya] = parse(s);

        getline(cin, s);
        auto [xb, yb] = parse(s);

        getline(cin, s);
        auto [x, y] = parse(s);

        assert(xa * yb != xb * ya); // assert independent equations

        sum1 += solve(xa, ya, xb, yb, x, y);
        sum2 += solve(xa, ya, xb, yb, x + Inc, y + Inc);
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

