#include <bits/stdc++.h>

using namespace std;

struct Move {
    char dir;
    int steps;
};

struct Point {
    int x{0};
    int y{0};

    auto operator<=>(const Point&) const = default;

    [[nodiscard]] auto is_touching(const Point& p) const
    {
        return abs(x - p.x) < 2 && abs(y - p.y) < 2;
    }

    [[nodiscard]] auto get_dir_to(const Point& p) const
    {
        auto get_dir = [](auto from, auto to) {
            if (from < to)
                return 1;
            else if (from > to)
                return -1;
            else
                return 0;
        };

        return make_pair(get_dir(x, p.x), get_dir(y, p.y));
    }
};

auto solve(const vector<Move>& moves, int num_points)
{
    vector<Point> points(num_points);

    set<Point> uniq;
    uniq.emplace(0, 0);

    for (auto [dir, steps] : moves)
        while (steps--) {
            switch (dir) {
            case 'U':
                ++points[0].x;
                break;
            case 'D':
                --points[0].x;
                break;
            case 'L':
                --points[0].y;
                break;
            case 'R':
                ++points[0].y;
                break;
            }

            for (auto i = 1; i < num_points; ++i)
                while (!points[i].is_touching(points[i - 1])) {
                    auto [dx, dy] = points[i].get_dir_to(points[i - 1]);
                    points[i].x += dx;
                    points[i].y += dy;

                    if (i == num_points - 1)
                        uniq.emplace(points[i]);
                }
        }

    return uniq.size();
}

int main()
{
    vector<Move> moves;

    string line;
    while (getline(cin, line)) {
        Move m;
        sscanf(line.data(), "%c %d", &m.dir, &m.steps);
        moves.push_back(m);
    }

    cout << solve(moves, 2) << '\n';
    cout << solve(moves, 10) << '\n';

    return 0;
}

