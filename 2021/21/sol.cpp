#include <bits/stdc++.h>

using namespace std;

auto solve1(array<int, 2> positions)
{
    static const auto ScoreThreshold = 1000;

    array<int, 2> scores{};
    auto dice = 0;
    auto turn = 0;

    while (true) {
        auto moves = 0;
        for (auto i = 0; i < 3; ++i)
            moves += ++dice;

        positions[turn] += moves;
        positions[turn] %= 10;
        if (positions[turn] == 0)
            positions[turn] = 10;

        scores[turn] += positions[turn];

        if (scores[turn] >= ScoreThreshold)
            return scores[1 - turn] * dice;

        turn = 1 - turn;
    }
}

struct State {
    array<int, 2> scores{};
    array<int, 2> positions;
    int turn{0};

    State(array<int, 2> positions) : positions{positions} {}
};

bool operator<(const State& s1, const State& s2)
{
    return tie(s1.scores, s1.positions, s1.turn) < tie(s2.scores, s2.positions, s2.turn);
}

auto solve2(array<int, 2> positions)
{
    static const auto ScoreThreshold = 21;

    auto moves2freq = []() {
        map<int, int> moves2freq;

        for (auto i = 1; i <= 3; ++i)
            for (auto j = 1; j <= 3; ++j)
                for (auto k = 1; k <= 3; ++k)
                    ++moves2freq[i + j + k];

        return moves2freq;
    }();

    map<State, long> state2freq;
    state2freq[State{positions}] = 1;

    array<long, 2> wins{};

    while (!state2freq.empty()) {
        auto [state, state_freq] = *begin(state2freq);
        state2freq.erase(begin(state2freq));

        for (const auto& [moves, moves_freq] : moves2freq) {
            auto freq = state_freq * moves_freq;

            State new_state{state};
            auto& [scores, positions, turn] = new_state;

            positions[turn] += moves;
            positions[turn] %= 10;
            if (positions[turn] == 0)
                positions[turn] = 10;

            scores[turn] += positions[turn];

            if (scores[turn] >= ScoreThreshold) {
                wins[turn] += freq;
                continue;
            }

            turn = 1 - turn;

            state2freq[new_state] += freq;
        }
    }

    return max(wins[0], wins[1]);
}

int main()
{
    static const regex r{R"(Player ([12]) starting position: ([0-9]+))"};

    auto to_num = [](const string& s) {
        return strtol(s.data(), nullptr, 10);
    };

    array<int, 2> positions;

    for (auto i = 0; i < 2; ++i) {
        string line;
        getline(cin, line);

        smatch matched;
        regex_match(line, matched, r);

        auto player = to_num(matched[1].str());
        auto position = to_num(matched[2].str());

        positions[player - 1] = position;
    }

    cout << solve1(positions) << '\n';
    cout << solve2(positions) << '\n';

    return 0;
}

