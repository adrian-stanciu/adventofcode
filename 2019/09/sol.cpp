#include <iostream>
#include <string>
#include <vector>

#include "int_computer.hpp"

auto run_in_mode(const std::vector<long long>& prog, long long mode)
{
    IntComputer ic(prog);

    ic.append_input(mode);

    while (ic.run());

    return *ic.get_last_output();
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << run_in_mode(prog, 1) << "\n";
    std::cout << run_in_mode(prog, 2) << "\n";

    return 0;
}

