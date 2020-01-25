#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

const auto R = 100;
const auto C = 100;

using Grid = std::array<std::array<char, C>, R>;

auto count_neighbours(const Grid& g, int r, int c, char type)
{
    auto cnt = 0;

    for (auto dr = -1; dr <= 1; ++dr)
        for (auto dc = -1; dc <= 1; ++dc)
            if (dr != 0 || dc != 0) {
                auto neigh_r = r + dr;
                if (neigh_r < 0 || neigh_r == R)
                    continue;

                auto neigh_c = c + dc;
                if (neigh_c < 0 || neigh_c == C)
                    continue;

                if (g[neigh_r][neigh_c] == type)
                    ++cnt;
            }

    return cnt;
}

auto next_state(const Grid& g, int r, int c)
{
    auto on_neighbours = count_neighbours(g, r, c, '#');

    bool on = on_neighbours == 3 ||
        (on_neighbours == 2 && g[r][c] == '#');

    return on ? '#' : '.';
}

auto next(const Grid& g)
{
    Grid next_g;

    for (auto r = 0; r < R; ++r)
        for (auto c = 0; c < C; ++c)
            next_g[r][c] = next_state(g, r, c);

    return next_g;
}

auto next_after(Grid g, const std::vector<std::pair<int, int>>& always_on, int iters)
{
    Grid next_g{std::move(g)};

    for (auto [r, c] : always_on)
        next_g[r][c] = '#';

    while (iters--) {
        next_g = next(next_g);

        for (auto [r, c] : always_on)
            next_g[r][c] = '#';
    }

    return next_g;
}

auto sum_in_grid(const Grid& g)
{
    auto cnt = 0;

    for (const auto& row : g)
        cnt += std::count(row.begin(), row.end(), '#');

    return cnt;
}

int main()
{
    Grid g{};

    for (auto r = 0; r < R; ++r)
        for (auto c = 0; c < C; ++c)
            std::cin >> g[r][c];

    std::cout << sum_in_grid(next_after(g, {}, 100)) << "\n";

    std::vector<std::pair<int, int>> always_on {
        {0, 0},
        {0, C - 1},
        {R - 1, C - 1},
        {R - 1, 0},
    };

    std::cout << sum_in_grid(next_after(g, always_on, 100)) << "\n";

    return 0;
}

