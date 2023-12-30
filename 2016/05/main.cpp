#include <iostream>
#include <string>

#include "md5.hpp"

auto pass(const std::string& id)
{
    std::string pass;

    auto idx = 0L;
    while (pass.size() < 8) {
        auto hex = hex_md5sum(id + std::to_string(idx), false);

        if (hex.compare(0, 5, "00000") == 0)
            pass.push_back(hex[5]);

        ++idx;
    }

    return pass;
}

auto pass2(const std::string& id)
{
    const auto Empty = '_';
    const auto len = 8;

    std::string pass(len, Empty);
    auto rem = len;
    auto idx = 0L;

    while (rem) {
        auto hex = hex_md5sum(id + std::to_string(idx), false);

        if (hex.compare(0, 5, "00000") == 0) {
            auto i = hex2dec(hex[5]);

            if (i < len && pass[i] == Empty) {
                pass[i] = hex[6];
                --rem;
            }
        }

        ++idx;
    }

    return pass;
}

int main()
{
    std::string id;
    std::cin >> id;

    std::cout << pass(id) << "\n";
    std::cout << pass2(id) << "\n";

    return 0;
}

