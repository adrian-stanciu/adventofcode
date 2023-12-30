#include <charconv>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "hasher.hpp"
#include "parser.hpp"

struct Rectangle {
    int id;
    int from_left;
    int from_top;
    int width;
    int height;
};

auto count_overlapping(const std::vector<Rectangle>& rectangles)
{
    std::unordered_set<std::pair<int, int>, PairHasher<int>> original;
    std::unordered_set<std::pair<int, int>, PairHasher<int>> overlapped;

    for (auto r : rectangles)
        for (auto i = r.from_left; i < r.from_left + r.width; ++i)
            for (auto j = r.from_top; j < r.from_top + r.height; ++j) {
                auto p = std::make_pair(i, j);
                auto res = original.emplace(p);
                if (!res.second)
                    overlapped.emplace(move(p));
            }

    return overlapped.size();
}

auto get_untouched(const std::vector<Rectangle>& rectangles)
{
    std::unordered_map<std::pair<int, int>, int, PairHasher<int>> map;

    for (auto r : rectangles)
        for (auto i = r.from_left; i < r.from_left + r.width; ++i)
            for (auto j = r.from_top; j < r.from_top + r.height; ++j) {
                auto p = std::make_pair(i, j);
                auto it = map.find(p);
                if (it == map.end())
                    map[move(p)] = r.id;
                else
                    it->second = 0; // overlapping
            }

    for (auto r : rectangles) {
        bool untouched = true;

        for (auto i = r.from_left; untouched && i < r.from_left + r.width; ++i)
            for (auto j = r.from_top; j < r.from_top + r.height; ++j) {
                auto p = std::make_pair(i, j);
                if (map[move(p)] == 0) {
                    untouched = false;
                    break;
                }
            }

        if (untouched)
            return r.id;
    }

    return 0;
}

int main()
{
    // #CLAIM_NUMBER @ FROM_LEFT,FROM_TOP: WIDTHxHEIGHT
    static const std::regex re {"#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)"};

    std::vector<Rectangle> rectangles;

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;
        regex_match(line, matched, re);

        int claim_num = str2num(matched[1].str());
        int from_left = str2num(matched[2].str());
        int from_top = str2num(matched[3].str());
        int width = str2num(matched[4].str());
        int height = str2num(matched[5].str());

        rectangles.push_back({claim_num, from_left, from_top, width, height});
    }

    std::cout << count_overlapping(rectangles) << "\n";
    std::cout << get_untouched(rectangles) << "\n";

    return 0;
}

