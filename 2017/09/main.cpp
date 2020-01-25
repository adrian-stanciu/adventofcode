#include <iostream>

constexpr auto BeginGroup = '{';
constexpr auto EndGroup = '}';
constexpr auto BeginGarbage = '<';
constexpr auto EndGarbage = '>';
constexpr auto IgnoreGarbage = '!';

void cleanup()
{
    unsigned long score = 0;
    unsigned long level = 0;
    bool is_garbage = false;
    unsigned long garbage = 0;

    char c;
    while (std::cin >> c) {
        if (is_garbage) {
            if (c == IgnoreGarbage)
                std::cin.ignore(); // skip next char
            else if (c == EndGarbage)
                is_garbage = false;
            else
                ++garbage;
        } else {
            if (c == BeginGarbage)
                is_garbage = true;
            else if (c == BeginGroup)
                ++level;
            else if (c == EndGroup) {
                score += level;
                --level;
            }
        }
    }

    std::cout << score << "\n";
    std::cout << garbage << "\n";
}

int main()
{
    cleanup();

    return 0;
}

