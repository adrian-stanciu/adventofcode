#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

struct HexPath {
    private:
    enum Direction {
        NW,
        N,
        NE,
        SE,
        S,
        SW,
    };

    static constexpr auto size = 6;
    std::array<unsigned long, size> counters;

    void add(int dir_idx)
    {
        int opposite_dir_idx = (dir_idx + (size / 2)) % size;

        if (counters[opposite_dir_idx] > 0) {
            --counters[opposite_dir_idx];
        } else {
            int first_left_idx = (dir_idx - 1 + size) % size;
            int second_left_idx = (dir_idx - 2 + size) % size;
            int first_right_idx = (dir_idx + 1) % size;
            int second_right_idx = (dir_idx + 2) % size;

            if (counters[second_left_idx] > 0) {
                --counters[second_left_idx];
                ++counters[first_left_idx];
            } else if (counters[second_right_idx] > 0) {
                --counters[second_right_idx];
                ++counters[first_right_idx];
            } else
                ++counters[dir_idx];
        }
    }

    public:
    HexPath()
    : counters{}
    {}

    void add(const std::string& step)
    {
        if (step.compare("nw") == 0)
            add(Direction::NW);
        else if (step.compare("n") == 0)
            add(Direction::N);
        else if (step.compare("ne") == 0)
            add(Direction::NE);
        else if (step.compare("se") == 0)
            add(Direction::SE);
        else if (step.compare("s") == 0)
            add(Direction::S);
        else if (step.compare("sw") == 0)
            add(Direction::SW);
    }

    unsigned long dist() const
    {
        return std::accumulate(begin(counters), end(counters), 0UL);
    }
};

void read_and_process()
{
    std::string line;
    getline(std::cin, line);
    std::stringstream ss(line);

    HexPath path;
    unsigned long dist = 0;
    unsigned long max_dist = 0;

    std::string step;
    while (getline(ss, step, ',')) {
        path.add(step);

        dist = path.dist();

        if (dist > max_dist)
            max_dist = dist;
    }

    std::cout << dist << "\n";
    std::cout << max_dist << "\n";
}

int main()
{
    read_and_process();

    return 0;
}

