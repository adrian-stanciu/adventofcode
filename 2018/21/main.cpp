#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "asm_computer.hpp"

using AsmC = AsmComputer<6>;

auto find_instr_with_src_reg(const std::vector<std::string>& instructions,
    const std::string& instr_name, long src_reg)
{
    for (auto ip = 0L; ip < static_cast<long>(instructions.size()); ++ip) {
        std::stringstream ss(instructions[ip]);

        std::string op;
        ss >> op;

        if (op == instr_name) {
            std::string src_reg1_str, src_reg2_str;
            ss >> src_reg1_str >> src_reg2_str;

            auto src_reg1 = strtol(src_reg1_str.data(), nullptr, 10);
            auto src_reg2 = strtol(src_reg2_str.data(), nullptr, 10);

            if (src_reg1 == src_reg)
                return std::make_pair(ip, src_reg2);
            else if (src_reg2 == src_reg)
                return std::make_pair(ip, src_reg1);
        }
    }

    return std::make_pair(-1L, -1L);
}

auto find_first_value_to_halt(AsmC& ac, long ip, long reg)
{
    while (ac.run_step())
        if (ac.get_ip() == ip)
            return ac.get_reg(reg);

    return -1L;
}

auto find_last_value_to_halt(AsmC& ac, long ip, long reg)
{
    std::unordered_set<long> values;

    auto last_value = -1L;

    while (ac.run_step()) {
        if (ac.get_ip() == ip) {
            auto value = ac.get_reg(reg);
            if (!values.emplace(value).second)
                break;
            last_value = value;
        }
    }

    return last_value;
}

int main()
{
    long ip_reg;
    std::vector<std::string> instructions;

    std::string line;
    getline(std::cin, line);
    sscanf(line.data(), "#ip %ld", &ip_reg);

    while (getline(std::cin, line))
        instructions.push_back(std::move(line));

    // assume that the asm code has only one "eqrr" instruction which compares
    // the provided value in register 0 with some computed value in other register;
    // find that instruction and the other register for monitoring
    auto [ip, other_src_reg] = find_instr_with_src_reg(instructions, "eqrr", 0);

    AsmC ac{ip_reg, instructions};
    std::cout << find_first_value_to_halt(ac, ip, other_src_reg) << "\n";

    ac.reset();
    std::cout << find_last_value_to_halt(ac, ip, other_src_reg) << "\n";

    return 0;
}

