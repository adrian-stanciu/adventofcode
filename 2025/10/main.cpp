#include <bits/stdc++.h>

#include <z3++.h>

#include "parser.hpp"

using namespace std;

static constexpr auto MaxSubsetSize{16};

auto solve1(const bitset<MaxSubsetSize>& expected, const vector<vector<int>>& buttons)
{
    auto min_cnt = numeric_limits<int>::max();

    auto num_buttons = ssize(buttons);
    auto num_subsets = pow(2, num_buttons);

    for (auto i = 0u; i < num_subsets; ++i) {
        bitset<MaxSubsetSize> subset{i};

        bitset<MaxSubsetSize> current;
        for (auto j = 0u; j < num_buttons; ++j)
            if (subset[j]) {
                for (auto bit : buttons[j])
                    current.flip(bit);
            }

        if (current == expected)
            min_cnt = min(min_cnt, static_cast<int>(subset.count()));
    }

    return min_cnt;
}

auto solve2(const vector<int>& expected, const vector<vector<int>>& buttons,
    z3::context& z3_ctx)
{
    z3::optimize z3_opt{z3_ctx};

    vector<z3::expr> vars;
    vars.reserve(ssize(buttons));
    for (auto i = 0; i < ssize(buttons); ++i)
        vars.emplace_back(z3_ctx.int_const(to_string(i).data()));

    for (const auto& var : vars)
        z3_opt.add(var >= z3_ctx.int_val(0));

    for (auto i = 0; i < ssize(expected); ++i) {
        auto expr = z3_ctx.int_val(0);
        for (auto j = 0; j < ssize(buttons); ++j)
            if (count(begin(buttons[j]), end(buttons[j]), i) == 1)
                expr = expr + vars[j];
        z3_opt.add(expr == z3_ctx.int_val(expected[i]));
    }

    auto sum = z3_ctx.int_val(0);
    for (const auto& var : vars)
        sum = sum + var;
    z3_opt.minimize(sum);

    auto ret = z3_opt.check();
    assert(ret == z3::sat);

    return z3_opt.get_model().eval(sum, true).get_numeral_int();
}

int main()
{
    auto trim = [](auto& sv) {
        sv.remove_prefix(1);
        sv.remove_suffix(1);
        return sv;
    };

    auto sum1 = 0l;
    auto sum2 = 0l;

    z3::context z3_ctx;

    string s;
    while (getline(cin, s)) {
        auto groups = split(string_view{s}, ' ');

        bitset<MaxSubsetSize> expected_bits;
        auto expected_bits_sv = trim(groups[0]);
        assert(ssize(expected_bits_sv) <= MaxSubsetSize);
        for (auto i = 0; i < ssize(expected_bits_sv); ++i)
            if (expected_bits_sv[i] == '#')
                expected_bits.set(i);

        vector<vector<int>> buttons;
        for (auto i = 1; i < ssize(groups) - 1; ++i) {
            vector<int> button;
            for (auto action : split(trim(groups[i]), ','))
                button.push_back(str2num(action));
            buttons.push_back(move(button));
        }
        assert(ssize(buttons) <= MaxSubsetSize);

        vector<int> expected_nums;
        for (auto num_sv : split(trim(groups.back()), ','))
            expected_nums.push_back(str2num(num_sv));

        sum1 += solve1(expected_bits, buttons);
        sum2 += solve2(expected_nums, buttons, z3_ctx);
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

