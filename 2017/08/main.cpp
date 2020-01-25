#include <algorithm>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

auto max_value_stored = std::numeric_limits<long>::min();

struct Instruction {
    enum class Op {
        Inc,
        Dec,
        Nop,
    };

    enum class CondOp {
        Eq,
        Ne,
        Gt,
        Lt,
        Ge,
        Le,
        Nop,
    };

    Op op;
    std::string op_reg;
    long op_param;

    CondOp cop;
    std::string cop_reg;
    long cop_param;

    Instruction(Op op, std::string&& op_reg, long op_param,
        CondOp cop, std::string&& cop_reg, long cop_param)
    : op(op)
    , op_reg(std::move(op_reg))
    , op_param(op_param)
    , cop(cop)
    , cop_reg(std::move(cop_reg))
    , cop_param(cop_param)
    {}
};

Instruction::Op translate_op(const std::string& op_str)
{
    if (op_str.compare("inc") == 0)
        return Instruction::Op::Inc;
    if (op_str.compare("dec") == 0)
        return Instruction::Op::Dec;

    return Instruction::Op::Nop;
}

Instruction::CondOp translate_cop(const std::string& cop_str)
{
    if (cop_str.compare("==") == 0)
        return Instruction::CondOp::Eq;
    if (cop_str.compare("!=") == 0)
        return Instruction::CondOp::Ne;
    if (cop_str.compare(">") == 0)
        return Instruction::CondOp::Gt;
    if (cop_str.compare("<") == 0)
        return Instruction::CondOp::Lt;
    if (cop_str.compare(">=") == 0)
        return Instruction::CondOp::Ge;
    if (cop_str.compare("<=") == 0)
        return Instruction::CondOp::Le;

    return Instruction::CondOp::Nop;
}

auto read_instructions()
{
    auto parse = [] (const std::string& s) {
        std::stringstream ss(s);
        std::string reg;
        std::string code;
        long param;
        ss >> reg >> code >> param;
        return std::make_tuple(reg, code, param);
    };

    std::vector<Instruction> instructions;

    std::string line;
    while (getline(std::cin, line)) {
        static const std::regex re{"(.*) if (.*)"};
        std::smatch matched;
        regex_match(line, matched, re);

        auto [op_reg, op_code, op_param] = parse(matched[1].str());
        auto [cop_reg, cop_code, cop_param] = parse(matched[2].str());

        Instruction::Op op = translate_op(op_code);
        Instruction::CondOp cop = translate_cop(cop_code);

        instructions.emplace_back(op, std::move(op_reg), op_param,
            cop, std::move(cop_reg), cop_param);
    }

    return instructions;
}

long load_reg(const std::string& reg, const std::unordered_map<std::string, long>& regs)
{
    auto it = regs.find(reg);
    if (it != regs.end())
        return it->second;
    else
        return 0;
}

void store_reg(const std::string& reg, long val, std::unordered_map<std::string, long>& regs)
{
    auto it = regs.find(reg);
    if (it != regs.end())
        it->second = val;
    else
        regs[reg] = val;

    if (max_value_stored < val)
        max_value_stored = val;
}

bool test_instruction(const Instruction& i, const std::unordered_map<std::string, long>& regs)
{
    long cop_reg_val = load_reg(i.cop_reg, regs);

    switch (i.cop) {
    case Instruction::CondOp::Eq:
        return cop_reg_val == i.cop_param;
    case Instruction::CondOp::Ne:
        return cop_reg_val != i.cop_param;
    case Instruction::CondOp::Gt:
        return cop_reg_val > i.cop_param;
    case Instruction::CondOp::Lt:
        return cop_reg_val < i.cop_param;
    case Instruction::CondOp::Ge:
        return cop_reg_val >= i.cop_param;
    case Instruction::CondOp::Le:
        return cop_reg_val <= i.cop_param;
    case Instruction::CondOp::Nop:
        return false;
    }

    return false;
}

bool run_instruction(const Instruction& i, std::unordered_map<std::string, long>& regs)
{
    long op_reg_val = load_reg(i.op_reg, regs);

    switch (i.op) {
    case Instruction::Op::Inc:
        op_reg_val += i.op_param;
        break;
    case Instruction::Op::Dec:
        op_reg_val -= i.op_param;
        break;
    case Instruction::Op::Nop:
        return false;
    }

    store_reg(i.op_reg, op_reg_val, regs);

    return true;
}

auto interpret(const std::vector<Instruction>& instructions)
{
    std::unordered_map<std::string, long> regs;

    for (auto& i :  instructions)
        if (test_instruction(i, regs))
            run_instruction(i, regs);

    return regs;
}

long max_value(const std::vector<Instruction>& instructions)
{
    long max = std::numeric_limits<long>::min();

    auto regs = interpret(instructions);

    for (auto& reg : regs)
        if (reg.second > max)
            max = reg.second;

    return max;
}

int main()
{
    auto instructions = read_instructions();

    std::cout << max_value(instructions) << "\n";
    std::cout << max_value_stored << "\n";

    return 0;
}

