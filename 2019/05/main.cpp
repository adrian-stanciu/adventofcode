#include <iostream>
#include <string>
#include <vector>

#include "int_computer.h"

auto run(const std::vector<long long>& prog, long long in)
{
    IntComputer ic(prog);

    ic.append_input(in);

    while (ic.run());

    return *ic.get_last_output();
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << run(prog, 1) << "\n";
    std::cout << run(prog, 5) << "\n";

    return 0;
}

