#include <algorithm>
#include <iostream>
#include <string>

#include "md5.h"

auto find_number(const std::string& key, int count)
{
    auto i = 0;

    while (true) {
        auto hex = hex_md5sum(key + std::to_string(++i), false);

        if (all_of(hex.begin(), hex.begin() + count, [] (auto c) { return c == '0'; }))
            return i;
    }
}

int main()
{
    std::string key;
    std::cin >> key;

    std::cout << find_number(key, 5) << "\n";
    std::cout << find_number(key, 6) << "\n";

    return 0;
}

