#include <array>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <string>

auto is_translation(const std::set<long>& set1, const std::set<long>& set2)
{
    if (set1.size() != set2.size())
        return false;

    auto it1 = set1.begin();
    auto it2 = set2.begin();

    auto step = *it2 - *it1;

    while (it1 != set1.end()) {
        if (*it1 + step != *it2)
            return false;

        ++it1;
        ++it2;
    }

    return true;
}

void check_pot(long pot, const std::set<long>& pots, const std::array<bool, 32>& rules,
    std::set<long>& new_pots)
{
    std::array<bool, 9> has_plant;
    for (auto k = pot - 4; k <= pot + 4; ++k)
        has_plant[k - pot + 4] = (pots.find(k) != pots.end());

    auto key = 0;
    for (auto k = 0; k < 4; ++k)
        key = (key << 1) | has_plant[k];

    for (auto k = 0; k < 5; ++k) {
        key = (key << 1) | has_plant[k + 4];
        if (rules[key])
            new_pots.emplace(pot - 2 + k);
        key &= 0xf;
    }
}

auto sum(std::set<long> pots, const std::array<bool, 32>& rules, long iters)
{
    for (auto i = 1L; i <= iters; ++i) {
        std::set<long> new_pots;

        for (auto pot : pots)
            check_pot(pot, pots, rules, new_pots);

        if (is_translation(pots, new_pots)) {
            auto step = *new_pots.begin() - *pots.begin();

            return static_cast<long>(new_pots.size()) * (iters - i) * step +
                accumulate(new_pots.begin(), new_pots.end(), 0L);
        }

        pots.swap(new_pots);
    }

    return accumulate(pots.begin(), pots.end(), 0L);
}

int main()
{
    std::string line;
    std::smatch matched;

    const std::regex initial_state_re {"initial state: (.+)"};
    const std::regex rule_re {"(.+) => (.)"};

    getline(std::cin, line);
    regex_match(line, matched, initial_state_re);
    auto pots_s = matched[1].str();

    std::set<long> pots;
    for (auto k = 0U; k < pots_s.size(); ++k)
        if (pots_s[k] == '#')
            pots.emplace(k);

    std::array<bool, 32> rules;
    while (getline(std::cin, line)) {
        regex_match(line, matched, rule_re);

        auto in = matched[1].str();
        auto out = matched[2].str()[0];

        auto key = 0;
        for (auto c : in)
            key = (key << 1) | (c == '#');
        auto val = (out == '#');
        rules[std::move(key)] = val;
    }

    std::cout << sum(pots, rules, 20) << "\n";
    std::cout << sum(pots, rules, 50000000000) << "\n";

    return 0;
}

