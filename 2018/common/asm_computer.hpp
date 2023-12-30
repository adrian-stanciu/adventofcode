#pragma once

#include <array>
#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace {
    inline auto addr_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto addi_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.c >= num_regs);
    };

    inline auto mulr_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto muli_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.c >= num_regs);
    };

    inline auto banr_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto bani_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.c >= num_regs);
    };

    inline auto borr_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto bori_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.c >= num_regs);
    };

    inline auto setr_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.c >= num_regs);
    };

    inline auto seti_precond = [](auto num_regs, const auto& instr) {
        return !(instr.c >= num_regs);
    };

    inline auto gtir_precond = [](auto num_regs, const auto& instr) {
        return !(instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto gtri_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.c >= num_regs);
    };

    inline auto gtrr_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto eqir_precond = [](auto num_regs, const auto& instr) {
        return !(instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto eqri_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.c >= num_regs);
    };

    inline auto eqrr_precond = [](auto num_regs, const auto& instr) {
        return !(instr.a >= num_regs || instr.b >= num_regs || instr.c >= num_regs);
    };

    inline auto addr = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] + regs[instr.b];
    };

    inline auto addi = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] + instr.b;
    };

    inline auto mulr = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] * regs[instr.b];
    };

    inline auto muli = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] * instr.b;
    };

    inline auto banr = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] & regs[instr.b];
    };

    inline auto bani = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] & instr.b;
    };

    inline auto borr = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] | regs[instr.b];
    };

    inline auto bori = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a] | instr.b;
    };

    inline auto setr = [](auto& regs, const auto& instr) {
        regs[instr.c] = regs[instr.a];
    };

    inline auto seti = [](auto& regs, const auto& instr) {
        regs[instr.c] = instr.a;
    };

    inline auto gtir = [](auto& regs, const auto& instr) {
        regs[instr.c] = (instr.a > regs[instr.b]) ? 1 : 0;
    };

    inline auto gtri = [](auto& regs, const auto& instr) {
        regs[instr.c] = (regs[instr.a] > instr.b) ? 1 : 0;
    };

    inline auto gtrr = [](auto& regs, const auto& instr) {
        regs[instr.c] = (regs[instr.a] > regs[instr.b]) ? 1 : 0;
    };

    inline auto eqir = [](auto& regs, const auto& instr) {
        regs[instr.c] = (instr.a == regs[instr.b]) ? 1 : 0;
    };

    inline auto eqri = [](auto& regs, const auto& instr) {
        regs[instr.c] = (regs[instr.a] == instr.b) ? 1 : 0;
    };

    inline auto eqrr = [](auto& regs, const auto& instr) {
        regs[instr.c] = (regs[instr.a] == regs[instr.b]) ? 1 : 0;
    };

    template <long NumRegs>
    struct AsmComputer {
    private:
        using Regs = std::array<long, NumRegs>;

        enum class Opcode {
            Addr,
            Addi,
            Mulr,
            Muli,
            Banr,
            Bani,
            Borr,
            Bori,
            Setr,
            Seti,
            Gtir,
            Gtri,
            Gtrr,
            Eqir,
            Eqri,
            Eqrr,
        };

        struct Instr {
            Opcode opcode;
            long a;
            long b;
            long c;
        };

        void exec(const Instr& instr)
        {
            switch (instr.opcode) {
            case Opcode::Addr:
                addr(regs, instr);
                break;
            case Opcode::Addi:
                addi(regs, instr);
                break;
            case Opcode::Mulr:
                mulr(regs, instr);
                break;
            case Opcode::Muli:
                muli(regs, instr);
                break;
            case Opcode::Banr:
                banr(regs, instr);
                break;
            case Opcode::Bani:
                bani(regs, instr);
                break;
            case Opcode::Borr:
                borr(regs, instr);
                break;
            case Opcode::Bori:
                bori(regs, instr);
                break;
            case Opcode::Setr:
                setr(regs, instr);
                break;
            case Opcode::Seti:
                seti(regs, instr);
                break;
            case Opcode::Gtir:
                gtir(regs, instr);
                break;
            case Opcode::Gtri:
                gtri(regs, instr);
                break;
            case Opcode::Gtrr:
                gtrr(regs, instr);
                break;
            case Opcode::Eqir:
                eqir(regs, instr);
                break;
            case Opcode::Eqri:
                eqri(regs, instr);
                break;
            case Opcode::Eqrr:
                eqrr(regs, instr);
                break;
            }
        }

        static auto to_number(const std::string& s)
        {
            return strtol(s.data(), nullptr, 10);
        }

        static std::optional<Instr> decode_instr(const std::string& instr)
        {
            std::stringstream ss(instr);

            std::string op, a, b, c;
            ss >> op >> a >> b >> c;

            std::optional<Opcode> opcode;

            if (op == "addr")
                opcode = Opcode::Addr;
            else if (op == "addi")
                opcode = Opcode::Addi;
            else if (op == "mulr")
                opcode = Opcode::Mulr;
            else if (op == "muli")
                opcode = Opcode::Muli;
            else if (op == "banr")
                opcode = Opcode::Banr;
            else if (op == "bani")
                opcode = Opcode::Bani;
            else if (op == "borr")
                opcode = Opcode::Borr;
            else if (op == "bori")
                opcode = Opcode::Bori;
            else if (op == "setr")
                opcode = Opcode::Setr;
            else if (op == "seti")
                opcode = Opcode::Seti;
            else if (op == "gtir")
                opcode = Opcode::Gtir;
            else if (op == "gtri")
                opcode = Opcode::Gtri;
            else if (op == "gtrr")
                opcode = Opcode::Gtrr;
            else if (op == "eqir")
                opcode = Opcode::Eqir;
            else if (op == "eqri")
                opcode = Opcode::Eqri;
            else if (op == "eqrr")
                opcode = Opcode::Eqrr;

            if (!opcode.has_value())
                return std::nullopt;

            return Instr{opcode.value(), to_number(a), to_number(b), to_number(c)};
        }

        enum class Status {
            Ok,
            Halted,
        };

        Regs regs{};
        long ip_reg;
        std::vector<Instr> prog;
        Status status{Status::Ok};

    public:
        AsmComputer(long ip_reg, const std::vector<std::string>& instructions)
        : ip_reg(ip_reg)
        {
            for (const auto& instr : instructions) {
                auto decoded_instr = decode_instr(instr);
                if (decoded_instr.has_value())
                    prog.push_back(std::move(decoded_instr.value()));
            }
        }

        void reset()
        {
            regs.fill(0);
            status = Status::Ok;
        }

        [[nodiscard]] auto get_ip() const
        {
            return regs[ip_reg];
        }

        [[nodiscard]] auto num_regs() const
        {
            return static_cast<long>(regs.size());
        }

        [[nodiscard]] long get_reg(long reg) const
        {
            return regs[reg];
        }

        void set_reg(long reg, long val)
        {
            regs[reg] = val;
        }

        [[nodiscard]] bool halted() const
        {
            return status == Status::Halted;
        }

        void run()
        {
            if (status != Status::Ok)
                return;

            while (regs[ip_reg] < static_cast<long>(prog.size())) {
                exec(prog[regs[ip_reg]]);
                ++regs[ip_reg];
            }

            status = Status::Halted;
        }

        // returns true if suspended (stopped), false if halted
        bool run_until(const std::function<bool()>& stop_fn)
        {
            while (regs[ip_reg] < static_cast<long>(prog.size())) {
                if (stop_fn())
                    return true;

                exec(prog[regs[ip_reg]]);
                ++regs[ip_reg];
            }

            status = Status::Halted;
            return false;
        }
    };
}

