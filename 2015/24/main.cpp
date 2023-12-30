#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

static constexpr auto Inf = std::numeric_limits<long>::max();

using Group = std::vector<long>;
using Groups = std::vector<Group>;

auto sum(const Group& g)
{
    return std::accumulate(g.begin(), g.end(), 0L);
}

auto prod(const Group& g)
{
    return std::accumulate(g.begin(), g.end(), 1L, [] (auto acc, auto val) {
        return acc * val;
    });
}

bool is_splitable_into_subgroups_with_sum(const Group& g, size_t i, long sum, Groups& subgroups)
{
    if (i == g.size())
        return all_of(subgroups.begin(), subgroups.end(), [&] (const auto& sg) {
            return ::sum(sg) == sum;
        });

    for (auto& sg : subgroups)
        if (::sum(sg) + g[i] <= sum) {
            sg.push_back(g[i]);

            if (is_splitable_into_subgroups_with_sum(g, i + 1, sum, subgroups))
                return true;

            sg.pop_back();
        }

    return false;
}

bool is_splitable_into_num_subgroups_with_sum(const Group& g, long num, long sum)
{
    Groups subgroups(num);
    return is_splitable_into_subgroups_with_sum(g, 0, sum, subgroups);
}

auto diff(const Group& g1, const Group& g2)
{
    auto diff_g{g1};

    for (auto val : g2)
        diff_g.erase(find(diff_g.begin(), diff_g.end(), val));

    return diff_g;
}

auto min_prod_of_subgroup_with_sum_and_size(const Group& g, size_t i, long num,
    long sum, size_t size, Group& sg)
{
    if (sg.size() == size) {
        if (::sum(sg) == sum &&
            // remaining elements must also be splitable into num - 1 subgroups with the same sum
            is_splitable_into_num_subgroups_with_sum(diff(g, sg), num - 1, sum))
            return prod(sg);
        else
            return Inf;
    }

    if (i == g.size())
        return Inf;

    auto min_prod = min_prod_of_subgroup_with_sum_and_size(g, i + 1, num, sum, size, sg);

    if (::sum(sg) + g[i] <= sum) {
        sg.push_back(g[i]);

        min_prod = std::min(min_prod,
            min_prod_of_subgroup_with_sum_and_size(g, i + 1, num, sum, size, sg));

        sg.pop_back();
    }

    return min_prod;
}

auto split_into_num_subgroups_with_sum_and_take_min_prod_of_smallest_subgroup(const Group& g,
    long num, long sum)
{
    for (auto size = 0U; size <= g.size(); ++size) {
        Group sg;
        auto min_prod = min_prod_of_subgroup_with_sum_and_size(g, 0, num, sum, size, sg);
        if (min_prod != Inf)
            return min_prod;
    }

    return -1L;
}

auto solve(const Group& g, long num)
{
    return split_into_num_subgroups_with_sum_and_take_min_prod_of_smallest_subgroup(g,
        num, ::sum(g) / num);
}

int main()
{
    Group g;

    long val;
    while (std::cin >> val)
        g.push_back(val);

    std::cout << solve(g, 3) << "\n";
    std::cout << solve(g, 4) << "\n";

    return 0;
}

