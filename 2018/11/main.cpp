#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <tuple>

const auto Sz {300};

int compute_cell_power(int x, int y, int serial)
{
    long rank = x + 10;
    long power = rank * y;
    power += serial;
    power *= rank;
    int power_hundreds_digit = 0;
    if (power > 99)
        power_hundreds_digit = (power / 100) % 10;
    return power_hundreds_digit - 5;
}

void fill_grid_power(std::array<std::array<int, Sz + 1>, Sz + 1>& grid, int serial)
{
    for (auto y = 1; y <= Sz; ++y)
        for (auto x = 1; x <= Sz; ++x)
            grid[y][x] = compute_cell_power(x, y, serial);
}

void make_prefix_sum_grid(std::array<std::array<int, Sz + 1>, Sz + 1>& grid)
{
    for (auto y = 2; y <= Sz; ++y)
        grid[y][1] += grid[y - 1][1];
    for (auto x = 2; x <= Sz; ++x)
        grid[1][x] += grid[1][x - 1];

    for (auto y = 2; y <= Sz; ++y)
        for (auto x = 2; x <= Sz; ++x)
            grid[y][x] += grid[y - 1][x] + grid[y][x - 1] - grid[y - 1][x - 1];
}

auto max_power_subgrid(const std::array<std::array<int, Sz + 1>, Sz + 1>& grid, int k)
{
    auto max_power = std::numeric_limits<int>::min();
    auto max_x = 0;
    auto max_y = 0;

    for (auto y = 1; y <= Sz - k + 1; ++y)
        for (auto x = 1; x <= Sz - k + 1; ++x) {
            auto power = grid[y + k - 1][x + k - 1]
                - grid[y - 1][x + k - 1]
                - grid[y + k - 1][x - 1]
                + grid[y - 1][x - 1];
            if (power > max_power) {
                max_power = power;
                max_x = x;
                max_y = y;
            }
        }

    return std::make_tuple(max_power, max_x, max_y);
}

auto max_power_subgrid_k(const std::array<std::array<int, Sz + 1>, Sz + 1>& grid)
{
    auto max_k_power = std::numeric_limits<int>::min();
    auto max_k = 0;
    auto max_k_x = 0;
    auto max_k_y = 0;

    for (auto k = 1; k <= Sz; ++k) {
        auto [max_power, max_x, max_y] = max_power_subgrid(grid, k);
        if (max_power > max_k_power) {
            max_k_power = max_power;
            max_k = k;
            max_k_x = max_x;
            max_k_y = max_y;
        }
    }

    return std::make_tuple(max_k_power, max_k_x, max_k_y, max_k);
}

void check(int serial, int expected_max_3_x, int expected_max_3_y,
    int expected_max_k_x, int expected_max_k_y, int expected_max_k)
{
    std::array<std::array<int, Sz + 1>, Sz + 1> grid {0};
    fill_grid_power(grid, serial);
    make_prefix_sum_grid(grid);

    auto [max_3_power, max_3_x, max_3_y] = max_power_subgrid(grid, 3);
    assert(max_3_x == expected_max_3_x);
    assert(max_3_y == expected_max_3_y);

    auto [max_k_power, max_k_x, max_k_y, max_k] = max_power_subgrid_k(grid);
    assert(max_k_x == expected_max_k_x);
    assert(max_k_y == expected_max_k_y);
    assert(max_k == expected_max_k);
}

int main()
{
    check(18, 33, 45, 90, 269, 16);
    check(42, 21, 61, 232, 251, 12);

    int serial;
    std::cin >> serial;

    std::array<std::array<int, Sz + 1>, Sz + 1> grid {0};
    fill_grid_power(grid, serial);
    make_prefix_sum_grid(grid);

    auto [max_3_power, max_3_x, max_3_y] = max_power_subgrid(grid, 3);
    std::cout << max_3_x << "," << max_3_y << "\n";

    auto [max_k_power, max_k_x, max_k_y, max_k] = max_power_subgrid_k(grid);
    std::cout << max_k_x << "," << max_k_y << "," << max_k << "\n";

    return 0;
}

