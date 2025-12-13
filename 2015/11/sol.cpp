#include <iostream>
#include <optional>
#include <string>

auto inc(std::string& pass)
{
    auto pos = static_cast<int>(pass.size()) - 1;
    while (pos >= 0)
        if (pass[pos] == 'z') {
            pass[pos] = 'a';
            --pos;
        } else {
            ++pass[pos];
            break;
        }
}

bool has_min_3_straight_increasing_letters(const std::string& s)
{
    if (s.size() < 3)
        return false;

    for (auto i = 0U; i < s.size() - 2; ++i)
        if (s[i] == s[i + 1] - 1 && s[i] == s[i + 2] - 2)
            return true;

    return false;
}

bool has_2_different_pairs_of_equal_letters(const std::string& s)
{
    if (s.size() < 4)
        return false;

    std::optional<char> first_pair;

    for (auto i = 0U; i < s.size() - 1; ++i)
        if (s[i] == s[i + 1]) {
            if (first_pair.has_value()) {
                if (first_pair.value() != s[i])
                    return true;
            } else {
                first_pair = s[i];
            }
            ++i;
        }

    return false;
}

bool is_valid(const std::string& pass)
{
    if (!has_min_3_straight_increasing_letters(pass))
        return false;

    if (pass.find('i') != std::string::npos)
        return false;
    if (pass.find('o') != std::string::npos)
        return false;
    if (pass.find('l') != std::string::npos)
        return false;

    if (!has_2_different_pairs_of_equal_letters(pass))
        return false;

    return true;
}

auto next_pass(const std::string& pass)
{
    auto next_pass{pass};

    while (true) {
        inc(next_pass);
        if (is_valid(next_pass))
            return next_pass;
    }
}

int main()
{
    std::string pass;
    std::cin >> pass;

    auto next_pass = ::next_pass(pass);

    std::cout << next_pass << "\n";
    std::cout << ::next_pass(next_pass) << "\n";

    return 0;
}

