#include <bits/stdc++.h>

using namespace std;

struct Display {
    vector<string> all_signal_patterns;
    vector<string> out_signal_patterns;

    Display(vector<string> all_signal_patterns, vector<string> out_signal_patterns)
    : all_signal_patterns{move(all_signal_patterns)}
    , out_signal_patterns{move(out_signal_patterns)}
    {}
};

auto solve1(const vector<Display>& v)
{
    auto cnt = 0l;

    for (const auto& [_, out_signal_patterns] : v)
        for (const auto& sig_pat : out_signal_patterns) {
            auto sz = sig_pat.size();
            cnt += (sz >= 2 && sz <= 4) || sz == 7;
        }

    return cnt;
}

auto decode(const vector<string>& all_signal_patterns)
{
    unordered_map<string, int> signal_pattern2digit;
    array<string, 10> digit2signal_pattern;

    auto update_if = [&](auto digit, const auto& predicate) {
        for (const auto& sig_pat : all_signal_patterns)
            if (predicate(sig_pat)) {
                digit2signal_pattern[digit] = sig_pat;
                signal_pattern2digit[sig_pat] = digit;
            }
    };

    for (const auto& sig_pat : all_signal_patterns)
        switch (sig_pat.size()) {
        case 2:
            signal_pattern2digit[sig_pat] = 1;
            digit2signal_pattern[1] = sig_pat;
            break;
        case 3:
            signal_pattern2digit[sig_pat] = 7;
            digit2signal_pattern[7] = sig_pat;
            break;
        case 4:
            signal_pattern2digit[sig_pat] = 4;
            digit2signal_pattern[4] = sig_pat;
            break;
        case 7:
            signal_pattern2digit[sig_pat] = 8;
            digit2signal_pattern[8] = sig_pat;
            break;
        }

    auto contains_signals = [](const auto& sig_pat, const auto& signals) {
        for (auto sig : signals)
            if (find(begin(sig_pat), end(sig_pat), sig) == end(sig_pat))
                return false;
        return true;
    };

    update_if(3, [&](const auto& sig_pat) {
        return sig_pat.size() == 5 && contains_signals(sig_pat, digit2signal_pattern[1]);
    });
    update_if(6, [&](const auto& sig_pat) {
        return sig_pat.size() == 6 && !contains_signals(sig_pat, digit2signal_pattern[1]);
    });

    digit2signal_pattern[9] = [&]() {
        auto sig_pat = digit2signal_pattern[3] + digit2signal_pattern[4];

        sort(begin(sig_pat), end(sig_pat));
        sig_pat.erase(unique(begin(sig_pat), end(sig_pat)), end(sig_pat));

        return sig_pat;

    }();
    signal_pattern2digit[digit2signal_pattern[9]] = 9;

    update_if(0, [&](const auto& sig_pat) {
        return sig_pat.size() == 6 &&
            sig_pat != digit2signal_pattern[6] && sig_pat != digit2signal_pattern[9];
    });

    auto left_down_signal = [&]() {
        auto sig_pat_9 = digit2signal_pattern[9];
        for (auto sig : digit2signal_pattern[8])
            if (find(begin(sig_pat_9), end(sig_pat_9), sig) == end(sig_pat_9))
                return sig;
        return '?';
    }();

    update_if(2, [&](const auto& sig_pat) {
        return sig_pat.size() == 5 &&
            find(begin(sig_pat), end(sig_pat), left_down_signal) != end(sig_pat);
    });
    update_if(5, [&](const auto& sig_pat) {
        return sig_pat.size() == 5 && sig_pat != digit2signal_pattern[3] &&
            find(begin(sig_pat), end(sig_pat), left_down_signal) == end(sig_pat);
    });

    return signal_pattern2digit;
}

auto solve2(const vector<Display>& v)
{
    auto sum = 0l;

    for (const auto& [all_signal_patterns, out_signal_patterns] : v) {
        auto signal_pattern2digit = decode(all_signal_patterns);

        auto val = 0;

        for (const auto& sig_pat : out_signal_patterns) {
            val *= 10;
            val += signal_pattern2digit[sig_pat];
        }

        sum += val;
    }

    return sum;
}

int main()
{
    static const regex r{R"((.+) \| (.+))"};

    auto split = [](const string& s) {
        vector<string> v;

        stringstream ss{s};
        string x;
        while (ss >> x) {
            sort(begin(x), end(x));

            v.push_back(move(x));
        }

        return v;
    };

    vector<Display> v;

    string line;
    while (getline(cin, line)) {
        smatch matched;
        regex_match(line, matched, r);

        v.emplace_back(split(matched[1].str()), split(matched[2].str()));
    }

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

