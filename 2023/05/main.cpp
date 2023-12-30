#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto str2num(string_view sv)
{
    auto n = 0l;
    from_chars(sv.data(), sv.data() + sv.size(), n);
    return n;
}

struct RangeConversion {
    long dst;
    long src;
    long len;
};

bool operator<(const RangeConversion& lhs, const RangeConversion& rhs)
{
    return tie(lhs.src, lhs.dst, lhs.len) < tie(rhs.src, rhs.dst, rhs.len);
}

using Map = vector<RangeConversion>;

auto solve1(const vector<long>& seeds, const vector<Map>& maps)
{
    auto min_loc = numeric_limits<long>::max();

    for (auto x : seeds) {
        for (const auto& map : maps) {
            auto it = upper_bound(begin(map), end(map), RangeConversion{-1, x, -1});

            if (it != begin(map)) {
                --it;
                if (it->src + it->len > x)
                    x = it->dst + (x - it->src);
            }
        }

        min_loc = min(min_loc, x);
    }

    return min_loc;
}

using Range = pair<long, long>;

auto convert(const vector<Range>& ranges, const Map& map)
{
    vector<Range> new_ranges;

    auto append = [&](auto l, auto r) {
        if (l < r)
            new_ranges.emplace_back(l, r);
    };

    auto convert = [&](auto l, auto r, const auto& range_conv) {
        auto intersection_begin = clamp(range_conv.src, l, r);
        append(l, intersection_begin);

        auto intersection_end = clamp(range_conv.src + range_conv.len, l, r);
        append(range_conv.dst + (intersection_begin - range_conv.src),
            range_conv.dst + (intersection_end - range_conv.src));

        return intersection_end;
    };

    auto map_it = begin(map);
    for (auto [l, r] : ranges) {
        auto it = upper_bound(map_it, end(map), RangeConversion{-1, l, -1});

        if (it != map_it)
            l = convert(l, r, *prev(it));

        for (; l < r && it != end(map); ++it)
            l = convert(l, r, *it);

        append(l, r);

        if (it != map_it)
            map_it = prev(it);
    }

    sort(begin(new_ranges), end(new_ranges));

    vector<Range> merged_ranges;
    for (auto [l, r] : new_ranges)
        if (!merged_ranges.empty() && merged_ranges.back().second == l)
            merged_ranges.back().second = r;
        else
            merged_ranges.emplace_back(l, r);
    return merged_ranges;
}

auto solve2(const vector<long>& seeds, const vector<Map>& maps)
{
    vector<Range> ranges;
    ranges.reserve(ssize(seeds) / 2);
    for (auto i = 0; i < ssize(seeds); i += 2)
        ranges.emplace_back(seeds[i], seeds[i] + seeds[i + 1]);

    sort(begin(ranges), end(ranges));

    for (const auto& map : maps)
        ranges = convert(ranges, map);

    return ranges.front().first;
}

int main()
{
    auto seeds = []() {
        string s;
        getline(cin, s);

        string_view sv{s};
        sv.remove_prefix(strlen("seeds:"));
        auto parts = split(sv, ' ');

        vector<long> seeds;
        seeds.reserve(ssize(parts));
        for (auto part : parts)
            seeds.push_back(str2num(part));
        return seeds;
    }();

    auto maps = []() {
        unordered_map<string, pair<string, Map>> conversions;

        string src;
        string dst;
        Map map;

        string s;
        while (getline(cin, s)) {
            if (s.empty()) {
                if (!map.empty()) {
                    conversions.try_emplace(move(src), move(dst), move(map));
                    map = {};
                }

                if (getline(cin, s)) {
                    string_view sv{s};
                    sv.remove_suffix(sv.size() - sv.find(' '));
                    auto parts = split(sv, '-');
                    src = parts.front();
                    dst = parts.back();
                }
            } else {
                auto parts = split(string_view{s}, ' ');
                map.emplace_back(str2num(parts[0]), str2num(parts[1]), str2num(parts[2]));
            }
        }

        if (!map.empty())
            conversions.try_emplace(move(src), move(dst), move(map));

        vector<Map> maps;
        for (string src{"seed"}; src != "location";) {
            auto& [dst, map] = conversions[src];
            sort(begin(map), end(map));
            maps.push_back(move(map));
            src = move(dst);
        }
        return maps;
    }();

    cout << solve1(seeds, maps) << '\n';
    cout << solve2(seeds, maps) << '\n';

    return 0;
}

