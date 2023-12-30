#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "int_computer.hpp"

struct Chamber {
    std::string name;
    std::vector<std::string> doors;
    std::vector<std::string> items;
};

auto parse_chamber(std::stringstream& ss)
{
    static const std::regex re_name("== (.+) ==");
    static const std::regex re_door("- (.+)");
    static const std::regex re_item("- (.+)");

    Chamber chamber;

    std::string line;
    while (getline(ss, line)) {
        if (line.empty())
            continue;

        std::smatch matched;
        if (regex_match(line, matched, re_name)) {
            chamber.name = matched[1].str();
        } else if (line == "Doors here lead:") {
            std::string door;
            while (getline(ss, door)) {
                std::smatch matched;
                if (!regex_match(door, matched, re_door))
                    break;
                chamber.doors.push_back(matched[1].str());
            }
        } else if (line == "Items here:") {
            std::string item;
            while (getline(ss, item)) {
                std::smatch matched;
                if (!regex_match(item, matched, re_item))
                    break;
                chamber.items.push_back(matched[1].str());
            }
        }
    }

    return chamber;
}

void append_cmd(IntComputer& ic, const std::string& cmd)
{
    for (auto c : cmd)
        ic.append_input(c);
    ic.append_input('\n');
}

void append_item_cmd(IntComputer& ic, const std::string& item, const std::string& cmd)
{
    std::string item_cmd{cmd};
    item_cmd.push_back(' ');
    item_cmd.append(item);

    append_cmd(ic, item_cmd);
}

auto run_with_output(IntComputer& ic)
{
    std::stringstream ss;

    while (ic.run())
        if (ic.output_empty())
            break;
        else
            ss << static_cast<char>(*ic.extract_first_output());

    return ss;
}

void run_without_output(IntComputer& ic)
{
    while (ic.run())
        if (ic.output_empty())
            break;
        else
            ic.extract_first_output();
}

void run_item_cmd(IntComputer& ic, const std::string& item, const std::string& cmd)
{
    append_item_cmd(ic, item, cmd);

    run_without_output(ic);
}

void run_item_cmd(IntComputer& ic, const std::vector<std::string>& items, const std::string& cmd)
{
    for (const auto& item : items)
        append_item_cmd(ic, item, cmd);

    run_without_output(ic);
}

void run_item_cmd(IntComputer& ic, const std::vector<std::string>& items,
    const std::string& cmd, int selected)
{
    for (auto i = 0U; i < items.size(); ++i)
        if ((selected >> i) & 0x1)
            append_item_cmd(ic, items[i], cmd);

    run_without_output(ic);
}

void run_cmd(IntComputer& ic, const std::string& cmd)
{
    append_cmd(ic, cmd);

    run_without_output(ic);
}

void run_cmd(IntComputer& ic, const std::vector<std::string>& cmds)
{
    for (const auto& cmd : cmds)
        append_cmd(ic, cmd);

    run_without_output(ic);
}

auto run_cmd_with_output(IntComputer& ic, const std::string& cmd)
{
    append_cmd(ic, cmd);

    return run_with_output(ic);
}

bool test_item_cmd(IntComputer& ic, const std::string& item, const std::string& cmd)
{
    append_item_cmd(ic, item, cmd);

    std::string res_line{"You "};
    res_line.append(cmd);
    res_line.append(" the ");
    res_line.append(item);
    res_line.push_back('.');

    std::queue<std::string> expected_lines;
    expected_lines.push(std::move(res_line));
    expected_lines.push("Command?");

    std::string line;
    while (ic.run()) {
        if (ic.output_empty())
            break;

        auto c = static_cast<char>(*ic.extract_first_output());

        if (c != '\n') {
            line.push_back(c);
            continue;
        }

        if (!line.empty()) {
            if (line != expected_lines.front())
                return false;

            expected_lines.pop();
            if (expected_lines.empty())
                return true;

            line.clear();
        }
    }

    return false;
}

bool test_item(IntComputer ic, const std::string& item)
{
    return test_item_cmd(ic, item, "take") && test_item_cmd(ic, item, "drop");
}

auto test_items(IntComputer& ic, std::vector<std::string> items,
    std::unordered_set<std::string>& bad_items)
{
    items.erase(remove_if(items.begin(), items.end(),
        [&] (const std::string& item) {
            return bad_items.find(item) != bad_items.end();
        }), items.end());

    std::vector<std::string> good_items;

    for (const auto& item : items)
        if (test_item(ic, item))
            good_items.push_back(item);
        else
            bad_items.insert(item);

    return good_items;
}

void explore(IntComputer& ic, std::vector<std::string>& collected_items,
    std::unordered_set<std::string>& bad_items,
    std::deque<std::string>& path, std::vector<std::string>& path2target)
{
    static const std::unordered_map<std::string, std::string> opposite = {
        {"north", "south"},
        {"south", "north"},
        {"east", "west"},
        {"west", "east"},
    };

    auto ss = run_with_output(ic);
    auto chamber = parse_chamber(ss);

    auto good_items = test_items(ic, chamber.items, bad_items);
    run_item_cmd(ic, good_items, "take");
    move(good_items.begin(), good_items.end(), back_inserter(collected_items));

    if (chamber.name == "Security Checkpoint") {
        for (const auto& door : path)
            path2target.push_back(door);

        // add final door
        if (chamber.doors[0] == opposite.at(path.back()))
            path2target.push_back(chamber.doors[1]);
        else
            path2target.push_back(chamber.doors[0]);
    } else {
        for (auto& door : chamber.doors)
            if (path.empty() || door != opposite.at(path.back())) {
                path.push_back(door);

                append_cmd(ic, door);
                explore(ic, collected_items, bad_items, path, path2target);
            }
    }

    // go back
    if (!path.empty()) {
        run_cmd(ic, opposite.at(path.back()));

        path.pop_back();
    }
}

auto is_iteam_too_heavy(IntComputer ic, const std::string& item, const std::string& final_door)
{
    static const std::string too_heavy{"Droids on this ship are lighter than the detected value"};

    run_item_cmd(ic, item, "take");

    auto ss = run_cmd_with_output(ic, final_door);

    std::string line;
    while (getline(ss, line))
        if (line.find(too_heavy) != std::string::npos)
            return true;

    return false;
}

auto find_candidate_items(IntComputer& ic, const std::vector<std::string>& items,
    const std::string& final_door)
{
    std::vector<std::string> candidate_items;

    for (const auto& item : items)
        if (!is_iteam_too_heavy(ic, item, final_door))
            candidate_items.push_back(item);

    return candidate_items;
}

auto find_sol(IntComputer& ic, const std::vector<std::string>& items, const std::string& final_door)
{
    static const std::regex re_sol{R"("Oh, hello! You should be able to get in by typing ([1-9][0-9]*) on the keypad at the main airlock.")"};

    auto limit = pow(2, items.size());

    for (auto i = 0; i < limit; ++i) {
        run_item_cmd(ic, items, "take", i);

        auto ss = run_cmd_with_output(ic, final_door);

        if (ic.halted()) { // halted => solution was found
            std::string line;
            while (getline(ss, line)) {
                std::smatch matched;
                if (regex_match(line, matched, re_sol))
                    return matched[1].str();
            }
        }

        run_item_cmd(ic, items, "drop", i);
    }

    return std::string();
}

auto solve(const std::vector<long long>& prog)
{
    IntComputer ic(prog);

    std::vector<std::string> collected_items;
    std::unordered_set<std::string> bad_items;
    std::deque<std::string> path;
    std::vector<std::string> path2target;

    // collect all good items and find path to target room
    explore(ic, collected_items, bad_items, path, path2target);

    auto final_door = path2target.back();
    path2target.pop_back();

    // go to "Security Checkpoint" (one room before target) and drop all items
    run_cmd(ic, path2target);
    run_item_cmd(ic, collected_items, "drop");

    // find all not too heavy items
    auto candidate_items = find_candidate_items(ic, collected_items, final_door);

    // try until the right combination of items is found
    return find_sol(ic, candidate_items, final_door);
}

int main()
{
    std::vector<long long> prog;

    std::string line;
    while (getline(std::cin, line, ','))
        prog.push_back(strtoll(line.data(), nullptr, 10));

    std::cout << solve(prog) << "\n";

    return 0;
}

