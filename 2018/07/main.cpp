#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

auto compute_order(const std::unordered_map<char, std::set<char>>& next_states,
    std::unordered_map<char, std::set<char>> prev_states,
    std::set<char> avail_states)
{
    std::string order;

    while (!avail_states.empty()) {
        auto s = *avail_states.begin();
        avail_states.erase(s);

        order.push_back(s);

        auto it = next_states.find(s);
        if (it == next_states.end())
            continue;

        // check if next states are ready
        for (auto next_s : it->second) {
            prev_states[next_s].erase(s);
            if (prev_states[next_s].empty())
                avail_states.emplace(next_s);
        }
    }

    return order;
}

auto compute_duration(const std::unordered_map<char, std::set<char>>& next_states,
    std::unordered_map<char, std::set<char>> prev_states,
    std::set<char> avail_states,
    size_t num_workers)
{
    auto t = 0;
    std::vector<std::pair<size_t, char>> workers; // pair of duration to complete and state

    while (!avail_states.empty() || !workers.empty()) {
        // assign states (if available) to workers (if available)
        while (!avail_states.empty() && workers.size() < num_workers) {
            auto s = *avail_states.begin();
            avail_states.erase(s);

            workers.push_back(std::make_pair(60 + s - 'A' + 1, s));
        }

        // compute duration until a first task is done
        auto min_t = (*min_element(workers.begin(), workers.end())).first;
        t += min_t;

        // deduce that duration from all active workers
        for (auto& w : workers) {
            w.first -= min_t;
            if (w.first == 0) {
                auto it = next_states.find(w.second);
                if (it == next_states.end())
                    continue;

                // check if next states are ready
                for (auto next_s : it->second) {
                    prev_states[next_s].erase(w.second);
                    if (prev_states[next_s].empty())
                        avail_states.emplace(next_s);
                }
            }
        }

        // idle workers can be reused in next iteration
        workers.erase(remove_if(workers.begin(), workers.end(),
            [](const auto& w) {
                return w.first == 0;
            }), workers.end());
    }

    return t;
}

int main()
{
    std::unordered_map<char, std::set<char>> next_states;
    std::unordered_map<char, std::set<char>> prev_states;
    std::set<char> start_states;

    for (auto c = 'A'; c <= 'Z'; ++c)
        start_states.emplace(c);

    std::string line;
    while (getline(std::cin, line)) {
        char from, to;
        sscanf(line.data(), "Step %c must be finished before step %c can begin.",
            &from, &to);

        next_states[from].emplace(to);
        prev_states[to].emplace(from);
        start_states.erase(to);
    }

    std::cout << compute_order(next_states, prev_states, start_states) << "\n";
    std::cout << compute_duration(next_states, prev_states, start_states, 5) << "\n";

    return 0;
}

