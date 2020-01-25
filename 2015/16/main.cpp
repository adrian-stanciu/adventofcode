#include <iostream>
#include <functional>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

using Counters = std::unordered_map<std::string, int>;
using Criteria = std::unordered_map<std::string, std::function<bool(int)>>;

auto parse(const std::string& line)
{
    static const std::regex re{"Sue ([1-9][0-9]*): (.*)"};

    std::smatch matched;
    regex_match(line, matched, re);

    auto id = strtol(matched[1].str().data(), nullptr, 10);

    Counters counters{};

    std::stringstream ss{matched[2].str()};
    std::string name;
    while (ss >> name) {
        name.pop_back(); // colon

        int cnt;
        ss >> cnt;

        counters[std::move(name)] = cnt;

        ss.ignore(); // comma
    }

    return std::make_pair(id, counters);
}

bool are_accepted(const Counters& counters, const Criteria& criteria)
{
    for (const auto& [name, cnt] : counters)
        if (!criteria.at(name)(cnt))
            return false;

    return true;
}

int main()
{
    const auto eq = [] (auto expected) {
        return [=] (auto n) { return n == expected; };
    };

    const auto gt = [] (auto expected) {
        return [=] (auto n) { return n > expected; };
    };

    const auto lt = [] (auto expected) {
        return [=] (auto n) { return n < expected; };
    };

    const Criteria criteria1 {
        {"children",    eq(3)},
        {"cats",        eq(7)},
        {"samoyeds",    eq(2)},
        {"pomeranians", eq(3)},
        {"akitas",      eq(0)},
        {"vizslas",     eq(0)},
        {"goldfish",    eq(5)},
        {"trees",       eq(3)},
        {"cars",        eq(2)},
        {"perfumes",    eq(1)},
    };

    const Criteria criteria2 {
        {"children",    eq(3)},
        {"cats",        gt(7)},
        {"samoyeds",    eq(2)},
        {"pomeranians", lt(3)},
        {"akitas",      eq(0)},
        {"vizslas",     eq(0)},
        {"goldfish",    lt(5)},
        {"trees",       gt(3)},
        {"cars",        eq(2)},
        {"perfumes",    eq(1)},
    };

    auto first_id = 0;
    auto second_id = 0;

    std::string line;
    while (getline(std::cin, line)) {
        auto [id, counters] = parse(line);

        if (are_accepted(counters, criteria1))
            first_id = id;
        if (are_accepted(counters, criteria2))
            second_id = id;
    }

    std::cout << first_id << "\n";
    std::cout << second_id << "\n";

    return 0;
}

