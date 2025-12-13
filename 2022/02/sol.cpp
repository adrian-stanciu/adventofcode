#include <bits/stdc++.h>

using namespace std;

enum class Move : int {
    Rock = 1,
    Paper = 2,
    Scissors = 3,
};

auto char2move(char c)
{
    using enum Move;
    switch (c) {
    case 'A':
        [[fallthrough]];
    case 'X':
        return Rock;
    case 'B':
        [[fallthrough]];
    case 'Y':
        return Paper;
    case 'C':
        [[fallthrough]];
    case 'Z':
        return Scissors;
    }
    assert(false);
}

auto move2int(Move m)
{
    return static_cast<int>(m);
}

Move wins(Move m)
{
    using enum Move;
    switch (m) {
    case Rock:
        return Paper;
    case Paper:
        return Scissors;
    case Scissors:
        return Rock;
    }
    assert(false);
}

Move loses(Move m)
{
    using enum Move;
    switch (m) {
    case Rock:
        return Scissors;
    case Paper:
        return Rock;
    case Scissors:
        return Paper;
    }
    assert(false);
}

enum class Result : int {
    Lose = 0,
    Draw = 3,
    Win = 6,
};

auto char2result(char c)
{
    using enum Result;
    switch (c) {
    case 'X':
        return Lose;
    case 'Y':
        return Draw;
    case 'Z':
        return Win;
    }
    assert(false);
}

auto result2int(Result r)
{
    return static_cast<int>(r);
}

Result play(Move his_move, Move my_move)
{
    using enum Result;
    if (his_move == my_move)
        return Draw;
    return wins(his_move) == my_move ? Win : Lose;
}

Move deduce_my_move(Move his_move, Result result)
{
    using enum Result;
    if (result == Draw)
        return his_move;
    return result == Win ? wins(his_move) : loses(his_move);
}

int main()
{
    auto score1 = 0;
    auto score2 = 0;

    string line;
    while (getline(cin, line)) {
        auto his_move = char2move(line.front());

        auto my_move = char2move(line.back());
        score1 += move2int(my_move) + result2int(play(his_move, my_move));

        auto result = char2result(line.back());
        score2 += move2int(deduce_my_move(his_move, result)) + result2int(result);
    }

    cout << score1 << '\n';
    cout << score2 << '\n';

    return 0;
}

