#include <iostream>
#include <regex>
#include <set>
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

void add_next_molecules(const std::string& molecule, int pos, int len,
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
    ReplacementMap reversed_repl_map;

    for (const auto& [from, to_list] : repl_map)
        for (const auto& to : to_list)
            reversed_repl_map[to].push_back(from);

    return reversed_repl_map;
}

auto min_steps_to_reduce_to_electron(const std::string& molecule, const ReplacementMap& repl_map)
{
    struct State {
        std::string molecule;
        long dist;

        unsigned long score() const
        {
            // molecule's size (how much remains to be reduced) + number of replacements made so far
            return molecule.size() + dist;
        }

        bool operator==(const State& s) const
        {
            return molecule == s.molecule;
        }

        bool operator<(const State& s) const
        {
            if (score() == s.score()) {
                if (dist == s.dist)
                    return !(*this == s);
                else
                    return dist < s.dist;
            } else
                return score() < s.score();
        }
    };

    struct StateHasher {
        auto operator()(const State& s) const
        {
            // based on boost's hash_combine()
            size_t h = 0;
            h ^= std::hash<std::string>{}(s.molecule) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };

    std::set<State> states;
    std::unordered_set<State, StateHasher> open_set;
    std::unordered_set<State, StateHasher> closed_set;

    State s{molecule, 0};
    states.emplace(s);
    states.emplace(std::move(s));

    while (!states.empty()) {
        auto s = *states.begin();
        states.erase(states.begin());

        if (s.molecule == "e")
            return s.dist;

        open_set.erase(s);
        closed_set.emplace(s);

        auto next_molecules = find_unique_after_one_step(s.molecule, repl_map);
        if (next_molecules.empty())
            continue;

        for (const auto& next_molecule : next_molecules) {
            State next_s{next_molecule, s.dist + 1};
            if (closed_set.find(next_s) != closed_set.end())
                continue;

            auto it = open_set.find(next_s);
            if (it == open_set.end()) {
                states.emplace(next_s);
                open_set.emplace(std::move(next_s));
            } else if (it->dist > next_s.dist) {
                states.erase(*it);
                states.emplace(std::move(next_s));
            }
        }
    }

    return -1L;
}

int main()
{
    auto fw_repl_map = read_replacements();

    std::string molecule;
    getline(std::cin, molecule);

    std::cout << find_unique_after_one_step(molecule, fw_repl_map).size() << "\n";

    // go in reverse direction by reducing the molecule to an electron
    auto bw_repl_map = reverse_replacements(fw_repl_map);
    std::cout << min_steps_to_reduce_to_electron(molecule, bw_repl_map) << "\n";

    return 0;
}

