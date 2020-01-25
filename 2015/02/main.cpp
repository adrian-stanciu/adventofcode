#include <iostream>
#include <string>

int main()
{
    auto paper = 0L;
    auto ribbon = 0L;

    std::string line;
    while (std::cin >> line) {
        long l, w, h;
        sscanf(line.data(), "%ldx%ldx%ld", &l, &w, &h);

        auto a1 = l * w;
        auto a2 = l * h;
        auto a3 = w * h;
        paper += 2 * (a1 + a2 + a3) + std::min(std::min(a1, a2), a3);

        ribbon += 2 * (l + w + h - std::max(std::max(l, w), h)) + l * w * h;
    }

    std::cout << paper << "\n";
    std::cout << ribbon << "\n";

    return 0;
}

