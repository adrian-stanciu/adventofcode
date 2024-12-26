#include <algorithm>
#include <iostream>
#include <vector>

void print_10_after(int n)
{
    std::vector<int> recipes {3, 7};
    auto e1 = 0;
    auto e2 = 1;

    while (ssize(recipes) < n + 10) {
        auto r1 = recipes[e1];
        auto r2 = recipes[e2];

        auto r = r1 + r2;
        if (r > 9) {
            recipes.push_back(1);
            recipes.push_back(r % 10);
        } else {
            recipes.push_back(r);
        }

        e1 += 1 + r1;
        e1 %= recipes.size();
        e2 += 1 + r2;
        e2 %= recipes.size();
    }

    for (auto i = n; i < n + 10; ++i)
        std::cout << recipes[i];
    std::cout << "\n";
}

auto count_until(int n)
{
    std::vector<int> pattern;
    while (n) {
        auto d = n % 10;
        pattern.insert(pattern.begin(), d);
        n /= 10;
    }
    auto pattern_len = ssize(pattern);

    std::vector<int> recipes {3, 7};
    auto e1 = 0;
    auto e2 = 1;

    auto last_recipes_match_pattern = [&recipes, &pattern, pattern_len]() {
        auto recipes_len = ssize(recipes);

        if (recipes_len >= pattern_len) {
            for (auto i = 0U; i < pattern_len; ++i)
                if (recipes[recipes_len - pattern_len + i] != pattern[i])
                    return false;
            return true;
        }

        return false;
    };

    while (true) {
        auto r1 = recipes[e1];
        auto r2 = recipes[e2];

        auto r = r1 + r2;
        if (r > 9) {
            recipes.push_back(1);
            if (last_recipes_match_pattern())
                break;

            recipes.push_back(r % 10);
            if (last_recipes_match_pattern())
                break;
        } else {
            recipes.push_back(r);
            if (last_recipes_match_pattern())
                break;
        }

        e1 += 1 + r1;
        e1 %= recipes.size();
        e2 += 1 + r2;
        e2 %= recipes.size();
    }

    return recipes.size() - pattern.size();
}

int main()
{
    int n;
    std::cin >> n;

    print_10_after(n);
    std::cout << count_until(n) << "\n";

    return 0;
}

