#include <bits/stdc++.h>

using namespace std;

auto str2num(string_view sv)
{
    auto n = 0;
    from_chars(sv.data(), sv.data() + sv.size(), n);
    return n;
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

auto solve(const string& s, const vector<int>& v)
{
    // dp[i][j] (i > 0, j > 0) is the count of different arrangements using
    // the first i values from v and the first j chars from s,
    // such that v[i - 1] ended at s[j - 1]
    vector<vector<long>> dp(ssize(v) + 1, vector<long>(ssize(s) + 1, 0));

    // base case: 1 way to arrange 0 values using 0 chars
    dp[0][0] = 1;

    for (auto i = 0; i < ssize(v); ++i)
        for (auto j = 0; j < ssize(s); ++j)
            if (dp[i][j] != 0) {
                // the first index in s where v[i] can start:
                // * v[0] can start from s[j]
                // * v[i] (i > 0) can start from s[j + 1] because
                // v[i - 1] ended at s[j - 1] and at least one space must exist in s
                // between two consecutive v values
                const auto s_first_idx = i == 0 ? j : j + 1;

                // the last index in s where v[i] can start
                const auto s_last_idx = ssize(s) - v[i];

                for (auto begin = s_first_idx; begin <= s_last_idx; ++begin) {
                    const auto end = begin + v[i];

                    auto matched = true;

                    // a match cannot be followed immediately by a '#'
                    if (end < ssize(s) && s[end] == '#')
                        matched = false;

                    if (matched) {
                        // a match cannot span over '.'s
                        for (auto k = begin; k < end; ++k)
                            if (s[k] == '.') {
                                matched = false;
                                break;
                            }

                        // v[i] can end at s[end - 1]
                        if (matched)
                            dp[i + 1][end] += dp[i][j];
                    }

                    // a '#' cannot be skipped
                    if (s[begin] == '#')
                        break;
                }
            }

    // the result is the sum of dp values in the last row starting from the position
    // corresponding to the last '#' in s (or from the beginning if there are no '#'s
    // in s) because all '#'s from s must be used
    auto pos = s.rfind('#');
    if (pos == s.npos)
        pos = 0;
    return accumulate(next(begin(dp.back()), pos + 1), end(dp.back()), 0l);
}

int main()
{
    auto parse = [](const string& line) {
        const string_view sv{line};
        auto sep = sv.find(' ');

        string s{sv.substr(0, sep)};

        vector<int> v;
        for (auto x : split(sv.substr(sep + 1), ','))
            v.push_back(str2num(x));

        return make_pair(move(s), move(v));
    };

    auto expand = [](const auto& s, const auto& v) {
        string s2{s};
        for (auto i = 0; i < 4; ++i) {
            s2.push_back('?');
            s2.append(s);
        }

        vector<int> v2{v};
        for (auto i = 0; i < 4; ++i)
            for (auto x : v)
                v2.push_back(x);

        return make_pair(move(s2), move(v2));
    };

    auto sum1 = 0l;
    auto sum2 = 0l;

    string line;
    while (getline(cin, line)) {
        auto [s, v] = parse(line);
        sum1 += solve(s, v);

        auto [s2, v2] = expand(s, v);
        sum2 += solve(s2, v2);
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

