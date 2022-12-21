#include <bits/stdc++.h>

using namespace std;

auto trim(string_view sv)
{
    while (isspace(sv.front()))
        sv.remove_prefix(1);
    while (isspace(sv.back()))
        sv.remove_suffix(1);
    return sv;
}

struct Result {
    optional<long> val;
    char op;
    string lhs;
    string rhs;
};

auto solve1(const map<string, Result>& res_map, const string& curr)
{
    const auto& res = res_map.at(curr);

    if (res.val)
        return *res.val;

    auto lhs = solve1(res_map, res.lhs);
    auto rhs = solve1(res_map, res.rhs);

    switch (res.op) {
    case '+':
        return lhs + rhs;
    case '-':
        return lhs - rhs;
    case '*':
        return lhs * rhs;
    case '/':
        return lhs / rhs;
    }
    assert(false);
}

auto precompute(map<string, Result>& res_map, const string& curr)
{
    auto& res = res_map.at(curr);

    if (curr == "humn") {
        res.val = {};
        return make_pair(0l, false);
    }

    if (res.val)
        return make_pair(*res.val, true);

    auto [lhs, lhs_avail] = precompute(res_map, res.lhs);
    auto [rhs, rhs_avail] = precompute(res_map, res.rhs);

    if (lhs_avail && rhs_avail) {
        switch (res.op) {
        case '+':
            res.val = lhs + rhs;
            break;
        case '-':
            res.val = lhs - rhs;
            break;
        case '*':
            res.val = lhs * rhs;
            break;
        case '/':
            res.val = lhs / rhs;
            break;
        }
        return make_pair(*res.val, true);
    }

    return make_pair(0l, false);
}

auto solve2(const map<string, Result>& res_map, long target, const string& curr)
{
    if (curr == "humn")
        return target;

    const auto& res = res_map.at(curr);

    auto lhs_val = res_map.at(res.lhs).val;
    auto rhs_val = res_map.at(res.rhs).val;

    if (lhs_val) {
        auto new_target = 0l;
        switch (res.op) {
        case '+':
            new_target = target - *lhs_val;
            break;
        case '-':
            new_target = *lhs_val - target;
            break;
        case '*':
            new_target = target / *lhs_val;
            break;
        case '/':
            new_target = *lhs_val / target;
            break;
        }
        return solve2(res_map, new_target, res.rhs);
    } else {
        auto new_target = 0l;
        switch (res.op) {
        case '+':
            new_target = target - *rhs_val;
            break;
        case '-':
            new_target = target + *rhs_val;
            break;
        case '*':
            new_target = target / *rhs_val;
            break;
        case '/':
            new_target = target * *rhs_val;
            break;
        }
        return solve2(res_map, new_target, res.lhs);
    }
}

auto solve2(map<string, Result>& res_map)
{
    precompute(res_map, "root");

    const auto& root = res_map.at("root");
    if (root.val)
        return *root.val;

    auto lhs_val = res_map.at(root.lhs).val;
    auto rhs_val = res_map.at(root.rhs).val;

    if (lhs_val)
        return solve2(res_map, *lhs_val, root.rhs);
    else
        return solve2(res_map, *rhs_val, root.lhs);
}

int main()
{
    map<string, Result> res_map;

    string line;
    while (getline(cin, line)) {
        string_view sv{line};

        string name{sv.substr(0, sv.find(':'))};
        sv.remove_prefix(name.size() + 1);

        Result res;
        auto op_pos = sv.find_first_of("+-*/");
        if (op_pos == sv.npos) {
            res.val = strtol(sv.data(), nullptr, 10);
        } else {
            res.lhs = trim(sv.substr(0, op_pos));
            res.op = sv[op_pos];
            res.rhs = trim(sv.substr(op_pos + 1));
        }

        res_map[move(name)] = res;
    }

    cout << solve1(res_map, "root") << '\n';
    cout << solve2(res_map) << '\n';

    return 0;
}

