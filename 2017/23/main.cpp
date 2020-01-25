#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "asm_computer.h"

using AsmC = AsmComputer<8>;

auto read_instructions()
{
    std::vector<std::string> instructions;

    std::string instruction;
    while (getline(std::cin, instruction))
        instructions.push_back(std::move(instruction));

    return instructions;
}

bool is_prime(long n)
{
    for (long i = 2; i <= static_cast<long>(sqrt(n)); ++i)
        if (n % i == 0)
            return false;
    return true;
}

long count_primes(long first, long last, long inc)
{
    long count = 0;
    for (long i = first; i <= last; i += inc)
        if (!is_prime(i))
            ++count;
    return count;
}

long optimize_and_run(AsmC& ac, const std::vector<std::string>& instructions)
{
    // the asm code iterates between two values using a certain step
    // and count the prime numbers;
    // the range is computed in the first part and
    // saved in 'b' (the first candidate) and 'c' (the last candidate) registers;
    // the second part is represented by a loop in which:
    // 1. a primality test is performed for each candidate (the one from register 'b')
    // 2. register 'b' is incremented with a certain step in order to obtain the next candidate

    // run until the 'b' and 'c' registers have their values computed
    // (register 'b' must have a value smaller than the one in register 'c')
    ac.run_until([&] () { return ac.get_reg('b') < ac.get_reg('c'); });

    // search the instruction which increments register 'b' and find the step
    // (it assumes that only a single "add"/"sub" instruction changes the register 'b'
    // after the range is computed)
    for (auto i = ac.get_ip(); i < instructions.size(); ++i) {
        std::stringstream ss(instructions[i]);

        std::string op;
        ss >> op;

        if (op == "add" || op == "sub") {
            std::string x, y;
            ss >> x >> y;

            if (x[0] == 'b')
                return count_primes(ac.get_reg('b'), ac.get_reg('c'),
                    strtol(y.data(), nullptr, 10) * (op == "sub" ? -1 : 1));
        }
    }

    return -1;
}

int main()
{
    auto instructions = read_instructions();

    AsmC ac{instructions};

    ac.run();
    std::cout << ac.get_stats("mul") << "\n";

    ac.reset();
    ac.set_reg('a', 1);
    std::cout << optimize_and_run(ac, instructions) << "\n";

    return 0;
}

