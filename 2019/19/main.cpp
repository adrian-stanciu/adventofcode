#include <iostream>
#include <string>
#include <vector>

#include "int_computer.hpp"

auto read_point_status(const std::vector<long long>& prog, int x, int y)
{
    IntComputer ic(prog);

    ic.append_input(x);
    ic.append_input(y);

    ic.run();

    return *ic.get_last_output();
}

auto count_points_in_beam(const std::vector<long long>& prog, int x_limit, int y_limit)
{
    auto cnt = 0;

    for (auto x = 0; x < x_limit; ++x)
        for (auto y = 0; y < y_limit; ++y)
            if (read_point_status(prog, x, y) == 1)
                ++cnt;

    return cnt;
}

auto find_first_point_in_beam(const std::vector<long long>& prog, int x)
{
    auto y = 0;

    while (read_point_status(prog, x, y) != 1)
        ++y;

    return y;
}

bool is_square_in_beam(const std::vector<long long>& prog, int x, int y, int edge)
{
    return read_point_status(prog, x - edge + 1, y + edge - 1) == 1 &&
        read_point_status(prog, x - edge + 1, y) == 1;
}

auto solve_square_in_beam(const std::vector<long long>& prog, int edge)
{
    auto x = edge - 1;
    auto y = find_first_point_in_beam(prog, x);

    while (!is_square_in_beam(prog, x, y, edge)) {
        ++x;
        while (read_point_status(prog, x, y) != 1)
            ++y;
    }

    return 10000 * (x - edge + 1) + y;
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << count_points_in_beam(prog, 50, 50) << "\n";

    std::cout << solve_square_in_beam(prog, 100) << "\n";

    return 0;
}

