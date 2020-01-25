#include <iostream>
#include <string>
#include <vector>

#include "asm_computer.h"

using AsmC = AsmComputer<4>;

int main()
{
    std::vector<std::string> instructions;

    std::string line;
    while (getline(std::cin, line))
        instructions.push_back(std::move(line));

    AsmC ac{instructions};
    ac.run();
    std::cout << ac.get_reg('a') << "\n";

    ac.reset();
    ac.set_reg('c', 1);
    ac.run();
    std::cout << ac.get_reg('a') << "\n";

    return 0;
}

