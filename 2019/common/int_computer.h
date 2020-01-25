#pragma once

#include <optional>
#include <queue>
#include <unordered_map>
#include <vector>

struct IntComputer {
private:
    enum OpCode {
        Add = 1,
        Mul = 2,
        Rd = 3,
        Wr = 4,
        Jnz = 5,
        Jz = 6,
        Lt = 7,
        Eq = 8,
        AddToBase = 9,
        Halt = 99,
    };

    enum Mode {
        Adr = 0,
        Imd = 1,
        Rel = 2,
    };

    enum class Status {
        Ok,
        Halted,
    };

    long long ip{0};
    long long base{0};
    std::unordered_map<long long, long long> mem;
    std::queue<long long> inputs;
    std::queue<long long> outputs;
    Status status{Status::Ok};

public:
    IntComputer(const std::vector<long long>& prog)
    {
        for (auto i = 0U; i < prog.size(); ++i)
            mem[i] = prog[i];
    }

    long long get_mem(long long idx)
    {
        return mem[idx];
    }

    void set_mem(long long idx, long long val)
    {
        mem[idx] = val;
    }

    bool input_empty() const
    {
        return inputs.empty();
    }

    void append_input(long long val)
    {
        inputs.push(val);
    }

    bool output_empty() const
    {
        return outputs.empty();
    }

    auto output_size() const
    {
        return outputs.size();
    }

    std::optional<long long> extract_first_output()
    {
        if (outputs.empty())
            return std::nullopt;

        auto val = outputs.front();
        outputs.pop();

        return val;
    }

    std::optional<long long> get_last_output() const
    {
        if (outputs.empty())
            return std::nullopt;

        return outputs.back();
    }

    bool halted() const
    {
        return status == Status::Halted;
    }

    // returns true if suspended (waiting for input or yielding an output),
    // false if halted or on errors (e.g. unknown instruction)
    bool run()
    {
        auto from_mem = [&] (int mode, long long idx) {
            switch (mode) {
                case Mode::Adr:
                    idx = mem[idx];
                    break;
                case Mode::Rel:
                    idx = mem[idx] + base;
                    break;
            }

            return mem[idx];
        };

        auto to_mem = [&] (int mode, long long idx, long long val) {
            switch (mode) {
                case Mode::Adr:
                    mem[mem[idx]] = val;
                    break;
                case Mode::Rel:
                    mem[mem[idx] + base] = val;
                    break;
            }
        };

        if (status != Status::Ok)
            return false;

        while (true) {
            auto instr = mem[ip];

            auto op = instr % 100;

            auto mode1 = (instr / 100) % 10;
            auto mode2 = (instr / 1000) % 10;
            auto mode3 = (instr / 10000) % 10;

            auto p1 = from_mem(mode1, ip + 1);

            if (op == OpCode::Add) {
                auto p2 = from_mem(mode2, ip + 2);

                to_mem(mode3, ip + 3, p1 + p2);

                ip += 4;
            } else if (op == OpCode::Mul) {
                auto p2 = from_mem(mode2, ip + 2);

                to_mem(mode3, ip + 3, p1 * p2);

                ip += 4;
            } else if (op == OpCode::Rd) {
                if (inputs.empty())
                    return true; // wait for input

                to_mem(mode1, ip + 1, inputs.front());

                inputs.pop();

                ip += 2;
            } else if (op == OpCode::Wr) {
                outputs.push(p1);

                ip += 2;

                return true; // output is available
            } else if (op == OpCode::Jnz) {
                auto p2 = from_mem(mode2, ip + 2);

                if (p1)
                    ip = p2;
                else
                    ip += 3;
            } else if (op == OpCode::Jz) {
                auto p2 = from_mem(mode2, ip + 2);

                if (!p1)
                    ip = p2;
                else
                    ip += 3;
            } else if (op == OpCode::Lt) {
                auto p2 = from_mem(mode2, ip + 2);

                if (p1 < p2)
                    to_mem(mode3, ip + 3, 1);
                else
                    to_mem(mode3, ip + 3, 0);

                ip += 4;
            } else if (op == OpCode::Eq) {
                auto p2 = from_mem(mode2, ip + 2);

                if (p1 == p2)
                    to_mem(mode3, ip + 3, 1);
                else
                    to_mem(mode3, ip + 3, 0);

                ip += 4;
            } else if (op == OpCode::AddToBase) {
                base += p1;

                ip += 2;
            } else if (op == OpCode::Halt) {
                status = Status::Halted;
                return false;
            } else {
                // some unknown op => halt
                status = Status::Halted;
                return false;
            }
        }
    }
};

