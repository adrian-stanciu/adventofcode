#include <bits/stdc++.h>

using namespace std;

auto solve(const string& s, const unordered_map<string, char>& rules, int num_iters)
{
    array<long, 26> freq{};
    for (auto c : s)
        ++freq[c - 'A'];

    unordered_map<string, long> pair2freq;
    for (auto i = 0u; i < s.size() - 1; ++i)
        ++pair2freq[s.substr(i, 2)];

    for (auto i = 0; i < num_iters; ++i) {
        unordered_map<string, long> new_pair2freq;

        for (const auto& [p, f] : pair2freq) {
            auto it = rules.find(p);
            if (it == end(rules))
                continue;

            freq[it->second - 'A'] += f;

            string p1({p.front(), it->second});
            new_pair2freq[move(p1)] += f;

            string p2({it->second, p.back()});
            new_pair2freq[move(p2)] += f;
        }

        pair2freq.swap(new_pair2freq);
    }

    auto max_f = *max_element(begin(freq), end(freq));

    auto min_f = numeric_limits<long>::max();
    for (auto f : freq)
        if (f > 0)
            min_f = min(min_f, f);

    return max_f - min_f;
}

int main()
{
    static const regex r{R"(([A-Z]{2}) -> ([A-Z]{1}))"};

    string s;
    cin >> s;

    unordered_map<string, char> rules;

    string line;
    while (getline(cin, line)) {
        if (line.empty())
            continue;

        smatch matched;
        regex_match(line, matched, r);

        rules[matched[1].str()] = matched[2].str()[0];
    }

    cout << solve(s, rules, 10) << '\n';
    cout << solve(s, rules, 40) << '\n';

    return 0;
}

