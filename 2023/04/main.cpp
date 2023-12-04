#include <bits/stdc++.h>

using namespace std;

void squeeze_repeated(string& s, char ch)
{
    s.erase(unique(begin(s), end(s), [&](auto lhs, auto rhs) {
        return lhs == ch && rhs == ch;
    }), end(s));
}

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

auto parse(string& s)
{
    squeeze_repeated(s, ' ');

    string_view sv{s};

    sv.remove_prefix(strlen("Card"));
    auto sep = sv.find(':');
    auto id = str2num(sv.substr(1, sep - 1));
    sv.remove_prefix(sep + 1);

    sep = sv.find('|');
    auto win_nums = split(sv.substr(0, sep), ' ');
    auto my_nums = split(sv.substr(sep + 1), ' ');

    return make_tuple(id, win_nums, my_nums);
}

int main()
{
    auto sum = 0;

    map<int, int> freq;

    string s;
    while (getline(cin, s)) {
        auto [id, win_nums, my_nums] = parse(s);

        set<int> winners;
        for (auto sv : win_nums)
            winners.insert(str2num(sv));

        auto matched = 0;
        for (auto sv : my_nums)
            if (winners.contains(str2num(sv)))
                ++matched;

        if (matched > 0)
            sum += 1 << (matched - 1);

        auto cnt = ++freq[id];
        for (auto i = id + 1; i <= id + matched; ++i)
            freq[i] += cnt;
    }

    cout << sum << '\n';

    cout << accumulate(begin(freq), end(freq), 0, [](auto acc, const auto& elem) {
        return acc + elem.second;
    }) << '\n';

    return 0;
}

