#include <bits/stdc++.h>

using namespace std;

struct Point {
    int x;
    int y;
    int z;

    auto operator<=>(const Point&) const = default;
};

const array<tuple<int, int, int>, 6> neigh{{
    {-1,  0,  0},
    { 1,  0,  0},
    { 0, -1,  0},
    { 0,  1,  0},
    { 0,  0, -1},
    { 0,  0,  1},
}};

auto count_adj(const set<Point>& points, const Point& p)
{
    auto cnt = 0;
    for (const auto& [dx, dy, dz] : neigh)
        cnt += points.contains(Point{p.x + dx, p.y + dy, p.z + dz});
    return cnt;
}

auto solve1(const set<Point>& points)
{
    auto cnt = 0;
    for (const auto& p : points)
        cnt += ssize(neigh) - count_adj(points, p);
    return cnt;
}

void bfs(const set<Point>& points,
    int min_x, int max_x, int min_y, int max_y, int min_z, int max_z, int& cnt)
{
    const Point src{min_x - 1, min_y - 1, min_z - 1};

    queue<Point> q;
    q.push(src);

    set<Point> seen;
    seen.emplace(src);

    while (!q.empty()) {
        auto [x, y, z] = q.front();
        q.pop();

        for (const auto& [dx, dy, dz] : neigh) {
            auto nx = x + dx;
            if (nx < min_x - 1 || nx > max_x + 1)
                continue;
            auto ny = y + dy;
            if (ny < min_y - 1 || ny > max_y + 1)
                continue;
            auto nz = z + dz;
            if (nz < min_z - 1 || nz > max_z + 1)
                continue;

            const Point p{nx, ny, nz};

            if (seen.contains(p))
                continue;
            if (points.contains(p))
                continue;

            cnt += count_adj(points, p);

            q.push(p);
            seen.emplace(p);
        }
    }
}

auto solve2(const set<Point>& points)
{
    auto min_x = numeric_limits<int>::max();
    auto max_x = numeric_limits<int>::min();
    auto min_y = numeric_limits<int>::max();
    auto max_y = numeric_limits<int>::min();
    auto min_z = numeric_limits<int>::max();
    auto max_z = numeric_limits<int>::min();

    for (const auto& [x, y, z] : points) {
        min_x = min(min_x, x);
        max_x = max(max_x, x);
        min_y = min(min_y, y);
        max_y = max(max_y, y);
        min_z = min(min_z, z);
        max_z = max(max_z, z);
    }

    auto cnt = 0;
    bfs(points, min_x, max_x, min_y, max_y, min_z, max_z, cnt);
    return cnt;
}

int main()
{
    set<Point> points;

    string line;
    while (getline(cin, line)) {
        int x, y, z;
        sscanf(line.data(), "%d,%d,%d", &x, &y, &z);
        points.emplace(x, y, z);
    }

    cout << solve1(points) << '\n';
    cout << solve2(points) << '\n';

    return 0;
}

