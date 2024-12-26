#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Scanner {
    unsigned long depth;
    unsigned long range;

    Scanner(unsigned long depth, unsigned long range)
    : depth(depth)
    , range(range)
    {}
};

auto read_scanners()
{
    std::vector<Scanner> scanners;

    std::string line;
    while (getline(std::cin, line)) {
        std::stringstream ss(line);

        unsigned long depth, range;
        ss >> depth;
        ss.ignore();
        ss >> range;

        scanners.emplace_back(depth, range);
    }

    return scanners;
}

unsigned long compute_severity(const std::vector<Scanner>& scanners)
{
    unsigned long severity = 0;

    for (auto& scanner : scanners) {
        const unsigned long t = scanner.depth;
        const unsigned long path_len = 2 * (scanner.range - 1);

        if (t % path_len == 0)
            severity += scanner.depth * scanner.range;
    }

    return severity;
}

bool is_caught(const std::vector<Scanner>& scanners, unsigned long delay)
{
    for (auto& scanner : scanners) {
        const unsigned long t = scanner.depth + delay;
        const unsigned long path_len = 2 * (scanner.range - 1);

        if (t % path_len == 0)
            return true;
    }

    return false;
}

unsigned long compute_delay(const std::vector<Scanner>& scanners)
{
    unsigned long delay = 0;

    while (true) {
        if (!is_caught(scanners, delay))
            return delay;

        ++delay;
    }
}

int main()
{
    auto scanners = read_scanners();

    std::cout << compute_severity(scanners) << "\n";
    std::cout << compute_delay(scanners) << "\n";

    return 0;
}

