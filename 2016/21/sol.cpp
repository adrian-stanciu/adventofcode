#include <algorithm>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

void rotate_l(std::string& data, long steps)
{
    rotate(data.begin(), data.begin() + (steps % ssize(data)), data.end());
}

void rotate_r(std::string& data, long steps)
{
    rotate(data.rbegin(), data.rbegin() + (steps % ssize(data)), data.rend());
}

struct SwapPos {
    long pos1;
    long pos2;

    void exec(std::string& data) const
    {
        std::swap(data[pos1], data[pos2]);
    }

    void undo(std::string& data) const
    {
        exec(data);
    }
};

struct SwapLetter {
    char let1;
    char let2;

    void exec(std::string& data) const
    {
        std::swap(data[data.find(let1)], data[data.find(let2)]);
    }

    void undo(std::string& data) const
    {
        exec(data);
    }
};

struct RotateR {
    long steps;

    void exec(std::string& data) const
    {
        rotate_r(data, steps);
    }

    void undo(std::string& data) const
    {
        rotate_l(data, steps);
    }
};

struct RotateL {
    long steps;

    void exec(std::string& data) const
    {
        rotate_l(data, steps);
    }

    void undo(std::string& data) const
    {
        rotate_r(data, steps);
    }
};

struct RotatePos {
    char let;

    void exec(std::string& data) const
    {
        auto pos = data.find(let);
        auto steps = (1 + pos + ((pos >= 4) ? 1 : 0));
        rotate_r(data, steps);
    }

    void undo(std::string& data) const
    {
        // data must have such a length (e.g. 8) that
        // each position after rotation is unique, so it can be reverted
        const auto steps_map = [&] () {
            std::unordered_map<long, long> steps_map;

            for (auto i = 0l; i < ssize(data); ++i) {
                auto steps = 1 + i + ((i >= 4) ? 1 : 0);
                steps_map[(i + steps) % ssize(data)] = steps;
            }

            return steps_map;
        } ();

        auto pos = data.find(let);
        rotate_l(data, steps_map.at(pos));
    }
};

struct Reverse {
    long pos1;
    long pos2;

    void exec(std::string& data) const
    {
        reverse(data.begin() + pos1, data.begin() + pos2 + 1);
    }

    void undo(std::string& data) const
    {
        exec(data);
    }
};

struct Move {
    long pos1;
    long pos2;

    void move(std::string& data, long src_pos, long dst_pos) const
    {
        auto let = data[src_pos];
        data.erase(data.begin() + src_pos);
        data.insert(dst_pos, 1, let);
    }

    void exec(std::string& data) const
    {
        move(data, pos1, pos2);
    }

    void undo(std::string& data) const
    {
        move(data, pos2, pos1);
    }
};

using Operation = std::variant<SwapPos, SwapLetter, RotateR, RotateL, RotatePos, Reverse, Move>;

auto scramble(const std::vector<Operation>& operations, std::string unscrambled)
{
    auto scrambled{std::move(unscrambled)};

    for (const auto& op : operations)
        visit([&] (const auto& _) { _.exec(scrambled); }, op);

    return scrambled;
}

auto unscramble(const std::vector<Operation>& operations, std::string scrambled)
{
    auto unscrambled{std::move(scrambled)};

    for (const auto& op : std::ranges::reverse_view(operations))
        visit([&] (const auto& _) { _.undo(unscrambled); }, op);

    return unscrambled;
}

int main()
{
    static const std::regex swap_pos_re{"swap position ([0-9]+) with position ([0-9]+)"};
    static const std::regex swap_letter_re{"swap letter ([a-z]) with letter ([a-z])"};
    static const std::regex rotate_r_re{"rotate right ([0-9]+) steps?"};
    static const std::regex rotate_l_re{"rotate left ([0-9]+) steps?"};
    static const std::regex rotate_pos_re{"rotate based on position of letter ([a-z])"};
    static const std::regex reverse_re{"reverse positions ([0-9]+) through ([0-9]+)"};
    static const std::regex move_re{"move position ([0-9]+) to position ([0-9]+)"};

    std::vector<Operation> operations;

    std::string line;
    while (getline(std::cin, line)) {
        std::smatch matched;

        if (regex_match(line, matched, swap_pos_re)) {
            auto pos1 = strtol(matched[1].str().data(), nullptr, 10);
            auto pos2 = strtol(matched[2].str().data(), nullptr, 10);

            operations.push_back(SwapPos{pos1, pos2});
        } else if (regex_match(line, matched, swap_letter_re)) {
            auto let1 = matched[1].str()[0];
            auto let2 = matched[2].str()[0];

            operations.push_back(SwapLetter{let1, let2});
        } else if (regex_match(line, matched, rotate_r_re)) {
            auto steps = strtol(matched[1].str().data(), nullptr, 10);

            operations.push_back(RotateR{steps});
        } else if (regex_match(line, matched, rotate_l_re)) {
            auto steps = strtol(matched[1].str().data(), nullptr, 10);

            operations.push_back(RotateL{steps});
        } else if (regex_match(line, matched, rotate_pos_re)) {
            auto let = matched[1].str()[0];

            operations.push_back(RotatePos{let});
        } else if (regex_match(line, matched, reverse_re)) {
            auto pos1 = strtol(matched[1].str().data(), nullptr, 10);
            auto pos2 = strtol(matched[2].str().data(), nullptr, 10);

            operations.push_back(Reverse{pos1, pos2});
        } else if (regex_match(line, matched, move_re)) {
            auto pos1 = strtol(matched[1].str().data(), nullptr, 10);
            auto pos2 = strtol(matched[2].str().data(), nullptr, 10);

            operations.push_back(Move{pos1, pos2});
        }
    }

    std::cout << scramble(operations, "abcdefgh") << "\n";
    std::cout << unscramble(operations, "fbgdceah") << "\n";

    return 0;
}

