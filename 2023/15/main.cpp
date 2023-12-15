#include <bits/stdc++.h>

using namespace std;

auto split(string_view sv, char sep)
{
    while (sv.front() == sep)
        sv.remove_prefix(1);
    while (sv.back() == sep)
        sv.remove_suffix(1);

    vector<string_view> split_sv;

    while (true)
        if (auto to = sv.find_first_of(sep); to == sv.npos) {
            split_sv.push_back(sv);
            break;
        } else {
            split_sv.push_back(sv.substr(0, to));
            sv.remove_prefix(to + 1);
        }

    return split_sv;
}

auto compute_hash(string_view sv)
{
    auto h = 0;
    for (auto ch : sv) {
        h += static_cast<int>(ch);
        h *= 17;
        h %= 256;
    }
    return h;
}

auto solve1(const vector<string_view>& cmds)
{
    return accumulate(begin(cmds), end(cmds), 0, [](auto acc, auto cmd) {
        return acc + compute_hash(cmd);
    });
}

auto solve2(const vector<string_view>& cmds)
{
    vector<vector<pair<string_view, int>>> boxes(256);

    for (auto cmd : cmds) {
        auto sep = cmd.find_first_of("=-");
        assert(sep != cmd.npos);

        auto label = cmd.substr(0, sep);
        auto h = compute_hash(label);

        if (cmd[sep] == '=') {
            auto value = cmd[sep + 1] - '0';

            auto it = find_if(begin(boxes[h]), end(boxes[h]), [&](const auto& elem) {
                return elem.first == label;
            });
            if (it == end(boxes[h]))
                boxes[h].emplace_back(label, value);
            else
                it->second = value;
        } else { // -
            for (auto it = begin(boxes[h]); it != end(boxes[h]); ++it)
                if (it->first == label) {
                    boxes[h].erase(it);
                    break;
                }
        }
    }

    auto sum = 0;
    for (auto h = 0; h < 256; ++h)
        for (auto i = 0; i < ssize(boxes[h]); ++i)
            sum += (h + 1) * (i + 1) * boxes[h][i].second;
    return sum;
}

int main()
{
    string line;
    getline(cin, line);

    auto cmds = split(string_view{line}, ',');

    cout << solve1(cmds) << '\n';
    cout << solve2(cmds) << '\n';

    return 0;
}

