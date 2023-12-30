#pragma once

#include <array>
#include <functional>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

template <long NumRegs>
struct AsmComputer {
private:
    using Regs = std::array<long, NumRegs>;

    bool is_reg(const std::string& s) const
    {
        return s.size() == 1 && ('a' <= s[0] && s[0] <= 'a' + NumRegs);
    }

    static long to_reg(char r)
    {
        return r - 'a';
    }

    static long to_reg(const std::string& s)
    {
        return to_reg(s[0]);
    }

    static long to_val(const std::string& s)
    {
        return strtol(s.data(), nullptr, 10);
    }

    struct Add {
        bool y_is_reg;
        long reg_x;
        long y;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Add)];
            ac.regs[reg_x] += y_is_reg ? ac.regs[y] : y;
            ++ac.ip;
            return true;
        }
    };

    struct Sub {
        bool y_is_reg;
        long reg_x;
        long y;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Sub)];
            ac.regs[reg_x] -= y_is_reg ? ac.regs[y] : y;
            ++ac.ip;
            return true;
        }
    };

    struct Mul {
        bool y_is_reg;
        long reg_x;
        long y;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Mul)];
            ac.regs[reg_x] *= y_is_reg ? ac.regs[y] : y;
            ++ac.ip;
            return true;
        }
    };

    struct Mod {
        bool y_is_reg;
        long reg_x;
        long y;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Mod)];
            ac.regs[reg_x] %= y_is_reg ? ac.regs[y] : y;
            ++ac.ip;
            return true;
        }
    };

    struct Set {
        bool y_is_reg;
        long reg_x;
        long y;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Set)];
            ac.regs[reg_x] = y_is_reg ? ac.regs[y] : y;
            ++ac.ip;
            return true;
        }
    };

    struct Jgz {
        bool x_is_reg;
        bool y_is_reg;
        long x;
        long y;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Jgz)];
            auto val_x = x_is_reg ? ac.regs[x] : x;
            auto val_y = y_is_reg ? ac.regs[y] : y;
            if (val_x > 0)
                ac.ip += val_y;
            else
                ++ac.ip;
            return true;
        }
    };

    struct Jnz {
        bool x_is_reg;
        bool y_is_reg;
        long x;
        long y;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Jnz)];
            auto val_x = x_is_reg ? ac.regs[x] : x;
            auto val_y = y_is_reg ? ac.regs[y] : y;
            if (val_x != 0)
                ac.ip += val_y;
            else
                ++ac.ip;
            return true;
        }
    };

    struct Snd {
        bool x_is_reg;
        long x;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Snd)];
            ac.outputs.push(x_is_reg ? ac.regs[x] : x);
            ++ac.ip;
            return true;
        }
    };

    struct Rcv {
        long reg_x;

        bool exec(AsmComputer& ac) const
        {
            ++ac.stats[static_cast<int>(Op::Rcv)];
            if (ac.inputs.empty())
                return false;
            ac.regs[reg_x] = ac.inputs.front();
            ac.inputs.pop();
            ++ac.ip;
            return true;
        }
    };

    using Instr = std::variant<Add, Sub, Mul, Mod, Set, Jgz, Jnz, Snd, Rcv>;

    std::optional<Instr> decode_instr(const std::string& instr) const
    {
        std::stringstream ss(instr);

        std::string op, x, y;
        ss >> op;

        if (op == "add") {
            ss >> x >> y;
            auto y_is_reg = is_reg(y);
            return Add{y_is_reg, to_reg(x), y_is_reg ? to_reg(y) : to_val(y)};
        } else if (op == "sub") {
            ss >> x >> y;
            auto y_is_reg = is_reg(y);
            return Sub{y_is_reg, to_reg(x), y_is_reg ? to_reg(y) : to_val(y)};
        } else if (op == "mul") {
            ss >> x >> y;
            auto y_is_reg = is_reg(y);
            return Mul{y_is_reg, to_reg(x), y_is_reg ? to_reg(y) : to_val(y)};
        } else if (op == "mod") {
            ss >> x >> y;
            auto y_is_reg = is_reg(y);
            return Mod{y_is_reg, to_reg(x), y_is_reg ? to_reg(y) : to_val(y)};
        } else if (op == "set") {
            ss >> x >> y;
            auto y_is_reg = is_reg(y);
            return Set{y_is_reg, to_reg(x), y_is_reg ? to_reg(y) : to_val(y)};
        } else if (op == "jgz") {
            ss >> x >> y;
            auto x_is_reg = is_reg(x);
            auto y_is_reg = is_reg(y);
            return Jgz{x_is_reg, y_is_reg,
                x_is_reg ? to_reg(x) : to_val(x),
                y_is_reg ? to_reg(y) : to_val(y)};
        } else if (op == "jnz") {
            ss >> x >> y;
            auto x_is_reg = is_reg(x);
            auto y_is_reg = is_reg(y);
            return Jnz{x_is_reg, y_is_reg,
                x_is_reg ? to_reg(x) : to_val(x),
                y_is_reg ? to_reg(y) : to_val(y)};
        } else if (op == "snd") {
            ss >> x;
            auto x_is_reg = is_reg(x);
            return Snd{x_is_reg, x_is_reg ? to_reg(x) : to_val(x)};
        } else if (op == "rcv") {
            ss >> x;
            return Rcv{to_reg(x)};
        }

        return std::nullopt;
    }

    enum class Status {
        Ok,
        Halted,
    };

    enum class Op {
        Add,
        Sub,
        Mul,
        Mod,
        Set,
        Jgz,
        Jnz,
        Snd,
        Rcv,
        N
    };

    unsigned long ip{0};
    Regs regs{};
    std::vector<Instr> prog;
    std::queue<long> inputs;
    std::queue<long> outputs;
    Status status{Status::Ok};
    std::array<long, static_cast<int>(Op::N)> stats{};

public:
    AsmComputer(const std::vector<std::string>& instructions)
    {
        for (const auto& instr : instructions) {
            auto decoded_instr = decode_instr(instr);
            if (decoded_instr.has_value())
                prog.push_back(std::move(decoded_instr.value()));
        }
    }

    void reset()
    {
        ip = 0;
        regs.fill(0);
        while (!inputs.empty())
            inputs.pop();
        while (!outputs.empty())
            outputs.pop();
        status = Status::Ok;
        stats.fill(0);
    }

    [[nodiscard]] auto get_ip() const
    {
        return ip;
    }

    [[nodiscard]] auto num_regs() const
    {
        return static_cast<long>(regs.size());
    }

    [[nodiscard]] long get_reg(char reg) const
    {
        return regs[to_reg(reg)];
    }

    void set_reg(char reg, long val)
    {
        regs[to_reg(reg)] = val;
    }

    [[nodiscard]] bool input_empty() const
    {
        return inputs.empty();
    }

    void append_input(long val)
    {
        inputs.push(val);
    }

    [[nodiscard]] bool output_empty() const
    {
        return outputs.empty();
    }

    [[nodiscard]] auto output_size() const
    {
        return outputs.size();
    }

    std::optional<long> extract_first_output()
    {
        if (outputs.empty())
            return std::nullopt;

        auto val = outputs.front();
        outputs.pop();

        return val;
    }

    [[nodiscard]] std::optional<long> get_last_output() const
    {
        if (outputs.empty())
            return std::nullopt;

        return outputs.back();
    }

    [[nodiscard]] bool halted() const
    {
        return status == Status::Halted;
    }

    // returns true if suspended (waiting for input), false if halted
    bool run()
    {
        if (status != Status::Ok)
            return false;

        while (ip < prog.size())
            if (!std::visit([&] (const auto& _) { return _.exec(*this); }, prog[ip]))
                return true;

        status = Status::Halted;
        return false;
    }

    // returns true if suspended (waiting for input or stopped), false if halted
    bool run_until(const std::function<bool()>& stop_fn)
    {
        if (status != Status::Ok)
            return false;

        while (ip < prog.size()) {
            if (stop_fn())
                return true;

            if (!std::visit([&] (const auto& _) { return _.exec(*this); }, prog[ip]))
                return true;
        }

        status = Status::Halted;
        return false;
    }

    [[nodiscard]] long get_stats(const std::string& op)
    {
        if (op == "add")
            return stats[static_cast<int>(Op::Add)];
        else if (op == "sub")
            return stats[static_cast<int>(Op::Sub)];
        else if (op == "mul")
            return stats[static_cast<int>(Op::Mul)];
        else if (op == "mod")
            return stats[static_cast<int>(Op::Mod)];
        else if (op == "set")
            return stats[static_cast<int>(Op::Set)];
        else if (op == "jgz")
            return stats[static_cast<int>(Op::Jgz)];
        else if (op == "jnz")
            return stats[static_cast<int>(Op::Jnz)];
        else if (op == "snd")
            return stats[static_cast<int>(Op::Snd)];
        else if (op == "rcv")
            return stats[static_cast<int>(Op::Rcv)];
        else
            return 0;
    }
};

