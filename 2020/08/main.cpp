#include <bits/stdc++.h>

using namespace std;

struct Ctx {
    long acc{0};
    size_t ip{0};
};

struct Instr {
    string op;
    long arg{0};

    Instr(const string& s)
    {
        stringstream ss{s};
        ss >> op >> arg;
    }

    void exec(Ctx& ctx) const
    {
        if (op == "acc") {
            ctx.acc += arg;
            ++ctx.ip;
        } else if (op == "jmp") {
            ctx.ip += arg;
        } else if (op == "nop") {
            ++ctx.ip;
        }
    }
};

auto run(const vector<Instr>& prog)
{
    Ctx ctx;
    vector<bool> done(prog.size(), false);

    while (ctx.ip < prog.size()) {
        if (done[ctx.ip])
            return make_pair(false, ctx.acc);

        done[ctx.ip] = true;
        prog[ctx.ip].exec(ctx);
    }

    return make_pair(true, ctx.acc);
}

void fix(vector<Instr>& prog)
{
    for (auto& instr : prog) {
        auto op = instr.op;
        if (op == "jmp" || op == "nop") {
            instr.op = op == "jmp" ? "nop" : "jmp";
            auto [halted, acc] = run(prog);
            instr.op = op;

            if (halted) {
                cout << acc << '\n';
                break;
            }
        }
    }
}

int main()
{
    vector<Instr> prog;

    string line;
    while (getline(cin, line))
        prog.emplace_back(line);

    cout << run(prog).second << '\n';

    fix(prog);

    return 0;
}

