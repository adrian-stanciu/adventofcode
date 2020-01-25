#include <array>
#include <iostream>
#include <string>
#include <queue>

#include "md5.h"

struct Cell {
    int x;
    int y;
    std::string path;

    Cell() = default;

    Cell(int x, int y, std::string path)
    : x(x), y(y), path(std::move(path))
    {}
};

bool is_door_open(char hex_digit)
{
    return hex_digit >= 'b' && hex_digit <= 'f';
}

// bfs
auto shortest_path(const std::string& passcode)
{
    static const std::array<int, 4> dx{0, 0, -1, 1};
    static const std::array<int, 4> dy{-1, 1, 0, 0};
    static const std::array<char, 4> dir{'U', 'D', 'L', 'R'};

    std::queue<Cell> q;
    q.emplace(0, 0, passcode);

    while (!q.empty()) {
        const auto& c = q.front();

        if (c.x == 3 && c.y == 3)
            return c.path.substr(passcode.size());

        auto hash = hex_md5sum(c.path, true);

        for (auto i = 0; i < 4; ++i) {
            if (!is_door_open(hash[i]))
                continue;

            auto x = c.x + dx[i];
            if (x < 0 || x == 4)
                continue;

            auto y = c.y + dy[i];
            if (y < 0 || y == 4)
                continue;

            auto path = c.path;
            path.push_back(dir[i]);

            q.emplace(x, y, path);
        }

        q.pop();
    }

    return std::string(); // not found
}

// bfs
auto longest_path_len(const std::string& passcode)
{
    static const std::array<int, 4> dx{0, 0, -1, 1};
    static const std::array<int, 4> dy{-1, 1, 0, 0};
    static const std::array<char, 4> dir{'U', 'D', 'L', 'R'};

    auto max_dist = -1;

    std::queue<std::pair<Cell, int>> q;
    q.emplace(Cell{0, 0, passcode}, 0);

    while (!q.empty()) {
        const auto& [c, dist] = q.front();

        if (c.x == 3 && c.y == 3) {
            max_dist = std::max(max_dist, dist);
            q.pop();
            continue;
        }

        auto hash = hex_md5sum(c.path, true);

        for (auto i = 0; i < 4; ++i) {
            if (!is_door_open(hash[i]))
                continue;

            auto x = c.x + dx[i];
            if (x < 0 || x == 4)
                continue;

            auto y = c.y + dy[i];
            if (y < 0 || y == 4)
                continue;

            auto path = c.path;
            path.push_back(dir[i]);

            q.emplace(Cell(x, y, path), dist + 1);
        }

        q.pop();
    }

    return max_dist;
}

int main()
{
    std::string passcode;
    std::cin >> passcode;

    std::cout << shortest_path(passcode) << "\n";
    std::cout << longest_path_len(passcode) << "\n";

    return 0;
}

