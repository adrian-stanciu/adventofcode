#include <iostream>
#include <unordered_set>
#include <vector>

#include "hasher.h"

auto move = [] (auto c, auto& x, auto& y) {
    switch (c) {
    case '^':
        ++y;
        break;
    case 'v':
        --y;
        break;
    case '<':
        --x;
        break;
    case '>':
        ++x;
        break;
    }
};

auto count1(const std::vector<char>& path)
{
    auto x = 0;
    auto y = 0;

    std::unordered_set<std::pair<int, int>, PairHasher<int>> uniq;
    uniq.emplace(x, y);

    for (auto c : path) {
        move(c, x, y);
        uniq.emplace(x, y);
    }

    return uniq.size();
}

auto count2(const std::vector<char>& path)
{
    auto x1 = 0;
    auto y1 = 0;
    auto x2 = 0;
    auto y2 = 0;

    std::unordered_set<std::pair<int, int>, PairHasher<int>> uniq;
    uniq.emplace(x1, y1);

    for (auto i = 0U; i < path.size(); ++i)
        if (i % 2) {
            move(path[i], x1, y1);
            uniq.emplace(x1, y1);
        } else {
            move(path[i], x2, y2);
            uniq.emplace(x2, y2);
        }

    return uniq.size();
}

int main()
{
    std::vector<char> path;

    char c;
    while (std::cin >> c)
        path.push_back(c);

    std::cout << count1(path) << "\n";
    std::cout << count2(path) << "\n";

    return 0;
}

