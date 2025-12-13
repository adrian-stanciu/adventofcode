#include <bits/stdc++.h>

using namespace std;

struct Leaf {
    int val;
    int lev;

    Leaf(int val, int lev) : val{val}, lev{lev} {}
};

using Number = vector<Leaf>;

auto parse(const string& s)
{
    Number num;

    auto lev = 0;
    for (auto c : s)
        if (c == '[')
            ++lev;
        else if (c == ']')
            --lev;
        else if (c >= '0' && c <= '9')
            num.emplace_back(c - '0', lev);

    return num;
}

auto explode(Number& num)
{
    auto n = static_cast<int>(num.size());

    for (auto i = 0; i < n; ++i)
        if (num[i].lev == 5) {
            assert(i + 1 < n && num[i + 1].lev == 5);

            if (i > 0)
                num[i - 1].val += num[i].val;
            if (i < n - 2)
                num[i + 2].val += num[i + 1].val;

            num[i].val = 0;
            --num[i].lev;

            num.erase(begin(num) + i + 1);

            return true;
        }

    return false;
}

auto split(Number& num)
{
    auto n = static_cast<int>(num.size());

    for (auto i = 0; i < n; ++i)
        if (num[i].val > 9) {
            auto l = num[i].val / 2;
            auto r = num[i].val - l;

            num[i].val = l;
            ++num[i].lev;

            num.emplace(begin(num) + i + 1, r, num[i].lev);

            return true;
        }

    return false;
}

auto reduce(Number& num)
{
    if (explode(num))
        return true;

    if (split(num))
        return true;

    return false;
}

auto add(const Number& lhs, const Number& rhs)
{
    Number sum;
    sum.reserve(lhs.size() + rhs.size());

    for (const auto& [val, lev] : lhs)
        sum.emplace_back(val, lev + 1);
    for (const auto& [val, lev] : rhs)
        sum.emplace_back(val, lev + 1);

    while (reduce(sum))
        ;

    return sum;
}

auto magnitude(const Number& num)
{
    stack<Leaf> st;

    for (const auto& [val, lev] : num) {
        auto target_val = val;
        auto target_lev = lev;

        while (!st.empty() && st.top().lev == target_lev) {
            auto mag = 3 * st.top().val + 2 * target_val;
            st.pop();

            target_val = mag;
            --target_lev;
        }

        st.emplace(target_val, target_lev);
    }

    return st.top().val;
}

auto solve1(const vector<Number>& numbers)
{
    auto sum = numbers[0];

    for (auto i = 1u; i < numbers.size(); ++i)
        sum = add(sum, numbers[i]);

    return magnitude(sum);
}

auto solve2(const vector<Number>& numbers)
{
    auto max_mag = 0;

    for (auto i = 0u; i < numbers.size(); ++i)
        for (auto j = 0u; j < numbers.size(); ++j)
            if (i != j) {
                max_mag = max(max_mag, magnitude(add(numbers[i], numbers[j])));
                max_mag = max(max_mag, magnitude(add(numbers[j], numbers[i])));
            }

    return max_mag;
}

int main()
{
    vector<Number> numbers;

    string line;
    while (getline(cin, line))
        numbers.push_back(parse(line));

    cout << solve1(numbers) << '\n';
    cout << solve2(numbers) << '\n';

    return 0;
}

