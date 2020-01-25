#pragma once

#include <array>
#include <optional>
#include <queue>
#include <regex>
#include <unordered_map>
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

    struct CpyInstr {
        bool src_is_reg;
        bool dst_is_reg;
        long src;
        long dst;

        void exec(AsmComputer& ac) const
        {
            if (dst_is_reg)
                ac.regs[dst] = src_is_reg ? ac.regs[src] : src;
            ++ac.ip;
        }
    };

    struct IncInstr {
        bool trg_is_reg;
        long trg;

        void exec(AsmComputer& ac) const
        {
            if (trg_is_reg)
                ++ac.regs[trg];
            ++ac.ip;
        }
    };

    struct DecInstr {
        bool trg_is_reg;
        long trg;

        void exec(AsmComputer& ac) const
        {
            if (trg_is_reg)
                --ac.regs[trg];
            ++ac.ip;
        }
    };

    struct JnzInstr {
        bool cmp_is_reg;
        bool jmp_is_reg;
        long cmp;
        long jmp;

        void exec(AsmComputer& ac) const
        {
            auto cmp_val = cmp_is_reg ? ac.regs[cmp] : cmp;
            auto jmp_val = jmp_is_reg ? ac.regs[jmp] : jmp;
            if (cmp_val)
                ac.ip += jmp_val;
            else
                ++ac.ip;
        }
    };

    struct OutInstr {
        bool out_is_reg;
        long out;

        void exec(AsmComputer& ac) const
        {
            ac.outputs.push(out_is_reg ? ac.regs[out] : out);
            ++ac.ip;
        }
    };

    struct TglInstr {
        bool off_is_reg;
        long off;

        void exec(AsmComputer& ac) const
        {
            static const std::unordered_map<std::string, std::string> tgl_map = {
                {"inc", "dec"},
                {"dec", "inc"},
                {"tgl", "inc"},
                {"jnz", "cpy"},
                {"cpy", "jnz"},
            };

            auto off_val = off_is_reg ? ac.regs[off] : off;
            auto i = ac.ip + off_val;

            if (i >= 0 && i < ac.instructions.size())
                ac.instructions[i].replace(0, 3, tgl_map.at(ac.instructions[i].substr(0, 3)));

            ++ac.ip;
        }
    };

    using Instr = std::variant<CpyInstr, IncInstr, DecInstr, JnzInstr, OutInstr>;

    auto parse_param(const std::string& param_str) const
    {
        auto param_is_reg = is_reg(param_str);
        auto param = param_is_reg ? to_reg(param_str) : strtol(param_str.data(), nullptr, 10);
        return std::make_pair(param_is_reg, param);
    }

    std::optional<Instr> decode_instr(const std::string& instr) const
    {
        static const std::regex cpy_re{"cpy (.*) (.*)"};
        static const std::regex inc_re{"inc (.*)"};
        static const std::regex dec_re{"dec (.*)"};
        static const std::regex jnz_re{"jnz (.*) (.*)"};
        static const std::regex out_re{"out (.*)"};

        std::smatch matched;
        if (regex_match(instr, matched, cpy_re)) {
            auto [src_is_reg, src] = parse_param(matched[1].str());
            auto [dst_is_reg, dst] = parse_param(matched[2].str());
            return CpyInstr{src_is_reg, dst_is_reg, src, dst};
        } else if (regex_match(instr, matched, inc_re)) {
            auto [trg_is_reg, trg] = parse_param(matched[1].str());
            return IncInstr{trg_is_reg, trg};
        } else if (regex_match(instr, matched, dec_re)) {
            auto [trg_is_reg, trg] = parse_param(matched[1].str());
            return DecInstr{trg_is_reg, trg};
        } else if (regex_match(instr, matched, jnz_re)) {
            auto [cmp_is_reg, cmp] = parse_param(matched[1].str());
            auto [jmp_is_reg, jmp] = parse_param(matched[2].str());
            return JnzInstr{cmp_is_reg, jmp_is_reg, cmp, jmp};
        } else if (regex_match(instr, matched, out_re)) {
            auto [out_is_reg, out] = parse_param(matched[1].str());
            return OutInstr{out_is_reg, out};
        }

        return std::nullopt;
    }

    void decode_and_exec_instr(const std::string& instr)
    {
        static const std::regex cpy_re{"cpy (.*) (.*)"};
        static const std::regex inc_re{"inc (.*)"};
        static const std::regex dec_re{"dec (.*)"};
        static const std::regex jnz_re{"jnz (.*) (.*)"};
        static const std::regex out_re{"out (.*)"};
        static const std::regex tgl_re{"tgl (.*)"};

        std::smatch matched;
        if (regex_match(instr, matched, cpy_re)) {
            auto [src_is_reg, src] = parse_param(matched[1].str());
            auto [dst_is_reg, dst] = parse_param(matched[2].str());
            CpyInstr{src_is_reg, dst_is_reg, src, dst}.exec(*this);
        } else if (regex_match(instr, matched, inc_re)) {
            auto [trg_is_reg, trg] = parse_param(matched[1].str());
            IncInstr{trg_is_reg, trg}.exec(*this);
        } else if (regex_match(instr, matched, dec_re)) {
            auto [trg_is_reg, trg] = parse_param(matched[1].str());
            DecInstr{trg_is_reg, trg}.exec(*this);
        } else if (regex_match(instr, matched, jnz_re)) {
            auto [cmp_is_reg, cmp] = parse_param(matched[1].str());
            auto [jmp_is_reg, jmp] = parse_param(matched[2].str());
            JnzInstr{cmp_is_reg, jmp_is_reg, cmp, jmp}.exec(*this);
        } else if (regex_match(instr, matched, out_re)) {
            auto [out_is_reg, out] = parse_param(matched[1].str());
            OutInstr{out_is_reg, out}.exec(*this);
        } else if (regex_match(instr, matched, tgl_re)) {
            auto [off_is_reg, off] = parse_param(matched[1].str());
            TglInstr{off_is_reg, off}.exec(*this);
        }
    }

    enum class Status {
        Ok,
        Halted,
    };

    unsigned long ip{0};
    Regs regs{};
    std::vector<std::string> instructions;
    std::vector<Instr> prog;
    std::queue<long> outputs;
    Status status{Status::Ok};

public:
    AsmComputer(const std::vector<std::string>& instructions)
    : instructions(instructions)
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
        while (!outputs.empty())
            outputs.pop();
        status = Status::Ok;
    }

    auto num_regs() const
    {
        return static_cast<long>(regs.size());
    }

    long get_reg(char reg) const
    {
        return regs[to_reg(reg)];
    }

    void set_reg(char reg, long val)
    {
        regs[to_reg(reg)] = val;
    }

    bool output_empty() const
    {
        return outputs.empty();
    }

    auto output_size() const
    {
        return outputs.size();
    }

    std::optional<long> extract_first_output()
    {
        if (outputs.empty())
            return {};

        auto val = outputs.front();
        outputs.pop();

        return val;
    }

    std::optional<long> get_last_output() const
    {
        if (outputs.empty())
            return {};

        return outputs.back();
    }

    bool halted() const
    {
        return status == Status::Halted;
    }

    void run()
    {
        if (status != Status::Ok)
            return;

        while (ip < prog.size())
            std::visit([&] (const auto& _) { return _.exec(*this); }, prog[ip]);

        status = Status::Halted;
    }

    // returns true if a step was done, false if not
    bool run_step()
    {
        if (status != Status::Ok)
            return false;

        std::visit([&] (const auto& _) { return _.exec(*this); }, prog[ip]);

        if (ip >= prog.size())
            status = Status::Halted;

        return true;
    }

    void run_with_tgl()
    {
        if (status != Status::Ok)
            return;

        while (ip < instructions.size())
            decode_and_exec_instr(instructions[ip]);

        status = Status::Halted;
    }
};

