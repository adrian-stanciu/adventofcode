#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Operation {
    enum class Type {
        Spin,
        Exchange,
        Partener,
        Nop,
    };

    Operation(const std::string& move)
    {
        std::stringstream ss(move);

        char op_code;
        ss >> op_code;

        switch (op_code) {
        case 's':
            {
                type = Type::Spin;

                std::string op_param1;
                ss >> op_param1;
                param1 = atoi(op_param1.data());
            }
            break;
        case 'x':
            {
                type = Type::Exchange;

                std::string op_param1;
                getline(ss, op_param1, '/');
                param1 = atoi(op_param1.data());

                std::string op_param2;
                ss >> op_param2;
                param2 = atoi(op_param2.data());
            }
            break;
        case 'p':
            {
                type = Type::Partener;

                ss >> param1;
                ss.ignore(); // skip '/'
                ss >> param2;
            }
            break;
        }
    }

    void execute(std::string& s) const
    {
        switch (type) {
        case Type::Spin:
            {
                const size_t d = s.size() - param1;
                reverse(begin(s), begin(s) + d);
                reverse(begin(s) + d, end(s));
                reverse(begin(s), end(s));
            }
            break;
        case Type::Exchange:
            {
                iter_swap(begin(s) + param1, begin(s) + param2);
            }
            break;
        case Type::Partener:
            {
                auto pos1 = find(begin(s), end(s), param1);
                auto pos2 = find(begin(s), end(s), param2);
                iter_swap(pos1, pos2);
            }
            break;
        case Type::Nop:
            break;
        }
    }

    [[nodiscard]] Type get_type() const
    {
        return type;
    }

private:
    Type type{Type::Nop};
    char param1{-1};
    char param2{-1};
};

auto read_moves()
{
    std::vector<std::string> moves;

    std::string line;
    getline(std::cin, line);
    std::stringstream ss(line);

    std::string move;
    while (getline(ss, move, ','))
        moves.push_back(std::move(move));

    return moves;
}

auto decode_moves(const std::vector<std::string>& moves)
{
    std::vector<Operation> operations;
    operations.reserve(moves.size());

    for (const auto& move : moves)
        operations.emplace_back(move);

    return operations;
}

void execute_operations(std::string& s1, std::string& s2,
    const std::vector<Operation>& operations)
{
    // consider two permutation types:
    // 1. Spin + Exchange: permute positions (indexes)
    // 2. Partener: permute the alphabet (letters)
    for (auto& op : operations)
        if (op.get_type() == Operation::Type::Partener)
            op.execute(s2);
        else
            op.execute(s1);
}

std::string prod_perm(const std::string& s1, const std::string& s2)
{
    std::string p;
    p.reserve(s1.size());

    for (auto c : s1)
        p.push_back(s2[c - 'a']);

    return p;
}

std::string power_perm(const std::string& s, size_t power)
{
    std::string res("abcdefghijklmnop");
    std::string p(s);

    while (power) {
        if (power % 2) {
            --power;
            std::string pp = prod_perm(res, p);
            res = std::move(pp);
        }

        std::string pp = prod_perm(p, p);
        p = std::move(pp);

        power >>= 1;
    }

    return res;
}

std::string prod_perm(const std::string& s1, const std::string& s2, size_t iters)
{
    auto p1 = power_perm(s1, iters);
    auto p2 = power_perm(s2, iters);

    return prod_perm(p1, p2);
}

int main()
{
    auto moves = read_moves();

    auto operations = decode_moves(moves);

    const std::string s("abcdefghijklmnop");
    std::string perm1(s);
    std::string perm2(s);
    execute_operations(perm1, perm2, operations);

    std::cout << prod_perm(perm1, perm2) << "\n";
    std::cout << prod_perm(perm1, perm2, 1000000000) << "\n";

    return 0;
}

