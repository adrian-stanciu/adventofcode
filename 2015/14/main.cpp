#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Reindeer {
    long speed;
    long flying_duration;
    long resting_duration;
    long cycle_duration;

    Reindeer(long speed, long flying_duration, long resting_duration)
    : speed(speed)
    , flying_duration(flying_duration)
    , resting_duration(resting_duration)
    , cycle_duration(flying_duration + resting_duration)
    {}

    auto distance_after(long t) const
    {
        auto num_completed_cycles = t / cycle_duration;
        auto remaining_duration = t % cycle_duration;

        auto total_flying_duration = num_completed_cycles * flying_duration;
        total_flying_duration += std::min(flying_duration, remaining_duration);

        return total_flying_duration * speed;
    }
};

auto parse(const std::string& line)
{
    static const std::regex re{"[a-zA-Z]+ can fly ([1-9][0-9]*) km/s for ([1-9][0-9]*) seconds, but then must rest for ([1-9][0-9]*) seconds."};

    static const auto to_number = [] (const auto& s) {
        return strtol(s.data(), nullptr, 10);
    };

    std::smatch matched;
    regex_match(line, matched, re);

    auto speed = to_number(matched[1].str());
    auto flying_duration = to_number(matched[2].str());
    auto resting_duration = to_number(matched[3].str());

    return Reindeer{speed, flying_duration, resting_duration};
}

auto max_distance_after(const std::vector<Reindeer>& reindeers, long t)
{
    auto max_dist = 0L;

    for (const auto& r : reindeers)
        max_dist = std::max(max_dist, r.distance_after(t));

    return max_dist;
}

auto max_points_after(const std::vector<Reindeer>& reindeers, long t)
{
    std::vector<long> distances(reindeers.size(), 0);
    std::vector<long> points(reindeers.size(), 0);

    for (auto i = 1; i <= t; ++i) {
        transform(reindeers.begin(), reindeers.end(), distances.begin(),
            [&] (const auto& r) { return r.distance_after(i); });

        auto max_dist = *max_element(distances.begin(), distances.end());

        transform(distances.begin(), distances.end(), points.begin(), points.begin(),
            [&] (const auto& d, const auto& p) { return p + (d == max_dist ? 1 : 0); });
    }

    return *max_element(points.begin(), points.end());
}

int main()
{
    std::vector<Reindeer> reindeers;

    std::string line;
    while (getline(std::cin, line))
        reindeers.push_back(parse(line));

    std::cout << max_distance_after(reindeers, 2503) << "\n";
    std::cout << max_points_after(reindeers, 2503) << "\n";

    return 0;
}

