#include <bits/stdc++.h>

using namespace std;

int main()
{
    static constexpr auto Mul{"mul("sv};
    static constexpr auto Do{"do()"sv};
    static constexpr auto DoNot{"don't()"sv};

    auto extract_char = [](auto& sv, auto ch) {
        if (sv.front() != ch)
            return false;
        sv.remove_prefix(1);
        return true;
    };

    auto extract_num = [](auto& sv) -> optional<int> {
        auto n = 0;
        auto cnt = 0;

        while (!sv.empty() && isdigit(sv.front())) {
            ++cnt;
            if (cnt > 3)
                return nullopt;

            n *= 10;
            n += sv.front() - '0';

            sv.remove_prefix(1);
        }

        if (cnt == 0)
            return nullopt;

        return n;
    };

    auto sol1 = 0l;
    auto sol2 = 0l;

    auto enabled = true;

    string s;
    while (getline(cin, s)) {
        string_view sv{s};

        while (!sv.empty())
            if (sv.starts_with(Mul)) {
                sv.remove_prefix(Mul.size());

                auto x = extract_num(sv);
                if (!x)
                    continue;

                if (!extract_char(sv, ','))
                    continue;

                auto y = extract_num(sv);
                if (!y)
                    continue;

                if (!extract_char(sv, ')'))
                    continue;

                sol1 += *x * *y;
                if (enabled)
                    sol2 += *x * *y;
            } else if (sv.starts_with(Do)) {
                sv.remove_prefix(Do.size());
                enabled = true;
            } else if (sv.starts_with(DoNot)) {
                sv.remove_prefix(DoNot.size());
                enabled = false;
            } else {
                sv.remove_prefix(1);
            }
    }

    cout << sol1 << '\n';
    cout << sol2 << '\n';

    return 0;
}

