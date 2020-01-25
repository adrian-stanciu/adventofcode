#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Disc {
    long id;
    long sz;
    long off;

    Disc(long id, long sz, long off)
    : id(id), sz(sz), off(off)
    {}
};

bool passthrough(const std::vector<Disc>& discs, long t)
{
    for (const auto& d : discs)
        if ((d.off + t + d.id) % d.sz != 0)
            return false;

    return true;
}

auto compute_first_passthrough_time(const std::vector<Disc>& discs)
{
    auto t = 0L;

    while (true) {
        if (passthrough(discs, t))
            return t;

        ++t;
    }
}

int main()
{
    static const std::regex re{"Disc #([1-9][0-9]*) has ([1-9][0-9]*) positions; at time=0, it is at position ([0-9]+)."};

    std::vector<Disc> discs;

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;

        if (!regex_match(line, matched, re))
            continue;

        auto id = strtol(matched[1].str().data(), nullptr, 10);
        auto sz = strtol(matched[2].str().data(), nullptr, 10);
        auto off = strtol(matched[3].str().data(), nullptr, 10);

        discs.emplace_back(id, sz, off);
    }

    sort(discs.begin(), discs.end(), [] (const auto& d1, const auto& d2) {
        return d1.id < d2.id;
    });

    std::cout << compute_first_passthrough_time(discs) << "\n";

    discs.emplace_back(discs.size() + 1, 11, 0);

    std::cout << compute_first_passthrough_time(discs) << "\n";

    return 0;
}

