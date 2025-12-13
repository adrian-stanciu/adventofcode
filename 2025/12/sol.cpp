#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

// assume 3x3 shapes
static constexpr auto ShapeSz{3};

int main()
{
    map<int, int> shape_areas;

    string s;
    while (getline(cin, s)) {
        if (s.empty())
            continue;
        if (s.back() != ':')
            break;

        string_view sv{s};
        sv.remove_suffix(1);
        auto idx = str2num(sv);

        auto area = 0;
        for (auto i = 0; i < ShapeSz; ++i) {
            getline(cin, s);
            assert(ssize(s) == ShapeSz);
            area += count(begin(s), end(s), '#');
        }

        shape_areas.emplace(idx, area);
    }

    auto cnt_definitely_fit = 0;
    auto cnt_maybe_fit = 0;

    do {
        const string_view sv{s};
        auto groups = split(sv, ':');

        auto sizes = split(groups[0], 'x');
        auto num_rows = str2num(sizes[0]);
        auto num_cols = str2num(sizes[1]);

        auto min_required_area = 0;
        auto num_shapes = 0;
        for (auto i = 0; auto freq_sv : split(groups[1], ' ')) {
            auto freq = str2num(freq_sv);
            min_required_area += freq * shape_areas[i++];
            num_shapes += freq;
        }

        if (num_rows * num_cols >= min_required_area) {
            if ((num_rows / ShapeSz) * (num_cols / ShapeSz) >= num_shapes)
                ++cnt_definitely_fit;
            else
                ++cnt_maybe_fit;
        }
    } while (getline(cin, s));

    // assume the input contains only shapes that definitely fit or
    // definitely do not fit
    assert(cnt_maybe_fit == 0);

    cout << cnt_definitely_fit << '\n';

    return 0;
}

