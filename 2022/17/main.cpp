#include <bits/stdc++.h>

using namespace std;

constexpr auto Width{7};
const string EmptyRow(Width, '.');
constexpr auto BottomPadding{3};
constexpr auto LeftPadding{2};
constexpr auto Moving{'#'};
constexpr auto Settled{'@'};

using Shape = vector<string>;

auto can_move_left(const Shape& all, int bottom, int height)
{
    return all_of(begin(all) + bottom - height, begin(all) + bottom, [](const auto& row) {
        if (row.front() == Moving)
            return false;
        for (auto i = 1; i < ssize(row); ++i)
            if (row[i] == Moving && row[i - 1] == Settled)
                return false;
        return true;
    });
}

auto move_left(Shape& all, int bottom, int height)
{
    if (!can_move_left(all, bottom, height))
        return;

    for (auto i = bottom - height; i < bottom; ++i)
        for (auto j = 1; j < ssize(all[i]); ++j)
            if (all[i][j] == Moving)
                swap(all[i][j], all[i][j - 1]);
}

auto can_move_right(Shape& all, int bottom, int height)
{
    return all_of(begin(all) + bottom - height, begin(all) + bottom, [](const auto& row) {
        if (row.back() == Moving)
            return false;
        for (auto i = 0; i <= ssize(row) - 2; ++i)
            if (row[i] == Moving && row[i + 1] == Settled)
                return false;
        return true;
    });
}

auto move_right(Shape& all, int bottom, int height)
{
    if (!can_move_right(all, bottom, height))
        return;

    for (auto i = bottom - height; i < bottom; ++i)
        for (auto j = ssize(all[i]) - 2; j >= 0; --j)
            if (all[i][j] == Moving)
                swap(all[i][j], all[i][j + 1]);
}

auto can_move_up(const Shape& all, int bottom, int height)
{
    if (bottom - height == 0)
        return false;
    for (auto i = bottom - height; i < bottom; ++i)
        for (auto j = 0; j < ssize(all[i]); ++j)
            if (all[i][j] == Moving && all[i - 1][j] == Settled)
                return false;
    return true;
}

auto move_up(Shape& all, int bottom, int height)
{
    if (!can_move_up(all, bottom, height))
        return false;

    for (auto i = bottom - height; i < bottom; ++i)
        for (auto j = 0; j < ssize(all[i]); ++j)
            if (all[i][j] == Moving)
                swap(all[i][j], all[i - 1][j]);

    return true;
}

auto settle(Shape& all, int bottom, int height)
{
    for (auto i = bottom - height; i < bottom; ++i)
        replace(begin(all[i]), end(all[i]), Moving, Settled);
}

void simulate(const Shape& s, const string& jet, int& jet_idx, Shape& all)
{
    all.reserve(all.size() + s.size());
    for (const auto& r : s)
        all.push_back(r);

    auto height = ssize(s) - BottomPadding;
    auto bottom = ssize(all);

    while (true) {
        auto dir = jet[jet_idx];
        jet_idx = (jet_idx + 1) % ssize(jet);

        if (dir == '<')
            move_left(all, bottom, height);
        else
            move_right(all, bottom, height);

        if (move_up(all, bottom, height)) {
            --bottom;
        } else {
            settle(all, bottom, height);
            break;
        }
    }

    while (all.back() == EmptyRow)
        all.erase(prev(end(all)));
}

auto build_shapes()
{
    vector<Shape> shapes{
        {
            "####",
        },
        {
            ".#.",
            "###",
            ".#.",
        },
        {
            "..#",
            "..#",
            "###",
        },
        {
            "#",
            "#",
            "#",
            "#",
        },
        {
            "##",
            "##",
        }
    };

    for (auto& shape : shapes) {
        for (auto& row : shape) {
            row.insert(0, LeftPadding, '.');
            row.append(Width - row.size(), '.');
        }
        for (auto i = 0; i < BottomPadding; ++i)
            shape.push_back(EmptyRow);
    }

    // reverse in order to append at the bottom
    for (auto& shape : shapes) {
        for (auto& row : shape)
            reverse(begin(row), end(row));
        reverse(begin(shape), end(shape));
    }

    return shapes;
}

auto solve1(const vector<Shape>& shapes, const string& jet, int iters)
{
    Shape all;
    for (auto jet_idx = 0, i = 0; i < iters; ++i)
        simulate(shapes[i % shapes.size()], jet, jet_idx, all);
    return ssize(all);
}

auto solve2(const vector<Shape>& shapes, const string& jet, long iters)
{
    Shape all;

    auto is_repeated = [&](auto bottom, auto pattern_height) {
        if (bottom < 2 * pattern_height)
            return false;
        auto prev_bottom = bottom - pattern_height;
        while (pattern_height--)
            if (all[--bottom] != all[--prev_bottom])
                return false;
        return true;
    };

    vector<int> iter2height;
    map<pair<int, int>, int> move2iter;

    for (auto prev_jet_idx = -1, jet_idx = 0, i = 0;; ++i, prev_jet_idx = jet_idx) {
        auto shape_idx = i % shapes.size();
        simulate(shapes[shape_idx], jet, jet_idx, all);

        auto curr_height = ssize(all);
        iter2height.push_back(curr_height);

        // try to detect a repeated pattern when the jet index wraps around
        if (jet_idx < prev_jet_idx) {
            auto move = make_pair(shape_idx, jet_idx);
            auto it = move2iter.find(move);
            if (it == end(move2iter)) {
                move2iter[move] = i;
                continue;
            }

            auto prev_i = exchange(it->second, i);
            auto cycle_len = i - prev_i;
            auto prev_height = iter2height[prev_i];
            auto pattern_height = curr_height - prev_height;

            // validate that the pattern is repeated indeed
            if (is_repeated(prev_height, pattern_height)) {
                iters -= prev_i + 1;
                return (iters / cycle_len) * pattern_height +
                    iter2height[prev_i + iters % cycle_len];
            }
        }
    }

    return ssize(all);
}

int main()
{
    auto shapes = build_shapes();

    string jet;
    cin >> jet;

    for (auto& c : jet)
        c = c == '<' ? '>' : '<';

    cout << solve1(shapes, jet, 2022) << '\n';
    cout << solve2(shapes, jet, 1'000'000'000'000) << '\n';

    return 0;
}

