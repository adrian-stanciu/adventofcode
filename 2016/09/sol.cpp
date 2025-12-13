#include <iostream>
#include <regex>
#include <string>

auto compute_decompressed_size(const std::string& line)
{
    static const std::regex marker_re{"([1-9][0-9]*)x([1-9][0-9]*)"};

    size_t sz = 0;
    size_t pos = 0;

    while (true) {
        auto lp = line.find_first_of('(', pos);
        if (lp != std::string::npos) {
            sz += lp - pos;

            auto rp = line.find_first_of(')', lp + 1);
            auto marker = line.substr(lp + 1, rp - lp - 1);

            std::smatch matched;
            regex_match(marker, matched, marker_re);
            auto len = strtoul(matched[1].str().data(), nullptr, 10);
            auto cnt = strtoul(matched[2].str().data(), nullptr, 10);

            sz += len * cnt;
            pos = rp + len + 1;
        } else {
            sz += line.size() - pos;
            break;
        }
    }

    return sz;
}

auto compute_decompressed_size_v2(const std::string& line) -> size_t
{
    static const std::regex marker_re{"([1-9][0-9]*)x([1-9][0-9]*)"};

    size_t sz = 0;
    size_t pos = 0;

    while (true) {
        auto lp = line.find_first_of('(', pos);
        if (lp != std::string::npos) {
            sz += lp - pos;

            auto rp = line.find_first_of(')', lp + 1);
            auto marker = line.substr(lp + 1, rp - lp - 1);

            std::smatch matched;
            regex_match(marker, matched, marker_re);
            auto len = strtoul(matched[1].str().data(), nullptr, 10);
            auto cnt = strtoul(matched[2].str().data(), nullptr, 10);

            sz += compute_decompressed_size_v2(line.substr(rp + 1, len)) * cnt;
            pos = rp + len + 1;
        } else {
            sz += line.size() - pos;
            break;
        }
    }

    return sz;
}

int main()
{
    std::string line;
    getline(std::cin, line);

    std::cout << compute_decompressed_size(line) << "\n";
    std::cout << compute_decompressed_size_v2(line) << "\n";

    return 0;
}

