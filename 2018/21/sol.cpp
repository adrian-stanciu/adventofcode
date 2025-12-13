#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

// an optimized program obtained through reverse engineering
void solve(const std::vector<std::string>& instructions)
{
    auto read_val_from_instr = [&](auto i, auto n) -> int {
        auto pos = 0;
        while (n--)
            pos = instructions[i].find_first_of(' ', pos) + 1;

        return strtol(instructions[i].data() + pos, nullptr, 10);
    };

    auto const1 = read_val_from_instr(7, 1);
    auto const2 = read_val_from_instr(11, 2);

    auto value = 0;
    auto first = true;
    std::unordered_set<int> values;
    auto last_value = -1;

    while (true) {
        auto tmp = value | 0x10000;
        value = const1;

        while (true) {
            value += tmp & 0xff;
            value &= 0xffffff;
            value *= const2;
            value &= 0xffffff;

            if (tmp < 0x100)
                break;
            tmp /= 0x100;
        }

        if (first) {
            std::cout << value << '\n';
            first = false;
        }

        if (!values.emplace(value).second) {
            std::cout << last_value << '\n';
            break;
        }
        last_value = value;
    }
}

int main()
{
    std::vector<std::string> instructions;

    std::string line;
    getline(std::cin, line); // ip register

    while (getline(std::cin, line))
        instructions.push_back(std::move(line));

    solve(instructions);

    return 0;
}

