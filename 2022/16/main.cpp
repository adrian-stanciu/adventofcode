#include <bits/stdc++.h>

using namespace std;

auto trim(string_view sv)
{
    while (isspace(sv.front()))
        sv.remove_prefix(1);
    while (isspace(sv.back()))
        sv.remove_suffix(1);
    return sv;
}

auto split(string_view sv, char sep)
{
    vector<string_view> split_sv;
    while (true) {
        auto to = sv.find_first_of(sep);
        if (to == sv.npos) {
            split_sv.push_back(trim(sv));
            return split_sv;
        } else {
            split_sv.push_back(trim(sv.substr(0, to)));
            sv.remove_prefix(to + 1);
        }
    }
}

struct Valve {
    int id;
    int flow;
    vector<int> adj_valve_ids;
};

auto comp_dist(const vector<Valve>& valves)
{
    const auto sz = ssize(valves);

    vector<vector<int>> dist(sz, vector<int>(sz, numeric_limits<int>::max() / 2));
    for (const auto& valve : valves) {
        dist[valve.id][valve.id] = 0;
        for (auto adj_valve_id : valve.adj_valve_ids)
            dist[valve.id][adj_valve_id] = 1;
    }

    for (auto k = 0; k < sz; ++k)
        for (auto i = 0; i < sz; ++i)
            for (auto j = 0; j < sz; ++j)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    return dist;
}

const auto BitsetSize = 64;
using Bitset = bitset<BitsetSize>;

void search1(const vector<Valve>& valves, int curr_id, const vector<vector<int>>& dist,
    Bitset& todo, int t, int t_lim, int score, int& max_score)
{
    max_score = max(max_score, score);

    for (auto bit = 0; bit < BitsetSize; ++bit)
        if (todo.test(bit)) {
            auto nt = t + dist[curr_id][bit] + 1;
            if (nt < t_lim) {
                todo.reset(bit);
                search1(valves, bit, dist, todo, nt, t_lim,
                    score + valves[bit].flow * (t_lim - nt), max_score);
                todo.set(bit);
            }
        }
}

auto solve1(const vector<Valve>& valves, int start_valve_id, const vector<vector<int>>& dist,
    Bitset& todo)
{
    auto max_score = 0;
    search1(valves, start_valve_id, dist, todo, 0, 30, 0, max_score);
    return max_score;
}

void search2(const vector<Valve>& valves, int curr_id, const vector<vector<int>>& dist,
    Bitset& todo, int t, int t_lim, int score, unordered_map<Bitset, int>& todo2score)
{
    auto it = todo2score.find(todo);
    if (it == end(todo2score))
        todo2score.emplace(todo, score);
    else
        it->second = max(it->second, score);

    for (auto bit = 0; bit < BitsetSize; ++bit)
        if (todo.test(bit)) {
            auto nt = t + dist[curr_id][bit] + 1;
            if (nt < t_lim) {
                todo.reset(bit);
                search2(valves, bit, dist, todo, nt, t_lim,
                    score + valves[bit].flow * (t_lim - nt), todo2score);
                todo.set(bit);
            }
        }
}

auto solve2(const vector<Valve>& valves, int start_valve_id, const vector<vector<int>>& dist,
    Bitset& todo)
{
    unordered_map<Bitset, int> todo2score;
    search2(valves, start_valve_id, dist, todo, 0, 26, 0, todo2score);

    vector<pair<Bitset, int>> done2score;
    for (const auto& [bs, score] : todo2score)
        done2score.emplace_back(bs ^ todo, score);

    auto max_score = 0;
    for (auto i = 0; i < ssize(done2score); ++i) {
        auto [bs_i, score_i] = done2score[i];
        for (auto j = i + 1; j < ssize(done2score); ++j) {
            auto [bs_j, score_j] = done2score[j];
            if ((bs_i & bs_j).none())
                max_score = max(max_score, score_i + score_j);
        }
    }
    return max_score;
}

auto read_valves()
{
    auto name2id = [](string_view name_sv) {
        static unordered_map<string, int> name2id_map;
        string name_s{name_sv};
        if (!name2id_map.contains(name_s))
            name2id_map.try_emplace(name_s, name2id_map.size());
        return name2id_map[name_s];
    };

    vector<Valve> valves;

    string line;
    while (getline(cin, line)) {
        Valve valve;
        string_view sv{line};
        sv.remove_prefix(strlen("Valve "));
        auto name{sv.substr(0, sv.find_first_of(' '))};
        valve.id = name2id(name);
        sv.remove_prefix(name.size());
        sv.remove_prefix(strlen(" has flow rate="));
        valve.flow = strtol(sv.data(), nullptr, 10);
        sv.remove_prefix(sv.find_first_of(';') + 1);
        if (sv.starts_with(" tunnel "))
            sv.remove_prefix(strlen(" tunnel leads to valve "));
        else
            sv.remove_prefix(strlen(" tunnels lead to valves "));
        for (auto adj_valve : split(sv, ','))
            valve.adj_valve_ids.push_back(name2id(adj_valve));
        valves.push_back(move(valve));
    }

    sort(begin(valves), end(valves), [](const auto& lhs, const auto& rhs) {
        return lhs.id < rhs.id;
    });

    return make_pair(valves, name2id("AA"));
}

int main()
{
    auto [valves, start_valve_id] = read_valves();
    assert(ssize(valves) <= BitsetSize);

    auto dist = comp_dist(valves);

    Bitset todo{};
    for (const auto& valve : valves)
        if (valve.flow)
            todo.set(valve.id);

    cout << solve1(valves, start_valve_id, dist, todo) << '\n';
    cout << solve2(valves, start_valve_id, dist, todo) << '\n';

    return 0;
}

