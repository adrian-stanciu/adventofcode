#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

bool has_equal_letters_n_steps_away(const std::string& s, unsigned int n)
{
    if (s.size() < n + 1)
        return false;

    for (auto i = 0U; i < s.size() - n; ++i)
        if (s[i] == s[i + n])
            return true;

    return false;
}

auto count1(const std::vector<std::string>& strings)
{
    auto cnt = 0;

    for (const auto& s : strings) {
        if (count_if(s.begin(), s.end(), [] (auto c) {
            return c == 'a' ||
                   c == 'e' ||
                   c == 'i' ||
                   c == 'o' ||
                   c == 'u';
            }) < 3)
            continue;

        if (!has_equal_letters_n_steps_away(s, 1))
            continue;

        if (s.find("ab") != std::string::npos)
            continue;
        if (s.find("cd") != std::string::npos)
            continue;
        if (s.find("pq") != std::string::npos)
            continue;
        if (s.find("xy") != std::string::npos)
            continue;

        ++cnt;
    }

    return cnt;
}

bool has_adjacent_letters_twice_without_overlapping(const std::string& s)
{
    if (s.size() < 4)
        return false;

    for (auto i = 0U; i < s.size() - 3; ++i)
        if (s.find(std::string({s[i], s[i + 1]}), i + 2) != std::string::npos)
            return true;

    return false;
}

auto count2(const std::vector<std::string>& strings)
{
    auto cnt = 0;

    for (const auto& s : strings) {
        if (!has_adjacent_letters_twice_without_overlapping(s))
            continue;

        if (!has_equal_letters_n_steps_away(s, 2))
            continue;

        ++cnt;
    }

    return cnt;
}

int main()
{
    std::vector<std::string> strings;

    std::string s;
    while (std::cin >> s)
        strings.push_back(std::move(s));

    std::cout << count1(strings) << "\n";
    std::cout << count2(strings) << "\n";

    return 0;
}

