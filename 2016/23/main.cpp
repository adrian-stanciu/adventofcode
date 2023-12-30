#include <iostream>
#include <string>
#include <vector>

#include "asm_computer.hpp"

using AsmC = AsmComputer<4>;

int main()
{
    std::vector<std::string> instructions;

    std::string line;
    while (getline(std::cin, line))
        instructions.push_back(std::move(line));

    AsmC ac{instructions};

    ac.set_reg('a', 7);
    ac.run_with_tgl();
    std::cout << ac.get_reg('a') << "\n";

    auto factorial = [] (auto n) {
        auto f = n;

        while (--n)
            f *= n;

        return f;
    };

    // the asm code computes the sum between
    // the factorial of the value in register 'a' and some constant
    std::cout << factorial(12) + ac.get_reg('a') - factorial(7) << "\n";

    return 0;
}

