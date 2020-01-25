#include <algorithm>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Properties {
    long capacity;
    long durability;
    long flavor;
    long texture;
    long calories;

    auto score() const
    {
        if (capacity < 0 || durability < 0 || flavor < 0 || texture < 0)
            return 0L;

        return capacity * durability * flavor * texture;
    }

    void add(const Properties& other, long quantity)
    {
        capacity += quantity * other.capacity;
        durability += quantity * other.durability;
        flavor += quantity * other.flavor;
        texture += quantity * other.texture;
        calories += quantity * other.calories;
    }
};

auto parse(const std::string& line)
{
    static const std::regex re{"[a-zA-z]+: capacity (-?[0-9]+), durability (-?[0-9]+), flavor (-?[0-9]+), texture (-?[0-9]+), calories (-?[0-9]+)"};

    static const auto to_number = [] (const auto& s) {
        return strtol(s.data(), nullptr, 10);
    };

    std::smatch matched;
    regex_match(line, matched, re);

    auto capacity = to_number(matched[1].str());
    auto durability = to_number(matched[2].str());
    auto flavor = to_number(matched[3].str());
    auto texture = to_number(matched[4].str());
    auto calories = to_number(matched[5].str());

    return Properties{capacity, durability, flavor, texture, calories};
}

auto max_score_rec(const std::vector<Properties>& properties,
    unsigned int i, long quantity, Properties total,
    const std::function<bool(const Properties&)>& is_valid)
{
    if (properties.size() - 1 == i) {
        total.add(properties[i], quantity);

        if (is_valid(total))
            return total.score();
        else
            return 0L;
    }

    auto max_score = 0L;

    for (auto q = 0; q <= quantity; ++q) {
        auto next_total{total};
        next_total.add(properties[i], q);

        max_score = std::max(max_score,
            max_score_rec(properties, i + 1, quantity - q, next_total, is_valid));
    }

    return max_score;
}

auto max_score(const std::vector<Properties>& properties, long quantity)
{
    return max_score_rec(properties, 0, quantity, {},
        [] (const auto&) { return true; });
}

auto max_score(const std::vector<Properties>& properties, long quantity,
    const std::function<bool(const Properties&)>& is_valid)
{
    return max_score_rec(properties, 0, quantity, {}, is_valid);
}

int main()
{
    std::vector<Properties> properties;

    std::string line;
    while (getline(std::cin, line))
        properties.push_back(parse(line));

    std::cout << max_score(properties, 100) << "\n";
    std::cout << max_score(properties, 100,
        [] (const auto& p) { return p.calories == 500; }) << "\n";

    return 0;
}

