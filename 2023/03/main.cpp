#include <bits/stdc++.h>

using namespace std;

auto str2num(string_view sv)
{
    auto n = 0l;
    from_chars(sv.data(), sv.data() + sv.size(), n);
    return n;
}

auto get_neighbours(const vector<string>& m, int i, int j, int len)
{
    vector<pair<int, int>> neighbours;

    if (i > 0) {
        if (j > 0)
            neighbours.emplace_back(i - 1, j - 1);
        for (auto k = j; k < j + len; ++k)
            neighbours.emplace_back(i - 1, k);
        if (j + len < ssize(m[i - 1]))
            neighbours.emplace_back(i - 1, j + len);
    }

    if (j > 0)
        neighbours.emplace_back(i, j - 1);
    if (j + len < ssize(m[i]))
        neighbours.emplace_back(i, j + len);

    if (i < ssize(m) - 1) {
        if (j > 0)
            neighbours.emplace_back(i + 1, j - 1);
        for (auto k = j; k < j + len; ++k)
            neighbours.emplace_back(i + 1, k);
        if (j + len < ssize(m[i + 1]))
            neighbours.emplace_back(i + 1, j + len);
    }

    return neighbours;
}

auto solve1(const vector<string>& m)
{
    auto is_valid = [&](auto i, auto j, auto len) {
        auto neighbours = get_neighbours(m, i, j, len);

        return any_of(begin(neighbours), end(neighbours), [&](auto pos) {
            auto ch = m[pos.first][pos.second];
            return ch != '.' && !isdigit(ch);
        });
    };

    auto sum = 0l;

    for (auto i = 0; i < ssize(m); ++i) {
        string n;

        for (auto j = 0; j < ssize(m[i]); ++j)
            if (isdigit(m[i][j])) {
                n.push_back(m[i][j]);
            } else if (!n.empty()) {
                if (is_valid(i, j - ssize(n), ssize(n)))
                    sum += str2num(n);
                n.clear();
            }

        if (!n.empty() && is_valid(i, ssize(m[i]) - ssize(n), ssize(n)))
            sum += str2num(n);
    }

    return sum;
}

auto solve2(const vector<string>& m)
{
    map<pair<int, int>, vector<long>> pos2nums;

    auto process = [&](auto i, auto j, auto len, auto n) {
        for (auto pos : get_neighbours(m, i, j, len))
            if (m[pos.first][pos.second] == '*')
                pos2nums[pos].push_back(n);
    };

    for (auto i = 0; i < ssize(m); ++i) {
        string n;

        for (auto j = 0; j < ssize(m[i]); ++j)
            if (isdigit(m[i][j])) {
                n.push_back(m[i][j]);
            } else if (!n.empty()) {
                process(i, j - ssize(n), ssize(n), str2num(n));
                n.clear();
            }

        if (!n.empty())
            process(i, ssize(m[i]) - ssize(n), ssize(n), str2num(n));
    }

    auto sum = 0l;

    for (auto [_, nums] : pos2nums)
        if (ssize(nums) == 2)
            sum += nums[0] * nums[1];

    return sum;
}

int main()
{
    vector<string> m;

    string s;
    while (cin >> s)
        m.push_back(move(s));

    cout << solve1(m) << '\n';
    cout << solve2(m) << '\n';

    return 0;
}

