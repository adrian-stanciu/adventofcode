#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

static constexpr auto MaxSubsetSize{16};
static constexpr auto Inf{numeric_limits<int>::max()};

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

auto solve_rec(int n, int m, const vector<vector<int>>& mat, const vector<int>& vec,
    const vector<int>& pivots, const vector<int>& free_vars_limits,
    vector<int>& free_vars_values, int& min_cnt)
{
    auto fixed = m - count(begin(pivots), end(pivots), -1);

    auto is_free = [&](auto var) {
        return pivots[var] == -1;
    };

    auto is_unresolved = [&](auto var) {
        return is_free(var) && free_vars_values[var] == -1;
    };

    auto count_unresolved_vars = [&]() {
        auto cnt = 0;
        for (auto var = 0; var < m; ++var)
            if (is_unresolved(var))
                ++cnt;
        return cnt;
    };

    auto find_unresolved_var_with_smallest_domain = [&]() {
        auto unresolved_var = -1;
        for (auto lim = Inf, var = 0; var < m; ++var)
            if (is_unresolved(var) && free_vars_limits[var] < lim) {
                unresolved_var = var;
                lim = free_vars_limits[var];
            }
        return unresolved_var;
    };

    auto eval_row_except = [&](auto r, auto skip_var) -> optional<int> {
        auto rhs = vec[r];
        for (auto var = 0; var < m; ++var) {
            if (var == skip_var || !is_free(var) || mat[r][var] == 0)
                continue;
            if (free_vars_values[var] == -1)
                return nullopt;
            rhs -= mat[r][var] * free_vars_values[var];
        }
        return rhs;
    };

    auto is_valid_equality = [&](auto r) {
        if (auto rhs = eval_row_except(r, -1); rhs)
            return *rhs == 0;
        else
            return true; // unresolved, cannot invalidate yet
    };

    auto are_equalities_valid = [&]() {
        for (auto r = fixed; r < n; ++r)
            if (!is_valid_equality(r))
                return false;
        return true;
    };

    auto compute_inequalities_sum = [&]() -> optional<int> {
        auto sum = 0;
        for (auto r = 0; r < fixed; ++r)
            if (auto rhs = *eval_row_except(r, -1); rhs < 0)
                return nullopt;
            else
                sum += rhs;
        return sum;
    };

    auto compute_total_sum = [&]() -> optional<int> {
        auto sum = compute_inequalities_sum();
        if (!sum)
            return nullopt;
        for (auto var = 0; var < m; ++var)
            if (is_free(var))
                *sum += free_vars_values[var];
        return sum;
    };

    auto try_resolve = [&](auto var) -> optional<int> {
        for (auto r = fixed; r < n; ++r) {
            if (mat[r][var] == 0)
                continue;
            auto rhs = eval_row_except(r, var);
            if (!rhs)
                continue;
            if (*rhs % mat[r][var] != 0)
                return -1;
            auto val = *rhs / mat[r][var];
            if (val < 0)
                return -1;
            return val;
        }
        return nullopt;
    };

    if (count_unresolved_vars() == 1) {
        auto var = find_unresolved_var_with_smallest_domain();

        auto val = try_resolve(var);
        if (val) {
            if (*val == -1)
                return;

            free_vars_values[var] = *val;

            if (!are_equalities_valid()) {
                free_vars_values[var] = -1;
                return;
            }

            if (auto total = compute_total_sum(); total)
                min_cnt = min(min_cnt, *total);

            free_vars_values[var] = -1;
            return;
        }

        auto lo = 0;
        auto hi = free_vars_limits[var];
        for (auto r = 0; r < fixed; ++r) {
            if (mat[r][var] == 0)
                continue;

            auto rhs = eval_row_except(r, var);

            if (*rhs >= 0 && mat[r][var] > 0)
                hi = min(hi, *rhs / mat[r][var]);
            else if (*rhs < 0 && mat[r][var] < 0)
                lo = max(lo, *rhs / mat[r][var]);
        }
        if (lo > hi)
            return;

        for (auto val = lo; val <= hi; ++val) {
            free_vars_values[var] = val;

            if (!are_equalities_valid())
                continue;

            if (auto total = compute_total_sum(); total)
                min_cnt = min(min_cnt, *total);
        }

        free_vars_values[var] = -1;
        return;
    }

    for (auto var = 0; var < m; ++var) {
        if (!is_unresolved(var))
            continue;

        auto val = try_resolve(var);
        if (!val)
            continue;

        if (*val == -1)
            return;

        free_vars_values[var] = *val;
        solve_rec(n, m, mat, vec, pivots, free_vars_limits, free_vars_values, min_cnt);
        free_vars_values[var] = -1;
        return;
    }

    auto var = find_unresolved_var_with_smallest_domain();
    for (auto val = 0; val <= free_vars_limits[var]; ++val) {
        free_vars_values[var] = val;
        solve_rec(n, m, mat, vec, pivots, free_vars_limits, free_vars_values, min_cnt);
    }
    free_vars_values[var] = -1;
}

// Gauss–Jordan elimination combined with branch-and-bound search on free variables
auto solve2(const vector<int>& expected, const vector<vector<int>>& buttons)
{
    auto n = ssize(expected);
    auto m = ssize(buttons);

    vector<vector<int>> mat(n, vector<int>(m, 0));
    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (find(begin(buttons[c]), end(buttons[c]), r) != end(buttons[c]))
                mat[r][c] = 1;

    auto vec{expected};

    auto is_valid_pivot = [&](auto r, auto c) {
        for (auto j = 0; j < m; ++j)
            if (mat[r][j] % mat[r][c] != 0)
                return false;
        return vec[r] % mat[r][c] == 0;
    };

    vector<int> pivots(m, -1);

    for (auto pivot = 0, c = 0; c < m; ++c) {
        for (auto r = pivot; r < n; ++r)
            if (mat[r][c] != 0 && is_valid_pivot(r, c)) {
                swap(mat[pivot], mat[r]);
                swap(vec[pivot], vec[r]);
                pivots[c] = pivot;
                break;
            }

        if (pivots[c] == -1)
            continue;

        auto divider = mat[pivot][c];
        for (auto c = 0; c < m; ++c)
            mat[pivot][c] /= divider;
        vec[pivot] /= divider;

        for (auto r = pivot + 1; r < n; ++r)
            if (mat[r][c] != 0) {
                auto multiplier = -mat[r][c];
                for (auto c = 0; c < m; ++c)
                    mat[r][c] += multiplier * mat[pivot][c];
                vec[r] += multiplier * vec[pivot];
            }

        ++pivot;
    }

    for (auto c = m - 1; c >= 0; --c)
        if (pivots[c] != -1)
            for (auto r = pivots[c] - 1; r >= 0; --r)
                if (mat[r][c] != 0) {
                    auto multiplier = -mat[r][c];
                    for (auto j = 0; j < m; ++j)
                        mat[r][j] += multiplier * mat[pivots[c]][j];
                    vec[r] += multiplier * vec[pivots[c]];
                }

    if (count(begin(pivots), end(pivots), -1) == 0)
        return accumulate(begin(vec), end(vec), 0);

    vector<int> free_vars_limits(m, Inf);
    for (auto c = 0; c < m; ++c) {
        if (pivots[c] != -1)
            continue;
        for (auto action : buttons[c])
            free_vars_limits[c] = min(free_vars_limits[c], expected[action]);
    }

    vector<int> free_vars_values(m, -1);
    auto min_cnt = Inf;
    solve_rec(n, m, mat, vec, pivots, free_vars_limits, free_vars_values, min_cnt);

    return min_cnt;
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
        sum2 += solve2(expected_nums, buttons);
    }

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

