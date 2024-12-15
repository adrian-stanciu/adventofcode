#include <bits/stdc++.h>

using namespace std;

auto find_start(const vector<string>& grid, int n, int m)
{
    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (grid[r][c] == '@')
                return make_pair(r, c);

    assert(false);
}

auto compute_score(const vector<string>& grid, int n, int m, char ch)
{
    auto score = 0;

    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (grid[r][c] == ch)
                score += 100 * r + c;

    return score;
}

auto simulate1(vector<string>& grid, int n, int m, const string& moves)
{
    auto [r, c] = find_start(grid, n, m);

    for (auto ch : moves) {
        auto dr = 0;
        auto dc = 0;

        switch (ch) {
        case '^':
            dr = -1;
            break;
        case 'v':
            dr = 1;
            break;
        case '<':
            dc = -1;
            break;
        case '>':
            dc = 1;
            break;
        }

        for (auto nr = r + dr, nc = c + dc; grid[nr][nc] != '#'; nr += dr, nc += dc)
            if (grid[nr][nc] == '.') {
                for (; nr != r || nc != c; nr -= dr, nc -= dc)
                    grid[nr][nc] = grid[nr - dr][nc - dc];

                grid[r][c] = '.';

                r += dr;
                c += dc;

                break;
            }
    }
}

auto solve1(vector<string>& grid, const string& moves)
{
    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    simulate1(grid, n, m, moves);

    return compute_score(grid, n, m, 'O');
}

auto simulate2(vector<string>& grid, int n, int m, const string& moves)
{
    auto move = [&](auto r, auto c, auto dr) {
        queue<pair<int, int>> q;
        q.emplace(r, c);

        vector<vector<bool>> in_q(n, vector<bool>(m));
        in_q[r][c] = true;

        vector<pair<int, int>> to_swap;

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            if (auto nr = r + dr; grid[nr][c] == '#') {
                return false;
            } else if (grid[nr][c] != '.') { // '[' or ']'
                if (!in_q[nr][c]) {
                    q.emplace(nr, c);
                    in_q[nr][c] = true;
                }

                if (auto nc = grid[nr][c] == '[' ? c + 1 : c - 1; !in_q[nr][nc]) {
                    q.emplace(nr, nc);
                    in_q[nr][nc] = true;
                }
            }

            to_swap.emplace_back(r, c);
        }

        reverse(begin(to_swap), end(to_swap));

        for (const auto& [r, c] : to_swap)
            swap(grid[r][c], grid[r + dr][c]);

        return true;
    };

    auto [r, c] = find_start(grid, n, m);

    for (auto ch : moves) {
        auto dr = 0;
        auto dc = 0;

        switch (ch) {
        case '^':
            dr = -1;
            break;
        case 'v':
            dr = 1;
            break;
        case '<':
            dc = -1;
            break;
        case '>':
            dc = 1;
            break;
        }

        if (dc) {
            for (auto nc = c + dc; grid[r][nc] != '#'; nc += dc)
                if (grid[r][nc] == '.') {
                    for (; nc != c; nc -= dc)
                        grid[r][nc] = grid[r][nc - dc];

                    grid[r][c] = '.';

                    c += dc;

                    break;
                }
        } else if (move(r, c, dr)) {
            r += dr;
        }
    }
}

auto solve2(vector<string>& grid, const string& moves)
{
    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    simulate2(grid, n, m, moves);

    return compute_score(grid, n, m, '[');
}

int main()
{
    auto grid1 = []() {
        vector<string> grid;

        string s;
        while (getline(cin, s)) {
            if (s.empty())
                break;

            grid.push_back(move(s));
        }

        return grid;
    }();

    auto grid2 = [&]() {
        vector<string> grid2;

        for (const auto& s1: grid1) {
            string s2;
            for (auto ch : s1)
                if (ch == '#')
                    s2.append("##");
                else if (ch == '.')
                    s2.append("..");
                else if (ch == 'O')
                    s2.append("[]");
                else if (ch == '@')
                    s2.append("@.");

            grid2.push_back(move(s2));
        }

        return grid2;
    }();

    auto moves = []() {
        string moves;

        string s;
        while (getline(cin, s))
            moves.append(s);

        return moves;
    }();

    cout << solve1(grid1, moves) << '\n';
    cout << solve2(grid2, moves) << '\n';

    return 0;
}

