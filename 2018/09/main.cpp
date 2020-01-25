#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>

auto score(int num_players, int last_val)
{
    std::vector<size_t> scores(num_players, 0);

    std::list<int> circle;
    circle.push_back(0);

    auto it = circle.begin();
    auto val = 1;

    while (val <= last_val)
        for (auto p = 0; p < num_players && val <= last_val; ++p) {
            if (val % 23 == 0) {
                scores[p] += val;

                auto i = 0;
                while (i < 7) {
                    if (it == circle.begin())
                        it = circle.end();
                    --it;
                    ++i;
                }

                scores[p] += *it;

                it = circle.erase(it);
                if (it == circle.end())
                    it = circle.begin();
            } else {
                ++it;
                if (it == circle.end())
                    it = circle.begin();
                ++it;

                circle.insert(it, val);
                --it;
            }
            ++val;
        }

    return *std::max_element(scores.begin(), scores.end());
}

int main()
{
    assert(score(10, 1618) == 8317);
    assert(score(13, 7999) == 146373);
    assert(score(17, 1104) == 2764);
    assert(score(21, 6111) == 54718);
    assert(score(30, 5807) == 37305);

    std::string line;
    getline(std::cin, line);

    int num_players, last_val;
    sscanf(line.data(), "%d players; last marble is worth %d points",
        &num_players, &last_val);

    std::cout << score(num_players, last_val) << "\n";
    std::cout << score(num_players, 100 * last_val) << "\n";

    return 0;
}

