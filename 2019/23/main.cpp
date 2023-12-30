#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "int_computer.hpp"

auto run(std::vector<IntComputer> network)
{
    while (true) {
        for (auto& comp : network)
            comp.run();

        for (auto& comp : network) {
            while (!comp.output_empty() && comp.output_size() % 3 == 0) {
                auto dst = *comp.extract_first_output();
                auto x = *comp.extract_first_output();
                auto y = *comp.extract_first_output();

                if (dst == 255)
                    return y;

                network[dst].append_input(x);
                network[dst].append_input(y);
            }
        }

        for (auto& comp : network)
            if (comp.input_empty())
                comp.append_input(-1);
    }
}

auto run_with_nat(std::vector<IntComputer> network)
{
    bool nat_avail = false;
    auto nat_x = 0LL;
    auto nat_y = 0LL;
    std::optional<long long> nat_last_delivered_y;

    auto is_idle = [&] () {
        return all_of(network.begin(), network.end(), [] (const auto& ic) {
            return ic.input_empty() && ic.output_empty();
        });
    };

    while (true) {
        for (auto& comp : network)
            comp.run();

        for (auto& comp : network) {
            while (!comp.output_empty() && comp.output_size() % 3 == 0) {
                auto dst = *comp.extract_first_output();
                auto x = *comp.extract_first_output();
                auto y = *comp.extract_first_output();

                if (dst == 255) {
                    nat_avail = true;
                    nat_x = x;
                    nat_y = y;
                } else {
                    network[dst].append_input(x);
                    network[dst].append_input(y);
                }
            }
        }

        if (nat_avail && is_idle()) {
            if (nat_last_delivered_y && nat_y == nat_last_delivered_y.value())
                return nat_y;

            network[0].append_input(nat_x);
            network[0].append_input(nat_y);

            nat_last_delivered_y = nat_y;
        } else {
            for (auto& comp : network)
                if (comp.input_empty())
                    comp.append_input(-1);
        }
    }
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::vector<IntComputer> network;

    for (auto i = 0; i < 50; ++i) {
        network.emplace_back(prog);
        network[i].append_input(i);
    }

    std::cout << run(network) << "\n";
    std::cout << run_with_nat(network) << "\n";

    return 0;
}

