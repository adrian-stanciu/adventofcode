#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

struct BotValue {
private:
    std::optional<long> value1;
    std::optional<long> value2;

public:
    bool is_full() const
    {
        return value1.has_value() && value2.has_value();
    }

    std::optional<long> min() const
    {
        if (!is_full())
            return {};

        return std::min(*value1, *value2);
    }

    std::optional<long> max() const
    {
        if (!is_full())
            return {};

        return std::max(*value1, *value2);
    }

    void add(long value)
    {
        if (!value1.has_value())
            value1 = value;
        else if (!value2.has_value())
            value2 = value;
    }

    void reset()
    {
        value1.reset();
        value2.reset();
    }
};

struct BotLink {
    std::optional<long> bot_low;
    std::optional<long> bot_high;
    std::optional<long> out_low;
    std::optional<long> out_high;
};

auto simulate1(std::unordered_map<long, BotValue> bot_values,
    const std::unordered_map<long, BotLink>& bot_links, long target_low, long target_high)
{
    while (true)
        for (auto& [bot, bot_val] : bot_values)
            if (bot_val.is_full()) {
                auto low = *bot_val.min();
                auto high = *bot_val.max();
                bot_val.reset();

                if (low == target_low && high == target_high)
                    return bot;

                auto bot_link = bot_links.at(bot);

                if (bot_link.bot_low.has_value())
                    bot_values[*bot_link.bot_low].add(low);
                if (bot_link.bot_high.has_value())
                    bot_values[*bot_link.bot_high].add(high);
            }
}

auto simulate2(std::unordered_map<long, BotValue> bot_values,
    const std::unordered_map<long, BotLink>& bot_links)
{
    std::array<std::optional<long>, 3> out;

    while (true)
        for (auto& [bot, bot_val] : bot_values)
            if (bot_val.is_full()) {
                auto low = *bot_val.min();
                auto high = *bot_val.max();
                bot_val.reset();

                auto bot_link = bot_links.at(bot);

                if (bot_link.bot_low.has_value())
                    bot_values[*bot_link.bot_low].add(low);
                else if (bot_link.out_low.has_value()) {
                    auto out_low = *bot_link.out_low;
                    if (out_low >= 0 && out_low <= 2)
                        out[out_low] = low;
                }

                if (bot_link.bot_high.has_value())
                    bot_values[*bot_link.bot_high].add(high);
                else if (bot_link.out_high.has_value()) {
                    auto out_high = *bot_link.out_high;
                    if (out_high >= 0 && out_high <= 2)
                        out[out_high] = high;
                }

                if (all_of(out.begin(), out.end(),
                    [] (const auto& o) {
                        return o.has_value();
                    })) {
                    return accumulate(out.begin(), out.end(), 1L,
                    [] (auto acc, const auto& o) {
                        return acc * o.value();
                    });
                }
            }
}

int main()
{
    static const std::regex bot_value_re{"value ([0-9]+) goes to bot ([0-9]+)"};
    static const std::regex bot_link_bot_bot_re{"bot ([0-9]+) gives low to bot ([0-9]+) and high to bot ([0-9]+)"};
    static const std::regex bot_link_bot_out_re{"bot ([0-9]+) gives low to bot ([0-9]+) and high to output ([0-9]+)"};
    static const std::regex bot_link_out_bot_re{"bot ([0-9]+) gives low to output ([0-9]+) and high to bot ([0-9]+)"};
    static const std::regex bot_link_out_out_re{"bot ([0-9]+) gives low to output ([0-9]+) and high to output ([0-9]+)"};

    std::unordered_map<long, BotValue> bot_values;
    std::unordered_map<long, BotLink> bot_links;

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;

        if (regex_match(line, matched, bot_value_re)) {
            auto value = strtol(matched[1].str().data(), nullptr, 10);
            auto bot = strtol(matched[2].str().data(), nullptr, 10);

            bot_values[bot].add(value);
        } else if (regex_match(line, matched, bot_link_bot_bot_re)) {
            auto bot = strtol(matched[1].str().data(), nullptr, 10);
            auto low = strtol(matched[2].str().data(), nullptr, 10);
            auto high = strtol(matched[3].str().data(), nullptr, 10);

            bot_links[bot].bot_low = low;
            bot_links[bot].bot_high = high;
        } else if (regex_match(line, matched, bot_link_bot_out_re)) {
            auto bot = strtol(matched[1].str().data(), nullptr, 10);
            auto low = strtol(matched[2].str().data(), nullptr, 10);
            auto high = strtol(matched[3].str().data(), nullptr, 10);

            bot_links[bot].bot_low = low;
            bot_links[bot].out_high = high;
        } else if (regex_match(line, matched, bot_link_out_bot_re)) {
            auto bot = strtol(matched[1].str().data(), nullptr, 10);
            auto low = strtol(matched[2].str().data(), nullptr, 10);
            auto high = strtol(matched[3].str().data(), nullptr, 10);

            bot_links[bot].out_low = low;
            bot_links[bot].bot_high = high;
        } else if (regex_match(line, matched, bot_link_out_out_re)) {
            auto bot = strtol(matched[1].str().data(), nullptr, 10);
            auto low = strtol(matched[2].str().data(), nullptr, 10);
            auto high = strtol(matched[3].str().data(), nullptr, 10);

            bot_links[bot].out_low = low;
            bot_links[bot].out_high = high;
        }
    }

    std::cout << simulate1(bot_values, bot_links, 17, 61) << "\n";
    std::cout << simulate2(bot_values, bot_links) << "\n";

    return 0;
}

