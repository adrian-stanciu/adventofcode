#include <iostream>
#include <vector>

auto compute_metadata_sum(const std::vector<unsigned int>& vec, size_t& idx)
{
    auto s = 0;

    if (vec.size() == idx)
        return s;

    auto children_num = vec[idx++];
    auto metadata_num = vec[idx++];

    for (auto i = 0U; i < children_num; ++i)
        s += compute_metadata_sum(vec, idx);

    for (auto i = 0U; i < metadata_num; ++i)
        s += vec[idx++];

    return s;
}

auto compute_value(const std::vector<unsigned int>& vec, size_t& idx)
{
    auto s = 0;

    if (vec.size() == idx)
        return s;

    auto children_num = vec[idx++];
    auto metadata_num = vec[idx++];

    std::vector<unsigned int> children;
    children.reserve(children_num);
    for (auto i = 0U; i < children_num; ++i)
        children.push_back(compute_value(vec, idx));

    if (children.empty()) {
        for (auto i = 0U; i < metadata_num; ++i)
            s += vec[idx++];
    } else {
        for (auto i = 0U; i < metadata_num; ++i) {
            auto child_ref = vec[idx++] - 1;
            if (child_ref < children.size())
                s += children[child_ref];
        }
    }

    return s;
}

int main()
{
    std::vector<unsigned int> vec;
    size_t idx;

    unsigned int n;
    while (std::cin >> n)
        vec.push_back(n);

    idx = 0;
    std::cout << compute_metadata_sum(vec, idx) << "\n";
    idx = 0;
    std::cout << compute_value(vec, idx) << "\n";

    return 0;
}

