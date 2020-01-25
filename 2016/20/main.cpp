#include <algorithm>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>

struct Interval {
    long left;
    long right;

    Interval(long left, long right)
    : left(left), right(right)
    {}
};

auto find_first_not_in_intervals(const std::vector<Interval>& intervals)
{
    auto k = 0L;

    for (const auto& i : intervals) {
        if (k < i.left)
            break;

        k = std::max(k, i.right + 1);
    }

    return k;
}

auto count_not_in_intervals(const std::vector<Interval>& intervals)
{
    const auto last = static_cast<long>(std::numeric_limits<unsigned int>::max());

    auto cnt = 0L;
    auto k = 0L;

    for (const auto& i : intervals) {
        if (k < i.left)
            cnt += i.left - k;

        k = std::max(k, i.right + 1);
    }

    cnt += last - k + 1;

    return cnt;
}

int main()
{
    static const std::regex interval_re{"([0-9]+)-([0-9]+)"};

    std::vector<Interval> intervals;

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;

        if (!regex_match(line, matched, interval_re))
            continue;

        auto left = strtol(matched[1].str().data(), nullptr, 10);
        auto right = strtol(matched[2].str().data(), nullptr, 10);

        intervals.emplace_back(left, right);
    }

    sort(intervals.begin(), intervals.end(), [] (const auto& i1, const auto& i2) {
        if (i1.left == i2.left)
            return i1.right < i2.right;
        else
            return i1.left < i2.left;
    });

    std::cout << find_first_not_in_intervals(intervals) << "\n";
    std::cout << count_not_in_intervals(intervals) << "\n";

    return 0;
}

