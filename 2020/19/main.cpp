#include <bits/stdc++.h>

using namespace std;

struct CharRule;
struct ListRule;

using Rule = variant<CharRule, ListRule>;

bool match(const string& s, size_t pos, const unordered_map<int, Rule>& rules, stack<int> todo);

struct CharRule {
    char ch;

    [[nodiscard]] bool match(const string& s, size_t pos,
        const unordered_map<int, Rule>& rules, stack<int> todo) const
    {
        return s[pos] == ch && ::match(s, pos + 1, rules, move(todo));
    }
};

struct ListRule {
    vector<vector<int>> or_lists;

    [[nodiscard]] bool match(const string& s, size_t pos,
        const unordered_map<int, Rule>& rules, const stack<int>& todo) const
    {
        for (auto& list : or_lists) {
            auto next_todo{todo};

            for (auto id : list)
                next_todo.push(id);

            if (::match(s, pos, rules, next_todo))
                return true;
        }

        return false;
    }
};

bool match(const string& s, size_t pos, const unordered_map<int, Rule>& rules, stack<int> todo)
{
    if (pos == s.size() && todo.empty())
        return true;
    if (pos == s.size() || todo.empty())
        return false;

    auto id = todo.top();
    todo.pop();

    return visit([&] (const auto& _) { return _.match(s, pos, rules, todo); }, rules.at(id));
}

void add_rule(unordered_map<int, Rule>& rules, const string& line)
{
    static const regex rule_r{"([0-9]+): (.+)"};
    static const regex char_rule_r{R"_("(.)")_"};

    smatch matched;
    regex_match(line, matched, rule_r);
    auto id = strtol(matched[1].str().data(), nullptr, 10);
    auto rule_s = matched[2].str();

    if (regex_match(rule_s, matched, char_rule_r)) {
        rules.emplace(id, CharRule{matched[1].str()[0]});
    } else {
        stringstream or_lists_ss{rule_s};
        vector<vector<int>> or_lists;

        string list_s;
        while (getline(or_lists_ss, list_s, '|')) {
            stringstream list_ss{list_s};
            vector<int> list;

            int id;
            while (list_ss >> id)
                list.push_back(id);

            reverse(begin(list), end(list));
            or_lists.push_back(move(list));
        }

        rules.emplace(id, ListRule{move(or_lists)});
    }
}

auto solve(const unordered_map<int, Rule>& rules, const vector<string>& msgs)
{
    auto match = [&] (const auto& s) {
        stack<int> todo;
        todo.push(0);
        return ::match(s, 0, rules, todo);
    };

    auto cnt = 0;
    for (auto& s : msgs)
        cnt += match(s);
    return cnt;
}

int main()
{
    unordered_map<int, Rule> rules1;
    unordered_map<int, Rule> rules2;
    vector<string> msgs;

    string line;
    while (getline(cin, line) && !line.empty()) {
        add_rule(rules1, line);
        if (line == "8: 42")
            line = "8: 42 | 42 8";
        else if (line == "11: 42 31")
            line = "11: 42 31 | 42 11 31";
        add_rule(rules2, line);
    }
    while (getline(cin, line) && !line.empty())
        msgs.push_back(move(line));

    cout << solve(rules1, msgs) << '\n';
    cout << solve(rules2, msgs) << '\n';

    return 0;
}

