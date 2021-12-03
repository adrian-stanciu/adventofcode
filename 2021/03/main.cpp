#include <bits/stdc++.h>

using namespace std;

auto bin2dec(const string& s)
{
    auto n = 0lu;

    for (auto c : s) {
        n *= 2;
        n += c == '1';
    }

    return n;
}

auto solve1(const vector<string>& v)
{
    auto n = v.size();

    string bin;
    for (auto i = 0lu, len = v[0].size(); i < len; ++i) {
        auto zeroes = 0lu;
        for (const auto& s : v)
            zeroes += s[i] == '0';

        auto ones = n - zeroes;

        if (zeroes > ones)
            bin.push_back('0');
        else if (ones > zeroes)
            bin.push_back('1');
    }

    auto flipped_bin{bin};
    transform(begin(flipped_bin), end(flipped_bin), begin(flipped_bin),
        [](auto c) { return c == '0' ? '1' : '0'; });

    return bin2dec(bin) * bin2dec(flipped_bin);
}

auto solve2_helper(vector<string> v, const function<char(size_t, size_t)>& compute_target)
{
    for (auto i = 0lu, len = v[0].size(); i < len; ++i) {
        auto n = v.size();
        if (n == 1)
            break;

        auto zeroes = 0lu;
        for (const auto& s : v)
            zeroes += s[i] == '0';

        auto ones = n - zeroes;

        auto target = compute_target(zeroes, ones);

        vector<string> w;
        for (const auto& x : v)
            if (x[i] == target)
                w.push_back(x);

        v.swap(w);
    }

    return bin2dec(v[0]);
}

auto solve2(const vector<string>& v)
{
    auto prefer_most_common = [](auto zeroes, auto ones) {
        return ones >= zeroes ? '1' : '0';
    };
    auto prefer_least_common = [](auto zeroes, auto ones) {
        return zeroes <= ones ? '0' : '1';
    };

    return solve2_helper(v, prefer_most_common) * solve2_helper(v, prefer_least_common);
}

int main()
{
    vector<string> v;

    string s;
    while (cin >> s)
        v.push_back(s);

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

