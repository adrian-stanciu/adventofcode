#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using Row = std::vector<unsigned long long>;
using Matrix = std::vector<Row>;

Matrix read_matrix()
{
    Matrix matrix;

    std::string line;
    while (getline(std::cin, line)) {
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        copy(std::istream_iterator<std::string>(ss),
            std::istream_iterator<std::string>(), back_inserter(tokens));

        Row row;
        for (auto& token : tokens)
            row.push_back(strtoull(token.data(), nullptr, 10));

        matrix.push_back(std::move(row));
    }

    return matrix;
}

unsigned long long checksum1(const Matrix& matrix)
{
    unsigned long long csum = 0;

    for (auto& row : matrix) {
        unsigned long long min = -1;
        unsigned long long max = 0;
        for (auto n : row) {
            if (n < min)
                min = n;
            if (n > max)
                max = n;
        }

        csum += (max - min);
    }

    return csum;
}

unsigned long long checksum2(const Matrix& matrix)
{
    unsigned long long csum = 0;

    for (auto& row : matrix) {
        bool found = false;
        for (auto n : row) {
            for (auto m : row) {
                if (n == m)
                    continue;

                unsigned long long min, max;
                if (n < m) {
                    min = n;
                    max = m;
                } else {
                    min = m;
                    max = n;
                }

                if (max % min == 0) {
                    csum += (max / min);
                    found = true;
                    break;
                }
            }

            if (found)
                break;
        }
    }

    return csum;
}

int main()
{
    auto matrix = read_matrix();

    std::cout << checksum1(matrix) << "\n";
    std::cout << checksum2(matrix) << "\n";

    return 0;
}

