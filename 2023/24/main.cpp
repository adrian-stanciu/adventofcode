#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

using int128 = __int128;

auto trim(string_view sv, char ch = ' ')
{
    while (sv.front() == ch)
        sv.remove_prefix(1);
    while (sv.back() == ch)
        sv.remove_suffix(1);
    return sv;
}

struct Point {
    int128 x;
    int128 y;
    int128 z;
    int128 vx;
    int128 vy;
    int128 vz;

    Point(int128 x, int128 y, int128 z, int128 vx, int128 vy, int128 vz)
    : x{x}, y{y}, z{z}, vx{vx}, vy{vy}, vz{vz}
    {}

    auto operator<=>(const Point&) const = default;
};

auto intersect(const Point& p1, const Point& p2)
{
    // (x - x1) / (x2 - x1) = (y - y1) / (y2 - y1)
    // (x - x1) / vx = (y - y1) / vy
    // (x - x1) * vy = (y - y1) * vx
    // x * vy - x1 * vy = y * vx - y1 * vx
    // x * vy - y * vx = x1 * vy - y1 * vx

    // a1 * x + b1 * y = c1
    auto a1 = p1.vy;
    auto b1 = -p1.vx;
    auto c1 = p1.x * p1.vy - p1.y * p1.vx;

    // a2 * x + b2 * y = c2
    auto a2 = p2.vy;
    auto b2 = -p2.vx;
    auto c2 = p2.x * p2.vy - p2.y * p2.vx;

    // a1 * x + b1 * y = c1 | * a2
    // a2 * x + b2 * y = c2 | * -a1
    // a2 * b1 * y - a1 * b2 * y = c1 * a2 - c2 * a1
    // (a2 * b1 - a1 * b2) * y = c1 * a2 - c2 * a1
    // y = (c1 * a2 - c2 * a1) / (a2 * b1 - a1 * b2)
    // x = (c1 - b1 * y) / a1

    auto y_den = a2 * b1 - a1 * b2;
    if (y_den == 0)
        return make_tuple(false, 0.0l, 0.0l);

    auto y = 1.0l * (c1 * a2 - c2 * a1) / y_den;
    auto x = (c1 - b1 * y) / a1;

    return make_tuple(true, x, y);
}

auto solve1(int n, const vector<Point>& v, long min_coord, long max_coord)
{
    auto cnt = 0;

    for (auto i = 0; i < n; ++i)
        for (auto j = i + 1; j < n; ++j) {
            auto [intersected, x, y] = intersect(v[i], v[j]);
            if (!intersected)
                continue;

            if (x < min_coord || x > max_coord || y < min_coord || y > max_coord )
                continue;

            auto t1 = (x - v[i].x) / v[i].vx;
            auto t2 = (x - v[j].x) / v[j].vx;
            if (t1 > 0 && t2 > 0)
                ++cnt;
        }

    return cnt;
}

long solve2(int n, vector<Point>& v)
{
    auto min_vx = min_element(begin(v), end(v), [](const auto& lhs, const auto& rhs) {
        return tie(lhs.vx, lhs) < tie(rhs.vx, rhs);
    })->vx;
    auto max_vx = max_element(begin(v), end(v), [](const auto& lhs, const auto& rhs) {
        return tie(lhs.vx, lhs) < tie(rhs.vx, rhs);
    })->vx;
    auto min_vy = min_element(begin(v), end(v), [](const auto& lhs, const auto& rhs) {
        return tie(lhs.vy, lhs) < tie(rhs.vy, rhs);
    })->vy;
    auto max_vy = max_element(begin(v), end(v), [](const auto& lhs, const auto& rhs) {
        return tie(lhs.vy, lhs) < tie(rhs.vy, rhs);
    })->vy;

    auto eq = [](auto value, auto expected) {
        return abs(value - expected) < 0.001;
    };

    for (auto vx = min_vx; vx <= max_vx; ++vx)
        for (auto vy = min_vy; vy <= max_vy; ++vy) {
            for (auto& p : v) {
                p.vx -= vx;
                p.vy -= vy;
            }

            auto [intersected, x_ref, y_ref] = intersect(v[0], v[1]);
            if (intersected) {
                auto ok = true;

                for (auto j = 2; j < n; ++j) {
                    auto [intersected, x, y] = intersect(v[0], v[j]);
                    if (!intersected || !eq(x, x_ref) || !eq(y, y_ref)) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    auto t0 = (x_ref - v[0].x) / v[0].vx;
                    auto t1 = (x_ref - v[1].x) / v[1].vx;

                    auto z0 = v[0].z + v[0].vz * t0;
                    auto z1 = v[1].z + v[1].vz * t1;

                    // z0 = z + vz * t0 | * t1
                    // z1 = z + vz * t1 | * -t0
                    // z0 * t1 - z1 * t0 = z * (t1 - t0)
                    // z = (z0 * t1 - z1 * t0) / (t1 - t0)
                    auto z = (z0 * t1 - z1 * t0) / (t1 - t0);

                    return x_ref + y_ref + z;
                }
            }

            for (auto& p : v) {
                p.vx += vx;
                p.vy += vy;
            }
        }

    assert(false);
}

int main()
{
    vector<Point> v;

    string s;
    while (getline(cin, s)) {
        string_view sv{s};

        auto sep = sv.find('@');
        auto pos = split(sv.substr(0, sep), ',');
        auto vel = split(sv.substr(sep + 1), ',');

        auto x = str2num(trim(pos[0]));
        auto y = str2num(trim(pos[1]));
        auto z = str2num(trim(pos[2]));
        auto vx = str2num(trim(vel[0]));
        auto vy = str2num(trim(vel[1]));
        auto vz = str2num(trim(vel[2]));

        v.emplace_back(x, y, z, vx, vy, vz);
    }

    auto n = ssize(v);

    cout << solve1(n, v, 200000000000000l, 400000000000000l) << '\n';
    cout << solve2(n, v) << '\n';

    return 0;
}

