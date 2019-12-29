#include <iostream>
#include <string>
#include <vector>

#include "int_computer.h"

auto jump_by_walking(const std::vector<long long>& prog)
{
    IntComputer ic(prog);

    // the robot can jump over 3 tiles;
    // the strategy is to jump now if any of the next 3 tiles is a hole
    // and the 4th tile is safe (were the robot will land)

    const std::vector<std::string> script {
        "NOT A J\n",    // jump if A is hole
        "NOT B T\n",    // B is hole
        "OR T J\n",     // jump if A or B are holes
        "NOT C T\n",    // C is hole
        "OR T J\n",     // jump if A or B or C are holes
        "AND D J\n",    // jump if A or B or C are holes and D is safe
        "WALK\n",
    };

    for (const auto& instr : script)
        for (auto c : instr)
            ic.append_input(c);

    while (ic.run());

    return *ic.get_last_output();
}

auto jump_by_running(const std::vector<long long>& prog)
{
    IntComputer ic(prog);

    // the robot can jump over 3 tiles;
    // the strategy is to jump now if any of the next 3 tiles is a hole
    // and the 4th tile is safe (were the robot will land),
    // except when both the 5th and 8th tiles are holes;
    // the 5th tile is the next tile after landing on the 4th tile and
    // the 8th tile is the landing tile if a second jump is performed on the 4th tile
    // (in order to jump over the hole in the 5th tile);
    // if both 5th and 8th tiles are holes then we defer to jump;
    // otherwise is safe to jump now

    const std::vector<std::string> script {
        "NOT A J\n",    // jump if A is hole
        "NOT B T\n",    // B is hole
        "OR T J\n",     // jump if A or B are holes
        "NOT C T\n",    // C is hole
        "OR T J\n",     // jump if A or B or C are holes
        "AND D J\n",    // jump if A or B or C are holes and D is safe
        "NOT E T\n",    // E is hole
        "AND H T\n",    // E is hole and H is safe
        "OR E T\n",     // E is safe or E is hole and H is safe
        "AND T J\n",    // jump if A or B or C are holes and D is safe and E is safe or E is hole and H is safe
        "RUN\n",
    };

    for (const auto& instr : script)
        for (auto c : instr)
            ic.append_input(c);

    while (ic.run());

    return *ic.get_last_output();
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << jump_by_walking(prog) << "\n";
    std::cout << jump_by_running(prog) << "\n";

    return 0;
}

