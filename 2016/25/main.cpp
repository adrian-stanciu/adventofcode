#include <iostream>
#include <string>
#include <vector>

#include "asm_computer.hpp"

using AsmC = AsmComputer<4>;

bool check_pattern(AsmC& ac)
{
    static constexpr auto iters_limit = 1000;

    auto iters = 0;
    auto expected = 0;

    while (true) {
        ac.run_step();

        if (!ac.output_empty()) {
            if (*ac.extract_first_output() != expected)
                return false;

            if (++iters == iters_limit)
                break;

            expected = 1 - expected;
        }
    }

    return true;
}

auto explore(const std::vector<std::string>& instructions)
{
    AsmC ac{instructions};

    auto i = 0;

    while (true) {
        ac.set_reg('a', ++i);

        if (check_pattern(ac))
            return i;

        ac.reset();
    }
}

int main()
{
    std::vector<std::string> instructions;

    std::string line;
    while (getline(std::cin, line))
        instructions.push_back(std::move(line));

    std::cout << explore(instructions) << "\n";

    return 0;
}

