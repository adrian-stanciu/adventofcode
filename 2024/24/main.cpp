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

enum class Operator {
    And,
    Or,
    Xor
};

struct Gate {
    string l_in;
    string r_in;
    Operator op;

    Gate(string_view l_in, string_view op_sv, string_view r_in)
    : l_in{l_in}
    , r_in{r_in}
    {
        if (op_sv == "AND")
            op = Operator::And;
        else if (op_sv == "OR")
            op = Operator::Or;
        else if (op_sv == "XOR")
            op = Operator::Xor;
        else
            assert(false);
    }

    void swap(Gate& other) noexcept
    {
        l_in.swap(other.l_in);
        r_in.swap(other.r_in);
        op = exchange(other.op, op);
    }
};

using ValueMap = unordered_map<string, bool>;
using GateMap = unordered_map<string, Gate>;

auto build_name(char ch, int n)
{
    string s;
    s.push_back(ch);
    if (n < 10)
        s.push_back('0');
    s.append(to_string(n));
    return s;
}

optional<bool> compute_rec(ValueMap& values, const GateMap& gates, const string& out,
    unordered_set<string>& visited)
{
    auto it = values.find(out);
    if (it != end(values))
        return it->second;

    if (visited.contains(out))
        return nullopt;
    visited.emplace(out);

    const auto& gate = gates.at(out);

    auto l_bit = compute_rec(values, gates, gate.l_in, visited);
    if (!l_bit)
        return nullopt;
    auto r_bit = compute_rec(values, gates, gate.r_in, visited);
    if (!r_bit)
        return nullopt;

    bool bit = [&]() {
        switch (gate.op) {
        case Operator::And:
            return *l_bit && *r_bit;
        case Operator::Or:
            return *l_bit || *r_bit;
        case Operator::Xor:
            return *l_bit != *r_bit;
        }
        assert(false);
    }();

    visited.erase(out);

    values.emplace(out, bit);

    return bit;
}

optional<long> compute(ValueMap& values, const GateMap& gates)
{
    vector<bool> bits;

    for (auto i = 0;; ++i) {
        auto out = build_name('z', i);

        if (!gates.contains(out))
            break;

        unordered_set<string> visited;
        auto bit = compute_rec(values, gates, out, visited);
        if (!bit)
            return nullopt;

        bits.push_back(*bit);
    }

    reverse(begin(bits), end(bits));

    auto n = 0l;
    for (auto bit : bits) {
        n *= 2;
        n += bit;
    }

    return n;
}

auto set_values(ValueMap& values, int num_in_gates, char ch, long n)
{
    vector<bool> bits;
    while (n) {
        bits.push_back(n % 2);
        n /= 2;
    }

    for (auto i = 0; i < ssize(bits); ++i)
        values.emplace(build_name(ch, i), bits[i]);

    for (auto i = ssize(bits); i < num_in_gates; ++i)
        values.emplace(build_name(ch, i), false);
}

auto compute(const GateMap& gates, int num_in_gates, long x, long y)
{
    ValueMap values;
    set_values(values, num_in_gates, 'x', x);
    set_values(values, num_in_gates, 'y', y);
    return compute(values, gates);
}

auto find_new_gates(const GateMap& gates, int k)
{
    queue<string> q;

    auto find_gates_excluding = [&](const auto& excluded) {
        set<string> included;

        while (!q.empty()) {
            auto s = q.front();
            q.pop();

            if (!excluded.contains(s))
                included.emplace(s);

            auto& gate = gates.at(s);

            if (gate.l_in[0] != 'x' && gate.l_in[0] != 'y')
                q.emplace(gate.l_in);
            if (gate.r_in[0] != 'x' && gate.r_in[0] != 'y')
                q.emplace(gate.r_in);
        }

        return included;
    };

    for (auto i = 0; i < k; ++i)
        q.emplace(build_name('z', i));
    auto old_gates = find_gates_excluding(set<string>{});

    q.emplace(build_name('z', k));
    return find_gates_excluding(old_gates);
}

auto swap(GateMap& gates, const string& l_out, const string& r_out)
{
    swap(gates.at(l_out), gates.at(r_out));
}

auto find_swapped(GateMap& gates, int num_out_gates)
{
    auto check = [&](auto a, auto b) {
        auto res = compute(gates, num_out_gates - 1, a, b);
        return res && *res == a + b;
    };
    auto is_correct = [&](auto i) {
        auto p = static_cast<long>(pow(2, i));
        // empirically found checks that detect a swap and validate a fix
        return check(0, p) && check(0, 2 * p) && check(p, p) && check(2 * p - 1, p - 1);
    };

    vector<string> sol;

    auto fix = [&](auto i) {
        vector<pair<string, string>> fixes;

        // empirically found that swaps occur between gates of adjacent bits
        for (const auto& out1 : find_new_gates(gates, i))
            for (const auto& out2 : find_new_gates(gates, i + 1)) {
                swap(gates, out1, out2);
                if (is_correct(i))
                    fixes.emplace_back(out1, out2);
                swap(gates, out1, out2);
            }

        if (fixes.empty())
            return;

        assert(fixes.size() == 1);
        swap(gates, fixes[0].first, fixes[0].second);
        sol.push_back(fixes[0].first);
        sol.push_back(fixes[0].second);
    };

    for (auto i = 0; i < num_out_gates - 1; ++i)
        if (!is_correct(i))
            fix(i);

    return sol;
}

int main()
{
    ValueMap values;

    string s;
    while (getline(cin, s)) {
        if (s.empty())
            break;
        auto parts = split(s, ':');
        values.emplace(parts[0], str2num(trim(parts[1])));
    }

    GateMap gates;
    auto num_out_gates = 0;

    while (getline(cin, s)) {
        auto parts = split(s, ' ');
        gates.emplace(parts[4], Gate{parts[0], parts[1], parts[2]});
        if (parts[4][0] == 'z')
            ++num_out_gates;
    }

    cout << *compute(values, gates) << '\n';

    auto sol = find_swapped(gates, num_out_gates);
    sort(begin(sol), end(sol));
    for (auto i = 0; i < ssize(sol) - 1; ++i)
        cout << sol[i] << ',';
    cout << sol.back() << '\n';

    return 0;
}

