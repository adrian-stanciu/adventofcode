#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto str2num(string_view sv)
{
    auto n = 0;
    from_chars(sv.data(), sv.data() + sv.size(), n);
    return n;
}

static constexpr array RegNames{'x', 'm', 'a', 's'};

auto reg_name2idx(char reg_name)
{
    return distance(begin(RegNames), find(begin(RegNames), end(RegNames), reg_name));
}

using Regs1 = array<int, ssize(RegNames)>;
using Regs2 = array<pair<int, int>, ssize(RegNames)>;

struct Cond {
    int reg_idx;
    char op;
    int val;

    Cond(int reg_idx, char op, int val) : reg_idx{reg_idx}, op{op}, val{val} {}

    [[nodiscard]] bool is_matching(const Regs1& regs) const
    {
        return op == '<' ? regs[reg_idx] < val : regs[reg_idx] > val;
    }
};

struct Rule {
    string dst;
    optional<Cond> cond;

    Rule(string_view dst) : dst{dst} {}
    Rule(string_view dst, int reg_idx, char op, int val)
    : dst{dst}, cond{in_place, reg_idx, op, val}
    {}

    [[nodiscard]] bool is_matching(const Regs1& regs) const
    {
        return cond ? cond->is_matching(regs) : true;
    }
};

using RulesMap = unordered_map<string, vector<Rule>>;

auto parse_rules()
{
    RulesMap rules_map;

    while (true) {
        string s;
        getline(cin, s);
        if (s.empty())
            break;

        string_view sv{s};

        auto open_curly_bracket_pos = sv.find('{');
        string rule_name{sv.substr(0, open_curly_bracket_pos)};
        sv.remove_prefix(open_curly_bracket_pos + 1);
        sv.remove_suffix(1);

        vector<Rule> rules;
        for (auto rule_sv : split(sv, ',')) {
            if (auto colon_pos = rule_sv.find(':'); colon_pos == rule_sv.npos) {
                rules.emplace_back(rule_sv);
            } else {
                auto cond_sv = rule_sv.substr(0, colon_pos);
                auto reg_idx = reg_name2idx(cond_sv[0]);
                auto op = cond_sv[1];
                auto val = str2num(cond_sv.substr(2));
                rules.emplace_back(rule_sv.substr(colon_pos + 1), reg_idx, op, val);
            }
        }

        rules_map.emplace(move(rule_name), move(rules));
    }

    return rules_map;
}

auto solve1(const RulesMap& rules_map)
{
    auto is_accepted = [&](const auto& regs) {
        string rule_name{"in"};

        while (true)
            for (const auto& rule : rules_map.at(rule_name))
                if (rule.is_matching(regs)) {
                    if (rule.dst == "A") {
                        return true;
                    } else if (rule.dst == "R") {
                        return false;
                    } else {
                        rule_name = rule.dst;
                        break;
                    }
                }
    };

    auto sum = 0l;

    string s;
    while (getline(cin, s)) {
        string_view sv{s};
        sv.remove_prefix(1);
        sv.remove_suffix(1);

        Regs1 regs;
        for (auto reg : split(sv, ','))
            regs[reg_name2idx(reg[0])] = str2num(reg.substr(2));

        if (is_accepted(regs))
            sum += accumulate(begin(regs), end(regs), 0l);
    }

    return sum;
}

void dfs(const RulesMap& rules_map, const string& rule_name, Regs2 regs, long& sum)
{
    for (const auto& rule : rules_map.at(rule_name)) {
        auto dst_regs{regs};

        if (rule.cond) {
            if (rule.cond->op == '<') {
                dst_regs[rule.cond->reg_idx].second = rule.cond->val - 1;
                regs[rule.cond->reg_idx].first = rule.cond->val;
            } else { // '>'
                dst_regs[rule.cond->reg_idx].first = rule.cond->val + 1;
                regs[rule.cond->reg_idx].second = rule.cond->val;
            }
        }

        if (rule.dst == "A") {
            sum += accumulate(begin(dst_regs), end(dst_regs), 1l,
                [](auto acc, auto reg) { return acc * (reg.second - reg.first + 1); });
            if (!rule.cond)
                break;
        } else if (rule.dst == "R") {
            if (!rule.cond)
                break;
        } else {
            dfs(rules_map, rule.dst, dst_regs, sum);
        }
    }
}

auto solve2(const RulesMap& rules_map)
{
    Regs2 regs;
    fill(begin(regs), end(regs), make_pair(1, 4000));

    auto sum = 0l;
    dfs(rules_map, "in", regs, sum);
    return sum;
}

int main()
{
    auto rules_map = parse_rules();

    cout << solve1(rules_map) << '\n';
    cout << solve2(rules_map) << '\n';

    return 0;
}

