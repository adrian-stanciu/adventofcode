#include <algorithm>
#include <limits>
#include <iostream>
#include <string>
#include <vector>

#include "int_computer.h"

auto compute_max_result(const std::vector<long long>& prog)
{
    auto max_res = std::numeric_limits<long long>::min();

    std::vector<int> p{0, 1, 2, 3, 4};
    do {
        std::vector<IntComputer> network;
        for (auto i = 0U; i < p.size(); ++i) {
            network.emplace_back(prog);
            network[i].append_input(p[i]);
        }

        network[0].append_input(0);

        auto res = 0LL;

        for (auto i = 0U; i < p.size(); ++i) {
            network[i].run();

            if (i == p.size() - 1)
                res = *network[i].get_last_output();
            else
                network[i + 1].append_input(*network[i].get_last_output());
        }

        max_res = std::max(max_res, res);
    } while(std::next_permutation(p.begin(), p.end()));

    return max_res;
}

auto compute_max_result_with_feedback_loop(const std::vector<long long>& prog)
{
    auto max_res = std::numeric_limits<long long>::min();

    std::vector<int> p{5, 6, 7, 8, 9};
    do {
        std::vector<IntComputer> network;

        for (auto i = 0U; i < p.size(); ++i) {
            network.emplace_back(prog);
            network[i].append_input(p[i]);
        }

        network[0].append_input(0);

        while (true) {
            auto last_halted = false;

            for (auto i = 0U; i < p.size(); ++i) {
                auto halted = !network[i].run();

                if (halted)
                    last_halted = i == p.size() - 1;
                else
                    network[(i + 1) % p.size()].append_input(*network[i].get_last_output());
            }

            if (last_halted)
                break;
        }

        max_res = std::max(max_res, *network[p.size() - 1].get_last_output());
    } while(std::next_permutation(p.begin(), p.end()));

    return max_res;
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << compute_max_result(prog) << "\n";
    std::cout << compute_max_result_with_feedback_loop(prog) << "\n";

    return 0;
}

