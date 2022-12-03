#include <bits/stdc++.h>

using namespace std;

auto score(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 1;
    else
        return c - 'A' + 26 + 1;
}

auto common_letter(string_view sv1, string_view sv2)
{
    for (auto c : sv1)
        if (sv2.find(c) != sv2.npos)
            return c;
    assert(false);
}

auto solve1(const vector<string>& v)
{
    return accumulate(begin(v), end(v), 0, [](auto acc, const auto& s) {
        return acc + score(common_letter(
            string_view(s.data(), s.size() / 2),
            string_view(s.data() + s.size() / 2)));
    });
}

auto common_letter(string_view sv1, string_view sv2, string_view sv3)
{
    for (auto c : sv1)
        if (sv2.find(c) != sv2.npos && sv3.find(c) != sv3.npos)
            return c;
    assert(false);
}

auto solve2(const vector<string>& v)
{
    auto sum = 0;
    for (auto i = 0; i < ssize(v); i += 3)
        sum += score(common_letter(v[i], v[i + 1], v[i + 2]));
    return sum;
}

int main()
{
    vector<string> v;

    string line;
    while (getline(cin, line))
        v.push_back(move(line));

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

