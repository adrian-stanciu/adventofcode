#include <bits/stdc++.h>

using namespace std;

const auto NumBits = 36;
const auto BitMask = (1l << NumBits) - 1;

struct Ctx {
    unordered_map<long, long> mem;
    long mask0;
    long mask1;
    string mask;

    auto sum() const
    {
        auto s = 0l;
        for (auto [_, val] : mem)
            s += val;
        return s;
    }
};

void set_mem(Ctx& ctx, long pos, const vector<int>& floating_bits, size_t idx, long val)
{
    if (idx == floating_bits.size()) {
        ctx.mem[pos] = val;
        return;
    }

    auto bit_idx = floating_bits[idx];

    auto pos0 = pos & (~(1l << bit_idx) & BitMask);
    set_mem(ctx, pos0, floating_bits, idx + 1, val);

    auto pos1 = pos | (1l << bit_idx);
    set_mem(ctx, pos1, floating_bits, idx + 1, val);
}

struct MaskInstr {
    string mask;

    void exec1(Ctx& ctx) const
    {
        ctx.mask0 = BitMask;
        ctx.mask1 = 0;

        auto bit_idx = NumBits;
        for (auto bit : mask) {
            --bit_idx;

            if (bit == '0')
                ctx.mask0 &= ~(1l << bit_idx) & BitMask;
            else if (bit == '1')
                ctx.mask1 |= 1l << bit_idx;
        }
    }

    void exec2(Ctx& ctx) const
    {
        ctx.mask = mask;
    }
};

struct MemInstr {
    long adr;
    long val;

    void exec1(Ctx& ctx) const
    {
        ctx.mem[adr] = (val & ctx.mask0) | ctx.mask1;
    }

    void exec2(Ctx& ctx) const
    {
        auto pos{adr};
        vector<int> floating_bits;

        auto bit_idx = NumBits;
        for (auto bit : ctx.mask) {
            --bit_idx;

            if (bit == '1')
                pos |= 1l << bit_idx;
            else if (bit == 'X')
                floating_bits.push_back(bit_idx);
        }

        set_mem(ctx, pos, floating_bits, 0, val);
    }
};

using Instr = variant<MaskInstr, MemInstr>;

auto solve1(const vector<Instr>& prog)
{
    Ctx ctx;

    for (const auto& instr : prog)
        visit([&] (const auto& _) { _.exec1(ctx); }, instr);

    return ctx.sum();
}

auto solve2(const vector<Instr>& prog)
{
    Ctx ctx;

    for (const auto& instr : prog)
        visit([&] (const auto& _) { _.exec2(ctx); }, instr);

    return ctx.sum();
}

int main()
{
    static const regex mask_r{R"(mask = ([01X]{36}))"};
    static const regex mem_r{R"(mem\[([0-9]+)\] = ([0-9]+))"};

    vector<Instr> prog;

    string line;
    while (getline(cin, line)) {
        smatch matched;
        if (regex_match(line, matched, mask_r)) {
            prog.push_back(MaskInstr{matched[1].str()});
        } else if (regex_match(line, matched, mem_r)) {
            auto adr = strtol(matched[1].str().data(), nullptr, 10);
            auto val = strtol(matched[2].str().data(), nullptr, 10);
            prog.push_back(MemInstr{adr, val});
        }
    }

    cout << solve1(prog) << '\n';
    cout << solve2(prog) << '\n';

    return 0;
}

