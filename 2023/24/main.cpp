#include <bits/stdc++.h>

#include <z3++.h>

#include "parser.h"

using namespace std;

using int128 = __int128;

auto str2num(string_view sv)
{
    auto n = 0l;
    from_chars(sv.data(), sv.data() + sv.size(), n);
    return n;
}

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
};

auto intersect_inside_rectangle_in_future(const Point& p1, const Point& p2,
    long min_coord, long max_coord)
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
        return false;

    auto y = 1.0l * (c1 * a2 - c2 * a1) / y_den;
    auto x = (c1 - b1 * y) / a1;

    if (x < min_coord || x > max_coord || y < min_coord || y > max_coord )
        return false;

    auto t1 = (x - p1.x) / p1.vx;
    auto t2 = (x - p2.x) / p2.vx;

    return t1 > 0 && t2 > 0;
}

auto solve1(const vector<Point>& v, long min_coord, long max_coord)
{
    auto n = ssize(v);

    auto cnt = 0;

    for (auto i = 0; i < n; ++i)
        for (auto j = i + 1; j < n; ++j)
            if (intersect_inside_rectangle_in_future(v[i], v[j], min_coord, max_coord))
                ++cnt;

    return cnt;
}

auto solve2(const vector<Point>& points)
{
    z3::context z3_ctx;
    z3::solver z3_solver{z3_ctx};

    z3::expr_vector p{z3_ctx};
    z3::expr_vector v{z3_ctx};
    z3::expr_vector t{z3_ctx};
    for (auto i = 0; i < 3; i++) {
        auto idx = to_string(i);
        p.push_back(z3_ctx.real_const(("p_" + idx).data()));
        v.push_back(z3_ctx.real_const(("v_" + idx).data()));
        t.push_back(z3_ctx.real_const(("t_" + idx).data()));
    }

    for (auto i = 0; i < 3; i++) {
        z3_solver.add(p[0] + v[0] * t[i] ==
            z3_ctx.real_val(to_string(static_cast<long>(points[i].x)).data()) +
            z3_ctx.real_val(to_string(static_cast<long>(points[i].vx)).data()) * t[i]);

        z3_solver.add(p[1] + v[1] * t[i] ==
            z3_ctx.real_val(to_string(static_cast<long>(points[i].y)).data()) +
            z3_ctx.real_val(to_string(static_cast<long>(points[i].vy)).data()) * t[i]);

        z3_solver.add(p[2] + v[2] * t[i] ==
            z3_ctx.real_val(to_string(static_cast<long>(points[i].z)).data()) +
            z3_ctx.real_val(to_string(static_cast<long>(points[i].vz)).data()) * t[i]);
    }

    auto ret = z3_solver.check();
    assert(ret == z3::sat);

    return z3_solver.get_model().eval(p[0] + p[1] + p[2], true).get_numeral_int64();
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

    cout << solve1(v, 200'000'000'000'000l, 400'000'000'000'000l) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

