#include <bits/stdc++.h>

using namespace std;

struct Point {
    int x;
    int y;
};

struct Pair {
    Point s;
    Point b;
};

struct Interval {
    int from;
    int to;

    auto operator<=>(const Interval&) const = default;
};

auto compute_x_intervals(const vector<Pair>& v, int y,
    int x_min_lim = numeric_limits<int>::min(),
    int x_max_lim = numeric_limits<int>::max())
{
    vector<Interval> intervals;
    intervals.reserve(v.size());
    for (const auto& [s, b] : v) {
        auto max_d = abs(s.x - b.x) + abs(s.y - b.y);
        auto y_d = abs(s.y - y);
        if (y_d <= max_d) {
            auto x_d = max_d - y_d;
            intervals.emplace_back(max(s.x - x_d, x_min_lim), min(s.x + x_d, x_max_lim));
        }
    }

    sort(begin(intervals), end(intervals));

    vector<Interval> merged_intervals;
    auto [prev_from, prev_to] = intervals[0];
    for (auto i = 1; i < ssize(intervals); ++i) {
        auto [curr_from, curr_to] = intervals[i];
        if (curr_from > prev_to) {
            merged_intervals.emplace_back(prev_from, prev_to);
            prev_from = curr_from;
            prev_to = curr_to;
        } else {
            prev_to = max(prev_to, curr_to);
        }
    }
    merged_intervals.emplace_back(prev_from, prev_to);

    return merged_intervals;
}

auto solve1(const vector<Pair>& v, int y)
{
    auto intervals = compute_x_intervals(v, y);

    auto sum = 0;
    set<int> reserved;

    for (const auto& [from, to] : intervals) {
        sum += to - from + 1;
        for (const auto& [s, b] : v) {
            if (s.y == y && from <= s.x && s.x <= to)
                reserved.emplace(s.x);
            if (b.y == y && from <= b.x && b.x <= to)
                reserved.emplace(b.x);
        }
    }

    return sum - ssize(reserved);
}

auto solve2(const vector<Pair>& v, int lim)
{
    for (auto y = 0; y <= lim; ++y) {
        auto intervals = compute_x_intervals(v, y, 0, lim);
        if (intervals.size() == 2)
            return (intervals[0].to + 1l) * lim + y;
    }
    assert(false);
}

int main()
{
    vector<Pair> v;

    string line;
    while (getline(cin, line)) {
        Point s, b;
        sscanf(line.data(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
            &s.x, &s.y, &b.x, &b.y);
        v.emplace_back(s, b);
    }

    cout << solve1(v, 2'000'000) << '\n';
    cout << solve2(v, 4'000'000) << '\n';

    return 0;
}

