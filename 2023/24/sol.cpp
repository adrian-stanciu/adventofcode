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

struct Vector {
    int128 x;
    int128 y;
    int128 z;

    Vector(int128 x, int128 y, int128 z) : x{x}, y{y}, z{z} {};
};

auto add(const Vector& lhs, const Vector& rhs)
{
    return Vector{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

auto sub(const Vector& lhs, const Vector& rhs)
{
    return Vector{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

auto mul(const Vector& vec, int128 k)
{
    return Vector{vec.x * k, vec.y * k, vec.z * k};
}

optional<Vector> exact_div(const Vector& vec, int128 k)
{
    if (vec.x % k != 0 || vec.y % k != 0 || vec.z % k != 0)
        return nullopt;
    return Vector{vec.x / k, vec.y / k, vec.z / k};
}

auto cross_product(const Vector& lhs, const Vector& rhs)
{
    return Vector{
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

auto dot_product(const Vector& lhs, const Vector& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

struct Stone {
    Vector p;
    Vector v;

    Stone(int128 x, int128 y, int128 z, int128 vx, int128 vy, int128 vz)
    : p{x, y, z}, v{vx, vy, vz}
    {}
};

auto intersect_inside_rectangle_in_future(const Stone& s1, const Stone& s2,
    long min_coord, long max_coord)
{
    // (x - x1) / (x2 - x1) = (y - y1) / (y2 - y1)
    // (x - x1) / vx = (y - y1) / vy
    // (x - x1) * vy = (y - y1) * vx
    // x * vy - x1 * vy = y * vx - y1 * vx
    // x * vy - y * vx = x1 * vy - y1 * vx

    // a1 * x + b1 * y = c1
    auto a1 = s1.v.y;
    auto b1 = -s1.v.x;
    auto c1 = s1.p.x * s1.v.y - s1.p.y * s1.v.x;

    // a2 * x + b2 * y = c2
    auto a2 = s2.v.y;
    auto b2 = -s2.v.x;
    auto c2 = s2.p.x * s2.v.y - s2.p.y * s2.v.x;

    // a1 * x + b1 * y = c1 | * a2
    // a2 * x + b2 * y = c2 | * -a1
    // a2 * b1 * y - a1 * b2 * y = c1 * a2 - c2 * a1
    // (a2 * b1 - a1 * b2) * y = c1 * a2 - c2 * a1
    // y = (c1 * a2 - c2 * a1) / (a2 * b1 - a1 * b2)
    // x = (c1 - b1 * y) / a1

    auto y_den = a2 * b1 - a1 * b2;
    if (y_den == 0)
        return false;

    auto y = 1.0l * (c1 * a2 - c2 * a1) / y_den;
    auto x = (c1 - b1 * y) / a1;

    if (x < min_coord || x > max_coord || y < min_coord || y > max_coord)
        return false;

    auto t1 = (x - s1.p.x) / s1.v.x;
    auto t2 = (x - s2.p.x) / s2.v.x;
    return t1 > 0 && t2 > 0;
}

auto solve1(int n, const vector<Stone>& stones, long min_coord, long max_coord)
{
    auto cnt = 0;

    for (auto i = 0; i < n; ++i)
        for (auto j = i + 1; j < n; ++j)
            if (intersect_inside_rectangle_in_future(stones[i], stones[j],
                min_coord, max_coord))
                ++cnt;

    return cnt;
}

auto solve2(int n, const vector<Stone>& stones)
{
    // consider stone 0 as origin
    auto rel_to_s0_stones{stones};
    for (auto& s : rel_to_s0_stones) {
        s.p = sub(s.p, stones[0].p);
        s.v = sub(s.v, stones[0].v);
    }

    for (auto i = 1; i < n; ++i)
        for (auto j = i + 1; j < n; ++j) {
            // collision points of stones 0, i and j are collinear
            // pi + ti * vi is collinear with pj + tj * vj
            // (pi + ti * vi) x (pj + tj * vj) = 0
            // (pi x pj) + tj * (pi x vj) + ti * (vi x pj) + ti * tj * (vi x vj) = 0
            // by dot product with vj: (pi x pj) * vj + ti * (vi x pj) * vj = 0
            // ti = -((pi x pj) * vj) / ((vi x pj) * vj)
            // similarly: tj = -((pi x pj) * vi) / ((pi x vj) * vi)

            auto pi_pj = cross_product(rel_to_s0_stones[i].p, rel_to_s0_stones[j].p);
            auto vi_pj = cross_product(rel_to_s0_stones[i].v, rel_to_s0_stones[j].p);
            auto pi_vj = cross_product(rel_to_s0_stones[i].p, rel_to_s0_stones[j].v);

            auto ti_den = dot_product(vi_pj, rel_to_s0_stones[j].v);
            if (ti_den == 0)
                continue;
            auto ti_num = -dot_product(pi_pj, rel_to_s0_stones[j].v);
            if (ti_num % ti_den != 0)
                continue;
            auto ti = ti_num / ti_den;
            if (ti < 0)
                continue;

            auto tj_den = dot_product(pi_vj, rel_to_s0_stones[i].v);
            if (tj_den == 0)
                continue;
            auto tj_num = -dot_product(pi_pj, rel_to_s0_stones[i].v);
            if (tj_num % tj_den != 0)
                continue;
            auto tj = tj_num / tj_den;
            if (tj < 0)
                continue;

            if (ti == tj)
                continue;

            auto collision_pi = add(stones[i].p, mul(stones[i].v, ti));
            auto collision_pj = add(stones[j].p, mul(stones[j].v, tj));

            auto vel = exact_div(sub(collision_pj, collision_pi), tj - ti);
            if (!vel)
                continue;

            auto pos = sub(collision_pi, mul(*vel, ti));

            return static_cast<long>(pos.x + pos.y + pos.z);
        }

    assert(false);
}

int main()
{
    vector<Stone> stones;

    string s;
    while (getline(cin, s)) {
        const string_view sv{s};

        auto sep = sv.find('@');
        auto pos = split(sv.substr(0, sep), ',');
        auto vel = split(sv.substr(sep + 1), ',');

        auto x = str2num(trim(pos[0]));
        auto y = str2num(trim(pos[1]));
        auto z = str2num(trim(pos[2]));
        auto vx = str2num(trim(vel[0]));
        auto vy = str2num(trim(vel[1]));
        auto vz = str2num(trim(vel[2]));

        stones.emplace_back(x, y, z, vx, vy, vz);
    }

    auto n = ssize(stones);

    cout << solve1(n, stones, 200'000'000'000'000l, 400'000'000'000'000l) << '\n';
    cout << solve2(n, stones) << '\n';

    return 0;
}

