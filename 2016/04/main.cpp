#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <vector>

struct Room {
    std::string enc_name;
    long id;
};

std::optional<Room> parse_line(const std::string& line)
{
    auto pos = line.find_last_of('-');

    const auto trailer = line.substr(pos + 1);
    std::smatch matched;
    static const std::regex trailer_re{R"(([1-9][0-9]*)\[(.+)\])"};
    if (!regex_match(trailer, matched, trailer_re))
        return {};

    std::vector<std::pair<char, int>> freq;
    for (auto c = 'a'; c <= 'z'; ++c)
        freq.emplace_back(c, 0);

    for (auto i = 0U; i < pos; ++i)
        if (line[i] != '-')
            ++freq[line[i] - 'a'].second;

    sort(freq.begin(), freq.end(), [] (const auto& f1, const auto& f2) {
        if (f1.second == f2.second)
            return f1.first < f2.first;
        else
            return f1.second > f2.second;
    });

    const auto csum = matched[2].str();

    for (auto i = 0U; i < csum.size(); ++i)
        if (freq[i].first != csum[i])
            return {};

    return Room{line.substr(0, pos), strtol(matched[1].str().data(), nullptr, 0L)};
}

auto sum(const std::vector<Room>& rooms)
{
    return accumulate(rooms.begin(), rooms.end(), 0L, [] (auto acc, const auto& room) {
        return acc + room.id;
    });
}

auto find(const std::vector<Room>& rooms, const std::string& target)
{
    auto decode_name = [] (const Room& room) {
        std::string name;

        for (auto c : room.enc_name)
            if (c == '-')
                name.push_back(' ');
            else
                name.push_back('a' + (c - 'a' + room.id % 26) % 26);

        return name;
    };

    for (const auto& room : rooms)
        if (decode_name(room) == target)
            return room.id;

    return 0L; // not found
}

int main()
{
    std::vector<Room> rooms;

    std::string line;
    while (getline(std::cin, line)) {
        auto room = parse_line(line);
        if (room.has_value())
            rooms.push_back(room.value());
    }

    std::cout << sum(rooms) << "\n";
    std::cout << find(rooms, "northpole object storage") << "\n";

    return 0;
}

