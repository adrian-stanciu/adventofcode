#include <bits/stdc++.h>

using namespace std;

// the program consists of 14 calls to a function which modifies a global variable (out)
// based on 3 known parameters (a, b, c) and an user-provided non-zero digit (in);
// initially, out[0] = 0;
// the goal is to obtain out[14] == 0 by choosing the input value for each call;

// for each i = 1..14 do
//      if out[i - 1] % 26 + b[i] != in[i] then
//          out[i] = 26 * (out[i - 1] / a[i]) + c[i] + in[i]
//      else
//          out[i] = out[i - 1] / a[i]

// analyzing each call parameters, we can observe that:
// * when a[i] == 1, the condition is always true because b[i] > 9 (max in[i]);
//   this happens 7 times (in half of the calls); a[i] == 26 in the other half of the calls
// * at any moment, the number of a[i] == 1 calls is gte to the number of a[i] == 26 calls
// * c[i] < 17 => c[i] + in[i] < 26

// in conclusion:
// * out is a number in base 26
// * when a[i] == 1, out will gain a digit
// * in order to obtain out[14] == 0, we must take the else branch when a[i] == 26,
//   forcing out to loss a digit

auto solve(int n, const vector<int>& a, const vector<int>& b, const vector<int>& c)
{
    struct Result {
        long max_val;
        long min_val;
    };

    map<long, Result> out2res;
    out2res[0] = Result{0, 0};

    for (auto i = 0; i < n; ++i) {
        map<long, Result> new_out2res;

        auto update = [&](auto new_out, auto max_val, auto min_val) {
            if (new_out2res.count(new_out)) {
                new_out2res[new_out].max_val = max(new_out2res[new_out].max_val, max_val);
                new_out2res[new_out].min_val = min(new_out2res[new_out].min_val, min_val);
            } else {
                new_out2res[new_out] = Result{max_val, min_val};
            }
        };

        if (a[i] == 1) {
            for (const auto& [out, res] : out2res)
                for (auto in = 1; in <= 9; ++in)
                    update(26 * out + c[i] + in, 10 * res.max_val + in, 10 * res.min_val + in);
        } else {
            for (const auto& [out, res] : out2res) {
                auto in = out % 26 + b[i];
                if (1 <= in && in <= 9)
                    update(out / 26, 10 * res.max_val + in, 10 * res.min_val + in);
            }
        }

        out2res.swap(new_out2res);
    }

    assert(out2res.count(0));

    cout << out2res[0].max_val << '\n';
    cout << out2res[0].min_val << '\n';
}

auto solve(const vector<string>& prog)
{
    static const auto NumCalls = 14;

    const auto prog_sz = static_cast<int>(prog.size());
    const auto fct_sz = prog_sz / NumCalls;

    auto second_operand = [](const string& instr) {
        static const auto SecondOperandOffset = 6;

        return instr.data() + SecondOperandOffset;
    };

    auto to_num = [](const char *p) {
        return strtol(p, nullptr, 10);
    };

    vector<int> a;
    for (auto i = 4; i < prog_sz; i += fct_sz)
        a.push_back(to_num(second_operand(prog[i])));

    vector<int> b;
    for (auto i = 5; i < prog_sz; i += fct_sz)
        b.push_back(to_num(second_operand(prog[i])));

    vector<int> c;
    for (auto i = 15; i < prog_sz; i += fct_sz)
        c.push_back(to_num(second_operand(prog[i])));

    solve(NumCalls, a, b, c);
}

int main()
{
    vector<string> prog;

    string line;
    while (getline(cin, line))
        prog.push_back(move(line));

    solve(prog);

    return 0;
}

