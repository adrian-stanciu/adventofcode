#include <bits/stdc++.h>

using namespace std;

const auto NegInf = numeric_limits<int>::min();

auto sim(int from_x, int to_x, int dx, int from_y, int to_y, int dy)
{
    auto x = 0;
    auto y = 0;

    bool ok = false;
    auto max_y = NegInf;

    while (x <= to_x && y >= from_y) {
        x += dx;
        y += dy;

        ok |= from_x <= x && x <= to_x && from_y <= y && y <= to_y;
        max_y = max(max_y, y);

        dx = max(dx - 1, 0);
        --dy;
    }

    if (ok)
        return max_y;
    else
        return NegInf;
}

void solve(int from_x, int to_x, int from_y, int to_y)
{
    auto max_y = NegInf;
    auto cnt = 0;

    for (auto dx = 1; dx <= to_x; ++dx)
        for (auto dy = from_y; dy < -from_y; ++dy) {
            auto res = sim(from_x, to_x, dx, from_y, to_y, dy);

            max_y = max(max_y, res);
            cnt += res != NegInf;
        }

    cout << max_y << '\n';
    cout << cnt << '\n';
}

int main()
{
    static const regex r{R"(target area: x=(-?[0-9]+)..(-?[0-9]+), y=(-[0-9]+)..(-[0-9]+))"};

    auto to_num = [](const string& s) {
        return strtol(s.data(), nullptr, 10);
    };

    string line;
    getline(cin, line);

    smatch matched;
    regex_match(line, matched, r);

    auto from_x = to_num(matched[1].str());
    auto to_x = to_num(matched[2].str());
    auto from_y = to_num(matched[3].str());
    auto to_y = to_num(matched[4].str());

    assert(from_x > 0 || to_x < 0);
    assert(to_y < 0);

    if (to_x < 0) {
        swap(from_x, to_x);
        from_x *= -1;
        to_x *= -1;
    }

    solve(from_x, to_x, from_y, to_y);

    return 0;
}

