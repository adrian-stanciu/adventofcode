#include <cassert>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "hasher.h"

using Tree = std::unordered_map<std::pair<int, int>, std::vector<std::pair<int, int>>, PairHasher<int>>;

void process_regex(const std::string& re,
    std::unordered_map<int, int>& left_par_2_right_par,
    std::unordered_map<int, int>& pipe_2_left_par,
    std::unordered_map<int, std::vector<int>>& left_par_2_pipes)
{
    std::stack<int> left_pars;

    auto i = 1;

    while (re[i] != '$') {
        if (re[i] == '(') {
            left_pars.push(i);
        } else if (re[i] == '|') {
            auto left_par = left_pars.top();
            pipe_2_left_par[i] = left_par;
            left_par_2_pipes[left_par].push_back(i);
        } else if (re[i] == ')') {
            auto left_par = left_pars.top();
            left_pars.pop();
            left_par_2_right_par[left_par] = i;
        }

        ++i;
    }
}

struct State {
    int x;
    int y;
    int re_idx;
};

bool operator==(const State& s1, const State& s2)
{
    return s1.x == s2.x && s1.y == s2.y && s1.re_idx == s2.re_idx;
}

struct StateHasher {
    auto operator()(const State& c) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= c.x + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= c.y + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= c.re_idx + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

void build_tree_rec(Tree& tree, const std::string& re, int& i, std::pair<int, int> curr,
    std::unordered_set<State, StateHasher>& visited,
    const std::unordered_map<int, int>& left_par_2_right_par,
    const std::unordered_map<int, int>& pipe_2_left_par,
    const std::unordered_map<int, std::vector<int>>& left_par_2_pipes)
{
    auto connect = [&tree] (auto p1, auto p2)
    {
        tree[p1].push_back(p2);
        tree[p2].push_back(p1);
    };

    auto& [x, y] = curr;

    while (re[i] != '$') {
        State s {x, y, i};

        if (visited.find(s) != visited.end())
            return;

        visited.emplace(std::move(s));

        switch (re[i]) {
        case 'N':
            {
                auto next = std::make_pair(x, y + 1);
                connect(curr, next);
                curr = std::move(next);
            }
            break;
        case 'S':
            {
                auto next = std::make_pair(x, y - 1);
                connect(curr, next);
                curr = std::move(next);
            }
            break;
        case 'E':
            {
                auto next = std::make_pair(x + 1, y);
                connect(curr, next);
                curr = std::move(next);
            }
            break;
        case 'W':
            {
                auto next = std::make_pair(x - 1, y);
                connect(curr, next);
                curr = std::move(next);
            }
            break;
        case '(':
            // recursively explore all alternatives except the first one
            for (auto pipe : left_par_2_pipes.at(i))
                build_tree_rec(tree, re, ++pipe, curr, visited,
                    left_par_2_right_par, pipe_2_left_par, left_par_2_pipes);
            // continue exploring the first alternative
            break;
        case '|':
            {
                // alternative completed => go forward
                auto left_par = pipe_2_left_par.at(i);
                auto right_par = left_par_2_right_par.at(left_par);
                i = right_par;
            }
            break;
        case ')':
            break;
        }

        ++i;
    }
}

auto build_tree(const std::string& re)
{
    std::unordered_map<int, int> left_par_2_right_par;
    std::unordered_map<int, int> pipe_2_left_par;
    std::unordered_map<int, std::vector<int>> left_par_2_pipes;

    process_regex(re, left_par_2_right_par, pipe_2_left_par, left_par_2_pipes);

    Tree tree;
    auto i = 1;
    auto curr = std::make_pair(0, 0);
    std::unordered_set<State, StateHasher> visited;

    build_tree_rec(tree, re, i, curr, visited,
        left_par_2_right_par, pipe_2_left_par, left_par_2_pipes);

    return tree;
}

auto shortest_path_to_furthest(const Tree& tree)
{
    auto max_d = 0L;

    std::queue<std::pair<std::pair<int, int>, long>> q;
    q.emplace(std::make_pair(0, 0), 0L);

    std::unordered_set<std::pair<int, int>, PairHasher<int>> visited;

    while (!q.empty()) {
        const auto& [curr, d] = q.front();

        if (!visited.count(curr)) {
            visited.emplace(curr);

            max_d = std::max(max_d, d);

            for (const auto& neigh : tree.at(curr))
                if (!visited.count(neigh))
                    q.emplace(neigh, d + 1);
        }

        q.pop();
    }

    return max_d;
}

auto count_further_than(const Tree& tree, int limit)
{
    auto count = 0L;

    std::queue<std::pair<std::pair<int, int>, long>> q;
    q.emplace(std::make_pair(0, 0), 0L);

    std::unordered_set<std::pair<int, int>, PairHasher<int>> visited;

    while (!q.empty()) {
        const auto& [curr, d] = q.front();

        if (!visited.count(curr)) {
            visited.emplace(curr);

            if (d >= limit)
                ++count;

            for (const auto& neigh : tree.at(curr))
                if (!visited.count(neigh))
                    q.emplace(neigh, d + 1);
        }

        q.pop();
    }

    return count;
}

int main()
{
    assert(shortest_path_to_furthest(build_tree("^WNE$")) == 3);
    assert(shortest_path_to_furthest(build_tree("^ENWWW(NEEE|SSE(EE|N))$")) == 10);
    assert(shortest_path_to_furthest(build_tree("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$")) == 18);
    assert(shortest_path_to_furthest(build_tree("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$")) == 23);
    assert(shortest_path_to_furthest(build_tree("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$")) == 31);
    assert(shortest_path_to_furthest(build_tree("^(NNN(E|W)|SS)$")) == 4);
    assert(shortest_path_to_furthest(build_tree("^(SS|NNN(E|W))$")) == 4);
    assert(shortest_path_to_furthest(build_tree("^NNN(E|W)$")) == 4);
    assert(shortest_path_to_furthest(build_tree("^NNN(E|W)NN$")) == 6);
    assert(shortest_path_to_furthest(build_tree("^NNN(E|W)(N|S)$")) == 5);
    assert(shortest_path_to_furthest(build_tree("^NNN(E|W)(NS|SN|)$")) == 5);
    assert(shortest_path_to_furthest(build_tree("^NNN(S|N)NEW(SS|S)$")) == 6);

    std::string regex;
    getline(std::cin, regex);

    auto tree = build_tree(regex);

    std::cout << shortest_path_to_furthest(tree) << "\n";
    std::cout << count_further_than(tree, 1000) << "\n";

    return 0;
}

