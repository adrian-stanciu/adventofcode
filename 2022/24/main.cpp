#include <bits/stdc++.h>

using namespace std;

struct Point {
    int r;
    int c;

    auto operator<=>(const Point&) const = default;
};

struct State {
    int eta;
    int t;
    Point p;

    State(const Point& p, int t, int d) : eta{t + d}, t{t}, p{p} {}

    auto operator<=>(const State&) const = default;
};

auto dist(const Point& src, const Point& dst)
{
    return abs(dst.r - src.r) + abs(dst.c - src.c);
}

auto is_busy(const vector<string>& m, const Point& p, int t, const Point& src, const Point& dst)
{
    if (p == src || p == dst)
        return false;

    auto h = ssize(m) - 2;
    auto w = ssize(m[0]) - 2;

    auto u = ((p.r - 1) - (t % h) + h) % h + 1;
    auto d = ((p.r - 1) + (t % h)) % h + 1;
    auto l = ((p.c - 1) - (t % w) + w) % w + 1;
    auto r = ((p.c - 1) + (t % w)) % w + 1;

    return m[p.r][l] == '>' || m[p.r][r] == '<' || m[u][p.c] == 'v' || m[d][p.c] == '^';
}

auto next_states(const vector<string>& m, const State& s, const Point& src, const Point& dst)
{
    const array<pair<int, int>, 5> neigh{{
        {0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    }};

    vector<State> next;

    for (auto [dr, dc] : neigh) {
        const Point p{s.p.r + dr, s.p.c + dc};
        if (p.r == -1 || p.r == ssize(m))
            continue;

        if (m[p.r][p.c] == '#')
            continue;

        if (is_busy(m, p, s.t + 1, src, dst))
            continue;

        next.emplace_back(p, s.t + 1, dist(p, dst));
    }

    return next;
}

auto search(const vector<string>& m, const Point& src, const Point& dst, int t)
{
    set<State> states;
    states.emplace(src, t, dist(src, dst));

    while (!states.empty()) {
        auto s = *begin(states);
        states.erase(begin(states));

        if (s.p == dst)
            return s.t;

        for (auto ns : next_states(m, s, src, dst))
            states.emplace(ns);
    }

    assert(false);
}

auto solve1(const vector<string>& m, const Point& src, const Point& dst)
{
    return search(m, src, dst, 0);
}

auto solve2(const vector<string>& m, const Point& src, const Point& dst, int t)
{
    t = search(m, dst, src, t);
    t = search(m, src, dst, t);
    return t;
}

int main()
{
    vector<string> m;

    string line;
    while (getline(cin, line))
        m.push_back(move(line));

    Point src{0, 0};
    while (m[src.r][src.c] != '.')
        ++src.c;

    Point dst{static_cast<int>(ssize(m)) - 1, 0};
    while (m[dst.r][dst.c] != '.')
        ++dst.c;

    auto t1 = solve1(m, src, dst);
    cout << t1 << '\n';
    cout << solve2(m, src, dst, t1) << '\n';

    return 0;
}

