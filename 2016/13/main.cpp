#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

struct Cell {
    int x;
    int y;

    Cell() = default;

    Cell(int x, int y)
    : x(x), y(y)
    {}
};

enum class CellType {
    Empty,
    Wall,
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

auto num_set_bits(int n)
{
    auto cnt = 0;

    while (n) {
        ++cnt;
        n &= n - 1;
    }

    return cnt;
}

auto compute_type(const Cell& c, int n)
{
    auto score = n;
    score += c.x * c.x;
    score += 3 * c.x;
    score += 2 * c.x * c.y;
    score += c.y;
    score += c.y * c.y;

    if (num_set_bits(score) % 2 == 0)
        return CellType::Empty;
    else
        return CellType::Wall;
}

// bfs
auto sp(int n, int target_x, int target_y)
{
    static const std::array<int, 4> dx{0, 0, -1, 1};
    static const std::array<int, 4> dy{-1, 1, 0, 0};

    Cell c{1, 1};

    std::queue<std::pair<Cell, int>> q;
    q.emplace(c, 0);

    std::unordered_set<Cell, CellHasher> visited;
    visited.insert(c);

    while (!q.empty()) {
        const auto& [c, dist] = q.front();

        if (c.x == target_x && c.y == target_y)
            return dist;

        for (auto i = 0; i < 4; ++i) {
            Cell next_c{c.x + dx[i], c.y + dy[i]};

            if (next_c.x < 0 || next_c.y < 0)
                continue;

            if (visited.count(next_c) != 0)
                continue;

            visited.insert(next_c);

            if (compute_type(next_c, n) == CellType::Wall)
                continue;

            q.emplace(next_c, dist + 1);
        }

        q.pop();
    }

    return -1; // not found
}

// bfs
auto cover(int n, int max_d)
{
    static const std::array<int, 4> dx{0, 0, -1, 1};
    static const std::array<int, 4> dy{-1, 1, 0, 0};

    Cell c{1, 1};

    std::queue<std::pair<Cell, int>> q;
    q.emplace(c, 0);

    std::unordered_set<Cell, CellHasher> visited;
    visited.insert(c);

    auto cnt = 0;

    while (!q.empty()) {
        ++cnt;

        const auto& [c, dist] = q.front();

        for (auto i = 0; i < 4; ++i) {
            Cell next_c{c.x + dx[i], c.y + dy[i]};

            if (next_c.x < 0 || next_c.y < 0)
                continue;

            if (visited.count(next_c) != 0)
                continue;

            visited.insert(next_c);

            if (compute_type(next_c, n) == CellType::Wall)
                continue;

            if (dist < max_d)
                q.emplace(next_c, dist + 1);
        }

        q.pop();
    }

    return cnt;
}

int main()
{
    int n;
    std::cin >> n;

    std::cout << sp(n, 31, 39) << "\n";
    std::cout << cover(n, 50) << "\n";

    return 0;
}

