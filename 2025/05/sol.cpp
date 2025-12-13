#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto merge_intervals(vector<pair<long, long>>& intervals)
{
    sort(begin(intervals), end(intervals));

    vector<pair<long, long>> merged_intervals{intervals[0]};

    for (auto i = 1; i < ssize(intervals); ++i) {
        auto [l, r] = intervals[i];
        auto& [prev_l, prev_r] = merged_intervals.back();

        if (l <= prev_r)
            prev_r = max(prev_r, r);
        else
            merged_intervals.emplace_back(l, r);
    }

    return merged_intervals;
}

auto count_values_in_intervals(const vector<pair<long, long>>& intervals,
    const vector<long>& values)
{
    auto cnt = 0;

    for (auto x : values) {
        auto it = lower_bound(begin(intervals), end(intervals), make_pair(x + 1, 0l));
        if (it != begin(intervals) && prev(it)->second >= x)
            ++cnt;
    }

    return cnt;
}

int main()
{
    vector<pair<long, long>> intervals;
    vector<long> values;

    string s;
    while (getline(cin, s)) {
        if (s.empty())
            break;

        const string_view sv{s};
        auto interval = split(sv, '-');
        intervals.emplace_back(str2num(interval[0]), str2num(interval[1]));
    }

    long x;
    while (cin >> x)
        values.push_back(x);

    auto merged_intervals = merge_intervals(intervals);

    cout << count_values_in_intervals(merged_intervals, values) << '\n';

    cout << accumulate(begin(merged_intervals), end(merged_intervals), 0l,
        [](auto acc, const auto& interval) {
            return acc + (interval.second - interval.first + 1);
        }) << '\n';

    return 0;
}

