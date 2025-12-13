#include <bits/stdc++.h>

using namespace std;

struct Point {
    int x;
    int y;

    Point(int x, int y) : x{x}, y{y} {}
};

struct Segment {
    Point from;
    Point to;

    Segment(int x1, int y1, int x2, int y2) : from{x1, y1}, to{x2, y2} {}
};

auto traverse(const Segment& seg, map<pair<int, int>, int>& point_freq_map)
{
    const auto& [from, to] = seg;

    auto dx = from.x < to.x ? 1 : from.x > to.x ? -1 : 0;
    auto dy = from.y < to.y ? 1 : from.y > to.y ? -1 : 0;

    for (auto x = from.x, y = from.y;; x += dx, y += dy) {
        ++point_freq_map[make_pair(x, y)];

        if (x == to.x && y == to.y)
            break;
    }
}

auto solve1(const vector<Segment>& segs)
{
    map<pair<int, int>, int> point_freq_map;

    for (const auto& seg : segs)
        if (seg.from.x == seg.to.x || seg.from.y == seg.to.y)
            traverse(seg, point_freq_map);

    return count_if(begin(point_freq_map), end(point_freq_map),
        [](const auto& point_freq) { return point_freq.second > 1; });
}

auto solve2(const vector<Segment>& segs)
{
    map<pair<int, int>, int> point_freq_map;

    for (const auto& seg : segs)
        traverse(seg, point_freq_map);

    return count_if(begin(point_freq_map), end(point_freq_map),
        [](const auto& point_freq) { return point_freq.second > 1; });
}

int main()
{
    static const regex r{R"(([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+))"};

    auto to_num = [](const string& s) {
        return strtol(s.data(), nullptr, 10);
    };

    vector<Segment> segs;

    string line;
    while (getline(cin, line)) {
        smatch matched;
        regex_match(line, matched, r);

        auto x1 = to_num(matched[1].str());
        auto y1 = to_num(matched[2].str());
        auto x2 = to_num(matched[3].str());
        auto y2 = to_num(matched[4].str());

        segs.emplace_back(x1, y1, x2, y2);
    }

    cout << solve1(segs) << '\n';
    cout << solve2(segs) << '\n';

    return 0;
}

