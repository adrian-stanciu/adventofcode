#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "hasher.hpp"

struct Point {
    int x;
    int y;
};

bool operator==(const Point& p1, const Point& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const Point& p1, const Point& p2)
{
    return !(p1 == p2);
}

enum Dir {
    Left = 'L',
    Right = 'R',
};

struct Move {
    Dir dir;
    int len;

    Move(char dir, int len)
    : dir(static_cast<Dir>(dir)), len(len)
    {}
};

auto read_moves()
{
    std::vector<Move> moves;

    std::string move;
    while (getline(std::cin, move, ',')) {
        auto pos = move.find_first_not_of(' ');
        moves.emplace_back(move[pos], atoi(move.data() + pos + 1));
    }

    return moves;
}

enum Orientation {
    N,
    S,
    E,
    W
};

auto find_location(const std::vector<Move>& moves)
{
    std::unordered_map<std::pair<Orientation, Dir>, Orientation, PairHasher<int>> o_map {
        {{N, Left}, W}, {{N, Right}, E},
        {{S, Left}, E}, {{S, Right}, W},
        {{E, Left}, N}, {{E, Right}, S},
        {{W, Left}, S}, {{W, Right}, N},
    };
    std::unordered_map<Orientation, int> dx{{E, 1}, {W, -1}};
    std::unordered_map<Orientation, int> dy{{N, 1}, {S, -1}};

    Point p{0, 0};
    Orientation o{N};

    for (const auto& m : moves) {
        o = o_map[std::make_pair(o, m.dir)];

        p.x += dx[o] * m.len;
        p.y += dy[o] * m.len;
    }

    return p;
}

struct Segment {
    Point p1;
    Point p2;
};

bool intersect(const Segment& x_seg, const Segment& y_seg)
{
    if (x_seg.p1.x > std::max(y_seg.p1.x, y_seg.p2.x) || x_seg.p1.x < std::min(y_seg.p1.x, y_seg.p2.x))
        return false;
    if (y_seg.p1.y > std::max(x_seg.p1.y, x_seg.p2.y) || y_seg.p1.y < std::min(x_seg.p1.y, x_seg.p2.y))
        return false;

    return true;
}

auto find_first_revisited_location(const std::vector<Move>& moves)
{
    std::unordered_map<std::pair<Orientation, Dir>, Orientation, PairHasher<int>> o_map {
        {{N, Left}, W}, {{N, Right}, E},
        {{S, Left}, E}, {{S, Right}, W},
        {{E, Left}, N}, {{E, Right}, S},
        {{W, Left}, S}, {{W, Right}, N},
    };
    std::unordered_map<Orientation, int> dx{{E, 1}, {W, -1}};
    std::unordered_map<Orientation, int> dy{{N, 1}, {S, -1}};

    Point p{0, 0};
    Orientation o{N};

    std::vector<Segment> x_segments;
    std::vector<Segment> y_segments;

    auto check_intersection = [] (const auto& x_seg, const auto& y_seg) -> std::optional<Point> {
        if (intersect(x_seg, y_seg))
            return Point{x_seg.p1.x, y_seg.p1.y};
        else
            return {};
    };

    for (const auto& m : moves) {
        o = o_map[std::make_pair(o, m.dir)];

        const Point next_p{p.x + dx[o] * m.len, p.y + dy[o] * m.len};

        if (p.x == next_p.x) {
            const Segment x_seg(p, next_p);

            for (const auto& y_seg : y_segments) {
                auto i = check_intersection(x_seg, y_seg);
                if (i.has_value() && i.value() != p)
                    return i.value();
            }

            x_segments.push_back(x_seg);
        } else {
            const Segment y_seg(p, next_p);

            for (const auto& x_seg : x_segments) {
                auto i = check_intersection(x_seg, y_seg);
                if (i.has_value() && i.value() != p)
                    return i.value();
            }

            y_segments.push_back(y_seg);
        }

        p = next_p;
    }

    return p;
}

int main()
{
    auto moves = read_moves();

    auto loc = find_location(moves);
    std::cout << abs(loc.x) + abs(loc.y) << "\n";

    auto loc2 = find_first_revisited_location(moves);
    std::cout << abs(loc2.x) + abs(loc2.y) << "\n";

    return 0;
}

