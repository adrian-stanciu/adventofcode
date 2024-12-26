#include <bits/stdc++.h>

using namespace std;

struct Point {
    int y;
    int x;

    auto operator<=>(const Point&) const = default;
};

struct PointHasher {
    auto operator()(const Point& p) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

using Neigh = array<array<Point, 3>, 4>;

auto make_moves(unordered_set<Point, PointHasher>& pos, const Neigh& neigh)
{
    // only 2 points can request to move to the same point
    unordered_map<Point, Point, PointHasher> dst2src;

    for (const auto& from : pos) {
        array<bool, 4> is_busy{};
        for (auto i = 0; i < 4; ++i)
            for (auto [dy, dx] : neigh[i])
                if (pos.contains(Point{from.y + dy, from.x + dx})) {
                    is_busy[i] = true;
                    break;
                }

        auto cnt = count(begin(is_busy), end(is_busy), false);
        if (cnt == 0 || cnt == 4)
            continue;

        auto i = distance(begin(is_busy), find(begin(is_busy), end(is_busy), false));
        const Point to{from.y + neigh[i][1].y, from.x + neigh[i][1].x};

        auto it = dst2src.find(to);
        if (it == end(dst2src)) {
            dst2src.try_emplace(to, from);
        } else {
            // two points want to move here but none is allowed
            dst2src.erase(it);
        }
    }

    for (const auto& [dst, src] : dst2src) {
        pos.erase(src);
        pos.emplace(dst);
    }

    return !dst2src.empty();
}

auto solve1(Neigh neigh, unordered_set<Point, PointHasher> pos, int iters)
{
    while (iters--) {
        make_moves(pos, neigh);
        rotate(begin(neigh), begin(neigh) + 1, end(neigh));
    }

    auto min_y = numeric_limits<int>::max();
    auto max_y = numeric_limits<int>::min();
    auto min_x = numeric_limits<int>::max();
    auto max_x = numeric_limits<int>::min();
    for (auto [y, x] : pos) {
        min_y = min(min_y, y);
        max_y = max(max_y, y);
        min_x = min(min_x, x);
        max_x = max(max_x, x);
    }

    return (max_y - min_y + 1l) * (max_x - min_x + 1l) - ssize(pos);
}

auto solve2(Neigh neigh, unordered_set<Point, PointHasher> pos)
{
    for (auto iter = 1;; ++iter) {
        if (!make_moves(pos, neigh))
            return iter;
        rotate(begin(neigh), begin(neigh) + 1, end(neigh));
    }
}

int main()
{
    static constexpr Neigh Neigh{{
        {{{-1, -1}, {-1,  0}, {-1,  1}}},
        {{{ 1, -1}, { 1,  0}, { 1,  1}}},
        {{{-1, -1}, { 0, -1}, { 1, -1}}},
        {{{-1,  1}, { 0,  1}, { 1,  1}}},
    }};

    unordered_set<Point, PointHasher> pos;

    string line;
    for (auto y = 0; getline(cin, line); ++y)
        for (auto x = 0; x < ssize(line); ++x)
            if (line[x] == '#')
                pos.emplace(y, x);

    cout << solve1(Neigh, pos, 10) << '\n';
    cout << solve2(Neigh, pos) << '\n';

    return 0;
}

