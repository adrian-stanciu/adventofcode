#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

auto recover_msg(const std::vector<std::string>& signals)
{
    std::string max_f_msg;
    std::string min_f_msg;

    for (auto i = 0U; i < signals[0].size(); ++i) {
        std::array<int, 26> freq{};

        for (const auto& signal : signals)
            ++freq[signal[i] - 'a'];

        auto max_f_it = std::max_element(freq.begin(), freq.end());
        max_f_msg.push_back('a' + std::distance(freq.begin(), max_f_it));

        auto min_f_it = std::min_element(freq.begin(), freq.end());
        min_f_msg.push_back('a' + std::distance(freq.begin(), min_f_it));
    }

    std::cout << max_f_msg << "\n";
    std::cout << min_f_msg << "\n";
}

int main()
{
    std::vector<std::string> signals;

    std::string line;
    while (getline(std::cin, line))
        signals.push_back(std::move(line));

    recover_msg(signals);

    return 0;
}

