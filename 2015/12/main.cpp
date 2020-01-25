#include <iostream>
#include <sstream>
#include <string>
#include <vector>

long compute_sum(std::stringstream& ss,
    const std::vector<std::string>& ignore_list, bool should_ignore)
{
    auto sum = 0L;
    auto sign = 1;
    auto num = 0L;

    auto add_num = [&] () {
        sum += sign * num;
        sign = 1;
        num = 0;
    };

    std::string seen;

    char c;
    while (ss >> c) {
        if (c == ']' || c == '}') {
            add_num();
            break;
        } else if (c == '[') {
            sum += compute_sum(ss, ignore_list, false);
        } else if (c == '{') {
            sum += compute_sum(ss, ignore_list, true);
        } else if (c == '-') {
            sign = -1;
        } else if (c >= '0' && c <= '9') {
            num *= 10;
            num += c - '0';
        } else {
            add_num();
            seen.push_back(c);
        }
    }

    if (should_ignore)
        for (const auto& ignore : ignore_list)
            if (seen.find(ignore) != std::string::npos)
                return 0;

    return sum;
}

auto compute_sum(const std::string& json, const std::vector<std::string>& ignore_list)
{
    std::stringstream ss(json);
    return compute_sum(ss, ignore_list, json[0] == '{');
}

int main()
{
    std::string json;

    char c;
    while (std::cin >> c)
        json.push_back(c);

    std::cout << compute_sum(json, {}) << "\n";
    std::cout << compute_sum(json, {":\"red\""}) << "\n";

    return 0;
}

