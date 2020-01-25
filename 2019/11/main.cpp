#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "hasher.h"
#include "int_computer.h"

struct Cell {
    long long x;
    long long y;

    Cell(long long x, long long y)
    : x(x), y(y)
    {}
};

bool operator==(const Cell& c1, const Cell& c2)
{
    return c1.x == c2.x && c1.y == c2.y;
}

struct CellHasher {
    auto operator()(const Cell& c) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= c.x + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= c.y + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

enum Color {
    Black = 0,
    White = 1,
};

enum Orientation {
    N,
    S,
    E,
    W
};

enum Dir {
    Left = 0,
    Right = 1,
};

using Map = std::unordered_map<Cell, Color, CellHasher>;

auto compute_map(const std::vector<long long>& prog, Color starting_color)
{
    Map map;
    Cell c{0, 0};
    Orientation o{N};

    std::unordered_map<std::pair<Orientation, Dir>, Orientation, PairHasher<int>> o_map {
        {{N, Left}, W}, {{N, Right}, E},
        {{S, Left}, E}, {{S, Right}, W},
        {{E, Left}, N}, {{E, Right}, S},
        {{W, Left}, S}, {{W, Right}, N},
    };
    std::unordered_map<Orientation, int> dx{{E, 1}, {W, -1}};
    std::unordered_map<Orientation, int> dy{{N, 1}, {S, -1}};

    IntComputer ic(prog);

    ic.append_input(starting_color);

    while (true) {
        if (!ic.run())
            break;

        map[c] = static_cast<Color>(*ic.get_last_output());

        if (!ic.run())
            break;

        o = o_map[std::make_pair(o, static_cast<Dir>(*ic.get_last_output()))];

        c.x += dx[o];
        c.y += dy[o];

        ic.append_input(map[c]);
    }

    return map;
}

void paint(const Map& map)
{
    auto min_x = 0LL;
    auto max_x = 0LL;
    auto min_y = 0LL;
    auto max_y = 0LL;

    for (const auto& p : map) {
        min_x = std::min(min_x, p.first.x);
        max_x = std::max(max_x, p.first.x);
        min_y = std::min(min_y, p.first.y);
        max_y = std::max(max_y, p.first.y);
    }

    std::vector<std::vector<Color>> img(max_y - min_y + 1,
        std::vector<Color>(max_x - min_x + 1, Black));

    for (const auto& p : map)
        img[p.first.y - min_y][p.first.x - min_x] = p.second;

    for (auto i = static_cast<int>(img.size()) - 1; i >= 0; --i) {
        for (auto j = 0U; j < img[i].size(); ++j)
            if (img[i][j] == Black)
                std::cout << ' ';
            else
                std::cout << '#';
        std::cout << "\n";
    }
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << compute_map(prog, Black).size() << "\n";

    paint(compute_map(prog, White));

    return 0;
}

