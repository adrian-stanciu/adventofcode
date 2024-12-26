#include <algorithm>
#include <iostream>
#include <string>

bool is_trap(char left, char right)
{
    return (left == '^' && right == '.') || (left == '.' && right == '^');
}

auto generate_next_tiles(const std::string& tiles)
{
    std::string next_tiles;
    next_tiles.reserve(tiles.size() - 2);

    for (auto i = 1U; i < tiles.size() - 1; ++i)
        next_tiles.push_back(is_trap(tiles[i - 1], tiles[i + 1]) ? '^' : '.');

    return next_tiles;
}

auto count_safe_tiles(const std::string& first_row, int num_rows)
{
    auto cnt = 0L;

    if (num_rows <= 0)
        return cnt;

    cnt += count(first_row.begin(), first_row.end(), '.');
    --num_rows;

    std::string tiles;
    tiles.push_back('.');
    tiles.append(first_row);
    tiles.push_back('.');

    while (num_rows) {
        std::string next_tiles;
        next_tiles.push_back('.');
        next_tiles.append(generate_next_tiles(tiles));
        next_tiles.push_back('.');

        swap(tiles, next_tiles);

        cnt += count(tiles.begin(), tiles.end(), '.') - 2;
        --num_rows;
    }

    return cnt;
}

int main()
{
    std::string first_row;
    std::cin >> first_row;

    std::cout << count_safe_tiles(first_row, 40) << "\n";
    std::cout << count_safe_tiles(first_row, 400000) << "\n";

    return 0;
}

