#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "asm_computer.h"

using Regs = std::array<int, 4>;

struct Instr {
    int opcode;
    int a;
    int b;
    int c;
};

struct Sample {
    Regs regs_before;
    Instr instr;
    Regs regs_after;

    Sample(Regs regs_before, Instr instr, Regs regs_after)
    : regs_before(std::move(regs_before))
    , instr(std::move(instr))
    , regs_after(std::move(regs_after))
    {}
};

using Precond = std::function<bool(int, const Instr&)>;
using Operation = std::function<void(Regs&, const Instr&)>;

struct {
    Precond precond;
    Operation op;
} functions[] = {
    {addr_precond, addr},
    {addi_precond, addi},
    {mulr_precond, mulr},
    {muli_precond, muli},
    {banr_precond, banr},
    {bani_precond, bani},
    {borr_precond, borr},
    {bori_precond, bori},
    {setr_precond, setr},
    {seti_precond, seti},
    {gtir_precond, gtir},
    {gtri_precond, gtri},
    {gtrr_precond, gtrr},
    {eqir_precond, eqir},
    {eqri_precond, eqri},
    {eqrr_precond, eqrr},
};

auto count_samples_with_n_or_more_opcodes_behaviour(const std::vector<Sample>& samples, int n)
{
    auto count = 0;

    for (auto& sample : samples) {
        auto possible_opcodes = 0;

        for (const auto& [precond, op] : functions)
            if (precond(sample.regs_before.size(), sample.instr)) {
                auto regs = sample.regs_before;
                op(regs, sample.instr);
                if (regs == sample.regs_after)
                    ++possible_opcodes;
            }

        if (possible_opcodes >= n)
            ++count;
    }

    return count;
}

auto translate_opcodes_to_functions(const std::vector<Sample>& samples)
{
    std::unordered_map<int, int> opcode2func_table;
    std::array<bool, sizeof functions / sizeof functions[0]> available {};

    while (!std::all_of(available.begin(), available.end(), [](bool b) { return b; })) {
        for (auto& sample : samples) {
            auto possible_opcodes = 0;
            auto idx = 0;

            for (auto i = 0U; i < sizeof functions / sizeof functions[0]; ++i)
                if (!available[i] &&
                    functions[i].precond(sample.regs_before.size(), sample.instr)) {
                    auto regs = sample.regs_before;
                    functions[i].op(regs, sample.instr);
                    if (regs == sample.regs_after) {
                        ++possible_opcodes;
                        idx = i;
                    }
                }

            if (possible_opcodes == 1) {
                opcode2func_table[sample.instr.opcode] = idx;
                available[idx] = true;
            }
        }
    }

    return opcode2func_table;
}

Regs run_instructions(Regs regs, const std::vector<Instr>& instructions,
    const std::unordered_map<int, int>& opcode2func_table)
{
    for (const auto& instr : instructions)
        functions[opcode2func_table.at(instr.opcode)].op(regs, instr);

    return regs;
}

int main()
{
    std::vector<Sample> samples;
    std::vector<Instr> instructions;

    while (true) {
        std::string before_line;
        getline(std::cin, before_line);
        std::string instr_line;
        getline(std::cin, instr_line);

        if (before_line.empty() && instr_line.empty())
            break;

        std::string after_line;
        getline(std::cin, after_line);
        std::string empty_line;
        getline(std::cin, empty_line);

        Regs regs_before;
        sscanf(before_line.data(), "Before: [%d, %d, %d, %d]",
            &regs_before[0], &regs_before[1], &regs_before[2], &regs_before[3]);
        Instr instr;
        sscanf(instr_line.data(), "%d %d %d %d",
            &instr.opcode, &instr.a, &instr.b, &instr.c);
        Regs regs_after;
        sscanf(after_line.data(), "After: [%d, %d, %d, %d]",
            &regs_after[0], &regs_after[1], &regs_after[2], &regs_after[3]);

        samples.emplace_back(std::move(regs_before), std::move(instr), std::move(regs_after));
    }

    std::cout << count_samples_with_n_or_more_opcodes_behaviour(samples, 3) << "\n";

    std::string instr_line;
    while (getline(std::cin, instr_line)) {
        Instr instr;
        sscanf(instr_line.data(), "%d %d %d %d",
            &instr.opcode, &instr.a, &instr.b, &instr.c);

        instructions.push_back(std::move(instr));
    }

    auto opcode2func_table = translate_opcodes_to_functions(samples);
    auto regs = run_instructions({}, instructions, opcode2func_table);
    std::cout << regs[0] << "\n";

    return 0;
}

