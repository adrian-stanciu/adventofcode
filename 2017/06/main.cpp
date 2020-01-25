#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

auto read_vector()
{
    std::vector<unsigned int> vec;

    unsigned int n;
    while (std::cin >> n)
        vec.push_back(n);

    return vec;
}

void reallocate(std::vector<unsigned int>& vec)
{
    unsigned int max = vec[0];
    unsigned int max_index = 0;

    for (size_t i = 1; i < vec.size(); ++i)
        if (vec[i] > max) {
            max = vec[i];
            max_index = i;
        }

    std::vector<unsigned int> increments(vec.size(), 0);
    for (size_t i = max_index + 1; i < vec.size(); ++i)
        if (max > 0) {
            increments[i] = 1;
            --max;
        } else
            break;
    if (max > 0) {
        unsigned int q = max / vec.size();
        if (q > 0)
            transform(increments.begin(), increments.end(), increments.begin(),
                [q](auto x) { return (x + q); });

        unsigned int r = max % vec.size();
        for (auto& e : increments)
            if (r > 0) {
                ++e;
                --r;
            } else
                break;
    }

    vec[max_index] = 0;
    for (size_t i = 0; i < vec.size(); ++i)
        vec[i] += increments[i];
}

std::string stringify(const std::vector<unsigned int>& vec)
{
    std::stringstream ss;

    for (auto e : vec)
        ss << e << "-";

    return ss.str();
}

unsigned int count_steps_to_cycle(std::vector<unsigned int> vec)
{
    unsigned int count = 0;
    std::unordered_set<std::string> cache;

    while (true) {
        ++count;

        reallocate(vec);

        std::string vec_str = stringify(vec);

        if (cache.find(vec_str) != cache.end())
            return count;
        else
            cache.insert(vec_str);
    }
}

unsigned int count_cycle_size(std::vector<unsigned int> vec)
{
    unsigned int count = 0;
    std::unordered_map<std::string, unsigned int> cache;

    while (true) {
        ++count;

        reallocate(vec);

        std::string vec_str = stringify(vec);

        if (cache.find(vec_str) != cache.end())
            return (count - cache[vec_str]);
        else
            cache.insert({vec_str, count});
    }
}

int main()
{
    auto vec = read_vector();

    std::cout << count_steps_to_cycle(vec) << "\n";
    std::cout << count_cycle_size(vec) << "\n";

    return 0;
}

