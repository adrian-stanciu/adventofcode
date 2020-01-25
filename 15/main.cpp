#include <array>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "int_computer.h"

struct Cell {
    int x;
    int y;

    Cell() = default;

    Cell(int x, int y)
    : x(x), y(y)
    {}
};

enum CellType {
    Wall = 0,
    Empty = 1,
    Oxigen = 2,
};

bool operator==(const Cell& c1, const Cell& c2)
{
    return c1.x == c2.x && c1.y == c2.y;
}

struct CellHasher {
    auto operator()(const Cell& c) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= c.x + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= c.y + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

using Map = std::unordered_map<Cell, CellType, CellHasher>;

// dfs
auto explore(const std::vector<long long>& prog)
{
    static const std::array<int, 5> dx{0, 0, 0, -1, 1};
    static const std::array<int, 5> dy{0, -1, 1, 0, 0};

    auto get_move_cmd = [&] (const Cell& from, const Cell& to) {
        auto delta_x = to.x - from.x;
        auto delta_y = to.y - from.y;

        for (auto i = 1; i <= 4; ++i)
            if (delta_x == dx[i] && delta_y == dy[i])
                return i;

        return 0;
    };

    IntComputer ic(prog);

    Cell c{0, 0};

    Map map;
    map[c] = Empty;

    std::stack<Cell> st;
    st.push(c);

    while (!st.empty()) {
        auto c = st.top();

        auto moved = false;

        for (auto i = 1; i <= 4; ++i) {
            Cell next_c{c.x + dx[i], c.y + dy[i]};

            if (map.count(next_c) != 0)
                continue;

            ic.append_input(i);
            ic.run();

            auto out = *ic.get_last_output();

            map[next_c] = static_cast<CellType>(out);

            if (out != Wall) {
                st.push(next_c);
                moved = true;
                break;
            }
        }

        if (!moved) {
            st.pop();

            // move back
            if (!st.empty()) {
                auto prev_c = st.top();

                ic.append_input(get_move_cmd(c, prev_c));
                ic.run();
            }
        }
    }

    return map;
}

// bfs
auto find_oxigen(const Map& map)
{
    static const std::array<int, 5> dx{0, 0, 0, -1, 1};
    static const std::array<int, 5> dy{0, -1, 1, 0, 0};

    Cell c{0, 0};

    std::queue<std::pair<Cell, int>> q;
    q.emplace(c, 0);

    std::unordered_set<Cell, CellHasher> visited;
    visited.insert(c);

    while (!q.empty()) {
        const auto& [c, dist] = q.front();

        for (auto i = 1; i <= 4; ++i) {
            Cell next_c{c.x + dx[i], c.y + dy[i]};

            if (visited.count(next_c) != 0)
                continue;

            if (map.at(next_c) == Wall)
                continue;

            if (map.at(next_c) == Oxigen)
                return std::make_pair(next_c, dist + 1);

            q.emplace(next_c, dist + 1);
            visited.insert(next_c);
        }

        q.pop();
    }

    return std::make_pair(Cell{0, 0}, -1); // not found
}

// bfs
auto compute_time_to_fill_map(const Map& map, const Cell& oxigen)
{
    static const std::array<int, 5> dx{0, 0, 0, -1, 1};
    static const std::array<int, 5> dy{0, -1, 1, 0, 0};

    auto max_t = 0;

    std::queue<std::pair<Cell, int>> q;
    q.emplace(oxigen, 0);

    std::unordered_set<Cell, CellHasher> visited;
    visited.insert(oxigen);

    while (!q.empty()) {
        const auto& [c, t] = q.front();

        max_t = std::max(max_t, t);

        for (auto i = 1; i <= 4; ++i) {
            Cell next_c{c.x + dx[i], c.y + dy[i]};

            if (visited.count(next_c) != 0)
                continue;

            if (map.at(next_c) == Wall)
                continue;

            if (map.at(next_c) == Empty) {
                q.emplace(next_c, t + 1);
                visited.insert(next_c);
            }
        }

        q.pop();
    }

    return max_t;
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    auto map = explore(prog);

    auto [oxigen_cell, oxigen_dist] = find_oxigen(map);
    std::cout << oxigen_dist << "\n";

    std::cout << compute_time_to_fill_map(map, oxigen_cell) << "\n";

    return 0;
}

