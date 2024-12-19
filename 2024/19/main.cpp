#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto trim(string_view sv)
{
    while (isspace(sv.front()))
        sv.remove_prefix(1);
    while (isspace(sv.back()))
        sv.remove_suffix(1);
    return sv;
}

auto solve(const vector<string_view>& v, string_view sv,
    unordered_map<string_view, long>& cache)
{
    if (sv.empty())
        return 1l;

    if (auto it = cache.find(sv); it != end(cache))
        return it->second;

    auto cnt = 0l;

    for (auto prefix : v)
        if (sv.starts_with(prefix))
            cnt += solve(v, sv.substr(prefix.size()), cache);

    cache.emplace(sv, cnt);

    return cnt;
}

int main()
{
    vector<string_view> v;
    string str;
    getline(cin, str);
    for (auto sv : split(str, ','))
        v.push_back(trim(sv));

    auto cnt1 = 0;
    auto cnt2 = 0l;

    string s;
    getline(cin, s); // empty line
    while (getline(cin, s)) {
        const string_view sv{s};

        unordered_map<string_view, long> cache;
        auto cnt = solve(v, sv, cache);

        cnt1 += cnt != 0;
        cnt2 += cnt;
    }

    cout << cnt1 << '\n';
    cout << cnt2 << '\n';

    return 0;
}

