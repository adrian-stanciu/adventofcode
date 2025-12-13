#include <iostream>
#include <string>
#include <vector>

#include "asm_computer.hpp"

using AsmC = AsmComputer<26>;

auto read_instructions()
{
    std::vector<std::string> instructions;

    std::string instruction;
    while (getline(std::cin, instruction))
        instructions.push_back(std::move(instruction));

    return instructions;
}

long run_instructions_until_first_rcv(const std::vector<std::string>& instructions)
{
    AsmC ac{instructions};

    ac.run();

    return *ac.get_last_output();
}

long run_instructions_in_parallel(const std::vector<std::string>& instructions)
{
    AsmC ac0{instructions};
    ac0.set_reg('p', 0);
    AsmC ac1{instructions};
    ac1.set_reg('p', 1);

    while (!ac0.halted() && !ac1.halted()) {
        ac0.run();
        while (!ac0.output_empty())
            ac1.append_input(*ac0.extract_first_output());

        ac1.run();
        while (!ac1.output_empty())
            ac0.append_input(*ac1.extract_first_output());

        if (ac0.input_empty()) // deadlock
            break;
    }

    return ac1.get_stats("snd");
}

int main()
{
    auto instructions = read_instructions();

    std::cout << run_instructions_until_first_rcv(instructions) << "\n";
    std::cout << run_instructions_in_parallel(instructions) << "\n";

    return 0;
}

