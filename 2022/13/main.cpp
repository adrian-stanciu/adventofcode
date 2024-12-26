#include <bits/stdc++.h>

using namespace std;

auto split_list(string_view sv)
{
    vector<string_view> v;

    auto from = 0;
    for (auto level = 0, to = 0; to < ssize(sv); ++to)
        if (sv[to] == '[') {
            ++level;
        } else if (sv[to] == ']') {
            --level;
        } else if (sv[to] == ',' && level == 0) {
            v.push_back(sv.substr(from, to - from));
            from = to + 1;
        }
    if (from < ssize(sv))
        v.push_back(sv.substr(from));

    return v;
}

int cmp(string_view lhs, string_view rhs);

auto cmp_val(long a, long b)
{
    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    else
        return 0;
}

int cmp_list(string_view lhs, string_view rhs)
{
    auto lhs_list = split_list(lhs);
    auto rhs_list = split_list(rhs);

    for (auto i = 0; i < ssize(lhs_list) && i < ssize(rhs_list); ++i) {
        auto res = cmp(lhs_list[i], rhs_list[i]);
        if (res)
            return res;
    }

    return cmp_val(ssize(lhs_list), ssize(rhs_list));
}

int cmp(string_view lhs, string_view rhs)
{
    auto make_list = [](auto sv) {
        string s;
        s.push_back('[');
        s.append(sv);
        s.push_back(']');
        return s;
    };

    auto lhs_is_digit = isdigit(lhs[0]);
    auto rhs_is_digit = isdigit(rhs[0]);

    if (lhs_is_digit && rhs_is_digit)
        return cmp_val(strtol(lhs.data(), nullptr, 10), strtol(rhs.data(), nullptr, 10));
    else if (!lhs_is_digit && !rhs_is_digit)
        return cmp_list(lhs.substr(1, lhs.size() - 2), rhs.substr(1, rhs.size() - 2));
    else if (lhs_is_digit)
        return cmp(make_list(lhs), rhs);
    else if (rhs_is_digit)
        return cmp(lhs, make_list(rhs));

    assert(false);
}

auto solve1(const vector<string>& v)
{
    auto sum = 0;
    for (auto i = 0; i < ssize(v); i += 2)
        if (cmp(v[i], v[i + 1]) < 0)
            sum += i / 2 + 1;
    return sum;
}

auto solve2(vector<string>& v)
{
    const string p1{"[[2]]"};
    v.push_back(p1);
    const string p2{"[[6]]"};
    v.push_back(p2);

    sort(begin(v), end(v), [](const auto& lhs, const auto& rhs) {
        return cmp(lhs, rhs) < 0;
    });

    auto idx1 = std::distance(begin(v), find(begin(v), end(v), p1)) + 1;
    auto idx2 = std::distance(begin(v), find(begin(v), end(v), p2)) + 1;
    return idx1 * idx2;
}

int main()
{
    vector<string> v;

    string line;
    while (getline(cin, line))
        if (!line.empty())
            v.push_back(move(line));

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

