#pragma once

#include <charconv>
#include <string_view>
#include <vector>

namespace {
    [[maybe_unused]] inline auto split(std::string_view sv, char sep)
    {
        while (sv.front() == sep)
            sv.remove_prefix(1);
        while (sv.back() == sep)
            sv.remove_suffix(1);

        std::vector<std::string_view> splitted;

        while (true)
            if (auto to = sv.find_first_of(sep); to == sv.npos) {
                splitted.push_back(sv);
                break;
            } else {
                splitted.push_back(sv.substr(0, to));
                sv.remove_prefix(to + 1);
            }

        return splitted;
    }

    [[maybe_unused]] inline auto str2num(std::string_view sv)
    {
        auto n = 0l;
        std::from_chars(sv.data(), sv.data() + sv.size(), n);
        return n;
    }
}

