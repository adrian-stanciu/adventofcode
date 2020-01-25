#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

constexpr auto FactorA = 16807;
constexpr auto Factorb = 48271;
constexpr auto Mod = 2147483647;

auto read_seed()
{
    static const std::regex re{"Generator ([A-B]) starts with ([1-9][0-9]*)"};

    std::string line;
    getline(std::cin, line);

    std::smatch matched;
    regex_match(line, matched, re);

    return std::make_pair(matched[1].str()[0], strtoul(matched[2].str().data(), nullptr, 10));
}

auto read_seeds()
{
    auto [gen1, seed1] = read_seed();
    auto [gen2, seed2] = read_seed();

    if (gen1 == 'A')
        return std::make_pair(seed1, seed2);
    else
        return std::make_pair(seed2, seed1);
}

unsigned long generate(unsigned long seed, unsigned long factor,
    const std::function<bool(unsigned long)>& is_valid)
{
    while (true) {
        seed = (seed * factor) % Mod;
        if (is_valid(seed))
            return seed;
    }
}

bool match_lowest_16_bits(unsigned long a, unsigned long b)
{
    for (size_t bit = 0; bit < 16; ++bit)
        if (((a >> bit) & 1) != ((b >> bit) & 1))
            return false;

    return true;
}

unsigned long compute_gen_score(unsigned long seed_a, unsigned long seed_b,
    const std::function<bool(unsigned long)>& is_valid_a,
    const std::function<bool(unsigned long)>& is_valid_b,
    size_t iterations)
{
    unsigned long score = 0;

    for (size_t i = 0; i < iterations; ++i) {
        seed_a = generate(seed_a, FactorA, is_valid_a);
        seed_b = generate(seed_b, Factorb, is_valid_b);

        if (match_lowest_16_bits(seed_a, seed_b))
            ++score;
    }

    return score;
}

int main()
{
    auto [seed_a, seed_b] = read_seeds();

    auto is_always_valid = [] (unsigned long) { return true; };
    auto is_multiple_of_4 = [] (unsigned long x) { return x % 4 == 0; };
    auto is_multiple_of_8 = [] (unsigned long x) { return x % 8 == 0; };

    std::cout << compute_gen_score(seed_a, seed_b,
        is_always_valid, is_always_valid, 40000000) << "\n";
    std::cout << compute_gen_score(seed_a, seed_b,
        is_multiple_of_4, is_multiple_of_8, 5000000) << "\n";

    return 0;
}

