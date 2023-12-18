#include <bits/stdc++.h>

using namespace std;

auto str2num(string_view sv)
{
    auto n = 0;
    from_chars(sv.data(), sv.data() + sv.size(), n);
    return n;
}

auto split(string_view sv, char sep)
{
    while (sv.front() == sep)
        sv.remove_prefix(1);
    while (sv.back() == sep)
        sv.remove_suffix(1);

    vector<string_view> split_sv;

    while (true)
        if (auto to = sv.find_first_of(sep); to == sv.npos) {
            split_sv.push_back(sv);
            break;
        } else {
            split_sv.push_back(sv.substr(0, to));
            sv.remove_prefix(to + 1);
        }

    return split_sv;
}

auto hex2dec(char hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    else
        return 10 + hex - 'a';
}

auto hex2dec(string_view hex)
{
    auto dec = 0l;
    for (auto hex_digit : hex) {
        dec *= 16;
        dec += hex2dec(hex_digit);
    }
    return dec;
}

auto turns_left(char curr_dir, char next_dir)
{
    switch (curr_dir) {
    case 'U':
        return next_dir == 'L';
    case 'D':
        return next_dir == 'R';
    case 'L':
        return next_dir == 'D';
    case 'R':
        return next_dir == 'U';
    }
    assert(false);
}

auto comp_area(const vector<pair<int, int>>& points)
{
    // shoelace formula for polygon area

    auto area = 0l;

    for (auto i = 0; i < ssize(points) - 1; ++i)
        area += 1l * points[i].first * points[i + 1].second -
            1l * points[i + 1].first * points[i].second;

    return abs(area / 2);
}

auto solve(const vector<pair<char, int>>& moves)
{
    vector<pair<int, int>> points; // top-left vertices of endpoint squares

    auto x = 0;
    auto y = 0;
    for (const auto& [dir, steps] : moves) {
        auto dy = 0;
        auto dx = 0;
        switch (dir) {
        case 'U':
            dy = 1;
            break;
        case 'D':
            dy = -1;
            break;
        case 'L':
            dx = -1;
            break;
        case 'R':
            dx = 1;
            break;
        }

        x += dx * steps;
        y += dy * steps;

        points.emplace_back(x, y);
    }

    vector<pair<int, int>> l_points; // vertices of left polygon
    vector<pair<int, int>> r_points; // vertices of right polygon

    for (auto i = 0; i < ssize(moves); ++i) {
        l_points.push_back(points[i]);
        r_points.push_back(points[i]);

        auto curr_dir = moves[i].first;
        auto next_dir = (i + 1 < ssize(moves)) ? moves[i + 1].first : moves[0].first;

        switch (curr_dir) {
        case 'U':
            r_points.back().first += 1;
            if (turns_left(curr_dir, next_dir))
                l_points.back().second -= 1;
            else
                r_points.back().second -= 1;
            break;
        case 'D':
            l_points.back().first += 1;
            l_points.back().second -= 1;
            r_points.back().second -= 1;
            if (turns_left(curr_dir, next_dir))
                l_points.back().second += 1;
            else
                r_points.back().second += 1;
            break;
        case 'L':
            l_points.back().second -= 1;
            if (turns_left(curr_dir, next_dir))
                l_points.back().first += 1;
            else
                r_points.back().first += 1;
            break;
        case 'R':
            l_points.back().first += 1;
            r_points.back().first += 1;
            r_points.back().second -= 1;
            if (turns_left(curr_dir, next_dir))
                l_points.back().first -= 1;
            else
                r_points.back().first -= 1;
            break;
        }
    }

    l_points.push_back(l_points.front());
    r_points.push_back(r_points.front());

    return max(comp_area(l_points), comp_area(r_points));
}

int main()
{
    vector<pair<char, int>> moves1;
    vector<pair<char, int>> moves2;

    string s;
    while (getline(cin, s)) {
        auto parts = split(string_view{s}, ' ');

        moves1.emplace_back(parts[0].front(), str2num(parts[1]));

        parts[2].remove_prefix(2);
        parts[2].remove_suffix(1);
        auto dir = parts[2].back();
        switch (dir) {
        case '0':
            dir = 'R';
            break;
        case '1':
            dir = 'D';
            break;
        case '2':
            dir = 'L';
            break;
        case '3':
            dir = 'U';
            break;
        }
        parts[2].remove_suffix(1);

        moves2.emplace_back(dir, hex2dec(parts[2]));
    }

    cout << solve(moves1) << '\n';
    cout << solve(moves2) << '\n';

    return 0;
}

