#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

// a * x + b * y = gcd(a, b)
auto gcd_extended(int a, int b)
{
    if (b == 0)
        return make_tuple(a, 1, 0);

    auto [gcd, x, y] = gcd_extended(b, a % b);

    return make_tuple(gcd, y, x - (a / b) * y);
}

struct Point {
    array<int, 2> pos;
    array<int, 2> vel;

    Point(int x, int y, int vx, int vy) : pos{x, y}, vel{vx, vy} {}
};

auto solve1(vector<Point> v, int w, int h, int iters)
{
    for (auto& p : v) {
        p.pos[0] = (p.pos[0] + (p.vel[0] * iters) % w + w) % w;
        p.pos[1] = (p.pos[1] + (p.vel[1] * iters) % h + h) % h;
    }

    auto tl = 0;
    auto tr = 0;
    auto bl = 0;
    auto br = 0;

    for (const auto& p : v) {
        if (p.pos[0] == w / 2 || p.pos[1] == h / 2)
            continue;

        if (p.pos[0] < w / 2) {
            if (p.pos[1] < h / 2)
                ++tl;
            else
                ++bl;
        } else {
            if (p.pos[1] < h/ 2)
                ++tr;
            else
                ++br;
        }
    }

    return 1l * tl * tr * bl * br;
}

auto solve2(vector<Point> v, int w, int h)
{
    // assume that the searched image is not chaotic,
    // but rather the points are close together

    auto n = ssize(v);

    auto find_iter_with_lowest_total_distance = [&](auto idx, auto sz) {
        auto min_d = numeric_limits<int>::max();
        auto min_d_iter = -1;

        for (auto iter = 1; iter <= sz; ++iter) {
            for (auto& p : v)
                p.pos[idx] = (p.pos[idx] + p.vel[idx] + sz) % sz;

            auto d = 0;
            for (auto i = 0; i < n; ++i)
                for (auto j = i + 1; j < n; ++j)
                    d += abs(v[i].pos[idx] - v[j].pos[idx]);

            if (d < min_d) {
                min_d = d;
                min_d_iter = iter;
            }
        }

        return min_d_iter;
    };

    auto x_iter = find_iter_with_lowest_total_distance(0, w);
    auto y_iter = find_iter_with_lowest_total_distance(1, h);

    auto [gcd, x, y] = gcd_extended(w, h);
    assert(gcd == 1); // the chinese remainder theorem requires that moduli are coprime

    auto prod = w * h;

    return ((x_iter * h * y + y_iter * w * x) % prod + prod) % prod;
}

int main()
{
    auto parse = [](string_view sv) {
        auto parts = split(split(sv, '=')[1], ',');
        return make_pair(str2num(parts[0]), str2num(parts[1]));
    };

    vector<Point> v;

    string s;
    while (getline(cin, s)) {
        auto parts = split(s, ' ');
        auto [x, y] = parse(parts[0]);
        auto [vx, vy] = parse(parts[1]);
        v.emplace_back(x, y, vx, vy);
    }

    cout << solve1(v, 101, 103, 100) << '\n';
    cout << solve2(v, 101, 103) << '\n';

    return 0;
}

