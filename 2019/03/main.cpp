#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <vector>

struct Move {
    char dir;
    int len;

    Move(char dir, int len)
    : dir(dir), len(len)
    {}
};

struct Segment {
    int x1;
    int y1;
    int x2;
    int y2;
    int steps;

    Segment(int x1, int y1, int x2, int y2, int steps)
    : x1(x1), y1(y1), x2(x2), y2(y2), steps(steps)
    {}
};

auto compute_segments(const std::vector<Move>& moves)
{
    std::vector<Segment> seg_x;
    std::vector<Segment> seg_y;

    auto x = 0;
    auto y = 0;
    auto steps = 0;

    for (const auto& move : moves) {
        switch (move.dir) {
        case 'L':
            seg_y.emplace_back(x, y, x - move.len, y, steps);
            x -= move.len;
            break;
        case 'R':
            seg_y.emplace_back(x, y, x + move.len, y, steps);
            x += move.len;
            break;
        case 'U':
            seg_x.emplace_back(x, y, x, y - move.len, steps);
            y -= move.len;
            break;
        case 'D':
            seg_x.emplace_back(x, y, x, y + move.len, steps);
            y += move.len;
            break;
        default:
            continue;
        }

        steps += move.len;
    }

    return make_pair(seg_x, seg_y);
}

bool intersect(const Segment& s_x, const Segment& s_y)
{
    if (s_x.x1 > std::max(s_y.x1, s_y.x2) || s_x.x1 < std::min(s_y.x1, s_y.x2))
        return false;
    if (s_y.y1 > std::max(s_x.y1, s_x.y2) || s_y.y1 < std::min(s_x.y1, s_x.y2))
        return false;

    return true;
}

void solve(const std::vector<Move>& moves1, const std::vector<Move>& moves2)
{
    auto seg1 = compute_segments(moves1);
    auto seg2 = compute_segments(moves2);

    auto min_dist = std::numeric_limits<int>::max();
    auto min_steps = std::numeric_limits<int>::max();

    auto check_intersections = [&] (const auto& seg_x, const auto& seg_y) {
        for (const auto& s_x : seg_x)
            for (const auto& s_y : seg_y)
                if (intersect(s_x, s_y)) {
                    auto x = s_x.x1;
                    auto y = s_y.y1;

                    if (x == 0 && y == 0)
                        continue;

                    min_dist = std::min(min_dist, abs(x) + abs(y));
                    min_steps = std::min(min_steps,
                        s_x.steps + abs(y - s_x.y1) +
                        s_y.steps + abs(x - s_y.x1));
                }
    };

    check_intersections(seg1.first, seg2.second);
    check_intersections(seg2.first, seg1.second);

    std::cout << min_dist << "\n";
    std::cout << min_steps << "\n";
}

auto read_moves()
{
    std::vector<Move> moves;

    std::string path;
    getline(std::cin, path);

    std::stringstream ss(path);

    std::string move;
    while (getline(ss, move, ','))
        moves.emplace_back(move[0], atoi(move.data() + 1));

    return moves;
}

int main()
{
    auto moves1 = read_moves();
    auto moves2 = read_moves();

    solve(moves1, moves2);

    return 0;
}

