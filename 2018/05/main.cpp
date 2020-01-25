#include <cctype>
#include <iostream>
#include <string>
#include <stack>

auto compute_reacted_polymer_size(const std::string& poly, char skip_c)
{
    std::stack<char> s;

    for (auto c : poly) {
        if (c == skip_c || tolower(c) == skip_c)
            continue;

        if (s.empty())
            s.push(c);
        else {
            auto top_c = s.top();
            if (top_c != c && tolower(top_c) == tolower(c))
                s.pop();
            else
                s.push(c);
        }
    }

    return s.size();
}

auto compute_shortest_reacted_improved_polymer_size(const std::string& poly)
{
    size_t min_len = poly.size();

    for (auto c = 'a'; c <= 'z'; ++c)
        min_len = std::min(min_len, compute_reacted_polymer_size(poly, c));

    return min_len;
}

int main()
{
    std::string polymer;
    getline(std::cin, polymer);

    std::cout << compute_reacted_polymer_size(polymer, 0) << "\n";
    std::cout << compute_shortest_reacted_improved_polymer_size(polymer) << "\n";

    return 0;
}

