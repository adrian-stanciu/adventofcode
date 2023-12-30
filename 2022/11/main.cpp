#include <bits/stdc++.h>

#include "parser.h"

using namespace std;

struct Monkey {
    enum class Op {
        Add,
        Mul,
    };

    static inline auto Mod{1};

    int id{-1};
    queue<long> q;
    Op op;
    optional<int> operand; // it uses the parameter if the operand is empty
    int div;
    int dst_true;
    int dst_false;
    int cnt{0};

    [[nodiscard]] auto new_value1(auto old_val) const
    {
        using enum Op;
        switch (op) {
        case Add:
            return (old_val + (operand ? *operand : old_val)) / 3;
        case Mul:
            return (old_val * (operand ? *operand : old_val)) / 3;
        }
        assert(false);
    }

    [[nodiscard]] auto new_value2(auto old_val) const
    {
        using enum Op;
        switch (op) {
        case Add:
            return (old_val + (operand ? *operand : old_val)) % Mod;
        case Mul:
            return (old_val * (operand ? *operand : old_val)) % Mod;
        }
        assert(false);
    }
};

auto ltrim(string& s)
{
    s.erase(begin(s), find_if(begin(s), end(s), [](auto c) {
        return !isspace(c);
    }));
}
auto rtrim(string& s)
{
    s.erase(find_if(rbegin(s), rend(s), [](auto c) {
        return !isspace(c);
    }).base(), end(s));
}
auto trim(string& s)
{
    ltrim(s);
    rtrim(s);
}

auto read_monkey()
{
    Monkey m;

    string line;
    while (getline(cin, line)) {
        trim(line);
        if (line.empty())
            break;

        if (line.starts_with("Monkey")) {
            sscanf(line.data(), "Monkey %d:", &m.id);
        } else if (line.starts_with("Starting items")) {
            string_view sv{line};
            sv.remove_prefix(strlen("Starting items:"));
            for (auto item : split(sv, ','))
                m.q.push(strtol(item.data(), nullptr, 10));
        } else if (line.starts_with("Operation")) {
            char op;
            if (line.ends_with("old")) {
                sscanf(line.data(), "Operation: new = old %c old", &op);
            } else {
                int operand;
                sscanf(line.data(), "Operation: new = old %c %d", &op, &operand);
                m.operand = operand;
            }
            using enum Monkey::Op;
            switch (op) {
            case '+':
                m.op = Add;
                break;
            case '*':
                m.op = Mul;
                break;
            }
        } else if (line.starts_with("Test")) {
            sscanf(line.data(), "Test: divisible by %d", &m.div);
            Monkey::Mod = lcm(Monkey::Mod, m.div);
        } else if (line.starts_with("If true")) {
            sscanf(line.data(), "If true: throw to monkey %d", &m.dst_true);
        } else if (line.starts_with("If false")) {
            sscanf(line.data(), "If false: throw to monkey %d", &m.dst_false);
        }
    }

    return m;
}

auto solve(vector<Monkey> monkeys, int iters,
    const function<long(const Monkey&, long)>& new_value_fct)
{
    while (iters--)
        for (auto& m : monkeys)
            while (!m.q.empty()) {
                auto new_val = new_value_fct(m, m.q.front());
                m.q.pop();
                if (new_val % m.div == 0)
                    monkeys[m.dst_true].q.push(new_val);
                else
                    monkeys[m.dst_false].q.push(new_val);
                ++m.cnt;
            }

    vector<long> top;
    top.reserve(monkeys.size());
    for (const auto& m : monkeys)
        top.push_back(m.cnt);
    sort(rbegin(top), rend(top));
    return top[0] * top[1];
}

int main()
{
    vector<Monkey> monkeys;
    while (true) {
        auto m = read_monkey();
        if (m.id == -1)
            break;
        monkeys.push_back(move(m));
    }
    sort(begin(monkeys), end(monkeys), [](const auto& lhs, const auto& rhs) {
        return lhs.id < rhs.id;
    });

    cout << solve(monkeys, 20, [](const auto& monkey, auto old_val) {
        return monkey.new_value1(old_val);
    }) << '\n';
    cout << solve(monkeys, 10000, [](const auto& monkey, auto old_val) {
        return monkey.new_value2(old_val);
    }) << '\n';

    return 0;
}

