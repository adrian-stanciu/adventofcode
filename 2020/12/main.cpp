#include <bits/stdc++.h>

using namespace std;

struct Instr {
    char action;
    int value;

    Instr(char action, int value)
    : action(action)
    , value(value)
    {}
};

auto solve1(const vector<Instr>& instr_vec, int dx, int dy)
{
    auto x = 0;
    auto y = 0;

    auto turn_back = [&] () {
        dx *= -1;
        dy *= -1;
    };
    auto turn_left = [&] () {
        swap(dx, dy);
        dx *= -1;
    };
    auto turn_right = [&] () {
        swap(dx, dy);
        dy *= -1;
    };

    for (auto [action, value] : instr_vec)
        switch (action) {
        case 'F':
            x += dx * value;
            y += dy * value;
            break;
        case 'N':
            y += value;
            break;
        case 'S':
            y -= value;
            break;
        case 'W':
            x -= value;
            break;
        case 'E':
            x += value;
            break;
        case 'L':
            if (value == 90)
                turn_left();
            else if (value == 180)
                turn_back();
            else
                turn_right();
            break;
        case 'R':
            if (value == 90)
                turn_right();
            else if (value == 180)
                turn_back();
            else
                turn_left();
            break;
        }

    return abs(x) + abs(y);
}

auto solve2(const vector<Instr>& instr_vec, int dx, int dy)
{
    auto x = 0;
    auto y = 0;

    auto turn_back = [&] () {
        dx *= -1;
        dy *= -1;
    };
    auto turn_left = [&] () {
        swap(dx, dy);
        dx *= -1;
    };
    auto turn_right = [&] () {
        swap(dx, dy);
        dy *= -1;
    };

    for (auto [action, value] : instr_vec)
        switch (action) {
        case 'F':
            x += dx * value;
            y += dy * value;
            break;
        case 'N':
            dy += value;
            break;
        case 'S':
            dy -= value;
            break;
        case 'W':
            dx -= value;
            break;
        case 'E':
            dx += value;
            break;
        case 'L':
            if (value == 90)
                turn_left();
            else if (value == 180)
                turn_back();
            else
                turn_right();
            break;
        case 'R':
            if (value == 90)
                turn_right();
            else if (value == 180)
                turn_back();
            else
                turn_left();
            break;
        }

    return abs(x) + abs(y);
}

int main()
{
    vector<Instr> instr_vec;

    char action;
    while (cin >> action) {
        int value;
        cin >> value;
        instr_vec.emplace_back(action, value);
    }

    cout << solve1(instr_vec, 1, 0) << '\n';
    cout << solve2(instr_vec, 10, 1) << '\n';

    return 0;
}

