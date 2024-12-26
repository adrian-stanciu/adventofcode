#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>

struct Player {
    int life;
    int damage;
    int armor;
};

auto read_boss()
{
    static const std::regex life_re{"Hit Points: ([1-9][0-9]*)"};
    static const std::regex damage_re{"Damage: ([1-9][0-9]*)"};
    static const std::regex armor_re{"Armor: ([1-9][0-9]*)"};

    static const auto to_number = [] (const auto& s) {
        return strtol(s.data(), nullptr, 10);
    };

    Player boss{};

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;
        if (regex_match(line, matched, life_re)) {
            boss.life = to_number(matched[1].str());
        } else if (regex_match(line, matched, damage_re)) {
            boss.damage = to_number(matched[1].str());
        } else if (regex_match(line, matched, armor_re)) {
            boss.armor = to_number(matched[1].str());
        }
    }

    return boss;
}

struct Item {
    int cost;
    int damage;
    int armor;
};

bool am_i_winner(Player me, Player boss)
{
    auto boss_injury = std::max(me.damage - boss.armor, 1);
    auto me_injury = std::max(boss.damage - me.armor, 1);

    auto rounds_boss_alive = boss.life / boss_injury +
        (boss.life % boss_injury ? 1 : 0);
    auto rounds_me_alive = me.life / me_injury +
        (me.life % me_injury ? 1 : 0);

    return rounds_me_alive >= rounds_boss_alive;
}

auto compute_costs(const Player& boss,
    const std::vector<Item>& weapons,
    const std::vector<Item>& armor,
    const std::vector<Item>& rings)
{
    auto min_cost_to_win = std::numeric_limits<int>::max();
    auto max_cost_to_lose = std::numeric_limits<int>::min();

    for (const auto& w : weapons)
        for (const auto& a : armor)
            for (const auto& r : rings) {
                const Player me{100, w.damage + r.damage, a.armor + r.armor };

                if (am_i_winner(me, boss))
                    min_cost_to_win = std::min(min_cost_to_win,
                        w.cost + a.cost + r.cost);
                else
                    max_cost_to_lose = std::max(max_cost_to_lose,
                        w.cost + a.cost + r.cost);
            }

    return std::make_pair(min_cost_to_win, max_cost_to_lose);
}

int main()
{
    const std::vector<Item> weapons {
        {8, 4, 0},
        {10, 5, 0},
        {25, 6, 0},
        {40, 7, 0},
        {74, 8, 0},
    };
    std::vector<Item> armor {
        {13, 0, 1},
        {31, 0, 2},
        {53, 0, 3},
        {75, 0, 4},
        {102, 0, 5},
    };
    std::vector<Item> rings {
        {25, 1, 0},
        {50, 2, 0},
        {100, 3, 0},
        {20, 0, 1},
        {40, 0, 2},
        {80, 0, 3},
    };

    // add options to buy 2 rings
    auto rings_sz = rings.size();
    for (auto i = 0U; i < rings_sz - 1; ++i)
        for (auto j = i + 1; j < rings_sz; ++j)
            rings.push_back({
                rings[i].cost + rings[j].cost,
                rings[i].damage + rings[j].damage,
                rings[i].armor + rings[j].armor,
                });

    // add options to buy 0 armor/rings
    armor.push_back({0, 0, 0});
    rings.push_back({0, 0, 0});

    auto [min_cost_to_win, max_cost_to_loss] =
        compute_costs(read_boss(), weapons, armor, rings);

    std::cout << min_cost_to_win << "\n";
    std::cout << max_cost_to_loss << "\n";

    return 0;
}

