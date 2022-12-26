#include <bits/stdc++.h>

using namespace std;

auto score(int r, int c, int dr, int dc)
{
    auto dir_score = 0;
    if (dr == 1)
        dir_score = 1;
    else if (dc == -1)
        dir_score = 2;
    else if (dr == -1)
        dir_score = 3;
    return 1000 * r + 4 * c + dir_score;
}

auto solve(const vector<string>& m, const string& s,
    const function<tuple<int, int, int, int>(const vector<string>&, int, int, int, int)>& move)
{
    auto r = 1;
    auto c = 1;
    while (m[r][c] != '.')
        ++c;
    auto dr = 0;
    auto dc = 1;

    for (auto i = 0; i < ssize(s); ++i)
        if (s[i] == 'L') {
            if (dc)
                dc *= -1;
            swap(dr, dc);
        } else if (s[i] == 'R') {
            if (dr)
                dr *= -1;
            swap(dr, dc);
        } else {
            auto n = 0;
            while (i < ssize(s) && isdigit(s[i])) {
                n *= 10;
                n += s[i] - '0';
                ++i;
            }
            --i;

            for (auto i = 0; i < n; ++i) {
                auto [nr, nc, ndr, ndc] = move(m, r, c, dr, dc);
                if (m[nr][nc] == '#')
                    break;
                r = nr;
                c = nc;
                dr = ndr;
                dc = ndc;
            }
        }

    return score(r, c, dr, dc);
}

auto move_flat(const vector<string>& m, int r, int c, int dr, int dc)
{
    auto nr = r + dr;
    auto nc = c + dc;

    if (m[nr][nc] == ' ') {
        if (dr) {
            if (dr == 1)
                nr = 0;
            else
                nr = ssize(m) - 1;
            while (m[nr][nc] == ' ')
                nr += dr;
        } else {
            if (dc == 1)
                nc = 0;
            else
                nc = ssize(m[nr]) - 1;
            while (m[nr][nc] == ' ')
                nc += dc;
        }
    }

    return make_tuple(nr, nc, dr, dc);
}

struct Edge {
    static const auto Size{50};

    function<bool(int, int, int, int)> contains_point;
    function<tuple<int, int, int, int>(int, int)> change_face;
};

//   -----
//   |U|R|
//   -----
//   |B|
// -----
// |L|D|
// -----
// |F|
// ---
const vector<Edge> edges{
    { // nord of Up face
        .contains_point = [](int nr, int nc, int, int) {
            return nr == 0 && (Edge::Size < nc && nc <= 2 * Edge::Size);
        },
        .change_face = [](int, int nc) {
            return make_tuple(3 * Edge::Size + (nc - Edge::Size), 1, 0, 1);
        }
    }, { // nord of Right face
        .contains_point = [](int nr, int nc, int, int) {
            return nr == 0 && (2 * Edge::Size < nc && nc <= 3 * Edge::Size);
        },
        .change_face = [](int, int nc) {
            return make_tuple(4 * Edge::Size, nc - 2 * Edge::Size, -1, 0);
        }
    }, { // east of Right face
        .contains_point = [](int, int nc, int, int) {
            return nc == 3 * Edge::Size + 1;
        },
        .change_face = [](int nr, int) {
            return make_tuple(3 * Edge::Size - nr + 1, 2 * Edge::Size, 0, -1);
        }
    }, { // south of Right face
        .contains_point = [](int nr, int nc, int dr, int) {
            return nr == Edge::Size + 1 && (2 * Edge::Size < nc && nc <= 3 * Edge::Size) && dr == 1;
        },
        .change_face = [](int, int nc) {
            return make_tuple(Edge::Size + (nc - 2 * Edge::Size), 2 * Edge::Size, 0, -1);
        }
    }, { // est of Back face
        .contains_point = [](int nr, int nc, int, int dc) {
            return (Edge::Size < nr && nr <= 2 * Edge::Size) && nc == 2 * Edge::Size + 1 && dc == 1;
        },
        .change_face = [](int nr, int) {
            return make_tuple(Edge::Size, 2 * Edge::Size + (nr - Edge::Size), -1, 0);
        }
    }, { // east of Down face
        .contains_point = [](int nr, int nc, int, int) {
            return (2 * Edge::Size < nr && nr <= 3 * Edge::Size) && nc == 2 * Edge::Size + 1;
        },
        .change_face = [](int nr, int) {
            return make_tuple(3 * Edge::Size - nr + 1, 3 * Edge::Size, 0, -1);
        }
    }, { // south of Down face
        .contains_point = [](int nr, int nc, int dr, int) {
            return nr == 3 * Edge::Size + 1 && (Edge::Size < nc && nc <= 2 * Edge::Size) && dr == 1;
        },
        .change_face = [](int, int nc) {
            return make_tuple(3 * Edge::Size + (nc - Edge::Size), Edge::Size, 0, -1);
        }
    }, { // east of Front face
        .contains_point = [](int nr, int nc, int, int dc) {
            return (3 * Edge::Size < nr && nr <= 4 * Edge::Size) && nc == Edge::Size + 1 && dc == 1;
        },
        .change_face = [](int nr, int) {
            return make_tuple(3 * Edge::Size, Edge::Size + nr - 3 * Edge::Size, -1, 0);
        }
    }, { // south of Front face
        .contains_point = [](int nr, int, int, int) {
            return nr == 4 * Edge::Size + 1;
        },
        .change_face = [](int, int nc) {
            return make_tuple(1, 2 * Edge::Size + nc, 1, 0);
        }
    }, { // west of Front face
        .contains_point = [](int nr, int nc, int, int) {
            return (3 * Edge::Size < nr && nr <= 4 * Edge::Size) && nc == 0;
        },
        .change_face = [](int nr, int) {
            return make_tuple(1, Edge::Size + (nr - 3 * Edge::Size), 1, 0);
        }
    }, { // west of Left face
        .contains_point = [](int nr, int nc, int, int) {
            return (2 * Edge::Size < nr && nr <= 3 * Edge::Size) && nc == 0;
        },
        .change_face = [](int nr, int) {
            return make_tuple(3 * Edge::Size - nr + 1, Edge::Size + 1, 0, 1);
        }
    }, { // nord of Left face
        .contains_point = [](int nr, int nc, int dr, int) {
            return nr == 2 * Edge::Size && (0 < nc && nc <= Edge::Size) && dr == -1;
        },
        .change_face = [](int, int nc) {
            return make_tuple(Edge::Size + nc, Edge::Size + 1, 0, 1);
        }
    }, { // west of Back face
        .contains_point = [](int nr, int nc, int, int dc) {
            return (Edge::Size < nr && nr <= 2 * Edge::Size) && nc == Edge::Size && dc == -1;
        },
        .change_face = [](int nr, int) {
            return make_tuple(2 * Edge::Size + 1, nr - Edge::Size, 1, 0);
        }
    }, { // west of Up face
        .contains_point = [](int nr, int nc, int, int) {
            return (0 < nr && nr <= Edge::Size) && nc == Edge::Size;
        },
        .change_face = [](int nr, int) {
            return make_tuple(3 * Edge::Size - nr + 1, 1, 0, 1);
        }
    }
};

auto move_cube(const vector<string>& m, int r, int c, int dr, int dc)
{
    auto nr = r + dr;
    auto nc = c + dc;

    if (m[nr][nc] != ' ')
        return make_tuple(nr, nc, dr, dc);

    for (const auto& e : edges)
        if (e.contains_point(nr, nc, dr, dc))
            return e.change_face(nr, nc);
    assert(false);
}

int main()
{
    vector<string> m;
    auto max_width = 0l;

    string line;
    while (getline(cin, line)) {
        if (line.empty())
            break;

        line.insert(begin(line), ' ');
        line.append(1, ' ');

        max_width = max(max_width, ssize(line));
        m.push_back(move(line));
    }

    for (auto& row : m)
        row.append(max_width - ssize(row), ' ');

    m.insert(begin(m), string(max_width, ' '));
    m.insert(end(m), string(max_width, ' '));

    string s;
    cin >> s;

    cout << solve(m, s, move_flat) << '\n';
    cout << solve(m, s, move_cube) << '\n';

    return 0;
}

