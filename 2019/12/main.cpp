#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "hasher.hpp"

struct Point {
    long x;
    long y;
    long z;

    long vx{0};
    long vy{0};
    long vz{0};

    Point(long x, long y, long z)
    : x(x), y(y), z(z)
    {}
};

auto simulate(std::vector<Point> points, long iters)
{
    while (iters--) {
        for (auto p1 = 0U; p1 < points.size(); ++p1)
            for (auto p2 = p1 + 1; p2 < points.size(); ++p2) {
                if (points[p1].x < points[p2].x) {
                    ++points[p1].vx;
                    --points[p2].vx;
                } else if (points[p1].x > points[p2].x) {
                    --points[p1].vx;
                    ++points[p2].vx;
                }

                if (points[p1].y < points[p2].y) {
                    ++points[p1].vy;
                    --points[p2].vy;
                } else if (points[p1].y > points[p2].y) {
                    --points[p1].vy;
                    ++points[p2].vy;
                }

                if (points[p1].z < points[p2].z) {
                    ++points[p1].vz;
                    --points[p2].vz;
                } else if (points[p1].z > points[p2].z) {
                    --points[p1].vz;
                    ++points[p2].vz;
                }
            }

        for (auto& p : points) {
            p.x += p.vx;
            p.y += p.vy;
            p.z += p.vz;
        }
    }

    auto total_e = 0;

    for (const auto& p : points) {
        auto pot_e = std::abs(p.x) + std::abs(p.y) + std::abs(p.z);
        auto kin_e = std::abs(p.vx) + std::abs(p.vy) + std::abs(p.vz);
        total_e += pot_e * kin_e;
    }

    return total_e;
}

auto simulate_until_repeat(std::vector<Point> points,
    const std::function<long(const Point&)>& get_position,
    const std::function<long(const Point&)>& get_velocity,
    const std::function<void(Point&, long)>& update_position,
    const std::function<void(Point&, long)>& update_velocity)
{
    auto iter = 0L;

    std::unordered_set<std::vector<long>, VectorHasher<long>> uniq;

    std::vector<long> v;
    for (const auto& p : points) {
        v.push_back(get_position(p));
        v.push_back(get_velocity(p));
    }
    uniq.insert(v);

    while (true) {
        ++iter;

        for (auto p1 = 0U; p1 < points.size(); ++p1)
            for (auto p2 = p1 + 1; p2 < points.size(); ++p2) {
                auto p1_pos = get_position(points[p1]);
                auto p2_pos = get_position(points[p2]);

                if (p1_pos < p2_pos) {
                    update_velocity(points[p1], 1);
                    update_velocity(points[p2], -1);
                } else if (p1_pos > p2_pos) {
                    update_velocity(points[p1], -1);
                    update_velocity(points[p2], 1);
                }
            }

        for (auto& p : points)
            update_position(p, get_velocity(p));

        std::vector<long> v;
        for (const auto& p : points) {
            v.push_back(get_position(p));
            v.push_back(get_velocity(p));
        }
        if (!uniq.insert(v).second)
            break;
    }

    return iter;
}

auto gcd(long a, long b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

auto lcm(long a, long b)
{
    if (a == 0 || b == 0)
        return 0L;

    return a * b / gcd(a, b);
}

int main()
{
    std::vector<Point> points;

    std::string line;
    while (getline(std::cin, line)) {
        long x, y, z;
        sscanf(line.data(), "<x=%ld, y=%ld, z=%ld>", &x, &y, &z);
        points.emplace_back(x, y, z);
    }

    std::cout << simulate(points, 1000) << "\n";

    auto iters_x = simulate_until_repeat(points,
        [] (const Point& p) { return p.x; },
        [] (const Point& p) { return p.vx; },
        [] (Point& p, long delta) { p.x += delta; },
        [] (Point& p, long delta) { p.vx += delta; });

    auto iters_y = simulate_until_repeat(points,
        [] (const Point& p) { return p.y; },
        [] (const Point& p) { return p.vy; },
        [] (Point& p, long delta) { p.y += delta; },
        [] (Point& p, long delta) { p.vy += delta; });

    auto iters_z = simulate_until_repeat(points,
        [] (const Point& p) { return p.z; },
        [] (const Point& p) { return p.vz; },
        [] (Point& p, long delta) { p.z += delta; },
        [] (Point& p, long delta) { p.vz += delta; });

    std::cout << lcm(lcm(iters_x, iters_y), iters_z) << "\n";

    return 0;
}

