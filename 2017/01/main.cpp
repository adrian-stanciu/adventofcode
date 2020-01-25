#include <iostream>
#include <string>

auto sum_if_next_digit_is_equal(const std::string& str)
{
    auto sum = 0L;
    size_t last_index = str.size() - 1;

    for (size_t i = 0; i < last_index; ++i)
        if (str[i] == str[i + 1])
            sum += str[i] - '0';

    if (str[last_index] == str[0])
        sum += str[0] - '0';

    return sum;
}

auto sum_if_halfway_around_digit_is_equal(const std::string& str)
{
    auto sum = 0L;
    size_t halfway = str.size() / 2;

    for (size_t i = 0; i < halfway; ++i)
        if (str[i] == str[i + halfway])
            sum += 2 * (str[i] - '0');

    return sum;
}

int main()
{
    std::string str;
    getline(std::cin, str);

    std::cout << sum_if_next_digit_is_equal(str) << "\n";
    std::cout << sum_if_halfway_around_digit_is_equal(str) << "\n";

    return 0;
}

