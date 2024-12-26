#include <bits/stdc++.h>

using namespace std;

const auto NumRooms = 4;

auto cost_of(int x)
{
    static const array<int, NumRooms> cost{1, 10, 100, 1000};

    return cost[x];
}

struct Board {
    static const auto EmptyCell = -1;

    vector<int> hall;
    array<vector<int>, NumRooms> rooms;

    Board(int hall_sz) : hall(hall_sz, EmptyCell) {}

    bool operator==(const Board& b) const
    {
        return hall == b.hall && rooms == b.rooms;
    }

    bool operator<(const Board& b) const
    {
        return tie(hall, rooms) < tie(b.hall, b.rooms);
    }

    [[nodiscard]] auto is_sol() const
    {
        for (auto r = 0; r < static_cast<int>(rooms.size()); ++r)
            for (auto x : rooms[r])
                if (x != r)
                    return false;
        return true;
    }
};

struct BoardHasher {
    auto operator()(const Board& b) const
    {
        size_t h = 0;

        // based on boost's hash_combine()
        auto hash_combine = [&](auto x) {
            h ^= hash<decltype(x)>{}(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
        };

        for (auto x : b.hall)
            hash_combine(x);
        for (const auto& r : b.rooms)
            for (auto x : r)
                hash_combine(x);

        return h;
    }
};

struct State {
    Board b;
    int paid_cost{0};
    int estimated_remaining_cost{0};

    State(const Board& b, const array<int, NumRooms>& room2col)
    : b(b)
    {
        for (auto r = 0; r < NumRooms; ++r) {
            auto must_move = false;
            for (auto p = static_cast<int>(b.rooms[r].size()) - 1; p >= 0; --p) {
                auto x = b.rooms[r][p];

                if (!must_move && x == r)
                    continue;
                else
                    must_move = true;

                auto dist = max(abs(room2col[x] - room2col[r]), 2) + p + 2;
                estimated_remaining_cost += cost_of(x) * dist;
            }
        }
    }

    [[nodiscard]] auto cost() const
    {
        return paid_cost + estimated_remaining_cost;
    }

    bool operator==(const State& s) const
    {
        return b == s.b;
    }

    bool operator<(const State& s) const
    {
        auto my_cost = cost();
        auto his_cost = s.cost();

        return tie(my_cost, paid_cost, b) < tie(his_cost, s.paid_cost, s.b);
    }
};

struct StateHasher {
    auto operator()(const State& s) const
    {
        return BoardHasher{}(s.b);
    }
};

auto next_states(State s, const array<int, NumRooms>& room2col)
{
    vector<State> next;

    for (auto r = 0; r < NumRooms; ++r) {
        auto col = room2col[r];

        auto send_from = -1;
        auto recv_to = -1;

        for (auto first_occupied = -1, p = 0; p < static_cast<int>(s.b.rooms[r].size()); ++p) {
            auto x = s.b.rooms[r][p];
            if (x == Board::EmptyCell) {
                recv_to = p;
            } else {
                if (first_occupied == -1)
                    first_occupied = p;

                if (x != r) {
                    send_from = first_occupied;
                    break;
                }
            }
        }

        auto send = [&](int begin_col, int end_col, int step_col) {
            for (auto next_col = begin_col; next_col != end_col; next_col += step_col) {
                if (s.b.hall[next_col] != Board::EmptyCell)
                    break;

                if (find(begin(room2col), end(room2col), next_col) != end(room2col))
                    continue;

                auto x = s.b.rooms[r][send_from];
                auto cost = cost_of(x);
                auto extra_cost = cost * (abs(next_col - col) + send_from + 1);
                auto old_estimated_cost = cost * (max(abs(room2col[x] - col), 2) + send_from + 2);
                auto new_estimated_cost = cost * (abs(room2col[x] - next_col) + 1);

                swap(s.b.rooms[r][send_from], s.b.hall[next_col]);
                s.paid_cost += extra_cost;
                s.estimated_remaining_cost -= old_estimated_cost;
                s.estimated_remaining_cost += new_estimated_cost;
                next.push_back(s);
                s.estimated_remaining_cost -= new_estimated_cost;
                s.estimated_remaining_cost += old_estimated_cost;
                s.paid_cost -= extra_cost;
                swap(s.b.rooms[r][send_from], s.b.hall[next_col]);
            }
        };

        auto recv = [&](int begin_col, int end_col, int step_col) {
            auto prev_col = begin_col;
            while (prev_col != end_col && s.b.hall[prev_col] == Board::EmptyCell)
                prev_col += step_col;

            if (prev_col != end_col && s.b.hall[prev_col] == r) {
                auto cost = cost_of(r);
                auto old_estimated_cost = cost * (abs(col - prev_col) + 1);
                auto extra_cost = old_estimated_cost + cost * recv_to;

                swap(s.b.rooms[r][recv_to], s.b.hall[prev_col]);
                s.paid_cost += extra_cost;
                s.estimated_remaining_cost -= old_estimated_cost;
                next.push_back(s);
                s.estimated_remaining_cost += old_estimated_cost;
                s.paid_cost -= extra_cost;
                swap(s.b.rooms[r][recv_to], s.b.hall[prev_col]);
            }
        };

        if (send_from != -1) {
            send(col - 1, -1, -1);
            send(col + 1, static_cast<int>(s.b.hall.size()), 1);
        } else if (recv_to != -1) {
            recv(col - 1, -1, -1);
            recv(col + 1, static_cast<int>(s.b.hall.size()), 1);
        }
    }

    return next;
}

auto a_star(const Board& b, const array<int, NumRooms>& room2col)
{
    set<State> sorted_set;
    unordered_set<State, StateHasher> open_set;
    unordered_set<Board, BoardHasher> closed_set;

    State s{b, room2col};

    sorted_set.emplace(s);
    open_set.emplace(move(s));

    while (!open_set.empty()) {
        auto s = *begin(sorted_set);
        sorted_set.erase(begin(sorted_set));

        if (s.b.is_sol())
            return s.paid_cost;

        open_set.erase(s);
        closed_set.emplace(s.b);

        for (auto next_s : next_states(s, room2col))
            if (closed_set.find(next_s.b) == end(closed_set)) {
                auto it = open_set.find(next_s);
                if (it == end(open_set)) {
                    sorted_set.emplace(next_s);
                    open_set.emplace(move(next_s));
                } else if (next_s.cost() < it->cost()) {
                    sorted_set.erase(*it);
                    sorted_set.emplace(move(next_s));
                }
            }
    }

    return -1;
}

auto solve(const vector<string>& grid)
{
    auto hall_sz = static_cast<int>(grid[1].size()) - 2;

    array<int, NumRooms> room2col;
    for (auto r = 0, c = 0; c < static_cast<int>(grid[2].size()); ++c)
        if (grid[2][c] != '#')
            room2col[r++] = c - 1;

    Board b(hall_sz);
    for (auto r = 0; r < NumRooms; ++r)
        for (auto p = 2; p < static_cast<int>(grid.size()) - 1; ++p)
            b.rooms[r].push_back(grid[p][room2col[r] + 1] - 'A');

    return a_star(b, room2col);
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    cout << solve(grid) << '\n';

    grid.insert(begin(grid) + 3, "  #D#C#B#A#");
    grid.insert(begin(grid) + 4, "  #D#B#A#C#");

    cout << solve(grid) << '\n';

    return 0;
}

