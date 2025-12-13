#include <bits/stdc++.h>

#include "hasher.hpp"

using namespace std;

auto play_game1(deque<int> deck1, deque<int> deck2)
{
    while (!deck1.empty() && !deck2.empty()) {
        auto card1 = deck1.front();
        deck1.pop_front();
        auto card2 = deck2.front();
        deck2.pop_front();

        if (card1 > card2) {
            deck1.push_back(card1);
            deck1.push_back(card2);
        } else {
            deck2.push_back(card2);
            deck2.push_back(card1);
        }
    }

    return !deck1.empty() ? deck1 : deck2;
}

auto is_win_for_player1(deque<int>& deck1, deque<int>& deck2)
{
    unordered_set<vector<int>, VectorHasher<int>> seen;

    auto is_inf_game = [&] (const auto& deck1, const auto& deck2) {
        vector<int> fp;
        fp.reserve(deck1.size() + 1 + deck2.size());

        copy(begin(deck1), end(deck1), back_inserter(fp));
        fp.push_back(0);
        copy(begin(deck2), end(deck2), back_inserter(fp));

        if (seen.count(fp) > 0)
            return true;

        seen.insert(move(fp));
        return false;
    };

    while (!deck1.empty() && !deck2.empty()) {
        if (is_inf_game(deck1, deck2))
            return true;

        auto card1 = deck1.front();
        deck1.pop_front();
        auto card2 = deck2.front();
        deck2.pop_front();

        if (static_cast<int>(deck1.size()) >= card1 &&
            static_cast<int>(deck2.size()) >= card2) {
            deque<int> subdeck1{begin(deck1), begin(deck1) + card1};
            deque<int> subdeck2{begin(deck2), begin(deck2) + card2};

            if (is_win_for_player1(subdeck1, subdeck2)) {
                deck1.push_back(card1);
                deck1.push_back(card2);
            } else {
                deck2.push_back(card2);
                deck2.push_back(card1);
            }
        } else {
            if (card1 > card2) {
                deck1.push_back(card1);
                deck1.push_back(card2);
            } else {
                deck2.push_back(card2);
                deck2.push_back(card1);
            }
        }
    }

    return !deck1.empty();
}

auto play_game2(deque<int> deck1, deque<int> deck2)
{
    while (!deck1.empty() && !deck2.empty()) {
        auto card1 = deck1.front();
        deck1.pop_front();
        auto card2 = deck2.front();
        deck2.pop_front();

        if (static_cast<int>(deck1.size()) >= card1 &&
            static_cast<int>(deck2.size()) >= card2) {
            deque<int> subdeck1{begin(deck1), begin(deck1) + card1};
            deque<int> subdeck2{begin(deck2), begin(deck2) + card2};

            if (is_win_for_player1(subdeck1, subdeck2)) {
                deck1.push_back(card1);
                deck1.push_back(card2);
            } else {
                deck2.push_back(card2);
                deck2.push_back(card1);
            }
        } else {
            if (card1 > card2) {
                deck1.push_back(card1);
                deck1.push_back(card2);
            } else {
                deck2.push_back(card2);
                deck2.push_back(card1);
            }
        }
    }

    return !deck1.empty() ? deck1 : deck2;
}

int main()
{
    auto read_deck = [] () {
        deque<int> deck;
        string line;
        while (getline(cin, line) && !line.empty()) {
            auto card = strtol(line.data(), nullptr, 10);
            if (card > 0)
                deck.push_back(card);
        }
        return deck;
    };

    auto deck1 = read_deck();
    auto deck2 = read_deck();

    auto compute_score = [] (const auto& deck) {
        auto sz = deck.size();
        return accumulate(begin(deck), end(deck), 0l, [&] (auto acc, auto card) {
            return acc + sz-- * card;
        });
    };

    cout << compute_score(play_game1(deck1, deck2)) << '\n';
    cout << compute_score(play_game2(deck1, deck2)) << '\n';

    return 0;
}

