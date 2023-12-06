#include <bits/stdc++.h>

using namespace std;

void squeeze_repeated(string& s, char ch)
{
    s.erase(unique(begin(s), end(s), [&](auto lhs, auto rhs) {
        return lhs == ch && rhs == ch;
    }), end(s));
}

auto split(string_view sv, char sep)
{
    while (sv.front() == sep)
        sv.remove_prefix(1);
    while (sv.back() == sep)
        sv.remove_suffix(1);

    vector<string_view> split_sv;

    while (true)
        if (auto to = sv.find_first_of(sep); to == sv.npos) {
            split_sv.push_back(sv);
            break;
        } else {
            split_sv.push_back(sv.substr(0, to));
            sv.remove_prefix(to + 1);
        }

    return split_sv;
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

