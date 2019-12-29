#include <iostream>
#include <regex>
#include <string>
#include <variant>
#include <vector>

using int128 = __int128;

std::ostream& operator<<(std::ostream& out, int128 i)
{
    std::string s;

    while (i) {
        s.push_back(static_cast<char>((i % 10) + '0'));
        i /= 10;
    }

    reverse(s.begin(), s.end());

    out << s;

    return out;
}

// n^k modulo m
auto n_to_k(int128 n, int128 k, int128 m)
{
    int128 r = 1;

    while (k) {
        if (k & 1) {
            r = (r * n) % m;
            --k;
        }

        n = (n * n) % m;
        k /= 2;
    }

    return r;
}

// (1 / n) modulo m, where m is prime
auto inverse(int128 n, int128 m)
{
    // based on Fermat's little theorem:
    // n^(m - 1) = 1 (mod m) => n^(m - 2) = (1 / m) (mod m)
    return n_to_k(n, m - 2, m);
}

// 1 + q + q^2 + ... + q^k modulo m
auto compute_geo_prog_sum(int128 q, int128 k, int128 m)
{
    // q^k
    int128 q2k = n_to_k(q, k, m);

    // q^k - 1
    int128 sum = (q2k + m - 1) % m;

    // (q^k - 1) * (1 / (q - 1))
    sum *= inverse(q - 1, m);
    sum %= m;

    return sum;
}

struct Reverse {
    auto apply(int128 i, int128 sz) const
    {
        return sz - 1 - i;
    }

    void update_fn(int128& a, int128& b, int128 sz) const
    {
        // reverse a
        a = sz - a;

        // advance b one time by the new (reversed) a (go to the last element)
        b += a;
        b %= sz;
    }
};

struct Cut {
    int128 n;

    auto apply(int128 i, int128 sz) const
    {
        return (((i - n) % sz) + sz) % sz;
    }

    void update_fn(int128& a, int128& b, int128 sz) const
    {
        // a is unchanged

        // advance b n times
        b += (((n + sz) % sz) * a) % sz;
        b %= sz;
    }
};

struct Increment {
    int128 n;

    auto apply(int128 i, int128 sz) const
    {
        return (n * i) % sz;
    }

    void update_fn(int128& a, int128& b, int128 sz) const
    {
        // i * n = 1 (mod sz) => i = 1 / n (mod sz)
        // f(0) = b, f(1) = a * i + b => f(1) - f(0) = a * i
        a *= inverse(n, sz);
        a %= sz;

        // b is unchanged
    }
};

using Cmd = std::variant<Reverse, Cut, Increment>;

auto pos_of_card(const std::vector<Cmd>& cmd_vec, int128 card, int128 deck_sz)
{
    for (const auto& cmd : cmd_vec)
        card = visit([&] (const auto& _) { return _.apply(card, deck_sz); }, cmd);

    return card;
}

auto shuffle(const std::vector<Cmd>& cmd_vec, int128 deck_sz)
{
    // initially f(x) = x
    int128 a = 1;
    int128 b = 0;

    for (const auto& cmd : cmd_vec)
        visit([&] (const auto& _) { return _.update_fn(a, b, deck_sz); }, cmd);

    // f(x) = a * x + b
    return std::make_pair(a, b);
}

auto card_at_pos(const std::vector<Cmd>& cmd_vec, int128 pos, int128 deck_sz, int128 times)
{
    // compose all functions
    auto [a, b] = shuffle(cmd_vec, deck_sz);

    // f(x) = a * x + b
    // f(f(x)) = a * f(x) + b = a * (a * x + b) + b = a^2 * x + a * b + b = a^2 * x + b * (a + 1)
    // => n times composition:
    // (f o f o ... o f)(x) = a^n * x + b * [a^(n - 1) + a^(n - 2) + ... + a^2 + a + 1]
    auto a_repeated = n_to_k(a, times, deck_sz);
    auto b_repeated = (b * compute_geo_prog_sum(a, times, deck_sz)) % deck_sz;

    return ((a_repeated * pos) % deck_sz + b_repeated) % deck_sz;
}

int main()
{
    static const std::regex cut_re{"cut (-?[1-9][0-9]*)"};
    static const std::regex deal_re{"deal with increment ([1-9][0-9]*)"};

    std::vector<Cmd> cmd_vec;

    std::string line;
    while (getline(std::cin, line)) {
        if (line == "deal into new stack")
            cmd_vec.push_back(Reverse{});
        else {
            std::smatch matched;
            if (regex_match(line, matched, cut_re))
                cmd_vec.push_back(Cut{strtoll(matched[1].str().data(), nullptr, 10)});
            else if (regex_match(line, matched, deal_re))
                cmd_vec.push_back(Increment{strtoll(matched[1].str().data(), nullptr, 10)});
        }
    }

    std::cout << pos_of_card(cmd_vec, 2019, 10007) << "\n";
    std::cout << card_at_pos(cmd_vec, 2020, 119315717514047LL, 101741582076661LL) << "\n";

    return 0;
}

