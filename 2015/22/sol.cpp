#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

struct Player {
    int life;
    int mana;
    int cost;
};

struct Boss {
    int life;
    int damage;
};

auto read_boss()
{
    static const std::regex life_re{"Hit Points: ([1-9][0-9]*)"};
    static const std::regex damage_re{"Damage: ([1-9][0-9]*)"};

    static const auto to_number = [] (const auto& s) {
        return strtol(s.data(), nullptr, 10);
    };

    Boss boss{};

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;
        if (regex_match(line, matched, life_re)) {
            boss.life = to_number(matched[1].str());
        } else if (regex_match(line, matched, damage_re)) {
            boss.damage = to_number(matched[1].str());
        }
    }

    return boss;
}

struct Spell {
    int id;
    int cost;
    int life;
    int damage;
    int armor;
    int mana;
    mutable int duration;
};

bool operator==(const Spell& s1, const Spell& s2)
{
    return s1.id == s2.id;
}

struct SpellHasher {
    auto operator()(const Spell& s) const
    {
        return s.id;
    }
};

void find_min_cost_to_win_rec(Player me, Boss boss,
    const std::vector<Spell>& spells, std::unordered_set<Spell, SpellHasher> active_spells,
    bool dec_life, bool my_turn, int& min_cost_to_win)
{
    if (my_turn && dec_life) {
        --me.life;
        if (me.life <= 0)
            return;
    }

    // apply spell effects
    for (auto& as : active_spells) {
        me.life += as.life;
        boss.life -= as.damage;
        me.mana += as.mana;
        --as.duration;

        if (boss.life <= 0) {
            min_cost_to_win = std::min(min_cost_to_win, me.cost);
            return;
        }
    }

    // remove finished spells
    for (auto it = active_spells.begin(); it != active_spells.end();)
        if (it->duration == 0)
            it = active_spells.erase(it);
        else
            ++it;

    if (my_turn) {
        for (const auto& s : spells)
            if (active_spells.find(s) == active_spells.end() &&
                s.cost <= me.mana && me.cost + s.cost < min_cost_to_win) {
                auto next_me{me};
                auto next_boss{boss};
                auto next_active_spells{active_spells};

                next_me.mana -= s.cost;
                next_me.cost += s.cost;

                if (s.duration > 0) {
                    // activate spell with effects
                    next_active_spells.insert(s);
                } else {
                    // apply immediate spell
                    next_me.life += s.life;
                    next_boss.life -= s.damage;
                    next_me.mana += s.mana;

                    if (next_boss.life <= 0) {
                        min_cost_to_win = std::min(min_cost_to_win, next_me.cost);
                        return;
                    }
                }

                find_min_cost_to_win_rec(next_me, next_boss, spells,
                    std::move(next_active_spells), dec_life, false, min_cost_to_win);
            }
    } else {
        auto my_armor = 0;
        for (auto& as : active_spells)
            my_armor += as.armor;

        auto boss_attack = std::max(boss.damage - my_armor, 1);

        me.life -= boss_attack;
        if (me.life <= 0)
            return;

        find_min_cost_to_win_rec(me, boss, spells,
            std::move(active_spells), dec_life, true, min_cost_to_win);
    }
}

auto find_min_cost_to_win(const Player& me, Boss& boss,
    const std::vector<Spell>& spells, bool dec_life)
{
    auto min_cost_to_win = std::numeric_limits<int>::max();

    find_min_cost_to_win_rec(me, boss, spells, {}, dec_life, true, min_cost_to_win);

    return min_cost_to_win;
}

int main()
{
    const std::vector<Spell> spells {
        // id, cost, life, damage, armor, mana, duration
        {1, 53, 0, 4, 0, 0, 0},
        {2, 73, 2, 2, 0, 0, 0},
        {3, 113, 0, 0, 7, 0, 6},
        {4, 173, 0, 3, 0, 0, 6},
        {5, 229, 0, 0, 0, 101, 5},
    };

    const Player me{50, 500, 0};
    auto boss = read_boss();

    std::cout << find_min_cost_to_win(me, boss, spells, false) << "\n";
    std::cout << find_min_cost_to_win(me, boss, spells, true) << "\n";

    return 0;
}

