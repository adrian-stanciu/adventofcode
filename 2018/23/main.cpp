#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

struct Point {
    long x;
    long y;
    long z;

    Point() = default;

    Point(long x, long y, long z)
    : x(x), y(y), z(z)
    {}
};

bool operator<(const Point& p1, const Point& p2)
{
    if (p1.x == p2.x)
        if (p1.y == p2.y)
            return p1.z < p2.z;
        else
            return p1.y < p2.y;
    else
        return p1.x < p2.x;
}

auto dist_p2p(const Point& p1, const Point& p2)
{
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y) + std::abs(p1.z - p2.z);
}

auto dist_p2orig(const Point& p)
{
    return std::abs(p.x) + std::abs(p.y) + std::abs(p.z);
}

struct Robot {
    Point p;
    long r;

    Robot(long x, long y, long z, long r)
    : p(x, y, z), r(r)
    {}
};

auto count_in_range(const std::vector<Robot>& robots, long max_r)
{
    long x = 0;
    long y = 0;
    long z = 0;

    for (const auto& robot : robots) {
        if (robot.r == max_r) {
            x = robot.p.x;
            y = robot.p.y;
            z = robot.p.z;
            break;
        }
    }

    auto in_range = [x, y, z, max_r] (const auto& robot) {
        auto d = std::abs(x - robot.p.x) + std::abs(y - robot.p.y) + std::abs(z - robot.p.z);
        return d <= max_r;
    };

    auto count = 0;

    for (const auto& robot : robots)
        if (in_range(robot))
            ++count;

    return count;
}

auto ceil_to_power_of_2(long n)
{
    if (n <= 0)
        return 1L;

    // check if it is a power of 2
    if ((n & (n - 1)) == 0)
        return n;

    long prev_n;

    while (n) {
        prev_n = n;

        // reset lsb
        n = n & (n - 1);
    }

    return 2 * prev_n;
}

struct Cube {
    Point p; // bottom-left-front point
    long l;

    Cube(long x, long y, long z, long l)
    : p(x, y, z), l(l)
    {}
};

bool operator<(const Cube& c1, const Cube& c2)
{
    if (c1.l == c2.l)
        return c1.p < c2.p;
    else
        return c1.l < c2.l;
}

auto find_closest_point_in_cube_to_point(const Point& p, const Cube& c)
{
    auto x = std::clamp(p.x, c.p.x, c.p.x + c.l);
    auto y = std::clamp(p.y, c.p.y, c.p.y + c.l);
    auto z = std::clamp(p.z, c.p.z, c.p.z + c.l);

    return Point{x, y, z};
}

auto dist_p2cube(const Point& p, const Cube& c)
{
    return dist_p2p(p, find_closest_point_in_cube_to_point(p, c));
}

auto count_robots_in_cube(const std::vector<Robot>& robots, const Cube& cube)
{
    auto count = 0;

    for (const auto& robot : robots) {
        auto d = dist_p2cube(robot.p, cube);
        if (d <= robot.r)
            ++count;
    }

    return count;
}

auto dist_to_closest_point_in_range_of_most_robots(const std::vector<Robot>& robots, Cube cube)
{
    const Point vertices[] = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {1, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
        {0, 1, 1},
        {1, 1, 1},
    };

    std::priority_queue<std::pair<long, Cube>> pq;
    pq.push(std::make_pair(robots.size(), std::move(cube)));

    auto max_cnt = 0;
    auto max_cnt_dst = 0;
    Point sol {};
    bool first_candidate = true;

    while (!pq.empty()) {
        auto [cnt, cube] = pq.top();
        pq.pop();

        // remaining cubes have less robots
        if (cnt < max_cnt)
            break;

        if (cube.l == 0) {
            // check if this point is the closest point in range of most robots
            if (first_candidate) {
                // first point reached; take it as the first candidate
                max_cnt = cnt;
                max_cnt_dst = dist_p2orig(cube.p);
                sol = cube.p;
                first_candidate = false;
            } else {
                // by reaching here it means that
                // multiple points are in range of most robots;
                // check if this point is closer than the current candidate
                auto d = dist_p2orig(cube.p);
                if (d < max_cnt_dst) {
                    max_cnt_dst = d;
                    sol = cube.p;
                }
            }
        } else {
            // devide cube into 8 sub-cubes
            auto new_l = cube.l / 2;

            for (const auto& vertex : vertices) {
                auto new_x = cube.p.x + vertex.x * new_l;
                auto new_y = cube.p.y + vertex.y * new_l;
                auto new_z = cube.p.z + vertex.z * new_l;
                Cube new_cube {new_x, new_y, new_z, new_l};

                auto cnt = count_robots_in_cube(robots, new_cube);
                if (cnt > 0)
                    pq.push(std::make_pair(cnt, std::move(new_cube)));
            }
        }
    }

    return dist_p2orig(sol);
}

int main()
{
    auto max_x = std::numeric_limits<long>::min();
    auto max_y = std::numeric_limits<long>::min();
    auto max_z = std::numeric_limits<long>::min();
    auto min_x = std::numeric_limits<long>::max();
    auto min_y = std::numeric_limits<long>::max();
    auto min_z = std::numeric_limits<long>::max();
    auto max_r = std::numeric_limits<long>::min();

    std::vector<Robot> robots;

    std::string line;
    while (getline(std::cin, line)) {
        long x, y, z, r;
        sscanf(line.data(), "pos=<%ld,%ld,%ld>, r=%ld", &x, &y, &z, &r);

        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
        max_z = std::max(max_z, z);
        min_x = std::min(min_x, x);
        min_y = std::min(min_y, y);
        min_z = std::min(min_z, z);
        max_r = std::max(max_r, r);

        robots.emplace_back(x, y, z, r);
    }

    std::cout << count_in_range(robots, max_r) << "\n";

    auto max_span = 0L;
    max_span = std::max(max_span, max_x - min_x + 1);
    max_span = std::max(max_span, max_y - min_y + 1);
    max_span = std::max(max_span, max_z - min_z + 1);

    std::cout << dist_to_closest_point_in_range_of_most_robots(robots,
        Cube{min_x, min_y, min_z, ceil_to_power_of_2(max_span)}) << "\n";

    return 0;
}

