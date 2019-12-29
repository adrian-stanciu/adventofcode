#include <iostream>
#include <vector>

auto compute_total_full(const std::vector<int>& masses)
{
    auto total_f = 0;

    for (const auto m : masses)
        total_f += m / 3 - 2;

    return total_f;
}

auto compute_total_full_with_full(const std::vector<int>& masses)
{
    auto total_f = 0;

    for (auto m : masses)
        while (true) {
            auto f = m / 3 - 2;
            if (f <= 0)
                break;

            total_f += f;

            m = f;
        }

    return total_f;
}

int main()
{
    std::vector<int> masses;

    int m;
    while (std::cin >> m)
        masses.push_back(m);

    std::cout << compute_total_full(masses) << "\n";
    std::cout << compute_total_full_with_full(masses) << "\n";

    return 0;
}

