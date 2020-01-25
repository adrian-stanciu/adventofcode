#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <variant>
#include <vector>

using Grid1 = std::array<std::array<bool, 1000>, 1000>;
using Grid2 = std::array<std::array<int, 1000>, 1000>;

struct Turn {
    bool on;
    std::pair<int, int> up_le;
    std::pair<int, int> bo_ri;

    void exec1(Grid1& g) const
    {
        for (auto y = up_le.first; y <= bo_ri.first; ++y)
            std::transform(g[y].begin() + up_le.second, g[y].begin() + bo_ri.second + 1,
                g[y].begin() + up_le.second, [&] (auto) { return on; });
    }

    void exec2(Grid2& g) const
    {
        for (auto y = up_le.first; y <= bo_ri.first; ++y)
            std::transform(g[y].begin() + up_le.second, g[y].begin() + bo_ri.second + 1,
                g[y].begin() + up_le.second, [&] (auto val) {
                    if (on)
                        return val + 1;
                    else
                        return std::max(val - 1, 0);
                });
    }
};

struct Toggle {
    std::pair<int, int> up_le;
    std::pair<int, int> bo_ri;

    void exec1(Grid1& g) const
    {
        for (auto y = up_le.first; y <= bo_ri.first; ++y)
            std::transform(g[y].begin() + up_le.second, g[y].begin() + bo_ri.second + 1,
                g[y].begin() + up_le.second, [] (auto on) { return !on; });
    }

    void exec2(Grid2& g) const
    {
        for (auto y = up_le.first; y <= bo_ri.first; ++y)
            std::transform(g[y].begin() + up_le.second, g[y].begin() + bo_ri.second + 1,
                g[y].begin() + up_le.second, [] (auto val) { return val + 2; });
    }
};

using Instr = std::variant<Turn, Toggle>;

std::optional<Instr> parse_instr(const std::string& line)
{
    static const std::regex turn_re{"turn (on|off) ([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)"};
    static const std::regex toggle_re{"toggle ([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)"};

    static const auto to_number = [] (const auto& s) {
        return strtol(s.data(), nullptr, 10);
    };

    std::smatch matched;
    if (regex_match(line, matched, turn_re)) {
        auto on = matched[1].str() == "on";
        auto up_le_y = to_number(matched[2].str());
        auto up_le_x = to_number(matched[3].str());
        auto bo_ri_y = to_number(matched[4].str());
        auto bo_ri_x = to_number(matched[5].str());
        return Turn{on, std::make_pair(up_le_y, up_le_x), std::make_pair(bo_ri_y, bo_ri_x)};
    } else if (regex_match(line, matched, toggle_re)) {
        auto up_le_y = to_number(matched[1].str());
        auto up_le_x = to_number(matched[2].str());
        auto bo_ri_y = to_number(matched[3].str());
        auto bo_ri_x = to_number(matched[4].str());
        return Toggle{std::make_pair(up_le_y, up_le_x), std::make_pair(bo_ri_y, bo_ri_x)};
    }

    return std::nullopt;
}

void run_instructions(const std::vector<Instr>& instructions, Grid1& g)
{
    for (const auto& instr : instructions)
        std::visit([&] (const auto& _) { _.exec1(g); }, instr);
}

auto sum_in_grid(const Grid1& g)
{
    auto cnt = 0;

    for (const auto& row : g)
        cnt += std::count(row.begin(), row.end(), true);

    return cnt;
}

void run_instructions(const std::vector<Instr>& instructions, Grid2& g)
{
    for (const auto& instr : instructions)
        std::visit([&] (const auto& _) { _.exec2(g); }, instr);
}

auto sum_in_grid(const Grid2& g)
{
    auto cnt = 0;

    for (const auto& row : g)
        cnt += std::accumulate(row.begin(), row.end(), 0);

    return cnt;
}

int main()
{
    std::vector<Instr> instructions;

    std::string line;
    while (getline(std::cin, line)) {
        auto instr = parse_instr(line);
        if (instr.has_value())
            instructions.push_back(std::move(instr.value()));
    }

    Grid1 g1{};
    run_instructions(instructions, g1);
    std::cout << sum_in_grid(g1) << "\n";

    Grid2 g2{};
    run_instructions(instructions, g2);
    std::cout << sum_in_grid(g2) << "\n";

    return 0;
}

