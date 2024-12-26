#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

auto size_in_mem(const std::string& s)
{
    size_t sz = 0;

    size_t pos = 0;
    while (true) {
        auto bs_pos = s.find('\\', pos);
        if (bs_pos == std::string::npos) {
            sz += s.size() - pos;
            break;
        }

        sz += bs_pos - pos;

        if (s[bs_pos + 1] == 'x')
            pos = bs_pos + 4;
        else
            pos = bs_pos + 2;

        ++sz; // the escaped char
    }

    sz -= 2; // starting and ending double quotes

    return sz;
}

auto size_encoded(const std::string& s)
{
    auto sz = s.size();

    sz += count(s.begin(), s.end(), '\\');
    sz += count(s.begin(), s.end(), '\"');

    sz += 2; // starting and ending double quotes

    return sz;
}

int main()
{
    std::vector<std::string> strings;

    std::string s;
    while (std::cin >> s)
        strings.push_back(std::move(s));

    auto total_code = std::accumulate(strings.begin(), strings.end(), 0,
        [] (auto acc, const std::string& s) { return acc + s.size(); });

    auto total_in_mem = std::accumulate(strings.begin(), strings.end(), 0,
        [] (auto acc, const std::string& s) { return acc + size_in_mem(s); });

    auto total_encoded = std::accumulate(strings.begin(), strings.end(), 0,
        [] (auto acc, const std::string& s) { return acc + size_encoded(s); });

    std::cout << total_code - total_in_mem << "\n";
    std::cout << total_encoded - total_code << "\n";

    return 0;
}

