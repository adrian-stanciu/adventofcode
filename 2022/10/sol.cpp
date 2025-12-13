#include <bits/stdc++.h>

using namespace std;

auto solve1(const vector<int>& values)
{
    auto is_special_cycle = [](auto cycle) {
        return (cycle - 20) % 40 == 0;
    };

    auto sum{0};
    auto x{1};
    auto cycle{0};

    auto inc_cycle = [&]() {
        ++cycle;
        if (is_special_cycle(cycle))
            sum += cycle * x;
    };

    for (auto val : values) {
        inc_cycle();
        if (val) {
            inc_cycle();
            x += val;
        }
    }

    return sum;
}

auto solve2(const vector<int>& values)
{
    const auto size{240};
    const auto width{40};

    vector<char> img(size, ' ');
    auto x{1};
    auto cycle{0};

    auto inc_cycle = [&]() {
        auto pos = cycle % width;
        if (x - 1 <= pos && pos <= x + 1)
            img[cycle] = '#';
        ++cycle;
    };

    for (auto val : values) {
        inc_cycle();
        if (val) {
            inc_cycle();
            x += val;
        }
    }

    for (auto i = 0; i < size; ++i) {
        cout << img[i];
        if ((i + 1) % width == 0)
            cout << '\n';
    }
}

int main()
{
    vector<int> values;

    string line;
    while (getline(cin, line))
        if (line == "noop") {
            values.push_back(0);
        } else {
            int val;
            sscanf(line.data(), "addx %d", &val);
            values.push_back(val);
        }

    cout << solve1(values) << '\n';
    solve2(values);

    return 0;
}

