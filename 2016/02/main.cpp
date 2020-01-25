#include <iostream>
#include <string>
#include <vector>

using Pad = std::vector<std::vector<char>>;

const Pad pad1 = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
};

const Pad pad2 = {
    {' ', ' ', '1', ' ', ' '},
    {' ', '2', '3', '4', ' '},
    {'5', '6', '7', '8', '9'},
    {' ', 'A', 'B', 'C', ' '},
    {' ', ' ', 'D', ' ', ' '},
};

auto find_on_pad(const Pad& pad, char target)
{
    for (auto r = 0U; r < pad.size(); ++r)
        for (auto c = 0U; c < pad[0].size(); ++c)
            if (pad[r][c] == target)
                return std::make_pair(r, c);

    return std::make_pair(-1U, -1U); // not found
}

auto decode_instr(const std::string& instr, const Pad& pad, unsigned int& r, unsigned int& c)
{
    for (auto i : instr)
        switch (i) {
        case 'U':
            if (r > 0 && pad[r - 1][c] != ' ')
                --r;
            break;
        case 'D':
            if (r < pad.size() - 1 && pad[r + 1][c] != ' ')
                ++r;
            break;
        case 'L':
            if (c > 0 && pad[r][c - 1] != ' ')
                --c;
            break;
        case 'R':
            if (c < pad[0].size() - 1 && pad[r][c + 1] != ' ')
                ++c;
            break;
        }

    return pad[r][c];
}

auto decode(const std::vector<std::string>& instructions, const Pad& pad)
{
    std::string code;

    auto [r, c] = find_on_pad(pad, '5');

    for (const auto& instr : instructions)
        code.push_back(decode_instr(instr, pad, r, c));

    return code;
}

int main()
{
    std::vector<std::string> instructions;

    std::string instr;
    while (getline(std::cin, instr))
        instructions.push_back(std::move(instr));

    std::cout << decode(instructions, pad1) << "\n";
    std::cout << decode(instructions, pad2) << "\n";

    return 0;
}

