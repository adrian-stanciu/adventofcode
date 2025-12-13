#include <iostream>
#include <random>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using ReplacementMap = std::unordered_map<std::string, std::vector<std::string>>;

auto read_replacements()
{
    static const std::regex re{"([a-zA-Z]+) => ([a-zA-Z]+)"};

    ReplacementMap repl_map;

    std::string line;
    while (getline(std::cin, line)) {
        if (line.empty())
            break;

        std::smatch matched;
        regex_match(line, matched, re);

        repl_map[matched[1].str()].push_back(matched[2].str());
    }

    return repl_map;
}

void add_next_molecules(const std::string& molecule, size_t pos, size_t len,
    const std::vector<std::string>& replacements, std::unordered_set<std::string>& uniq)
{
    for (const auto& repl : replacements) {
        auto next_molecule{molecule};
        next_molecule.replace(pos, len, repl);
        uniq.insert(next_molecule);
    }
}

auto find_unique_after_one_step(const std::string& molecule, const ReplacementMap& repl_map)
{
    std::unordered_set<std::string> uniq;

    for (const auto& [from, to_list] : repl_map) {
        auto pos = 0UL;
        while (true) {
            pos = molecule.find(from, pos);
            if (pos == std::string::npos)
                break;

            add_next_molecules(molecule, pos, from.size(), to_list, uniq);

            ++pos;
        }
    }

    return uniq;
}

auto reverse_replacements(const ReplacementMap& repl_map)
{
    std::vector<std::pair<std::string, std::string>> reversed_repl_vec;

    for (const auto& [from, to_list] : repl_map)
        for (const auto& to : to_list)
            reversed_repl_vec.emplace_back(to, from);

    return reversed_repl_vec;
}

auto min_steps_to_reduce_to_electron(const std::string& molecule,
    std::vector<std::pair<std::string, std::string>>& repl_vec)
{
    std::random_device rd;
    std::mt19937 gen{rd()};

    while (true) {
        auto cnt = 0;

        auto m{molecule};

        std::shuffle(repl_vec.begin(), repl_vec.end(), gen);

        while (m != "e") {
            auto found = false;

            for (const auto& [from, to] : repl_vec)
                if (auto pos = m.find(from, 0); pos != m.npos) {
                    ++cnt;
                    m.replace(pos, from.size(), to);
                    found = true;
                    break;
                }

            if (!found)
                break;
        }

        if (m == "e")
            return cnt;
    }
}

int main()
{
    auto fw_repl_map = read_replacements();

    std::string molecule;
    getline(std::cin, molecule);

    std::cout << find_unique_after_one_step(molecule, fw_repl_map).size() << "\n";

    // go in reverse direction by reducing the molecule to an electron
    auto bw_repl_vec = reverse_replacements(fw_repl_map);
    std::cout << min_steps_to_reduce_to_electron(molecule, bw_repl_vec) << "\n";

    return 0;
}

