#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
    int x;
    int y;

    Point() = default;

    Point(int x, int y)
    : x(x), y(y)
    {}
};

bool operator==(const Point& p1, const Point& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const Point& p1, const Point& p2)
{
    return !(p1 == p2);
}

struct PointHasher {
    auto operator()(const Point& p) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= p.x + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= p.y + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto gcd(int a, int b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

struct Slope {
    int delta_y;
    int delta_x;
    int delta_sign;

    Slope(int dy, int dx)
    {
        auto gcd_dy_dx = gcd(dy, dx);

        delta_y = dy / gcd_dy_dx;
        delta_x = dx / gcd_dy_dx;

        if (dy < 0)
            delta_sign = -1;
        else if (dy > 0)
            delta_sign = 1;
        else {
            if (dx < 0)
                delta_sign = -1;
            else if (dx > 0)
                delta_sign = 1;
        }
    }
};

bool operator==(const Slope& s1, const Slope& s2)
{
    return s1.delta_y == s2.delta_y && s1.delta_x == s2.delta_x && s1.delta_sign == s2.delta_sign;
}

struct SlopeHasher {
    auto operator()(const Slope& s) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= s.delta_y + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= s.delta_x + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

auto find_max_sight(const std::unordered_set<Point, PointHasher>& points)
{
    Point max_p(-1, -1);
    auto max_cnt = 0U;

    for (const auto& src_p : points) {
        // only 2 points can be in sight on the same slope
        std::unordered_set<Slope, SlopeHasher> slopes;

        for (const auto& dst_p : points)
            if (src_p != dst_p)
                slopes.emplace(dst_p.y - src_p.y, dst_p.x - src_p.x);

        if (slopes.size() > max_cnt) {
            max_cnt = slopes.size();
            max_p = src_p;
        }
    }

    return std::make_pair(max_p, max_cnt);
}

bool are_collinear(const Point& p1, const Point& p2, const Point& p3)
{
    return (p2.y - p1.y) * (p3.x - p2.x) == (p3.y - p2.y) * (p2.x - p1.x);
}

bool is_on_segment(const Point& p, const Point& src_p, const Point& dst_p)
{
    if (p.x < std::min(src_p.x, dst_p.x) || p.x > std::max(src_p.x, dst_p.x))
        return false;

    if (p.y < std::min(src_p.y, dst_p.y) || p.y > std::max(src_p.y, dst_p.y))
        return false;

    return are_collinear(p, src_p, dst_p);
}

bool are_visible(const Point& src_p, const Point& dst_p,
    const std::unordered_set<Point, PointHasher>& points)
{
    for (const auto& p : points)
        if (p != src_p && p != dst_p && is_on_segment(p, src_p, dst_p))
            return false;

    return true;
}

auto n_vaporized(const Point& src_p, std::unordered_set<Point, PointHasher> points, unsigned int n)
{
    if (n <= 0 || n > points.size())
        return -1;

    points.erase(src_p);

    while (true) {
        std::unordered_set<Point, PointHasher> targets;

        for (const auto& dst_p : points)
            if (are_visible(src_p, dst_p, points))
                targets.insert(dst_p);

        if (n > targets.size()) {
            n -= targets.size();

            for (const auto& t : targets)
                points.erase(t);

            continue;
        }

        // consider src_p as origin
        // points above origin have y < 0
        // points to the left of origin have x < 0

        std::array<std::optional<Point>, 4> on_axes;
        std::array<std::vector<Point>, 4> quadrants;

        for (const auto& t : targets) {
            Point p{t.x - src_p.x, t.y - src_p.y};

            if (p.x == 0) {
                if (p.y < 0)
                    on_axes[0] = p;
                else
                    on_axes[2] = p;
            } else if (p.y == 0) {
                if (p.x > 0)
                    on_axes[1] = p;
                else
                    on_axes[3] = p;
            } else if (p.x > 0) {
                if (p.y < 0)
                    quadrants[0].push_back(p);
                else
                    quadrants[1].push_back(p);
            } else {
                if (p.y < 0)
                    quadrants[3].push_back(p);
                else
                    quadrants[2].push_back(p);
            }
        }

        for (auto i = 0; i < 4; ++i) {
            if (on_axes[i]) {
                --n;
                if (n == 0)
                    return 100 * (on_axes[i].value().x + src_p.x) +
                        (on_axes[i].value().y + src_p.y);
            }

            if (n <= quadrants[i].size()) {
                if (i % 2 == 0)
                    sort(quadrants[i].begin(), quadrants[i].end(),
                        [&] (const Point& p1, const Point& p2) {
                            return abs(p1.y) * abs(p2.x) > abs(p2.y) * abs(p1.x);
                        });
                else
                    sort(quadrants[i].begin(), quadrants[i].end(),
                        [&] (const Point& p1, const Point& p2) {
                            return abs(p1.y) * abs(p2.x) < abs(p2.y) * abs(p1.x);
                        });

                return 100 * (quadrants[i][n - 1].x + src_p.x) +
                    (quadrants[i][n - 1].y + src_p.y);
            } else
                n -= quadrants[i].size();
        }
    }
}

int main()
{
    std::unordered_set<Point, PointHasher> points;

    auto y = 0;
    std::string line;
    while (getline(std::cin, line)) {
        for (auto x = 0U; x < line.size(); ++x)
            if (line[x] == '#')
                points.emplace(x, y);
        ++y;
    }

    auto max_sight = find_max_sight(points);

    std::cout << max_sight.second << "\n";

    std::cout << n_vaporized(max_sight.first, points, 200) <<"\n";

    return 0;
}

