#include <bits/stdc++.h>

using namespace std;

int main()
{
    const array<string_view, 3> colors{"red", "green", "blue"};
    const array<int, 3> limits{12, 13, 14};

    auto is_valid = [&](const auto& counts) {
        for (auto i = 0; i < ssize(counts); ++i)
            if (counts[i] > limits[i])
                return false;

        return true;
    };

    auto str2num = [](auto sv, auto& n) {
        from_chars(sv.data(), sv.data() + sv.size(), n);
    };

    auto count_cube = [&](auto sv, auto& counts) {
        auto sep = sv.find(' ');
        auto cnt_sv = sv.substr(0, sep);
        auto col_sv = sv.substr(sep + 1);

        for (auto i = 0; i < ssize(colors); ++i)
            if (colors[i] == col_sv) {
                str2num(cnt_sv, counts[i]);
                break;
            }
    };

    auto count_round = [&](auto sv) {
        array<int, 3> counts{};

        while (true) {
            auto sep = sv.find(',');

            count_cube(sv.substr(1, sep - 1), counts);

            if (sep == sv.npos)
                break;

            sv.remove_prefix(sep + 1);
        }

        return counts;
    };

    auto count_game = [&](auto sv) {
        array<int, 3> counts{};

        while (true) {
            auto sep = sv.find(';');

            auto round_counts = count_round(sv.substr(0, sep));
            for (auto i = 0; i < ssize(counts); ++i)
                counts[i] = max(counts[i], round_counts[i]);

            if (sep == sv.npos)
                break;

            sv.remove_prefix(sep + 1);
        }

        return counts;
    };

    auto parse = [&](const auto& s) {
        string_view sv{s};

        sv.remove_prefix(strlen("Game"));

        auto sep = sv.find(':');

        auto id = 0;
        str2num(sv.substr(1, sep - 1), id);

        sv.remove_prefix(sep + 1);

        return make_pair(id, sv);
    };

    auto sum1 = 0;
    auto sum2 = 0;

    string s;
    while (getline(cin, s)) {
        auto [id, sv] = parse(s);

        auto counts = count_game(sv);

        if (is_valid(counts))
            sum1 += id;

        sum2 += accumulate(begin(counts), end(counts), 1, [](auto p, auto n) {
            return p * n;
        });
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

