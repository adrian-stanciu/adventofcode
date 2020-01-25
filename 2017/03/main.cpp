#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

unsigned long compute_matrix_size(unsigned long n)
{
    unsigned long size = 1;

    while ((size * size) < n)
        size += 2;

    return size;
}

unsigned long distance(unsigned long n)
{
    unsigned long size = compute_matrix_size(n);
    unsigned long max = size * size;

    unsigned long diff = 0;
    if (n >= (max - (size - 1)))
        diff = n - (max - (size - 1));
    else if (n >= (max - 2 * (size - 1)))
        diff = n - (max - 2 * (size - 1));
    else if (n >= (max - 3 * (size - 1)))
        diff = n - (max - 3 * (size - 1));
    else if (n >= (max - 4 * (size - 1)))
        diff = n - (max - 4 * (size - 1));

    return ((size / 2) + (((size / 2) > diff) ? ((size / 2) - diff) : (diff - (size / 2))));
}

std::string stringify(unsigned long r, unsigned long c)
{
    std::stringstream ss;
    ss << r;
    ss << ";";
    ss << c;

    return ss.str();
}

unsigned long get_value(const std::unordered_map<std::string, unsigned long>& spiral,
    unsigned long r, unsigned long c)
{
    auto it = spiral.find(stringify(r, c));
    if (it == spiral.end())
        return 0;
    else
        return it->second;
}

unsigned long compute_value(const std::unordered_map<std::string, unsigned long>& spiral,
    unsigned long r, unsigned long c)
{
    unsigned long val = 0;

    val += get_value(spiral, r - 1, c - 1);
    val += get_value(spiral, r - 1, c);
    val += get_value(spiral, r - 1, c + 1);
    val += get_value(spiral, r, c - 1);
    val += get_value(spiral, r, c + 1);
    val += get_value(spiral, r + 1, c - 1);
    val += get_value(spiral, r + 1, c);
    val += get_value(spiral, r + 1, c + 1);

    return val;
}

unsigned long first_greater(unsigned long n, unsigned long start_val)
{
    std::unordered_map<std::string, unsigned long> spiral;
    unsigned long size = 1;
    long r = 0;
    long c = 0;

    spiral[stringify(r, c)] = start_val;

    while (true) {
        size += 2;
        ++r;
        ++c;

        const std::array<long, 4> r_inc_v{-1, 0, 1, 0};
        const std::array<long, 4> c_inc_v{0, -1, 0, 1};

        for (int i = 0; i < 4; ++i) {
            long r_inc = r_inc_v[i];
            long c_inc = c_inc_v[i];

            for (unsigned long j = 0; j < size - 1; ++j) {
                r += r_inc;
                c += c_inc;

                unsigned long val = compute_value(spiral, r, c);
                if (val > n)
                    return val;

                spiral[stringify(r, c)] = val;
            }
        }
    }
}

int main()
{
    unsigned long n;
    std::cin >> n;

    std::cout << distance(n) << "\n";
    std::cout << first_greater(n, 1) << "\n";

    return 0;
}

