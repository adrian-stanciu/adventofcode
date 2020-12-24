#include <bits/stdc++.h>

using namespace std;

using Tile = vector<string>;

void flip(Tile& tile)
{
    for (auto& row : tile)
        reverse(begin(row), end(row));
}

void rotate(Tile& tile)
{
    auto n = tile.size();
    for (size_t i = 0; i < n / 2; ++i)
        for (auto j = i; j < n - i - 1; ++j) {
            swap(tile[i][j], tile[n - 1 - j][i]);
            swap(tile[n - 1 - j][i], tile[n - 1 - i][n - 1 - j]);
            swap(tile[n - 1 - i][n - 1 - j], tile[j][n - 1 - i]);
        }
}

auto top_row(const Tile& tile)
{
    return tile.front();
}
auto bot_row(const Tile& tile)
{
    return tile.back();
}
auto lhs_col(const Tile& tile)
{
    string col;
    for (const auto& row : tile)
        col.push_back(row.front());
    return col;
}
auto rhs_col(const Tile& tile)
{
    string col;
    for (const auto& row : tile)
        col.push_back(row.back());
    return col;
}

auto build_graph(const unordered_map<int, Tile>& tiles)
{
    unordered_map<int, set<string>> borders_map;

    for (const auto& [id, tile] : tiles) {
        set<string> borders;

        auto top = top_row(tile);
        borders.insert(top);
        reverse(begin(top), end(top));
        borders.insert(move(top));

        auto bot = bot_row(tile);
        borders.insert(bot);
        reverse(begin(bot), end(bot));
        borders.insert(move(bot));

        auto lhs = lhs_col(tile);
        borders.insert(lhs);
        reverse(begin(lhs), end(lhs));
        borders.insert(move(lhs));

        auto rhs = rhs_col(tile);
        borders.insert(rhs);
        reverse(begin(rhs), end(rhs));
        borders.insert(move(rhs));

        borders_map.emplace(id, move(borders));
    }

    unordered_map<int, set<int>> g;

    for (const auto& [id1, borders1] : borders_map)
        for (const auto& [id2, borders2] : borders_map)
            if (id1 < id2) {
                vector<string> intersection;
                set_intersection(begin(borders1), end(borders1), begin(borders2), end(borders2),
                    inserter(intersection, begin(intersection)));
                if (!intersection.empty()) {
                    g[id1].emplace(id2);
                    g[id2].emplace(id1);
                }
            }

    return g;
}

auto place_tiles(const unordered_map<int, set<int>>& g, int img_sz)
{
    vector<vector<int>> ids_matrix(img_sz, vector<int>(img_sz));

    auto top_lhs_corner = find_if(begin(g), end(g), [] (const auto& node) {
        return node.second.size() == 2;
    })->first;

    ids_matrix[0][0] = top_lhs_corner;
    auto it = begin(g.at(top_lhs_corner));
    ids_matrix[0][1] = *it;
    ids_matrix[1][0] = *next(it);

    auto avail = [&] (auto row, auto col) {
        const auto& top = g.at(ids_matrix[row - 1][col]);
        const auto& lhs = g.at(ids_matrix[row][col - 1]);
        set<int> candidates;
        set_intersection(begin(top), end(top), begin(lhs), end(lhs),
            inserter(candidates, begin(candidates)));
        candidates.erase(ids_matrix[row - 1][col - 1]);
        return *begin(candidates);
    };
    auto avail_first_row = [&] (auto row, auto col) {
        auto candidates = g.at(ids_matrix[row][col]);
        candidates.erase(ids_matrix[row + 1][col]);
        candidates.erase(ids_matrix[row][col - 1]);
        return *begin(candidates);
    };
    auto avail_first_col = [&] (auto row, auto col) {
        auto candidates = g.at(ids_matrix[row][col]);
        candidates.erase(ids_matrix[row - 1][col]);
        candidates.erase(ids_matrix[row][col + 1]);
        return *begin(candidates);
    };

    for (auto k = 1; k < img_sz - 1; ++k) {
        for (auto row = 1, col = k; col >= 1; ++row, --col)
            ids_matrix[row][col] = avail(row, col);
        ids_matrix[0][k + 1] = avail_first_row(0, k);
        ids_matrix[k + 1][0] = avail_first_col(k, 0);
    }
    for (auto k = 1; k < img_sz; ++k)
        for (auto row = k, col = img_sz - 1; row < img_sz; ++row, --col)
            ids_matrix[row][col] = avail(row, col);

    return ids_matrix;
}

auto orient_tiles(const unordered_map<int, Tile>& tiles, int img_sz,
    const vector<vector<int>>& ids_matrix)
{
    unordered_map<int, Tile> oriented_tiles;

    auto orient_tile = [&] (auto row, auto col, const auto& fits) {
        auto id = ids_matrix[row][col];
        auto tile = tiles.at(id);
        for (auto i = 0; i < 2; ++i) {
            for (auto j = 0; j < 4; ++j) {
                if (fits(tile, row, col)) {
                    oriented_tiles.emplace(id, move(tile));
                    return;
                }
                rotate(tile);
            }
            flip(tile);
        }
    };

    auto common_border = [&] (const auto& tile1, auto tile2) {
        const set<string> borders{
            top_row(tile1), bot_row(tile1),
            lhs_col(tile1), rhs_col(tile1),
        };

        for (auto i = 0; i < 2; ++i) {
            for (auto j = 0; j < 4; ++j) {
                auto top = top_row(tile2);
                if (borders.count(top) > 0)
                    return top;
                rotate(tile2);
            }
            flip(tile2);
        }
        return std::string();
    };

    auto fits_top_lhs = [&] (const auto& tile, auto row, auto col) {
        return top_row(tile) == common_border(tile, tiles.at(ids_matrix[row - 1][col])) &&
               lhs_col(tile) == common_border(tile, tiles.at(ids_matrix[row][col - 1]));
    };
    auto fits_top_rhs = [&] (const auto& tile, auto row, auto col) {
        return top_row(tile) == common_border(tile, tiles.at(ids_matrix[row - 1][col])) &&
               rhs_col(tile) == common_border(tile, tiles.at(ids_matrix[row][col + 1]));
    };
    auto fits_bot_lhs = [&] (const auto& tile, auto row, auto col) {
        return bot_row(tile) == common_border(tile, tiles.at(ids_matrix[row + 1][col])) &&
               lhs_col(tile) == common_border(tile, tiles.at(ids_matrix[row][col - 1]));
    };
    auto fits_bot_rhs = [&] (const auto& tile, auto row, auto col) {
        return bot_row(tile) == common_border(tile, tiles.at(ids_matrix[row + 1][col])) &&
               rhs_col(tile) == common_border(tile, tiles.at(ids_matrix[row][col + 1]));
    };

    orient_tile(0, 0, fits_bot_rhs);
    for (auto col = 1; col < img_sz; ++col)
        orient_tile(0, col, fits_bot_lhs);
    for (auto row = 1; row < img_sz; ++row)
        orient_tile(row, 0, fits_top_rhs);
    for (auto row = 1; row < img_sz; ++row)
        for (auto col = 1; col < img_sz; ++col)
            orient_tile(row, col, fits_top_lhs);

    return oriented_tiles;
}

auto build_image(const unordered_map<int, Tile>& tiles, int img_sz,
    const vector<vector<int>>& ids_matrix)
{
    auto tile_sz = begin(tiles)->second.size();
    vector<string> img(img_sz * (tile_sz - 2));

    for (auto row = 0; row < img_sz; ++row)
        for (auto col = 0; col < img_sz; ++col) {
            auto tile = tiles.at(ids_matrix[row][col]);
            for (size_t r = 1; r < tile_sz - 1; ++r)
                for (size_t c = 1; c < tile_sz - 1; ++c)
                    img[row * (tile_sz - 2) + r - 1].push_back(tile[r][c]);
        }

    return img;
}

auto matches_pattern(const Tile& img, int start_row, int start_col, const Tile& pattern)
{
    for (size_t i = 0; i < pattern.size(); ++i)
        for (size_t j = 0; j < pattern[0].size(); ++j)
            if (pattern[i][j] == '#' && img[start_row + i][start_col + j] != '#')
                return false;
    return true;
}

auto count_pattern_matches(const Tile& img, const Tile& pattern)
{
    auto cnt = 0;
    for (size_t i = 0; i <= img.size() - pattern.size(); ++i)
        for (size_t j = 0; j <= img[0].size() - pattern[0].size(); ++j)
            cnt += matches_pattern(img, i, j, pattern);
    return cnt;
}

auto count_occurences(Tile img, const Tile& pattern)
{
    for (auto i = 0; i < 2; ++i) {
        for (auto j = 0; j < 4; ++j) {
            auto cnt = count_pattern_matches(img, pattern);
            if (cnt > 0)
                return cnt;
            rotate(img);
        }
        flip(img);
    }
    return 0;
}

auto solve(const unordered_map<int, Tile>& tiles, const Tile& pattern)
{
    auto g = build_graph(tiles);

    cout << accumulate(begin(g), end(g), 1l, [] (auto acc, const auto& node) {
        return acc * ((node.second.size() == 2) ? node.first : 1);
    }) << '\n';

    auto img_sz = static_cast<int>(sqrt(g.size()));
    auto ids_matrix = place_tiles(g, img_sz);
    auto oriented_tiles = orient_tiles(tiles, img_sz, ids_matrix);
    auto img = build_image(oriented_tiles, img_sz, ids_matrix);

    auto count_ch = [] (const auto& img, auto ch) {
        return accumulate(begin(img), end(img), 0, [&] (auto acc, const auto& row) {
            return acc + count(begin(row), end(row), ch);
        });
    };

    cout << count_ch(img, '#') - count_occurences(img, pattern) * count_ch(pattern, '#') << '\n';
}

int main()
{
    static const regex r{"Tile ([1-9][0-9]*):"};

    unordered_map<int, Tile> tiles;

    string line;
    while (getline(cin, line)) {
        smatch matched;
        regex_match(line, matched, r);
        auto id = strtol(matched[1].str().data(), nullptr, 10);
        Tile tile;
        while (getline(cin, line) && !line.empty())
            tile.push_back(move(line));
        tiles.emplace(id, move(tile));
    }

    solve(tiles, Tile{
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   ",
    });

    return 0;
}

