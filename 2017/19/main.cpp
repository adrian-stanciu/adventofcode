#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Row = std::vector<char>;
using Matrix = std::vector<Row>;

Matrix read_matrix()
{
    Matrix matrix;

    std::string line;
    while (getline(std::cin, line)) {
        Row row;
        for (auto ch : line)
            row.push_back(ch);
        matrix.push_back(std::move(row));
    }

    return matrix;
}

int find_start_col(const Row& row)
{
    for (size_t i = 0; i < row.size(); ++i)
        if (row[i] == '|')
            return i;
    return -1;
}

bool is_finish(const Matrix& matrix, int r, int c)
{
    if (r < 0 || r >= static_cast<int>(matrix.size()))
        return true;

    if (c < 0 || c >= static_cast<int>(matrix[r].size()))
        return true;

    if (matrix[r][c] == ' ')
        return true;

    return false;
}

void explore(const Matrix& matrix, int r, int c, int r_dir, int c_dir,
    std::ostringstream& oss, long& steps)
{
    if (is_finish(matrix, r, c))
        return;

    ++steps;

    if (matrix[r][c] == '|' || matrix[r][c] == '-') {
        // nothing to do
    } else if (matrix[r][c] == '+') {
        // change dirs
        if (r_dir != 0) {
            r_dir = 0;
            // change column: choose left or right
            if (c > 0 && matrix[r][c - 1] == '-')
                c_dir = -1;
            else
                c_dir = 1;
        } else {
            c_dir = 0;
            // change column: choose up or down
            if (r > 0 && matrix[r - 1][c] == '|')
                r_dir = -1;
            else
                r_dir = 1;
        }
    } else {
        // letter
        oss << matrix[r][c];
    }

    explore(matrix, r + r_dir, c + c_dir, r_dir, c_dir, oss, steps);
}

auto get_path_and_steps(const Matrix& matrix)
{
    std::ostringstream oss;
    long steps = 0;

    explore(matrix, 0, find_start_col(matrix[0]), 1, 0, oss, steps);

    return std::make_pair(oss.str(), steps);
}

int main()
{
    auto matrix = read_matrix();

    auto [path, steps] = get_path_and_steps(matrix);
    std::cout << path << "\n";
    std::cout << steps << "\n";

    return 0;
}

