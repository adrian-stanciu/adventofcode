#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr auto LastFloor = 4;

// first is generator's floor, second is microchip's floor
using Equipment = std::pair<int, int>;

auto parse_input()
{
    std::multiset<Equipment> equip_set;

    auto split_to = [] (const auto& s, char sep, auto& dest) {
        std::stringstream ss{s};

        std::string line;
        while (getline(ss, line, sep)) {
            auto from = line.find_first_not_of(' ');
            auto to = line.find_last_not_of(' ');

            dest.push_back(line.substr(from, to - from + 1));
        }
    };

    static const std::regex empty_floor_re {"The ([a-z]+) floor contains nothing relevant."};
    static const std::regex relevant_floor_re {"The ([a-z]+) floor contains (.+)."};
    static const std::regex and_single_equip_re {"and (.+)"};
    static const std::regex and_double_equip_re {"(.+) and (.+)"};
    static const std::regex gen_re {"a[n]? ([a-z]+) generator"};
    static const std::regex chip_re {"a[n]? ([a-z]+)-compatible microchip"};

    struct EquipPosition {
        int gen_floor;
        int chip_floor;
    };

    std::unordered_map<std::string, EquipPosition> equip_pos;

    for (int i = 1; i <= LastFloor; ++i) {
        std::string line;
        getline(std::cin, line);

        std::smatch matched;
        if (regex_match(line, matched, empty_floor_re))
            continue;
        if (!regex_match(line, matched, relevant_floor_re))
            continue;

        std::vector<std::string> comma_splitted_equip;
        split_to(matched[2].str(), ',', comma_splitted_equip);

        std::vector<std::string> and_splitted_equip;
        for (const auto& e : comma_splitted_equip) {
            if (regex_match(e, matched, and_single_equip_re))
                and_splitted_equip.push_back(matched[1]);
            else if (regex_match(e, matched, and_double_equip_re)) {
                and_splitted_equip.push_back(matched[1]);
                and_splitted_equip.push_back(matched[2]);
            } else
                and_splitted_equip.push_back(e);
        }

        for (auto& e : and_splitted_equip)
            if (regex_match(e, matched, gen_re))
                equip_pos[matched[1]].gen_floor = i;
            else if (regex_match(e, matched, chip_re))
                equip_pos[matched[1]].chip_floor = i;
    }

    for (const auto& e : equip_pos)
        equip_set.emplace(e.second.gen_floor, e.second.chip_floor);

    return equip_set;
}

struct State {
    int elevator; // elevator's floor

    std::multiset<Equipment> equip_set; // set of equipments

    int iters; // iterations done until reaching this state

    int score; // estimated score

    State(int elevator, std::multiset<Equipment> equip_set, int iters)
    : elevator(elevator)
    , equip_set(std::move(equip_set))
    , iters(iters)
    {
        score = iters;

        // adds diffs to last floor
        for (const auto& e : this->equip_set) {
            score += LastFloor - e.first;
            score += LastFloor - e.second;
        }
    }
};

bool operator==(const State& s1, const State& s2)
{
    return s1.elevator == s2.elevator && s1.equip_set == s2.equip_set;
}

bool operator<(const State& s1, const State& s2)
{
    if (s1.score == s2.score) {
        if (s1.iters == s2.iters)
            return std::tie(s1.elevator, s1.equip_set) <
                std::tie(s2.elevator, s2.equip_set);
        else
            return s1.iters < s2.iters;
    } else
        return s1.score < s2.score;
}

bool is_final_state(const State& s)
{
    return s.elevator == LastFloor &&
        s.equip_set.count({LastFloor, LastFloor}) == s.equip_set.size();
}

bool is_valid_state(const State& s)
{
    std::set<int> gen_floors;
    for (const auto& e : s.equip_set)
        gen_floors.emplace(e.first);

    for (const auto& e : s.equip_set) {
        // check if both gen and chip are on the same floor
        if (e.first == e.second)
            continue;

        // check if any gen on chip's floor
        if (gen_floors.find(e.second) != gen_floors.end())
            return false;
    }

    return true;
}

void remove_one(std::multiset<Equipment>& equip_set, const Equipment& equip)
{
    equip_set.erase(equip_set.find(equip));
}

std::set<State> next_states(const State& s)
{
    std::set<State> states;

    // move gen-chip pair
    if (s.equip_set.find({s.elevator, s.elevator}) != s.equip_set.end()) {
        auto equip_set(s.equip_set);

        remove_one(equip_set, {s.elevator, s.elevator});

        auto add_one = [&] (auto floor, auto equip_set) {
            equip_set.emplace(floor, floor);
            states.emplace(floor, equip_set, s.iters + 1);
        };

        if (s.elevator == LastFloor)
            add_one(s.elevator - 1, std::move(equip_set));
        else if (s.elevator == 1)
            add_one(s.elevator + 1, std::move(equip_set));
        else {
            add_one(s.elevator - 1, equip_set);
            add_one(s.elevator + 1, std::move(equip_set));
        }
    }

    std::multiset<Equipment> gens;
    std::multiset<Equipment> chips;
    for (const auto& e : s.equip_set) {
        if (e.first == s.elevator)
            gens.emplace(e);
        if (e.second == s.elevator)
            chips.emplace(e);
    }

    // move one gen
    for (const auto& e : gens) {
        auto equip_set(s.equip_set);

        remove_one(equip_set, {e.first, e.second});

        auto add_one = [&] (auto gen_floor, auto chip_floor, auto equip_set) {
            equip_set.emplace(gen_floor, chip_floor);
            states.emplace(gen_floor, equip_set, s.iters + 1);
        };

        if (s.elevator == LastFloor)
            add_one(s.elevator - 1, e.second, std::move(equip_set));
        else if (s.elevator == 1)
            add_one(s.elevator + 1, e.second, std::move(equip_set));
        else {
            add_one(s.elevator - 1, e.second, equip_set);
            add_one(s.elevator + 1, e.second, std::move(equip_set));
        }
    }

    // move two gens
    for (auto it1 = gens.begin(); it1 != gens.end(); ++it1)
        for (auto it2 = next(it1); it2 != gens.end(); ++it2) {
            auto equip_set(s.equip_set);

            remove_one(equip_set, {it1->first, it1->second});
            remove_one(equip_set, {it2->first, it2->second});

            auto add_one = [&] (auto gen_floor, auto chip1_floor, auto chip2_floor,
                auto equip_set) {
                equip_set.emplace(gen_floor, chip1_floor);
                equip_set.emplace(gen_floor, chip2_floor);
                states.emplace(gen_floor, equip_set, s.iters + 1);
            };

            if (s.elevator == LastFloor)
                add_one(s.elevator - 1, it1->second, it2->second, std::move(equip_set));
            else if (s.elevator == 1)
                add_one(s.elevator + 1, it1->second, it2->second, std::move(equip_set));
            else {
                add_one(s.elevator - 1, it1->second, it2->second, equip_set);
                add_one(s.elevator + 1, it1->second, it2->second, std::move(equip_set));
            }
        }

    // move one chip
    for (const auto& e : chips) {
        auto equip_set(s.equip_set);

        remove_one(equip_set, {e.first, e.second});

        auto add_one = [&] (auto gen_floor, auto chip_floor, auto equip_set) {
            equip_set.emplace(gen_floor, chip_floor);
            states.emplace(chip_floor, equip_set, s.iters + 1);
        };

        if (s.elevator == LastFloor)
            add_one(e.first, s.elevator - 1, std::move(equip_set));
        else if (s.elevator == 1)
            add_one(e.first, s.elevator + 1, std::move(equip_set));
        else {
            add_one(e.first, s.elevator - 1, equip_set);
            add_one(e.first, s.elevator + 1, std::move(equip_set));
        }
    }

    // move two chips
    for (auto it1 = chips.begin(); it1 != chips.end(); ++it1)
        for (auto it2 = next(it1); it2 != chips.end(); ++it2) {
            auto equip_set(s.equip_set);

            remove_one(equip_set, {it1->first, it1->second});
            remove_one(equip_set, {it2->first, it2->second});

            auto add_one = [&] (auto gen1_floor, auto gen2_floor, auto chip_floor,
                auto equip_set) {
                equip_set.emplace(gen1_floor, chip_floor);
                equip_set.emplace(gen2_floor, chip_floor);
                states.emplace(chip_floor, equip_set, s.iters + 1);
            };

            if (s.elevator == LastFloor)
                add_one(it1->first, it2->first, s.elevator - 1, std::move(equip_set));
            else if (s.elevator == 1)
                add_one(it1->first, it2->first, s.elevator + 1, std::move(equip_set));
            else {
                add_one(it1->first, it2->first, s.elevator - 1, equip_set);
                add_one(it1->first, it2->first, s.elevator + 1, std::move(equip_set));
            }
        }

    // filter invalid states
    for (auto it = states.begin(); it != states.end();)
        if (is_valid_state(*it))
            ++it;
        else
            it = states.erase(it);

    return states;
}

// A* search
auto count_iters(const State& s)
{
    std::set<State> open_set;
    std::set<State> closed_set;

    open_set.emplace(s);

    while (!open_set.empty()) {
        auto curr = *open_set.begin();
        open_set.erase(open_set.begin());

        if (is_final_state(curr))
            return curr.iters;

        closed_set.emplace(curr);

        auto next_s = next_states(curr);

        for (const auto& next : next_s)
            if (closed_set.find(next) == closed_set.end()) {
                auto it = open_set.find(next);
                if (it == open_set.end()) {
                    open_set.emplace(next);
                } else {
                    if (it->score > next.score ||
                        (it->score == next.score && it->iters >= next.iters)) {
                        open_set.erase(it);
                        open_set.emplace(next);
                    }
                }
            }
    }

    return -1;
}

int main()
{
    auto equip_set = parse_input();

    State s1{1, equip_set, 0};
    std::cout << count_iters(s1) << "\n";

    equip_set.emplace(1, 1);
    equip_set.emplace(1, 1);
    State s2{1, equip_set, 0};
    std::cout << count_iters(s2) << "\n";

    return 0;
}

