#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

struct Cell {
    int row;
    int col;

    Cell(int row, int col) : row{row}, col{col} {}
};

auto area(const Cell& c1, const Cell& c2)
{
    return 1l * (abs(c1.row - c2.row) + 1) * (abs(c1.col - c2.col) + 1);
}

auto solve1(const vector<Cell>& cells)
{
    auto n = ssize(cells);

    auto max_area = 0l;

    for (auto i = 0; i < n; ++i)
        for (auto j = i + 1; j < n; ++j)
            max_area = max(max_area, area(cells[i], cells[j]));

    return max_area;
}

auto fill(vector<vector<int>>& grid, int num_rows, int num_cols, int r, int c, int val)
{
    static constexpr array<pair<int, int>, 4> nei{{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    }};

    if (grid[r][c] != -1)
        return;

    grid[r][c] = val;

    queue<Cell> q;
    q.emplace(r, c);

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (const auto& [dr, dc] : nei) {
            auto nr = r + dr;
            if (nr < 0 || nr == num_rows)
                continue;
            auto nc = c + dc;
            if (nc < 0 || nc == num_cols)
                continue;

            if (grid[nr][nc] != -1)
                continue;

            grid[nr][nc] = val;

            q.emplace(nr, nc);
        }
    }
}

struct VerticalEdge {
    int src_row;
    int dst_row;
    bool downward;

    auto operator<=>(const VerticalEdge&) const = default;
};

// detect if a cell is inside the loop using ray tracing
auto is_inside(const unordered_map<int, vector<VerticalEdge>>& vertical_edges,
    int row, int col)
{
    auto cnt = 0;

    bool last_downward;
    for (auto c = 0; c < col; ++c)
        if (auto it_edges = vertical_edges.find(c); it_edges != end(vertical_edges)) {
            const auto& edges = it_edges->second;
            if (auto it_edge = upper_bound(begin(edges), end(edges),
                VerticalEdge{row, -1, false}); it_edge != begin(edges)) {
                const auto& edge = *prev(it_edge);
                if (edge.src_row <= row && row <= edge.dst_row) {
                    if (cnt == 0 || edge.downward != last_downward)
                        ++cnt;
                    last_downward = edge.downward;
                }
            }
        }

    return cnt % 2 == 1;
}

auto solve2(vector<Cell>& cells)
{
    auto n = ssize(cells);

    auto compress = [](auto& v) {
        map<int, int> m;

        sort(begin(v), end(v));
        v.erase(unique(begin(v), end(v)), end(v));

        for (auto x : v)
            if (m.empty()) {
                m.emplace(x, 0);
            } else {
                const auto& [key, val] = *prev(end(m));
                m.emplace(x, (x == key + 1) ? val + 1 : val + 2);
            }

        return m;
    };

    // compress rows
    vector<int> rows;
    rows.reserve(n);
    for (const auto& cell : cells)
        rows.push_back(cell.row);
    auto compressed_rows = compress(rows);
    auto num_rows = prev(compressed_rows.end())->second + 1;

    // compress cols
    vector<int> cols;
    cols.reserve(n);
    for (const auto& cell : cells)
        cols.push_back(cell.col);
    auto compressed_cols = compress(cols);
    auto num_cols = prev(compressed_cols.end())->second + 1;

    // build compressed grid
    auto grid = [&]() {
        vector<vector<int>> grid(num_rows, vector<int>(num_cols, -1));

        // set compressed cells
        for (const auto& cell : cells)
            grid[compressed_rows[cell.row]][compressed_cols[cell.col]] = 1;

        unordered_map<int, vector<VerticalEdge>> vertical_edges;

        // set compressed edges
        cells.push_back(cells[0]);
        for (auto i = 1; i <= n; ++i) {
            auto src_row = compressed_rows[min(cells[i - 1].row, cells[i].row)];
            auto dst_row = compressed_rows[max(cells[i - 1].row, cells[i].row)];
            auto src_col = compressed_cols[min(cells[i - 1].col, cells[i].col)];
            auto dst_col = compressed_cols[max(cells[i - 1].col, cells[i].col)];

            for (auto r = src_row; r <= dst_row; ++r)
                for (auto c = src_col; c <= dst_col; ++c)
                    grid[r][c] = 1;

            if (src_col == dst_col) {
                vertical_edges[src_col].emplace_back(src_row, dst_row,
                    cells[i - 1].row < cells[i].row);
            }
        }
        cells.pop_back();

        for (auto& [_, edges] : vertical_edges)
            sort(begin(edges), end(edges));

        // fill grid
        for (auto r = 0; r < num_rows; ++r)
            for (auto c = 0; c < num_cols; ++c)
                fill(grid, num_rows, num_cols, r, c, is_inside(vertical_edges, r, c) ? 1 : 0);

        return grid;
    }();

    // make prefix sum grid
    for (auto r = 1; r < num_rows; ++r)
        grid[r][0] += grid[r - 1][0];
    for (auto c = 1; c < num_cols; ++c)
        grid[0][c] += grid[0][c - 1];
    for (auto r = 1; r < num_rows; ++r)
        for (auto c = 1; c < num_cols; ++c)
            grid[r][c] += grid[r - 1][c] + grid[r][c - 1] - grid[r - 1][c - 1];

    auto count_ones = [&grid](auto src_row, auto dst_row, auto src_col, auto dst_col) {
        return grid[dst_row][dst_col]
            - (src_row > 0 ? grid[src_row - 1][dst_col] : 0)
            - (src_col > 0 ? grid[dst_row][src_col - 1] : 0)
            + ((src_row > 0 && src_col > 0) ? grid[src_row - 1][src_col - 1] : 0);
    };

    auto is_valid = [&](auto i, auto j) {
        auto src_row = compressed_rows[min(cells[i].row, cells[j].row)];
        auto dst_row = compressed_rows[max(cells[i].row, cells[j].row)];
        auto src_col = compressed_cols[min(cells[i].col, cells[j].col)];
        auto dst_col = compressed_cols[max(cells[i].col, cells[j].col)];

        auto area = [&]() {
            return (dst_row - src_row + 1) * (dst_col - src_col + 1);
        };

        return count_ones(src_row, dst_row, src_col, dst_col) == area();
    };

    vector<pair<long, pair<int, int>>> areas;
    for (auto i = 0; i < n; ++i)
        for (auto j = i + 1; j < n; ++j)
            areas.emplace_back(area(cells[i], cells[j]), make_pair(i, j));

    sort(rbegin(areas), rend(areas));

    for (const auto& [area, indexes] : areas)
        if (is_valid(indexes.first, indexes.second))
            return area;

    assert(false);
}

int main()
{
    vector<Cell> cells;

    string s;
    while (getline(cin, s)) {
        auto pos = split(string_view{s}, ',');
        cells.emplace_back(str2num(pos[1]), str2num(pos[0]));
    }

    cout << solve1(cells) << '\n';
    cout << solve2(cells) << '\n';

    return 0;
}

