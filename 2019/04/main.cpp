#include <iostream>
#include <vector>

auto extract_reversed_digits(int n)
{
    std::vector<int> digits;

    while (n) {
        digits.push_back(n % 10);
        n /= 10;
    }

    return digits;
}

bool has_non_increasing_digits(const std::vector<int>& reversed_digits)
{
    if (reversed_digits.size() < 2)
        return true;

    for (auto i = 0U; i < reversed_digits.size() - 1; ++i)
        if (reversed_digits[i] < reversed_digits[i + 1])
            return false;

    return true;
}

bool has_adjacent_equal_digits(const std::vector<int>& digits)
{
    if (digits.size() < 2)
        return false;

    for (auto i = 0U; i < digits.size() - 1; ++i)
        if (digits[i] == digits[i + 1])
            return true;

    return false;
}

bool has_k_adjacent_equal_digits(const std::vector<int>& digits, int k)
{
    auto cnt = 1;

    for (auto i = 1U; i < digits.size(); ++i) {
        if (digits[i] == digits[i - 1]) {
            ++cnt;
        } else {
            if (cnt == k)
                return true;

            cnt = 1;
        }
    }

    if (cnt == k)
        return true;

    return false;
}

auto count_valid_pass(int first, int last)
{
    auto cnt1 = 0;
    auto cnt2 = 0;

    for (auto i = first; i <= last; ++i) {
        auto reversed_digits = extract_reversed_digits(i);

        if (!has_non_increasing_digits(reversed_digits))
            continue;

        if (has_adjacent_equal_digits(reversed_digits)) {
            ++cnt1;

            if (has_k_adjacent_equal_digits(reversed_digits, 2))
                ++cnt2;
        }
    }

    std::cout << cnt1 << "\n";
    std::cout << cnt2 << "\n";
}

int main()
{
    int first;
    int last;

    std::cin >> first;
    std::cin.ignore();
    std::cin >> last;

    count_valid_pass(first, last);

    return 0;
}

