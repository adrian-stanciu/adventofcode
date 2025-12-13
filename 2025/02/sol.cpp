#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto is_invalid1(string_view sv)
{
    auto sz = sv.size();

    if (sz % 2 == 1)
        return false;

    return sv.substr(0, sz / 2) == sv.substr(sz / 2);
}

auto is_repeated(string_view sv, string_view pattern)
{
    if (sv.size() % pattern.size() != 0)
        return false;

    while (!sv.empty() && sv.starts_with(pattern))
        sv.remove_prefix(pattern.size());

    return sv.empty();
}

auto is_invalid2(string_view sv)
{
    for (size_t sz = 1; sz <= sv.size() / 2; ++sz)
        if (is_repeated(sv, sv.substr(0, sz)))
            return true;

    return false;
}

auto solve(const vector<pair<long, long>>& v)
{
    auto s1 = 0l;
    auto s2 = 0l;

    for (auto [l, r] : v)
        for (auto i = l; i <= r; ++i) {
            auto s = to_string(i);
            if (is_invalid1(s))
                s1 += i;
            if (is_invalid2(s))
                s2 += i;
        }

    cout << s1 << '\n';
    cout << s2 << '\n';
}

int main()
{
    vector<pair<long, long>> v;

    string s;
    getline(cin, s);
    const string_view sv{s};

    for (auto interval_sv : split(sv, ',')) {
        auto interval = split(interval_sv, '-');
        v.emplace_back(str2num(interval[0]), str2num(interval[1]));
    }

    solve(v);

    return 0;
}

