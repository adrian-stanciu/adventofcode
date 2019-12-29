#include <array>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

constexpr auto RecursionDepthLimit = 999;

struct Cell {
    int x;
    int y;

    Cell() = default;

    Cell(int x, int y)
    : x(x), y(y)
    {}
};

bool operator==(const Cell& c1, const Cell& c2)
{
    return c1.x == c2.x && c1.y == c2.y;
}

bool operator<(const Cell& c1, const Cell& c2)
{
    return std::tie(c1.x, c1.y) < std::tie(c2.x, c2.y);
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

using Map = std::vector<std::string>;
using PortalMap = std::unordered_map<std::string, Cell>;
using PortalEndpointMap = std::unordered_map<Cell, Cell, CellHasher>;

// bfs
auto find_exit(const Map& map, const Cell& entry_c, const Cell& exit_c,
    const PortalEndpointMap& in2out_portals, const PortalEndpointMap& out2in_portals)
{
    static const std::array<int, 4> dx{0, 0, -1, 1};
    static const std::array<int, 4> dy{-1, 1, 0, 0};

    std::set<std::pair<int, Cell>> s;
    s.emplace(0, entry_c);

    std::unordered_set<Cell, CellHasher> visited;
    visited.insert(entry_c);

    while (!s.empty()) {
        auto [d, c] = *s.begin();
        s.erase(s.begin());

        if (c == exit_c)
            return d;

        for (auto i = 0; i < 4; ++i) {
            auto x = c.x + dx[i];
            auto y = c.y + dy[i];

            if (map[x][y] != '.')
                continue;

            Cell next_c{x, y};
            auto next_d = d + 1;

            auto it = in2out_portals.find(next_c);
            if (it != in2out_portals.end()) {
                next_c = it->second;
                ++next_d;
            } else {
                auto it = out2in_portals.find(next_c);
                if (it != out2in_portals.end()) {
                    next_c = it->second;
                    ++next_d;
                }
            }

            if (visited.count(next_c) != 0)
                continue;

            s.emplace(next_d, next_c);
            visited.insert(std::move(next_c));
        }
    }

    return -1; // not found
}

struct MultiLevelCell {
    Cell cell;
    int level;

    MultiLevelCell() = default;

    MultiLevelCell(Cell cell, int level)
    : cell(std::move(cell)), level(level)
    {}

    MultiLevelCell(int x, int y, int level)
    : cell(x, y), level(level)
    {}
};

bool operator==(const MultiLevelCell& mlc1, const MultiLevelCell& mlc2)
{
    return mlc1.cell == mlc2.cell && mlc1.level == mlc2.level;
}

bool operator<(const MultiLevelCell& mlc1, const MultiLevelCell& mlc2)
{
    return std::tie(mlc1.cell, mlc1.level) < std::tie(mlc2.cell, mlc2.level);
}

struct MultiLevelCellHasher {
    auto operator()(const MultiLevelCell& mlc) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= CellHasher{}(mlc.cell) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= mlc.level + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

// bfs
auto find_exit_with_recursion(const Map& map, const Cell& entry_c, const Cell& exit_c,
    const PortalEndpointMap& in2out_portals, const PortalEndpointMap& out2in_portals)
{
    static const std::array<int, 4> dx{0, 0, -1, 1};
    static const std::array<int, 4> dy{-1, 1, 0, 0};

    MultiLevelCell mlc{entry_c, 0};

    std::set<std::pair<int, MultiLevelCell>> s;
    s.emplace(0, mlc);

    std::unordered_set<MultiLevelCell, MultiLevelCellHasher> visited;
    visited.insert(std::move(mlc));

    while (!s.empty()) {
        auto [d, mlc] = *s.begin();
        s.erase(s.begin());

        if (mlc.cell == exit_c && mlc.level == 0)
            return d;

        if (mlc.level > RecursionDepthLimit)
            return -1; // break recursion

        for (auto i = 0; i < 4; ++i) {
            auto x = mlc.cell.x + dx[i];
            auto y = mlc.cell.y + dy[i];

            if (map[x][y] != '.')
                continue;

            MultiLevelCell next_mlc{x, y, mlc.level};
            auto next_d = d + 1;

            auto it = in2out_portals.find(next_mlc.cell);
            if (it != in2out_portals.end()) {
                next_mlc.cell = it->second;
                ++next_mlc.level;
                ++next_d;
            } else if (next_mlc.level != 0) {
                auto it = out2in_portals.find(next_mlc.cell);
                if (it != out2in_portals.end()) {
                    next_mlc.cell = it->second;
                    --next_mlc.level;
                    ++next_d;
                }
            }

            if (visited.count(next_mlc) != 0)
                continue;

            s.emplace(next_d, next_mlc);
            visited.insert(std::move(next_mlc));
        }
    }

    return -1; // not found
}

void fill_vertical_portals(const Map& map, PortalMap& in_portals, PortalMap& out_portals)
{
    auto r = 0;

    for (auto i = 0; i < static_cast<int>(map.size());) {
        bool found = false;

        for (auto j = 0; j < static_cast<int>(map[0].size()); ++j)
            if (map[i][j] >= 'A' && map[i][j] <= 'Z' &&
                map[i + 1][j] >= 'A' && map[i + 1][j] <= 'Z') {
                if (!found) {
                    ++r;
                    found = true;
                }

                std::string portal;
                portal.push_back(map[i][j]);
                portal.push_back(map[i + 1][j]);

                switch (r) {
                case 1:
                    out_portals[std::move(portal)] = Cell{i + 2, j};
                    break;
                case 2:
                    in_portals[std::move(portal)] = Cell{i - 1, j};
                    break;
                case 3:
                    in_portals[std::move(portal)] = Cell{i + 2, j};
                    break;
                case 4:
                    out_portals[std::move(portal)] = Cell{i - 1, j};
                    break;
                }
            }

        if (found)
            i += 2;
        else
            ++i;
    }
}

void fill_horizontal_portals(const Map& map, PortalMap& in_portals, PortalMap& out_portals)
{
    auto c = 0;

    for (auto j = 0; j < static_cast<int>(map[0].size());) {
        bool found = false;

        for (auto i = 0; i < static_cast<int>(map.size()); ++i)
            if (map[i][j] >= 'A' && map[i][j] <= 'Z' &&
                map[i][j + 1] >= 'A' && map[i][j + 1] <= 'Z') {
                if (!found) {
                    ++c;
                    found = true;
                }

                std::string portal;
                portal.push_back(map[i][j]);
                portal.push_back(map[i][j + 1]);

                switch (c) {
                case 1:
                    out_portals[std::move(portal)] = Cell{i, j + 2};
                    break;
                case 2:
                    in_portals[std::move(portal)] = Cell{i, j - 1};
                    break;
                case 3:
                    in_portals[std::move(portal)] = Cell{i, j + 2};
                    break;
                case 4:
                    out_portals[std::move(portal)] = Cell{i, j - 1};
                    break;
                }
            }

        if (found)
            j += 2;
        else
            ++j;
    }
}

int main()
{
    Map map;

    std::string line;
    while (getline(std::cin, line))
        map.emplace_back(std::move(line));

    PortalMap in_portals;
    PortalMap out_portals;

    fill_vertical_portals(map, in_portals, out_portals);
    fill_horizontal_portals(map, in_portals, out_portals);

    auto entry_c = out_portals["AA"];
    out_portals.erase("AA");
    auto exit_c = out_portals["ZZ"];
    out_portals.erase("ZZ");

    PortalEndpointMap in2out_portals;
    PortalEndpointMap out2in_portals;

    for (auto& p : in_portals)
        in2out_portals[p.second] = out_portals[p.first];
    for (auto& p : out_portals)
        out2in_portals[p.second] = in_portals[p.first];

    std::cout << find_exit(map, entry_c, exit_c, in2out_portals, out2in_portals) << "\n";
    std::cout << find_exit_with_recursion(map, entry_c, exit_c, in2out_portals, out2in_portals) << "\n";

    return 0;
}

