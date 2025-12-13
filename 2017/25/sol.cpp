#include <array>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

struct State {
    char name;
    std::array<int, 2> outputs;
    std::array<int, 2> moves;
    std::array<char, 2> next_states;
};

auto read_string(const std::regex& re)
{
    std::string line;
    getline(std::cin, line);

    std::smatch matched;
    regex_match(line, matched, re);

    return matched[1].str();
}

auto read_long(const std::regex& re)
{
    return strtol(read_string(re).data(), nullptr, 10);
}

auto read_state()
{
    State s;

    static const std::regex name_re{"In state ([A-Z]):"};
    s.name = read_string(name_re)[0];

    for (size_t i = 0; i < 2; ++i) {
        static const std::regex curr_re{R"(\s+If the current value is ([0-1]):)"};
        auto idx = read_long(curr_re);

        static const std::regex out_re{R"(\s+- Write the value ([0-1]).)"};
        s.outputs[idx] = read_long(out_re);

        static const std::regex dir_re{R"(\s+- Move one slot to the (.+).)"};
        if (read_string(dir_re) == "left")
            s.moves[idx] = -1;
        else
            s.moves[idx] = 1;

        static const std::regex next_state_re{R"(\s+- Continue with state ([A-Z]).)"};
        s.next_states[idx] = read_string(next_state_re)[0];
    }

    return s;
}

auto read_states()
{
    std::unordered_map<char, State> states;

    std::string line;
    while (getline(std::cin, line)) { // read empty line
        auto s = read_state();
        states[s.name] = s;
    }

    return states;
}

auto run(long iters, char& curr_state, const std::unordered_map<char, State>& states)
{
    std::unordered_map<long, int> tape;
    long curr_pos = 0;

    for (long i = 0; i < iters; ++i) {
        auto& state = states.find(curr_state)->second;
        auto& val = tape[curr_pos];
        const int idx = val;

        val = state.outputs[idx];
        curr_pos += state.moves[idx];
        curr_state = state.next_states[idx];
    }

    return tape;
}

long count_1s(const std::unordered_map<long, int>& tape)
{
    long count = 0;

    for (auto& elem : tape)
        count += elem.second;

    return count;
}

int main()
{
    auto init_state = read_string(std::regex{"Begin in state ([A-Z])."})[0];
    auto iters  = read_long(std::regex{"Perform a diagnostic checksum after ([1-9][0-9]*) steps."});
    auto states = read_states();

    auto tape = run(iters, init_state, states);

    std::cout << count_1s(tape) << "\n";

    return 0;
}

