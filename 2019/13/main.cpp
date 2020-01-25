#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include "int_computer.h"

enum TileType {
    Empty,
    Wall,
    Block,
    Paddle,
    Ball,
};

std::unordered_map<TileType, char> alphabet {
    {Empty, ' '},
    {Wall, '#'},
    {Block, '*'},
    {Paddle, '_'},
    {Ball, 'o'},
};

struct Tile {
    long long x;
    long long y;
    TileType type;

    Tile(long long x, long long y, TileType type)
    : x(x), y(y), type(type)
    {}
};

auto count_tiles(const std::vector<long long>& prog, TileType type)
{
    IntComputer ic(prog);

    auto cnt = 0;

    auto i = 0;
    while (true) {
        ++i;

        if (!ic.run())
            break;

        if (i % 3 == 0 && *ic.get_last_output() == type)
            ++cnt;
    }

    return cnt;
}

auto play(const std::vector<long long>& prog)
{
    IntComputer ic(prog);
    ic.set_mem(0, 2);

    std::vector<Tile> tiles;
    long long num_tiles_on_row = -1;

    long long x = -1;
    long long y = -1;
    auto min_x = std::numeric_limits<long long>::max();
    auto max_x = std::numeric_limits<long long>::min();
    auto min_y = std::numeric_limits<long long>::max();

    long long ball_x = -1;
    long long paddle_x = -1;

    auto move_paddle = [&] () {
        if (ball_x < paddle_x)
            ic.append_input(-1);
        else if (ball_x > paddle_x)
            ic.append_input(1);
        else
            ic.append_input(0);
    };

    long long score = 0;
    bool has_new_score = false;

    bool started = false;

    auto i = 0;
    while (true) {
        ++i;

        if (!ic.run())
            break;

        auto out = *ic.get_last_output();

        if (i % 3 == 1) {
            if (out == -1)
                has_new_score = true;
            else
                x = out;
        } else if (i % 3 == 2) {
            if (!has_new_score)
                y = out;
        } else {
            if (has_new_score) {
                has_new_score = false;

                score = out;

                if (!started) {
                    started = true;

                    for (auto& t : tiles) {
                        t.x -= min_x;
                        t.y -= min_y;
                    }

                    ball_x -= min_x;
                    paddle_x -= min_x;

                    sort(tiles.begin(), tiles.end(), [] (const Tile& t1, const Tile& t2) {
                        if (t1.y == t2.y)
                            return t1.x < t2.x;
                        else
                            return t1.y < t2.y;
                    });

                    num_tiles_on_row = max_x - min_x + 1;

                    move_paddle();
                }
            } else {
                if (!started) {
                    min_x = std::min(min_x, x);
                    max_x = std::max(max_x, x);
                    min_y = std::min(min_y, y);

                    tiles.emplace_back(x, y, static_cast<TileType>(out));

                    if (static_cast<TileType>(out) == Ball)
                        ball_x = x;
                    else if (static_cast<TileType>(out) == Paddle)
                        paddle_x = x;
                } else {
                    x -= min_x;
                    y -= min_y;

                    if (static_cast<TileType>(out) == Ball) {
                        ball_x = x;
                        move_paddle();
                    } else if (static_cast<TileType>(out) == Paddle) {
                        paddle_x = x;
                    }

                    tiles[y * num_tiles_on_row + x].type = static_cast<TileType>(out);
                }
            }
        }
    }

    return score;
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << count_tiles(prog, Block) << "\n";

    std::cout << play(prog) << "\n";

    return 0;
}

