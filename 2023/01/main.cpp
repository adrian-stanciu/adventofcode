#include <bits/stdc++.h>

using namespace std;

auto solve1(const vector<string>& v)
{
    const char *digits{"0123456789"};

    auto sum = 0;

    for (const auto& s : v)
        sum += 10 * (s[s.find_first_of(digits)] - '0') + s[s.find_last_of(digits)] - '0';

    return sum;
}

auto solve2(vector<string>& v)
{
    const char *digits{"0123456789"};

    const array<string, 10> digits_with_letters{
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };

    auto first_digit = [&](const auto& s) {
        optional<int> digit;
        optional<size_t> digit_pos;

        auto pos = s.find_first_of(digits);
        if (pos != s.npos) {
            digit = s[pos] - '0';
            digit_pos = pos;
        }

        for (auto d = 0; d < 10; ++d) {
            auto pos = s.find(digits_with_letters[d]);
            if (pos != s.npos && (!digit_pos || pos < *digit_pos)) {
                digit = d;
                digit_pos = pos;
            }
        }

        return *digit;
    };

    auto last_digit = [&](const auto& s) {
        optional<int> digit;
        optional<size_t> digit_pos;

        auto pos = s.find_last_of(digits);
        if (pos != s.npos) {
            digit = s[pos] - '0';
            digit_pos = pos;
        }

        for (auto d = 0; d < 10; ++d) {
            auto pos = s.rfind(digits_with_letters[d]);
            if (pos != s.npos && (!digit_pos || pos > *digit_pos)) {
                digit = d;
                digit_pos = pos;
            }
        }

        return *digit;
    };

    auto sum = 0;

    for (const auto& s : v)
        sum += 10 * first_digit(s) + last_digit(s);

    return sum;
}

int main()
{
    vector<string> v;

    string s;
    while (cin >> s)
        v.push_back(move(s));

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

