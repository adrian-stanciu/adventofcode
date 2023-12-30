#include <bits/stdc++.h>

#include "parser.h"

using namespace std;

void squeeze_repeated(string& s, char ch)
{
    s.erase(unique(begin(s), end(s), [&](auto lhs, auto rhs) {
        return lhs == ch && rhs == ch;
    }), end(s));
}

auto str2num(string_view sv)
{
    auto n = 0l;
    from_chars(sv.data(), sv.data() + sv.size(), n);
    return n;
}

auto count(long t, long d)
{
    optional<long> ans;

    auto lo = 0l;
    auto hi = t / 2;

    while (lo <= hi) {
        auto mid = lo + (hi - lo) / 2;

        if (mid * (t - mid) > d) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    return ans ? t + 1 - 2 * *ans : 0l;
}

auto solve1(const vector<long>& times, const vector<long>& dists)
{
    auto p = 1l;
    for (auto i = 0; i < ssize(times); ++i)
        p *= count(times[i], dists[i]);
    return p;
}

auto solve2(const vector<long>& times, const vector<long>& dists)
{
    auto concat = [](const auto& v) {
        string s;
        for (auto x : v)
            s.append(to_string(x));
        return str2num(s);
    };

    return count(concat(times), concat(dists));
}

int main()
{
    auto read = [&]() {
        string s;
        getline(cin, s);
        squeeze_repeated(s, ' ');

        string_view sv{s};
        sv.remove_prefix(sv.find(':') + 1);
        auto parts = split(sv, ' ');

        vector<long> v;
        v.reserve(ssize(parts));
        for (auto part : parts)
            v.push_back(str2num(part));
        return v;
    };

    auto times = read();
    auto dists = read();

    cout << solve1(times, dists) << '\n';
    cout << solve2(times, dists) << '\n';

    return 0;
}

