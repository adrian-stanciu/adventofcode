#include <iostream>
#include <string>
#include <vector>

#include "asm_computer.hpp"

using AsmC = AsmComputer<6>;

auto sum_of_divisors(long n)
{
    auto sum = 0L;

    for (auto i = 1L; i <= n; ++i)
        if (n % i == 0)
            sum += i;

    return sum;
}

auto optimize_and_run(AsmC& ac, long result)
{
    // the asm code computes the sum of the divisors of some number N

    // run the small input until the accumulator (register 0) has a value greater than 1
    // (to be sure that only one register holds the number N)
    ac.reset();
    ac.run_until([&] () { return ac.get_reg(0) > 1; });

    // find which register holds the number N
    auto reg_idx = 0;
    while (reg_idx < ac.num_regs()) {
        if (sum_of_divisors(ac.get_reg(reg_idx)) == result)
            break;
        ++reg_idx;
    }

    // find the number N for the large input based on the fact that
    // the asm code computes N before resetting the accumulator
    ac.reset();
    ac.set_reg(0, 1);
    ac.run_until([&] () { return ac.get_reg(0) == 0; });

    return sum_of_divisors(ac.get_reg(reg_idx));
}

int main()
{
    int ip_reg;
    std::vector<std::string> instructions;

    std::string line;
    getline(std::cin, line);
    sscanf(line.data(), "#ip %d", &ip_reg);

    while (getline(std::cin, line))
        instructions.push_back(std::move(line));

    AsmC ac{ip_reg, instructions};
    ac.run();
    auto result = ac.get_reg(0);

    std::cout << result << "\n";
    std::cout << optimize_and_run(ac, result) << "\n";

    return 0;
}

