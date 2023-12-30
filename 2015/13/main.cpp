#include <algorithm>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

using HappinessMap = std::unordered_map<std::string,
                                        std::unordered_map<std::string, int>>;
auto parse(const std::string& line)
{
    static const std::regex re{"([a-zA-Z]+) would (gain|lose) ([1-9][0-9]*) happiness units by sitting next to ([a-zA-Z]+)."};

    std::smatch matched;
    regex_match(line, matched, re);

    auto person1 = matched[1].str();
    auto happiness = strtol(matched[3].str().data(), nullptr, 10);
    happiness *= matched[2].str() == "gain" ? 1 : -1;
    auto person2 = matched[4].str();

    return std::make_tuple(person1, person2, happiness);
}

auto happiness(const std::vector<std::string>& persons,
    const HappinessMap& happiness_map)
{
    if (persons.empty())
        return 0;

    auto happiness = 0;

    for (auto i = 0U; i < persons.size() - 1; ++i)
        happiness += happiness_map.at(persons[i]).at(persons[i + 1]);
    for (auto i = 1U; i < persons.size(); ++i)
        happiness += happiness_map.at(persons[i]).at(persons[i - 1]);

    happiness += happiness_map.at(persons.front()).at(persons.back());
    happiness += happiness_map.at(persons.back()).at(persons.front());

    return happiness;
}

auto max_happiness(std::vector<std::string> persons,
    const HappinessMap& happiness_map)
{
    sort(persons.begin(), persons.end());

    auto max_happiness = std::numeric_limits<int>::min();

    do {
        auto happiness = ::happiness(persons, happiness_map);
        max_happiness = std::max(max_happiness, happiness);
    } while (std::next_permutation(persons.begin(), persons.end()));

    return max_happiness;
}

int main()
{
    HappinessMap happiness_map;

    std::string line;
    while (getline(std::cin, line)) {
        auto [person1, person2, happiness] = parse(line);
        happiness_map[std::move(person1)][std::move(person2)] = happiness;
    }

    std::vector<std::string> persons;

    for (const auto& [p, _] : happiness_map)
        persons.push_back(p);

    std::cout << max_happiness(persons, happiness_map) << "\n";

    for (const auto& p : persons) {
        happiness_map["me"][p] = 0;
        happiness_map[p]["me"] = 0;
    }
    persons.push_back("me");

    std::cout << max_happiness(persons, happiness_map) << "\n";

    return 0;
}

