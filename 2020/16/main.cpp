#include <bits/stdc++.h>

using namespace std;

auto to_num(const string& s)
{
    return strtol(s.data(), nullptr, 10);
}

struct Rule {
    string name;
    vector<pair<int, int>> intervals;

    Rule(const string& s)
    {
        static const regex r{R"((.+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+))"};
        smatch matched;
        regex_match(s, matched, r);
        name = matched[1].str();
        intervals.emplace_back(to_num(matched[2].str()), to_num(matched[3].str()));
        intervals.emplace_back(to_num(matched[4].str()), to_num(matched[5].str()));
    }

    bool match(int val) const
    {
        for (auto [lo, hi] : intervals)
            if (val >= lo && val <= hi)
                return true;
        return false;
    }

    bool match(const vector<int>& values) const
    {
        for (auto val : values)
            if (!match(val))
                return false;
        return true;
    }
};

struct Ticket {
    vector<int> values;

    Ticket(const string& s)
    {
        stringstream ss{s};

        string line;
        while (getline(ss, line, ','))
            values.push_back(to_num(line));
    }
};

auto read_rules()
{
    vector<Rule> rules;

    string line;
    while (getline(cin, line) && !line.empty())
        rules.emplace_back(line);

    return rules;
}

auto read_my_ticket()
{
    string line;
    getline(cin, line); // skip "your ticket" line
    getline(cin, line);
    return Ticket{line};
}

auto read_nearby_tickets(const vector<Rule>& rules)
{
    auto check = [&] (const auto& ticket)
    {
        auto is_valid = [&] (auto val) {
            for (const auto& r : rules)
                if (r.match(val))
                    return true;
            return false;
        };

        auto ok = true;
        auto err = 0;
        for (auto val : ticket.values)
            if (!is_valid(val)) {
                ok = false;
                err += val;
            }
        return make_pair(ok, err);
    };

    auto err_sum = 0;
    vector<Ticket> tickets;

    string line;
    getline(cin, line); // skip empty line
    getline(cin, line); // skip "nearby tickets" line
    while (getline(cin, line) && !line.empty()) {
        tickets.emplace_back(line);

        auto [ok, err] = check(tickets.back());
        if (!ok) {
            err_sum += err;
            tickets.erase(prev(end(tickets)));
        }
    }

    return make_pair(err_sum, tickets);
}

auto is_sol(const string& s)
{
    return s.substr(0, s.find_first_of(' ')) == "departure";
}

auto solve(vector<Rule>& rules, const Ticket& my_ticket, const vector<Ticket>& tickets)
{
    struct Field {
        int id;
        vector<int> values;
    };

    vector<Field> fields(my_ticket.values.size());
    for (size_t i = 0; i < fields.size(); ++i) {
        fields[i].id = i;
        for (const auto& t : tickets)
            fields[i].values.push_back(t.values[i]);
    }

    auto res = 1l;

    while (!fields.empty())
        for (size_t i = 0; i < fields.size(); ++i) {
            vector<int> rules_matched;
            for (size_t j = 0; j < rules.size(); ++j)
                if (rules[j].match(fields[i].values))
                    rules_matched.push_back(j);

            if (rules_matched.size() == 1) {
                if (is_sol(rules[rules_matched[0]].name))
                    res *= my_ticket.values[fields[i].id];

                fields.erase(begin(fields) + i);
                rules.erase(begin(rules) + rules_matched[0]);
                break;
            }
        }

    return res;
}

int main()
{
    auto rules = read_rules();
    auto my_ticket = read_my_ticket();
    auto [err_sum, tickets] = read_nearby_tickets(rules);

    cout << err_sum << '\n';
    cout << solve(rules, my_ticket, tickets) << '\n';

    return 0;
}

