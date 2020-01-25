#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Point {
    int x;
    int y;
    int vx;
    int vy;
};

constexpr auto R = 15; // assume that letters are maximum 15 chars high
constexpr auto C = 150;

bool is_in_limits(const std::vector<Point>& points, int& min_r, int& min_c)
{
    min_r = std::numeric_limits<int>::max();
    auto max_r = std::numeric_limits<int>::min();
    min_c = std::numeric_limits<int>::max();
    auto max_c = std::numeric_limits<int>::min();

    for (const auto& p : points) {
        min_r = std::min(min_r, p.y);
        max_r = std::max(max_r, p.y);
        min_c = std::min(min_c, p.x);
        max_c = std::max(max_c, p.x);
    }

    if (max_r - min_r > R)
        return false;
    if (max_c - min_c > C)
        return false;

    return true;
}

void print_msg(const std::vector<Point>& points, int min_r, int min_c)
{
    std::vector<std::vector<char>> msg(R + 1, std::vector<char>(C + 1, '.'));

    for (const auto& p : points)
        msg[p.y - min_r][p.x - min_c] = '#';

    for (auto i = 0; i < R + 1; ++i) {
        for (auto j = 0; j < C + 1; ++j)
            std::cout << msg[i][j];
        std::cout << "\n";
    }
}

void search_msg(std::vector<Point>& points)
{
    auto t = 0;
    int min_r;
    int min_c;

    do {
        ++t;
        for (auto& p : points) {
            p.x += p.vx;
            p.y += p.vy;
        }
    } while (!is_in_limits(points, min_r, min_c));

    do {
        print_msg(points, min_r, min_c);
        std::cout << t << "\n";

        ++t;
        for (auto& p : points) {
            p.x += p.vx;
            p.y += p.vy;
        }
    } while (is_in_limits(points, min_r, min_c));
}

int main()
{
    std::vector<Point> points;

    std::string line;
    while (getline(std::cin, line)) {
        int x, y, vx, vy;
        sscanf(line.data(), "position=<%d, %d> velocity=<%d, %d>",
            &x, &y, &vx, &vy);
        points.push_back({x, y, vx, vy});
    }

    search_msg(points);

    return 0;
}

