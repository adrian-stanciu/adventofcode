#include <algorithm>
#include <charconv>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

struct Group {
    long id;
    bool is_immune_sys;

    long num;
    long hit;
    std::vector<std::string> immune;
    std::vector<std::string> weak;
    std::string damage_type;
    long damage;
    long initiative;

    long target_id;
    bool avail;
};

void parse_immune_weak(const std::string& immune_weak_s, Group& g)
{
    auto split_to = [] (const std::string& s, char sep, std::vector<std::string>& dest) {
        std::stringstream ss {s};
        std::string line;
        while (getline(ss, line, sep)) {
            auto from = line.find_first_not_of(' ');
            auto to = line.find_last_not_of(' ');
            dest.push_back(line.substr(from, to - from + 1));
        }
    };

    static const std::regex immune_weak_re {"\\(immune to (.+); weak to (.+)\\)"};
    static const std::regex weak_immune_re {"\\(weak to (.+); immune to (.+)\\)"};
    static const std::regex immune_re {"\\(immune to (.+)\\)"};
    static const std::regex weak_re {"\\(weak to (.+)\\)"};

    std::smatch matched;
    if (regex_match(immune_weak_s, matched, immune_weak_re)) {
        split_to(matched[1].str(), ',', g.immune);
        split_to(matched[2].str(), ',', g.weak);
    } else if (regex_match(immune_weak_s, matched, weak_immune_re)) {
        split_to(matched[1].str(), ',', g.weak);
        split_to(matched[2].str(), ',', g.immune);
    } else if (regex_match(immune_weak_s, matched, immune_re)) {
        split_to(matched[1].str(), ',', g.immune);
    } else if (regex_match(immune_weak_s, matched, weak_re)){
        split_to(matched[1].str(), ',', g.weak);
    }
}

auto parse_line(const std::string& line, long id, bool is_immune_sys)
{
    auto str2num = [] (const std::string& s, long& n) {
        std::from_chars(s.data(), s.data() + s.size(), n);
    };

    static const std::regex long_re {"([0-9]+) units each with ([0-9]+) hit points (.+) with an attack that does ([0-9]+) ([a-z]+) damage at initiative ([0-9]+)"};
    static const std::regex short_re {"([0-9]+) units each with ([0-9]+) hit points with an attack that does ([0-9]+) ([a-z]+) damage at initiative ([0-9]+)"};

    Group g;
    g.id = id;
    g.is_immune_sys = is_immune_sys;

    std::smatch matched;
    if (regex_match(line, matched, long_re)) {
        str2num(matched[1].str(), g.num);
        str2num(matched[2].str(), g.hit);
        parse_immune_weak(matched[3].str(), g);
        str2num(matched[4].str(), g.damage);
        g.damage_type = matched[5].str();
        str2num(matched[6].str(), g.initiative);
    } else if (regex_match(line, matched, short_re)) {
        str2num(matched[1].str(), g.num);
        str2num(matched[2].str(), g.hit);
        str2num(matched[3].str(), g.damage);
        g.damage_type = matched[4].str();
        str2num(matched[5].str(), g.initiative);
    }

    return g;
}

void select_targets(std::vector<Group>& groups)
{
    for (auto i = 0U; i < groups.size(); ++i) {
        auto best = -1;
        auto best_damage = 0L;

        for (auto j = 0U; j < groups.size(); ++j) {
            if (i == j)
                continue; // same unit
            if (groups[i].is_immune_sys == groups[j].is_immune_sys)
                continue; // same team
            if (!groups[j].avail)
                continue; // target already selected

            if (find(groups[j].immune.begin(), groups[j].immune.end(),
                groups[i].damage_type) != groups[j].immune.end())
                continue; // target is immune

            auto damage = groups[i].num * groups[i].damage;
            if (find(groups[j].weak.begin(), groups[j].weak.end(),
                groups[i].damage_type) != groups[j].weak.end())
                damage *= 2; // target is weak

            if (damage > best_damage) {
                best = j;
                best_damage = damage;
            }
        }

        if (best != -1) {
            groups[i].target_id = groups[best].id;
            groups[best].avail = false;
        }
    }
}

bool attack(std::vector<Group>& groups)
{
    auto changed = false;

    for (auto& g : groups) {
        if (g.num == 0)
            continue; // this unit was destroyed
        if (g.target_id == 0)
            continue; // this unit doesn't have a target

        auto t_id = g.target_id;
        auto target_it = find_if(groups.begin(), groups.end(), [t_id](const auto& g) {
            return g.id == t_id;
        });
        if (target_it == groups.end())
            continue; // target not found (should not happen)

        if (target_it->num == 0)
            continue; // target already dead

        auto power = g.damage * g.num;
        if (find(target_it->weak.begin(), target_it->weak.end(),
            g.damage_type) != target_it->weak.end())
            power *= 2; // target is weak

        auto dead = power / target_it->hit;

        if (dead > 0) {
            changed = true;

            if (target_it->num <= dead)
                target_it->num = 0;
            else
                target_it->num -= dead;
        }
    }

    return changed;
}

auto sum_remaining(std::vector<Group> groups, long boost)
{
    bool has_winner = true;
    bool immune_sys_won = false;

    auto power_sorting = [](const auto& g1, const auto& g2) {
        auto power1 = g1.num * g1.damage;
        auto power2 = g2.num * g2.damage;

        if (power1 == power2)
            return g1.initiative > g2.initiative;
        else
            return power1 > power2;
    };

    auto initiative_sorting = [](const auto& g1, const auto& g2) {
        return g1.initiative > g2.initiative;
    };

    for (auto& g : groups)
        if (g.is_immune_sys)
            g.damage += boost;

    while (true) {
        sort(groups.begin(), groups.end(), power_sorting);

        for (auto& g : groups) {
            g.target_id = 0;
            g.avail = true;
        }

        select_targets(groups);

        sort(groups.begin(), groups.end(), initiative_sorting);

        if (!attack(groups)) {
            has_winner = false;
            break;
        }

        groups.erase(remove_if(groups.begin(), groups.end(), [](const auto& g) {
            return g.num == 0;
        }), groups.end());

        if (count_if(groups.begin(), groups.end(), [](const auto& g) {
            return g.is_immune_sys;
        }) == 0)
            break;
        if (count_if(groups.begin(), groups.end(), [](const auto& g) {
            return !g.is_immune_sys;
        }) == 0) {
            immune_sys_won = true;
            break;
        }
    }

    if (has_winner) {
        auto sum = 0L;

        for (const auto& g : groups)
            sum += g.num;

        return std::make_pair(sum, immune_sys_won);
    } else
        return std::make_pair(0L, false);
}

int main()
{
    std::vector<Group> groups;
    auto id = 0L;

    std::string line;
    getline(std::cin, line);

    auto is_imm_sys = (line == "Immune System:");

    while (getline(std::cin, line)) {
        if (line.empty())
            break;
        auto group = parse_line(line, ++id, is_imm_sys);
        groups.push_back(std::move(group));
    }

    getline(std::cin, line);

    while (getline(std::cin, line)) {
        auto group = parse_line(line, ++id, !is_imm_sys);
        groups.push_back(std::move(group));
    }

    std::cout << sum_remaining(groups, 0).first << "\n";

    auto boost = 0L;
    while (true) {
        ++boost;
        auto res = sum_remaining(groups, boost);
        if (res.second) {
            std::cout << res.first << "\n";
            break;
        }
    }

    return 0;
}

