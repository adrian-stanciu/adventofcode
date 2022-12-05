#include <bits/stdc++.h>

using namespace std;

auto read_stacks()
{
    vector<string> stacks_lines;

    string line;
    while (getline(cin, line)) {
        if (line.empty())
            break;
        stacks_lines.push_back(move(line));
    }

    reverse(begin(stacks_lines), end(stacks_lines));

    // the numbers' positions on the first line are the stacks' positions in next lines
    vector<int> stacks_positions;
    for (auto i = 0; i < ssize(stacks_lines[0]); ++i)
        if (isdigit(stacks_lines[0][i]))
            stacks_positions.push_back(i);
    // assume the number of stacks is a one digit number
    assert(ssize(stacks_positions) < 10);

    stacks_lines.erase(begin(stacks_lines));

    vector<stack<char>> stacks(ssize(stacks_positions) + 1);

    for (const auto& line : stacks_lines)
        for (auto i = 0; i < ssize(stacks_positions); ++i)
            if (isupper(line[stacks_positions[i]]))
                stacks[i + 1].push(line[stacks_positions[i]]);

    return stacks;
}

struct Move {
    int cnt;
    int src;
    int dst;
};

auto read_moves()
{
    vector<Move> moves;

    string line;
    while (getline(cin, line)) {
        int cnt, src, dst;
        sscanf(line.data(), "move %d from %d to %d", &cnt, &src, &dst);
        moves.emplace_back(cnt, src, dst);
    }

    return moves;
}

void solve1(vector<stack<char>> stacks, const vector<Move>& moves)
{
    for (auto [cnt, src, dst] : moves)
        while (cnt--) {
            stacks[dst].push(stacks[src].top());
            stacks[src].pop();
        }

    for (auto i = 1; i < ssize(stacks); ++i)
        cout << stacks[i].top();
    cout << '\n';
}

void solve2(vector<stack<char>> stacks, const vector<Move>& moves)
{
    for (auto [cnt, src, dst] : moves) {
        stack<char> tmp_stack;

        while (cnt--) {
            tmp_stack.push(stacks[src].top());
            stacks[src].pop();
        }

        while (!tmp_stack.empty()) {
            stacks[dst].push(tmp_stack.top());
            tmp_stack.pop();
        }
    }

    for (auto i = 1; i < ssize(stacks); ++i)
        cout << stacks[i].top();
    cout << '\n';
}

int main()
{
    auto stacks{read_stacks()};
    auto moves{read_moves()};

    solve1(stacks, moves);
    solve2(stacks, moves);

    return 0;
}

