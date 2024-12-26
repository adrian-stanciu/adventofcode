#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using Line = std::vector<char>;
using Screen = std::vector<Line>;

void turn_on_rect(Screen& screen, int rows, int cols)
{
    for (auto r = 0; r < rows; ++r)
        for (auto c = 0; c < cols; ++c)
            screen[r][c] = '#';
}

void rotate_row(Screen& screen, int row, int n)
{
    rotate(screen[row].rbegin(), screen[row].rbegin() + n, screen[row].rend());
}

void rotate_col(Screen& screen, int col, int n)
{
    Line column;
    for (const auto& row : screen)
        column.push_back(row[col]);

    rotate(column.rbegin(), column.rbegin() + n, column.rend());

    for (auto r = 0U; r < screen.size(); ++r)
        screen[r][col] = column[r];
}

auto count_pixels(const Screen& screen)
{
    auto cnt = 0;

    for (const auto& row : screen)
        for (auto pixel : row)
            if (pixel == '#')
                ++cnt;

    return cnt;
}

void print(const Screen& screen)
{
    for (const auto& row : screen) {
        for (auto pixel : row)
            std::cout << (pixel == '.' ? ' ' : pixel);
        std::cout << '\n';
    }
}

int main()
{
    static const std::regex rect_re{"rect ([1-9][0-9]*)x([1-9][0-9]*)"};
    static const std::regex rotate_y_re{"rotate row y=([0-9]+) by ([1-9][0-9]*)"};
    static const std::regex rotate_x_re{"rotate column x=([0-9]+) by ([1-9][0-9]*)"};

    Screen screen(6, Line(50, '.'));

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;

        if (regex_match(line, matched, rect_re)) {
            auto cols = strtol(matched[1].str().data(), nullptr, 10);
            auto rows = strtol(matched[2].str().data(), nullptr, 10);

            turn_on_rect(screen, rows, cols);
        } else if (regex_match(line, matched, rotate_y_re)) {
            auto row = strtol(matched[1].str().data(), nullptr, 10);
            auto n = strtol(matched[2].str().data(), nullptr, 10);

            rotate_row(screen, row, n);
        } else if (regex_match(line, matched, rotate_x_re)) {
            auto col = strtol(matched[1].str().data(), nullptr, 10);
            auto n = strtol(matched[2].str().data(), nullptr, 10);

            rotate_col(screen, col, n);
        }
    }

    std::cout << count_pixels(screen) << "\n";

    print(screen);

    return 0;
}

