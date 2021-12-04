#include <bits/stdc++.h>

using namespace std;

const auto Marked = -1;

using Table = vector<vector<long>>;

void mark(Table& t, long x)
{
    for (auto& row : t)
        for (auto& val : row)
            if (val == x)
                val = Marked;
}

auto is_win(const Table& t)
{
    for (const auto& row : t)
        if (all_of(begin(row), end(row), [](auto x) { return x == Marked; }))
            return true;

    for (auto j = 0lu; j < t[0].size(); ++j) {
        auto win = true;

        for (const auto& row : t)
            if (row[j] != Marked) {
                win = false;
                break;
            }

        if (win)
            return true;
    }

    return false;
}

auto sum(const Table& t)
{
    auto sum = 0l;

    for (const auto& row : t)
        for (auto val : row)
            if (val != Marked)
                sum += val;

    return sum;
}

auto solve1(const vector<long>& seq, vector<Table> tables)
{
    for (auto x : seq)
        for (auto& t : tables) {
            mark(t, x);

            if (is_win(t))
                return sum(t) * x;
        }

    return -1l;
}

auto solve2(const vector<long>& seq, vector<Table> tables)
{
    for (auto x : seq)
        for (auto it = begin(tables); it != end(tables);) {
            auto& t = *it;

            mark(t, x);

            if (is_win(t)) {
                if (tables.size() == 1)
                    return sum(t) * x;
                else
                    it = tables.erase(it);
            } else {
                ++it;
            }
        }

    return -1l;
}

int main()
{
    auto seq = []() {
        string line;
        getline(cin, line);

        vector<long> seq;

        stringstream ss{line};
        string s;
        while (getline(ss, s, ','))
            seq.push_back(strtol(s.data(), nullptr, 10));

        return seq;
    }();

    auto tables = []() {
        static const auto NumRows = 5;

        vector<Table> tables;

        string line;
        while (getline(cin, line)) {
            if (line.empty())
                continue;

            static Table t;

            vector<long> row;

            stringstream ss{line};
            long x;
            while (ss >> x)
                row.push_back(x);

            t.push_back(move(row));

            if (t.size() == NumRows) {
                tables.push_back(move(t));
                t.clear();
            }
        }

        return tables;
    }();

    cout << solve1(seq, tables) << '\n';
    cout << solve2(seq, tables) << '\n';

    return 0;
}

