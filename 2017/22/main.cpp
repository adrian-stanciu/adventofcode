#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

enum State {
    Clean = 0,
    Weakened,
    Infected,
    Flagged,
};

auto read_grid()
{
    std::vector<std::string> grid;

    std::string line;
    while (getline(std::cin, line))
        grid.push_back(std::move(line));

    return grid;
}

std::string stringify(long r, long c)
{
    std::stringstream ss;
    ss << r;
    ss << ";";
    ss << c;

    return ss.str();
}

auto grid_to_map1(const std::vector<std::string>& grid)
{
    std::unordered_map<std::string, bool> map;

    long mid_r = grid.size() / 2;
    long mid_c = grid[mid_r].size() / 2;

    for (auto i = 0U; i < grid.size(); ++i)
        for (auto j = 0U; j < grid[i].size(); ++j)
            map[stringify(mid_r - i, j - mid_c)] = (grid[i][j] == '#');

    return map;
}

long count_turned_infected1(const std::vector<std::string>& grid,
    long r, long c, long r_dir, long c_dir, size_t iters)
{
    std::unordered_map<std::string, bool> map = grid_to_map1(grid);
    long turned_infected = 0;

    for (size_t i = 0; i < iters; ++i) {
        auto& infected = map[stringify(r, c)];

        if (infected) {
            // turn right
            if (r_dir != 0) {
                c_dir = r_dir;
                r_dir = 0;
            } else {
                r_dir = -c_dir;
                c_dir = 0;
            }
        } else {
            // turn left
            if (r_dir != 0) {
                c_dir = -r_dir;
                r_dir = 0;
            } else {
                r_dir = c_dir;
                c_dir = 0;
            }

            ++turned_infected;
        }

        infected = !infected;

        r += r_dir;
        c += c_dir;
    }

    return turned_infected;
}

auto grid_to_map2(const std::vector<std::string>& grid)
{
    std::unordered_map<std::string, State> map;

    long mid_r = grid.size() / 2;
    long mid_c = grid[mid_r].size() / 2;

    for (auto i = 0U; i < grid.size(); ++i)
        for (auto j = 0U; j < grid[i].size(); ++j)
            map[stringify(mid_r - i, j - mid_c)] =
                (grid[i][j] == '#') ? Infected : Clean;

    return map;
}

long count_turned_infected2(const std::vector<std::string>& grid,
    long r, long c, long r_dir, long c_dir, size_t iters)
{
    std::unordered_map<std::string, State> map = grid_to_map2(grid);
    long turned_infected = 0;

    for (size_t i = 0; i < iters; ++i) {
        auto& state = map[stringify(r, c)];

        switch (state) {
        case Clean:
            // turn left
            if (r_dir != 0) {
                c_dir = -r_dir;
                r_dir = 0;
            } else {
                r_dir = c_dir;
                c_dir = 0;
            }
            state = Weakened;
            break;

        case Weakened:
            // no turn
            ++turned_infected;
            state = Infected;
            break;

        case Infected:
            // turn right
            if (r_dir != 0) {
                c_dir = r_dir;
                r_dir = 0;
            } else {
                r_dir = -c_dir;
                c_dir = 0;
            }
            state = Flagged;
            break;

        case Flagged:
            // turn back
            r_dir = -r_dir;
            c_dir = -c_dir;
            state = Clean;
            break;
        }

        r += r_dir;
        c += c_dir;
    }

    return turned_infected;
}

int main()
{
    auto grid = read_grid();

    std::cout << count_turned_infected1(grid, 0, 0, 1, 0, 10000) << "\n";
    std::cout << count_turned_infected2(grid, 0, 0, 1, 0, 10000000) << "\n";

    return 0;
}

