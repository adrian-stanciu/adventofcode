#include <iostream>
#include <string>
#include <vector>

#include "int_computer.h"

auto run(const std::vector<long long>& prog, long long noun, long long verb)
{
    IntComputer ic(prog);

    ic.set_mem(1, noun);
    ic.set_mem(2, verb);

    while (ic.run());

    return ic.get_mem(0);
}

auto find_noun_and_verb(const std::vector<long long>& prog, long long expected)
{
    for (auto noun = 0; noun < 100; ++noun)
        for (auto verb = 0; verb < 100; ++verb)
            if (run(prog, noun, verb) == expected)
                return 100 * noun + verb;

    return -1;
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << run(prog, 12, 2) << "\n";
    std::cout << find_noun_and_verb(prog, 19690720) << "\n";

    return 0;
}

