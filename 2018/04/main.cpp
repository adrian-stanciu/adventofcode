#include <algorithm>
#include <array>
#include <charconv>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "parser.hpp"

enum class EventType {
    Guard,
    Asleep,
    Awake,
    None,
};

struct Event {
    int id;
    int mo;
    int d;
    int h;
    int mi;
    EventType type;
};

struct Guard {
    int id;
    std::array<int, 60> sleep_table;
};

auto find_strategy_1(const std::vector<Event>& events)
{
    std::unordered_map<int, int> sleep_table;

    // find guard ID with most minutes asleep
    auto last_id = 0;
    auto last_sleep_start = 0;

    for (auto& e : events) {
        if (e.type == EventType::Guard)
            last_id = e.id;
        else if (e.type == EventType::Asleep)
            last_sleep_start = e.mi;
        else
            sleep_table[last_id] += e.mi - last_sleep_start;
    }

    auto max_sleep = 0;
    auto max_sleep_id = 0;

    for (auto& s : sleep_table)
        if (s.second > max_sleep) {
            max_sleep = s.second;
            max_sleep_id = s.first;
        }

    // find the minute that guard spent asleep the most
    std::array<int, 60> sleep_per_mi {};
    last_id = 0;
    last_sleep_start = 0;

    for (auto& e : events) {
        if (e.type == EventType::Guard)
            last_id = e.id;
        else if (e.type == EventType::Asleep)
            last_sleep_start = e.mi;
        else
            if (last_id == max_sleep_id) {
                sleep_per_mi[last_sleep_start] += 1;
                sleep_per_mi[e.mi] -= 1;
            }
    }

    auto max_sleep_per_mi = sleep_per_mi[0];
    auto max_sleep_mi = 0;

    for (auto i = 1U; i < sleep_per_mi.size(); ++i) {
        sleep_per_mi[i] += sleep_per_mi[i - 1];

        if (sleep_per_mi[i] > max_sleep_per_mi) {
            max_sleep_per_mi = sleep_per_mi[i];
            max_sleep_mi = i;
        }
    }

    return max_sleep_id * max_sleep_mi;
}

auto find_strategy_2(const std::vector<Event>& events)
{
    std::unordered_map<int, Guard> guard_table;

    // find guard that is most frequently asleep on the same minute
    auto last_id = 0;
    auto last_sleep_start = 0;

    for (auto& e : events) {
        if (e.type == EventType::Guard)
            last_id = e.id;
        else if (e.type == EventType::Asleep)
            last_sleep_start = e.mi;
        else {
            guard_table[last_id].sleep_table[last_sleep_start] += 1;
            guard_table[last_id].sleep_table[e.mi] -= 1;
        }
    }

    auto max_sleep = 0;
    auto max_sleep_id = 0;
    auto max_sleep_mi = 0;

    for (auto& [id, g] : guard_table) {
        if (g.sleep_table[0] > max_sleep) {
            max_sleep = g.sleep_table[0];
            max_sleep_id = id;
            max_sleep_mi = 0;
        }

        for (auto i = 1U; i < g.sleep_table.size(); ++i) {
            g.sleep_table[i] += g.sleep_table[i - 1];

            if (g.sleep_table[i] > max_sleep) {
                max_sleep = g.sleep_table[i];
                max_sleep_id = id;
                max_sleep_mi = i;
            }
        }
    }

    return max_sleep_id * max_sleep_mi;
}

int main()
{
    // [Y-Mo-D H:Mi]
    static const std::regex time_re {R"(\[([0-9]+)-([0-9]+)-([0-9]+) ([0-9]+):([0-9]+)\] (.+))"};
    // Guard #ID begins shift
    static const std::regex guard_re {"Guard #([0-9]+) begins shift"};

    std::vector<Event> events;

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;
        regex_match(line, matched, time_re);

        int mo = str2num(matched[2].str());
        int d = str2num(matched[3].str());
        int h = str2num(matched[4].str());
        int mi = str2num(matched[5].str());

        auto type_str = matched[6].str();

        int id = -1;
        EventType type{EventType::None};

        if (regex_match(type_str, matched, guard_re)) {
            id = str2num(matched[1].str());
            type = EventType::Guard;
        } else if (type_str == "falls asleep")
            type = EventType::Asleep;
        else if (type_str == "wakes up")
            type = EventType::Awake;

        events.push_back({id, mo, d, h, mi, type});
    }

    sort(events.begin(), events.end(), [](const Event& e1, const Event& e2) {
        if (e1.mo != e2.mo)
            return e1.mo < e2.mo;
        if (e1.d != e2.d)
            return e1.d < e2.d;
        if (e1.h != e2.h)
            return e1.h < e2.h;
        return e1.mi < e2.mi;
    });

    // guards are asleep only during the midnight hour (00:00 - 00:59)
    std::cout << find_strategy_1(events) << "\n";
    std::cout << find_strategy_2(events) << "\n";

    return 0;
}

