#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "asm_computer.hpp"

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
    : regs_before(regs_before)
    , instr(instr)
    , regs_after(regs_after)
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

const auto NumFunctions = static_cast<int>(sizeof functions / sizeof functions[0]);

auto count_samples_with_n_or_more_opcodes_behaviour(const std::vector<Sample>& samples, int n)
{
    auto count = 0;

    for (auto& sample : samples) {
        auto possible_opcodes = 0;

        for (const auto& [precond, op] : functions)
            if (precond(ssize(sample.regs_before), sample.instr)) {
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
    std::vector<std::vector<bool>> func2opcodes(NumFunctions,
        std::vector<bool>(NumFunctions, true));

    for (auto i = 0; i < NumFunctions; ++i) {
        for (auto& sample : samples) {
            if (!functions[i].precond(ssize(sample.regs_before), sample.instr)) {
                func2opcodes[i][sample.instr.opcode] = false;
                continue;
            }

            auto regs = sample.regs_before;
            functions[i].op(regs, sample.instr);
            if (regs != sample.regs_after) {
                func2opcodes[i][sample.instr.opcode] = false;
                continue;
            }
        }
    }

    std::array<int, NumFunctions> opcode2func;

    for (auto k = 0; k < NumFunctions; ++k)
        for (auto i = 0; i < NumFunctions; ++i)
            if (std::count(func2opcodes[i].begin(), func2opcodes[i].end(), true) == 1) {
                auto it = std::find(func2opcodes[i].begin(), func2opcodes[i].end(), true);
                auto opcode = std::distance(func2opcodes[i].begin(), it);

                opcode2func[opcode] = i;

                for (auto i = 0; i < NumFunctions; ++i)
                    func2opcodes[i][opcode] = false;

                break;
            }

    return opcode2func;
}

Regs run_instructions(Regs regs, const std::vector<Instr>& instructions,
    const std::array<int, NumFunctions>& opcode2func)
{
    for (const auto& instr : instructions)
        functions[opcode2func.at(instr.opcode)].op(regs, instr);

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

        samples.emplace_back(regs_before, instr, regs_after);
    }

    std::cout << count_samples_with_n_or_more_opcodes_behaviour(samples, 3) << "\n";

    std::string instr_line;
    while (getline(std::cin, instr_line)) {
        Instr instr;
        sscanf(instr_line.data(), "%d %d %d %d",
            &instr.opcode, &instr.a, &instr.b, &instr.c);

        instructions.push_back(instr);
    }

    auto opcode2func = translate_opcodes_to_functions(samples);
    auto regs = run_instructions({}, instructions, opcode2func);
    std::cout << regs[0] << "\n";

    return 0;
}

