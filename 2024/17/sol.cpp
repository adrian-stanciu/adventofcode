#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

using Regs = array<long, 3>;
using Prog = vector<long>;

auto run(Regs regs, const Prog& prog)
{
    auto combo = [&](auto operand) {
        assert(operand != 7);
        return operand < 4 ? operand : regs[operand - 4];
    };

    auto power = [](auto n, auto p) {
        return static_cast<long>(pow(n, p));
    };

    vector<long> out;

    auto pc = 0l;
    while (pc < ssize(prog)) {
        switch (prog[pc]) {
        case 0:
            regs[0] = regs[0] / power(2, combo(prog[pc + 1]));
            break;
        case 1:
            regs[1] = regs[1] ^ prog[pc + 1];
            break;
        case 2:
            regs[1] = combo(prog[pc + 1]) % 8;
            break;
        case 3:
            if (regs[0]) {
                pc = prog[pc + 1];
                continue;
            }
            break;
        case 4:
            regs[1] = regs[1] ^ regs[2];
            break;
        case 5:
            out.push_back(combo(prog[pc + 1]) % 8);
            break;
        case 6:
            regs[1] = regs[0] / power(2, combo(prog[pc + 1]));
            break;
        case 7:
            regs[2] = regs[0] / power(2, combo(prog[pc + 1]));
            break;
        }

        pc += 2;
    }

    return out;
}

auto solve1(Regs regs, const Prog& prog)
{
    auto out = run(regs, prog);

    for (auto i = 0; i < ssize(out) - 1; ++i)
        cout << out[i] << ',';
    if (!out.empty())
        cout << out.back();
    cout << '\n';
}

auto solve2(Regs regs, const Prog& prog)
{
    // the program halts when regs[0] = 0
    // in each loop, regs[0] is divided by 8
    // => at the start of the last loop: 1 <= reg[0] <= 7
    // regs[1] and regs[2] are recomputed in each loop and do not retain their
    // previous values
    // => run a loop for each 1 <= regs[0] <= 7 and keep the values which produce
    // the expected output (the prog's last value); the candidate values for
    // regs[0] in the penultimate loop are in [8 * regs[0], 8 * (regs[0] + 1))
    // range for all regs[0] which produce the expected output in the last loop
    // => run the loops in reverse order until the full prog is obtained

    vector<long> v{0l};

    for (auto expected_out : ranges::reverse_view(prog)) {
        vector<long> w;

        for (auto n : v)
            for (auto m = max(1l, 8 * n); m < 8 * (n + 1); ++m) {
                regs[0] = m;
                auto out = run(regs, prog);
                if (!out.empty() && out[0] == expected_out)
                    w.push_back(m);
            }

        v.swap(w);
    }

    assert(!v.empty());
    return *min_element(begin(v), end(v));
}

int main()
{
    auto read_reg = []() {
        string s;
        getline(cin, s);
        return str2num(split(s, ' ')[2]);
    };

    Regs regs;
    for (auto& reg : regs)
        reg = read_reg();

    auto prog = []() {
        string s;
        getline(cin, s); // empty line
        getline(cin, s);
        Prog prog;
        for (auto str : split(split(s, ' ')[1], ','))
            prog.push_back(str2num(str));
        return prog;
    }();

    solve1(regs, prog);
    cout << solve2(regs, prog) << '\n';

    return 0;
}

