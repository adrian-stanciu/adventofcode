#include <algorithm>
#include <charconv>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <variant>

int output_of(const std::string& wire);

struct Value {
    int in;

    [[nodiscard]] int output() const
    {
        return in;
    }
};

struct Wire {
    std::string in;

    [[nodiscard]] int output() const
    {
        return output_of(in);
    }
};

struct Input {
    std::variant<Value, Wire> in;

    Input(const std::string& s)
    {
        int n;
        if (auto [_, ec] = std::from_chars(s.data(), s.data() + s.size(), n); ec == std::errc())
            in = Value{n};
        else
            in = Wire{s};
    }

    Input(int n)
    {
        in = Value{n};
    }

    [[nodiscard]] int output() const
    {
        return std::visit([] (const auto& _) { return _.output(); }, in);
    }
};

struct AndGate {
    Input in1;
    Input in2;

    [[nodiscard]] int output() const
    {
        return in1.output() & in2.output();
    }
};

struct OrGate {
    Input in1;
    Input in2;

    [[nodiscard]] int output() const
    {
        return in1.output() | in2.output();
    }
};

struct LShiftGate {
    Input in1;
    Input in2;

    [[nodiscard]] int output() const
    {
        return in1.output() << in2.output();
    }
};

struct RShiftGate {
    Input in1;
    Input in2;

    [[nodiscard]] int output() const
    {
        return in1.output() >> in2.output();
    }
};

struct NotGate {
    Input in;

    [[nodiscard]] int output() const
    {
        return ~in.output();
    }
};

using WireInput = std::variant<Input, AndGate, OrGate, LShiftGate, RShiftGate, NotGate>;
using WireInputMap = std::unordered_map<std::string, WireInput>;

std::optional<std::pair<std::string, WireInput>> parse(const std::string& line)
{
    static const std::regex input_re{"([a-z0-9]+) -> ([a-z]+)"};
    static const std::regex and_gate_re{"([a-z0-9]+) AND ([a-z0-9]+) -> ([a-z]+)"};
    static const std::regex or_gate_re{"([a-z0-9]+) OR ([a-z0-9]+) -> ([a-z]+)"};
    static const std::regex lshift_gate_re{"([a-z0-9]+) LSHIFT ([a-z0-9]+) -> ([a-z]+)"};
    static const std::regex rshift_gate_re{"([a-z0-9]+) RSHIFT ([a-z0-9]+) -> ([a-z]+)"};
    static const std::regex not_gate_re{"NOT ([a-z0-9]+) -> ([a-z]+)"};

    std::smatch matched;
    if (regex_match(line, matched, input_re)) {
        auto in = matched[1].str();
        auto out = matched[2].str();
        return std::make_pair(out, Input{in});
    } else if (regex_match(line, matched, and_gate_re)) {
        auto in1 = matched[1].str();
        auto in2 = matched[2].str();
        auto out = matched[3].str();
        return std::make_pair(out, AndGate{in1, in2});
    } else if (regex_match(line, matched, or_gate_re)) {
        auto in1 = matched[1].str();
        auto in2 = matched[2].str();
        auto out = matched[3].str();
        return std::make_pair(out, OrGate{in1, in2});
    } else if (regex_match(line, matched, lshift_gate_re)) {
        auto in1 = matched[1].str();
        auto in2 = matched[2].str();
        auto out = matched[3].str();
        return std::make_pair(out, LShiftGate{in1, in2});
    } else if (regex_match(line, matched, rshift_gate_re)) {
        auto in1 = matched[1].str();
        auto in2 = matched[2].str();
        auto out = matched[3].str();
        return std::make_pair(out, RShiftGate{in1, in2});
    } else if (regex_match(line, matched, not_gate_re)) {
        auto in = matched[1].str();
        auto out = matched[2].str();
        return std::make_pair(out, NotGate{in});
    }

    return std::nullopt;
}

static WireInputMap wire_inputs;
static std::unordered_map<std::string, int> wire_outputs;

int output_of(const std::string& wire)
{
    if (wire_outputs.find(wire) != wire_outputs.end())
        return wire_outputs[wire];

    auto output = std::visit([] (const auto& _) { return _.output(); }, wire_inputs.at(wire));

    wire_outputs[wire] = output;

    return output;
}

int main()
{
    std::string line;
    while (getline(std::cin, line)) {
        auto parsed = parse(line);
        if (parsed.has_value()) {
            auto [wire, input] = parsed.value();
            wire_inputs.try_emplace(std::move(wire), std::move(input));
        }
    }

    auto out = output_of("a");
    std::cout << out << "\n";

    wire_outputs.clear();
    wire_inputs.at("b") = Input{out};
    std::cout << output_of("a") << "\n";

    return 0;
}

