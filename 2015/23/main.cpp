#include <array>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <variant>
#include <vector>

using Regs = std::array<long, 2>;

struct Computer;

struct Hlf {
    long r;
    void exec(Computer& c) const;
};

struct Tpl {
    long r;
    void exec(Computer& c) const;
};

struct Inc {
    long r;
    void exec(Computer& c) const;
};

struct Jmp {
    long off;
    void exec(Computer& c) const;
};

struct Jie {
    long r;
    long off;
    void exec(Computer& c) const;
};

struct Jio {
    long r;
    long off;
    void exec(Computer& c) const;
};

using Instr = std::variant<Hlf, Tpl, Inc, Jmp, Jie, Jio>;

std::optional<Instr> decode(const std::string& instr)
{
    static const std::regex hlf_re{"hlf ([a-b])"};
    static const std::regex tpl_re{"tpl ([a-b])"};
    static const std::regex inc_re{"inc ([a-b])"};
    static const std::regex jmp_re{"jmp ([+-])([0-9]+)"};
    static const std::regex jie_re{"jie ([a-b]), ([+-])([0-9]+)"};
    static const std::regex jio_re{"jio ([a-b]), ([+-])([0-9]+)"};

    auto to_reg = [] (char r) {
        return r - 'a';
    };

    auto to_num = [] (char sign, const std::string& value) {
        return strtol(value.data(), nullptr, 10) * (sign == '-' ? -1 : 1);
    };

    std::smatch matched;
    if (regex_match(instr, matched, hlf_re))
        return Hlf{to_reg(matched[1].str()[0])};
    else if (regex_match(instr, matched, tpl_re))
        return Tpl{to_reg(matched[1].str()[0])};
    else if (regex_match(instr, matched, inc_re))
        return Inc{to_reg(matched[1].str()[0])};
    else if (regex_match(instr, matched, jmp_re))
        return Jmp{to_num(matched[1].str()[0], matched[2].str())};
    else if (regex_match(instr, matched, jie_re))
        return Jie{to_reg(matched[1].str()[0]), to_num(matched[2].str()[0], matched[3].str())};
    else if (regex_match(instr, matched, jio_re))
        return Jio{to_reg(matched[1].str()[0]), to_num(matched[2].str()[0], matched[3].str())};

    return std::nullopt;
}

struct Computer {
    unsigned long ip{0};
    Regs regs{};
    std::vector<Instr> prog;

    Computer(const std::vector<std::string>& instructions)
    {
        for (const auto& instr : instructions) {
            auto decoded_instr = decode(instr);
            if (decoded_instr.has_value())
                prog.push_back(std::move(decoded_instr.value()));
        }
    }

    void reset()
    {
        ip = 0;
        regs.fill(0);
    }

    auto get_reg(char r) const
    {
        return regs[r - 'a'];
    }

    void set_reg(char r, long value)
    {
        regs[r - 'a'] = value;
    }

    void run()
    {
        while (ip < prog.size())
            visit([&] (const auto& _) { _.exec(*this); }, prog[ip]);
    }
};

void Hlf::exec(Computer& c) const
{
    c.regs[r] /= 2;
    ++c.ip;
}

void Tpl::exec(Computer& c) const
{
    c.regs[r] *= 3;
    ++c.ip;
}

void Inc::exec(Computer& c) const
{
    ++c.regs[r];
    ++c.ip;
}

void Jmp::exec(Computer& c) const
{
    c.ip += off;
}

void Jie::exec(Computer& c) const
{
    if (c.regs[r] % 2 == 0)
        c.ip += off;
    else
        ++c.ip;
}

void Jio::exec(Computer& c) const
{
    if (c.regs[r] == 1)
        c.ip += off;
    else
        ++c.ip;
}

int main()
{
    std::vector<std::string> instructions;

    std::string line;
    while (getline(std::cin, line))
        instructions.push_back(std::move(line));

    Computer c{instructions};
    c.run();
    std::cout << c.get_reg('b') << "\n";

    c.reset();
    c.set_reg('a', 1);
    c.run();
    std::cout << c.get_reg('b') << "\n";

    return 0;
}

