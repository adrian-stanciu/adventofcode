#include <bits/stdc++.h>

using namespace std;

enum class Op {
    Forward,
    Up,
    Down,
};

struct Cmd {
    Op op;
    long val;

    Cmd(Op op, long val) : op{op}, val{val} {}
};

auto solve1(const vector<Cmd>& cmds)
{
    auto h = 0l;
    auto d = 0l;

    for (const auto& [op, val] : cmds)
        switch (op) {
        case Op::Forward:
            h += val;
            break;
        case Op::Up:
            d -= val;
            break;
        case Op::Down:
            d += val;
            break;
        }

    return h * d;
}

auto solve2(const vector<Cmd>& cmds)
{
    auto h = 0l;
    auto d = 0l;
    auto a = 0l;

    for (const auto& [op, val] : cmds)
        switch (op) {
        case Op::Forward:
            h += val;
            d += a * val;
            break;
        case Op::Up:
            a -= val;
            break;
        case Op::Down:
            a += val;
            break;
        }

    return h * d;
}

int main()
{
    vector<Cmd> cmds;

    string line;
    while (getline(cin, line)) {
        auto sep_pos = line.find(' ');

        auto op_s = line.substr(0, sep_pos);
        auto val_s = line.substr(sep_pos + 1);

        auto op = op_s == "up" ? Op::Up :
                  op_s == "down" ? Op::Down : Op::Forward;
        auto val = strtol(val_s.data(), nullptr, 10);

        cmds.emplace_back(op, val);
    }

    cout << solve1(cmds) << '\n';
    cout << solve2(cmds) << '\n';

    return 0;
}

