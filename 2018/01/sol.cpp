#include <iostream>
#include <unordered_set>
#include <vector>

auto compute_total_freq(const std::vector<int>& f_vec)
{
    auto total_f = 0;

    for (auto f : f_vec)
        total_f += f;

    return total_f;
}

auto compute_repeated_freq(const std::vector<int>& f_vec)
{
    auto total_f = 0;

    std::unordered_set<int> f_set;
    f_set.insert(total_f);

    while (true) {
        for (auto f : f_vec) {
            total_f += f;

            if (f_set.count(total_f) > 0)
                return total_f;

            f_set.insert(total_f);
        }
    }
}

int main()
{
    std::vector<int> f_vec;

    std::string line;
    while (getline(std::cin, line))
        f_vec.push_back(atoi(line.data()));

    std::cout << compute_total_freq(f_vec) << "\n";
    std::cout << compute_repeated_freq(f_vec) << "\n";

    return 0;
}

