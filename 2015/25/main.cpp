#include <iostream>
#include <regex>
#include <string>

auto read_position()
{
    static const std::regex re{R"(\s+Enter the code at row ([1-9][0-9]*), column ([1-9][0-9]*).)"};

    static const auto to_number = [] (const auto& s) {
        return strtol(s.data(), nullptr, 10);
    };

    std::string line;
    getline(std::cin, line);

    auto msg = line.substr(line.find_first_of('.') + 1);

    std::smatch matched;
    regex_match(msg, matched, re);

    return std::make_pair(to_number(matched[1].str()), to_number(matched[2].str()));
}

auto next(long n)
{
    return n * 252533 % 33554393;
}

auto next_after(long n, long iters)
{
    while (iters--)
        n = next(n);

    return n;
}

auto sum_1_to_n(long n)
{
    return n * (n + 1) / 2;
}

auto index_of_position(long row, long col)
{
    // find on which row the diagonal containing this position begins
    auto diag_row = row + col - 1;

    // count how many numbers are above that diagonal
    // (use the fact that the i-th diagonal contains i numbers)
    auto numbers_above_diag = sum_1_to_n(diag_row - 1);

    return numbers_above_diag + col;
}

int main()
{
    auto [row, col] = read_position();

    auto index = index_of_position(row, col);

    std::cout << next_after(20151125, index - 1) << "\n";

    return 0;
}

