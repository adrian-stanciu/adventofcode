#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct Component {
    long port;
    long idx;
    long next;

    Component(long port, long idx, long next)
    : port(port)
    , idx(idx)
    , next(next)
    {}

    friend bool operator<(const Component& c, const long port)
    {
        return c.port < port;
    }

    friend bool operator<(const long port, const Component& c)
    {
        return port < c.port;
    }
};

void read_components(std::vector<Component>& left, std::vector<Component>& right)
{
    long idx = 0;

    std::string line;
    while (getline(std::cin, line)) {
        std::stringstream ss(line);

        long port1, port2;
        ss >> port1;
        ss.ignore();
        ss >> port2;

        left.emplace_back(port1, idx, port2);
        right.emplace_back(port2, idx, port1);
        ++idx;
    }
}

long compute_strongest_bridge_score_helper(
    const std::vector<Component>& left,
    const std::vector<Component>& right,
    long prev, std::vector<bool>& used)
{
    long max_score = 0;

    auto compute = [&] (const Component& c) {
        used[c.idx] = true;
        long local_score = compute_strongest_bridge_score_helper(left, right, c.next, used);
        used[c.idx] = false;

        local_score += prev + c.next;

        if (local_score > max_score)
            max_score = local_score;
    };

    auto l_range = equal_range(left.begin(), left.end(), prev);
    for (auto c = l_range.first; c != l_range.second; ++c)
        if (!used[c->idx])
            compute(*c);

    auto r_range = equal_range(right.begin(), right.end(), prev);
    for (auto c = r_range.first; c != r_range.second; ++c)
        if (!used[c->idx] && prev != c->next)
            compute(*c);

    return max_score;
}

long compute_strongest_bridge_score(const std::vector<Component>& left,
    const std::vector<Component>& right)
{
    std::vector<bool> used(left.size(), false);

    return compute_strongest_bridge_score_helper(left, right, 0, used);
}

std::tuple<long, long> compute_longest_strongest_bridge_score_helper(
    const std::vector<Component>& left,
    const std::vector<Component>& right,
    long prev, std::vector<bool>& used)
{
    long max_score = 0;
    long max_len = 0;

    auto compute = [&] (const Component& c) {
        long local_score, local_len;
        used[c.idx] = true;
        std::tie(local_score, local_len) =
            compute_longest_strongest_bridge_score_helper(left, right, c.next, used);
        used[c.idx] = false;

        local_score += prev + c.next;
        local_len += 1;

        if (local_len > max_len) {
            max_len = local_len;
            max_score = local_score;
        } else if (local_len == max_len) {
            if (local_score > max_score)
                max_score = local_score;
        }
    };

    auto l_range = equal_range(left.begin(), left.end(), prev);
    for (auto c = l_range.first; c != l_range.second; ++c)
        if (!used[c->idx])
            compute(*c);

    auto r_range = equal_range(right.begin(), right.end(), prev);
    for (auto c = r_range.first; c != r_range.second; ++c)
        if (!used[c->idx] && prev != c->next)
            compute(*c);

    return std::make_tuple(max_score, max_len);
}

long compute_longest_strongest_bridge_score(const std::vector<Component>& left,
    const std::vector<Component>& right)
{
    std::vector<bool> used(left.size(), false);

    long max_score, max_len;
    std::tie(max_score, max_len) =
        compute_longest_strongest_bridge_score_helper(left, right, 0, used);

    return max_score;
}

int main()
{
    std::vector<Component> left;
    std::vector<Component> right;
    read_components(left, right);

    sort(left.begin(), left.end(),
        [] (const Component& c1, const Component& c2) {
            return c1.port < c2.port;
        });
    sort(right.begin(), right.end(),
        [] (const Component& c1, const Component& c2) {
            return c1.port < c2.port;
        });

    std::cout << compute_strongest_bridge_score(left, right) << "\n";
    std::cout << compute_longest_strongest_bridge_score(left, right) << "\n";

    return 0;
}

