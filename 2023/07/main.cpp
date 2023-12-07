#include <bits/stdc++.h>

using namespace std;

constexpr array Cards{
    '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'
};

constexpr auto NumCards{ssize(Cards)};

constexpr auto CardsWithJoker = []() {
    array<char, NumCards> cards_with_joker;

    cards_with_joker[0] = 'J';

    auto idx = 0;
    for (auto card : Cards)
        if (card != 'J')
            cards_with_joker[++idx] = card;

    return cards_with_joker;
}();

enum Category {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind
};

auto compute_category(const array<int, NumCards>& freq)
{
    auto has_n_of_a_kind = [&](auto n) {
        return find(begin(freq), end(freq), n) != end(freq);
    };

    auto count_n_of_a_kind = [&](auto n) {
        return count(begin(freq), end(freq), n);
    };

    if (has_n_of_a_kind(5))
        return Category::FiveOfAKind;
    else if (has_n_of_a_kind(4))
        return Category::FourOfAKind;
    else if (has_n_of_a_kind(3) && has_n_of_a_kind(2))
        return Category::FullHouse;
    else if (has_n_of_a_kind(3))
        return Category::ThreeOfAKind;
    else if (count_n_of_a_kind(2) == 2)
        return Category::TwoPair;
    else if (has_n_of_a_kind(2))
        return Category::OnePair;
    else
        return Category::HighCard;
}

auto compute_category_with_jokers(const array<int, NumCards>& freq)
{
    auto has_n_of_a_kind_without_jokers = [&](auto n_of_a_kind) {
        return find(next(begin(freq)), end(freq), n_of_a_kind) != end(freq);
    };

    auto count_n_of_a_kind_without_jokers = [&](auto n) {
        return count(next(begin(freq)), end(freq), n);
    };

    if (has_n_of_a_kind_without_jokers(5 - freq[0]))
        return Category::FiveOfAKind;
    else if (has_n_of_a_kind_without_jokers(4 - freq[0]))
        return Category::FourOfAKind;
    else if ((has_n_of_a_kind_without_jokers(3) && has_n_of_a_kind_without_jokers(2)) ||
        (count_n_of_a_kind_without_jokers(2) == 2 && freq[0] == 1))
        return Category::FullHouse;
    else if (has_n_of_a_kind_without_jokers(3 - freq[0]))
        return Category::ThreeOfAKind;
    else if (count_n_of_a_kind_without_jokers(2) == 2)
        return Category::TwoPair;
    else if (has_n_of_a_kind_without_jokers(2 - freq[0]))
        return Category::OnePair;
    else
        return Category::HighCard;
}

auto solve(vector<pair<string, long>>& hands,
    const array<char, NumCards>& cards,
    const function<int(const array<int, NumCards>&)> compute_category)
{
    auto compute_strength = [&](auto card) {
        return distance(begin(cards), find(begin(cards), end(cards), card));
    };

    auto compute_freq = [&](const auto& hand) {
        array<int, NumCards> freq{};
        for (auto card : hand)
            ++freq[compute_strength(card)];
        return freq;
    };

    sort(begin(hands), end(hands), [&](const auto& lhs, const auto& rhs) {
        auto lhs_category = compute_category(compute_freq(lhs.first));
        auto rhs_category = compute_category(compute_freq(rhs.first));

        if (lhs_category != rhs_category)
            return lhs_category < rhs_category;

        for (auto i = 0; i < ssize(lhs.first); ++i)
            if (lhs.first[i] != rhs.first[i])
                return compute_strength(lhs.first[i]) < compute_strength(rhs.first[i]);

        return false;
    });

    auto score = 0l;
    for (auto rank = 1l; rank <= ssize(hands); ++rank)
        score += rank * hands[rank - 1].second;
    return score;
}

int main()
{
    vector<pair<string, long>> hands;

    string hand;
    long bid;
    while (cin >> hand >> bid)
        hands.emplace_back(move(hand), bid);

    cout << solve(hands, Cards, compute_category) << '\n';
    cout << solve(hands, CardsWithJoker, compute_category_with_jokers) << '\n';

    return 0;
}

