#include <iostream>
#include <optional>

int main()
{
    auto position = 0;
    auto floor = 0;
    std::optional<int> basement_first_time_after;

    char c;
    while (std::cin >> c) {
        ++position;

        if (c == '(')
            ++floor;
        else {
            --floor;

            if (floor == -1 && !basement_first_time_after.has_value())
                basement_first_time_after = position;
        }
    }

    std::cout << floor << "\n";
    std::cout << basement_first_time_after.value() << "\n";

    return 0;
}

