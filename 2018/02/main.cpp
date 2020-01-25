#include <algorithm>
#include <iostream>
#include <vector>

auto compute_csum(const std::vector<std::string>& id_vec)
{
    auto total_2 = 0;
    auto total_3 = 0;

    auto ids {id_vec};

    for (auto id : ids) {
        if (id.size() < 2)
            continue;

        sort(id.begin(), id.end());

        auto has_2 = false;
        auto has_3 = false;

        char last_c = id[0];
        auto last_c_cnt = 1;

        for (auto i = 1U; i < id.size(); ++i) {
            if (id[i] == last_c) {
                ++last_c_cnt;
            } else {
                if (last_c_cnt == 2)
                    has_2 = true;
                else if (last_c_cnt == 3)
                    has_3 = true;

                last_c = id[i];
                last_c_cnt = 1;
            }
        }

        if (last_c_cnt == 2)
            has_2 = true;
        else if (last_c_cnt == 3)
            has_3 = true;

        if (has_2)
            ++total_2;
        if (has_3)
            ++total_3;
    }

    return total_2 * total_3;
}

auto compute_common(const std::vector<std::string>& id_vec)
{
    for (auto i = 0U; i < id_vec.size() - 1; ++i)
        for (auto j = i + 1; j < id_vec.size(); ++j) {
            auto diffs = 0;
            std::string common;

            // assume that all IDs have the same size
            for (auto k = 0U; k < id_vec[i].size(); ++k)
                if (id_vec[i][k] != id_vec[j][k]) {
                    ++diffs;
                    if (diffs > 1)
                        break;
                } else {
                    common.push_back(id_vec[i][k]);
                }

            if (diffs == 1)
                return common;
        }

    return std::string();
}

int main()
{
    std::vector<std::string> id_vec;

    std::string line;
    while (getline(std::cin, line))
        id_vec.push_back(std::move(line));

    std::cout << compute_csum(id_vec) << "\n";
    std::cout << compute_common(id_vec) << "\n";

    return 0;
}

