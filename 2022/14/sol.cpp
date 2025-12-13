#include <bits/stdc++.h>

using namespace std;

using Solid = vector<pair<int, int>>;

auto parse_point(string_view line)
{
    auto x = strtol(line.data(), nullptr, 10);
    auto y = strtol(line.data() + line.find_first_of(',') + 1, nullptr, 10);
    return make_pair(x, y);
}

auto parse_solid(string_view line)
{
    const auto sep{"->"};
    const auto sep_len{strlen(sep)};

    Solid solid;

    while (true) {
        solid.push_back(parse_point(line));

        auto sep_idx = line.find_first_of(sep);
        if (sep_idx == line.npos)
            return solid;
        else
            line.remove_prefix(sep_idx + sep_len);
    }
}

struct Simulator {
    Simulator(const vector<Solid>& solids, bool with_bottom)
    {
        auto min_x = src_x;
        auto max_x = src_x;
        for (const auto& solid: solids)
            for (const auto& [x, y] : solid) {
                min_x = min(min_x, x);
                max_x = max(max_x, x);
                max_y = max(max_y, y);
            }

        if (with_bottom)
            max_y += 2;

        // reduce x range
        max_x -= min_x;
        src_x -= min_x;
        // add left and right padding to x range
        max_x += 2 * max_y;
        src_x += max_y;

        g = vector<vector<char>>(max_y + 1, vector<char>(max_x + 1, '.'));
        for (const auto& solid: solids) {
            auto [prev_x, prev_y] = solid[0];
            for (auto i = 1; i < ssize(solid); ++i) {
                auto [curr_x, curr_y] = solid[i];
                for (auto y = min(prev_y, curr_y); y <= max(prev_y, curr_y); ++y)
                    for (auto x = min(prev_x, curr_x); x <= max(prev_x, curr_x); ++x)
                        g[y][x - min_x + max_y] = '#';
                prev_x = curr_x;
                prev_y = curr_y;
            }
        }

        if (with_bottom)
            fill(begin(g[max_y]), end(g[max_y]), '#');
    }

    [[nodiscard]] bool run()
    {
        auto x = src_x;
        auto y = 0;

        if (g[y][x] == 'o')
            return false;

        while (y < max_y) {
            if (g[y + 1][x] == '.') {
                ++y;
            } else if (g[y + 1][x - 1] == '.') {
                ++y;
                --x;
            } else if (g[y + 1][x + 1] == '.') {
                ++y;
                ++x;
            } else {
                g[y][x] = 'o';
                return true;
            }
        }

        return false;
    }

private:
    vector<vector<char>> g;
    int src_x{500};
    int max_y{0};
};

auto solve1(const vector<Solid>& solids)
{
    auto cnt = 0;

    Simulator sim{solids, false};
    while (sim.run())
        ++cnt;

    return cnt;
}

auto solve2(const vector<Solid>& solids)
{
    auto cnt = 0;

    Simulator sim{solids, true};
    while (sim.run())
        ++cnt;

    return cnt;
}

int main()
{
    vector<Solid> solids;

    string line;
    while (getline(cin, line))
        solids.push_back(parse_solid(line));

    cout << solve1(solids) << '\n';
    cout << solve2(solids) << '\n';

    return 0;
}

