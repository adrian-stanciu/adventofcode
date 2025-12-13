#include <cmath>
#include <iostream>
#include <vector>

auto sum(const std::vector<int>& capacities, int selected)
{
    auto sum = 0;

    for (auto i = 0U; i < capacities.size(); ++i)
        if ((selected >> i) & 0x1)
            sum += capacities[i];

    return sum;
}

auto count_comb(const std::vector<int>& capacities, int total)
{
    auto cnt = 0;

    for (auto i = 1; i < pow(2, capacities.size()); ++i)
        if (sum(capacities, i) == total)
            ++cnt;

    return cnt;
}

auto num_set_bits(int n)
{
    auto cnt = 0;

    while (n) {
        ++cnt;
        n &= n - 1;
    }

    return cnt;
}

auto count_comb_with_min_containers(const std::vector<int>& capacities, int total)
{
    std::vector<int> counters(capacities.size(), 0);

    for (auto i = 1; i < pow(2, capacities.size()); ++i)
        if (sum(capacities, i) == total)
            ++counters[num_set_bits(i)];

    for (auto cnt : counters)
        if (cnt != 0)
            return cnt;

    return -1;
}

int main()
{
    std::vector<int> capacities;

    int cap;
    while (std::cin >> cap)
        capacities.push_back(cap);

    std::cout << count_comb(capacities, 150) << "\n";
    std::cout << count_comb_with_min_containers(capacities, 150) << "\n";

    return 0;
}

