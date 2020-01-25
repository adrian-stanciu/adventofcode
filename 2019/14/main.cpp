#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Elem {
    std::string name;
    long count;

    Elem(std::string name, long count)
    : name(std::move(name)), count(count)
    {}
};

struct Reaction {
    std::vector<Elem> reactants;
    Elem result;

    Reaction(std::vector<Elem> reactants, Elem result)
    : reactants(std::move(reactants)), result(std::move(result))
    {}
};

using ReactionMap = std::unordered_map<std::string, Reaction>;

auto split_reactants(const std::string& list)
{
    static const std::regex re{"\\s*([1-9][0-9]*) ([A-Z]+)\\s*"};

    std::vector<Elem> reactants;

    std::stringstream ss_list(list);

    std::string element;
    while (getline(ss_list, element, ',')) {
        std::smatch matched;
        if (regex_match(element, matched, re))
            reactants.emplace_back(matched[2].str(), strtol(matched[1].str().data(), nullptr, 10));
    }

    return reactants;
}

void compute_min_ore_rec(const ReactionMap& r_map, std::unordered_map<std::string, long>& avail,
    const std::string& root, long needed, long& ore)
{
    if (root == "ORE") {
        ore += needed;
        return;
    }

    if (avail[root] >= needed) {
        avail[root] -= needed;
        return;
    }

    auto todo = needed - avail[root];
    avail[root] = 0;

    auto produced_in_1reaction = r_map.at(root).result.count;

    auto num_reactions = todo / produced_in_1reaction +
        (todo % produced_in_1reaction ? 1 : 0);

    for (const auto& reactant : r_map.at(root).reactants)
        compute_min_ore_rec(r_map, avail, reactant.name, num_reactions * reactant.count, ore);

    avail[root] = num_reactions * produced_in_1reaction - todo;
}

auto compute_min_ore(const ReactionMap& r_map, long num_fuel_needed)
{
    auto ore = 0L;

    std::unordered_map<std::string, long> avail;
    compute_min_ore_rec(r_map, avail, "FUEL", num_fuel_needed, ore);

    return ore;
}

auto compute_max_fuel(const ReactionMap& r_map, long max_ore_avail)
{
    auto lo = 0L;
    auto hi = max_ore_avail;

    auto sol = lo;

    while (lo <= hi) {
        auto mid = lo + (hi - lo) / 2;

        auto ore = compute_min_ore(r_map, mid);

        if (ore <= max_ore_avail) {
            sol = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    return sol;
}

int main()
{
    static const std::regex re{"\\s*(.*) => ([1-9][0-9]*) ([A-Z]+)\\s*"};

    ReactionMap r_map;

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;
        if (regex_match(line, matched, re)) {
            auto reactants = split_reactants(matched[1].str());
            if (reactants.empty())
                continue;

            Elem result{matched[3].str(), strtol(matched[2].str().data(), nullptr, 10)};
            Reaction reaction{std::move(reactants), std::move(result)};

            r_map.try_emplace(matched[3].str(), std::move(reaction));
        }
    }

    std::cout << compute_min_ore(r_map, 1) << "\n";
    std::cout << compute_max_fuel(r_map, 1000000000000L) << "\n";

    return 0;
}

