#include <bits/stdc++.h>

using namespace std;

struct Food {
    set<string> ingredients;
    set<string> allergens;

    Food(const string& line)
    {
        static const regex r{R"((.+) \(contains (.+)\))"};

        smatch matched;
        regex_match(line, matched, r);

        stringstream ingredients_ss{matched[1].str()};
        string ingredient;
        while (ingredients_ss >> ingredient)
            ingredients.insert(move(ingredient));

        stringstream allergens_ss{matched[2].str()};
        string allergen;
        while (allergens_ss >> allergen) {
            if (allergen.back() == ',')
                allergen.pop_back();
            allergens.insert(move(allergen));
        }
    }
};

void solve(const vector<Food>& foods)
{
    unordered_set<string> all_a;
    set<string> all_i;
    unordered_map<string, int> i_freq;
    for (const auto& [ingredients, allergens] : foods) {
        for (const auto& a : allergens)
            all_a.insert(a);
        for (const auto& i : ingredients) {
            all_i.insert(i);
            ++i_freq[i];
        }
    }

    set<string> all_bad_i;
    unordered_map<string, set<string>> candidate_a2i;
    for (const auto& a : all_a) {
        auto bad_i{all_i};

        for (const auto& [ingredients, allergens] : foods)
            if (allergens.count(a) > 0) {
                set<string> new_bad_i;
                set_intersection(begin(bad_i), end(bad_i), begin(ingredients), end(ingredients),
                    inserter(new_bad_i, begin(new_bad_i)));
                bad_i.swap(new_bad_i);
            }

        copy(begin(bad_i), end(bad_i), inserter(all_bad_i, begin(all_bad_i)));
        candidate_a2i.emplace(a, move(bad_i));
    }

    auto sum_freq = [&] (const auto& ingredients) {
        return accumulate(begin(ingredients), end(ingredients), 0,
            [&] (auto acc, const auto& i) {
                return acc + i_freq[i];
            });
    };
    cout << sum_freq(all_i) - sum_freq(all_bad_i) << '\n';

    map<string, string> a2i;
    while (!candidate_a2i.empty())
        for (const auto& [a, bad_i] : candidate_a2i)
            if (bad_i.size() == 1) {
                auto i = *begin(bad_i);
                a2i.emplace(a, i);
                for (auto& [_, bad_i] : candidate_a2i)
                    bad_i.erase(i);
                candidate_a2i.erase(a);
                break;
            }

    string bad_i_list;
    for (const auto& [_, i] : a2i) {
        bad_i_list.append(i);
        bad_i_list.push_back(',');
    }
    bad_i_list.pop_back();
    cout << bad_i_list << '\n';
}

int main()
{
    vector<Food> foods;

    string line;
    while (getline(cin, line))
        foods.emplace_back(line);

    solve(foods);

    return 0;
}

