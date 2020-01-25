#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

using Row = std::vector<char>;
using Matrix = std::vector<Row>;

constexpr auto Inf = std::numeric_limits<int>::max();
constexpr auto Sz  = 32;

struct Player;

void fill_dist(int dist[][Sz + 1], int x, int y,
    const Matrix& map, const std::vector<Player>& players);

struct Target {
    int x;
    int y;
    int d;

    Target(int x, int y, int d) : x(x), y(y), d(d) {}
};

struct Player {
    int x;
    int y;
    char type;
    int power {3};
    int life {200};

    Player(int x, int y, char type, int power)
    : x(x), y(y), type(type), power(power)
    {}

    bool attack(std::vector<Player>& players)
    {
        // check in range
        auto up = find_if(players.begin(), players.end(), [this](const Player& p) {
            return ((type != p.type) && (p.life > 0) && (p.x == x) && (p.y == y - 1));
        });
        auto left = find_if(players.begin(), players.end(), [this](const Player& p) {
            return ((type != p.type) && (p.life > 0) && (p.x == x - 1) && (p.y == y));
        });
        auto right = find_if(players.begin(), players.end(), [this](const Player& p) {
            return ((type != p.type) && (p.life > 0) && (p.x == x + 1) && (p.y == y));
        });
        auto down = find_if(players.begin(), players.end(), [this](const Player& p) {
            return ((type != p.type) && (p.life > 0) && (p.x == x) && (p.y == y + 1));
        });

        // select enemy
        auto min_life = Inf;
        if (up != players.end())
            min_life = std::min(min_life, up->life);
        if (left != players.end())
            min_life = std::min(min_life, left->life);
        if (right != players.end())
            min_life = std::min(min_life, right->life);
        if (down != players.end())
            min_life = std::min(min_life, down->life);

        // attack enemy
        if (min_life != Inf) {
            if (up != players.end() && up->life == min_life)
                up->life -= power;
            else if (left != players.end() && left->life == min_life)
                left->life -= power;
            else if (right != players.end() && right->life == min_life)
                right->life -= power;
            else if (down != players.end() && down->life == min_life)
                down->life -= power;
            return true;
        }

        return false;
    }

    bool move(const Matrix& map, std::vector<Player>& players)
    {
        int dist[Sz + 1][Sz + 1];
        for (auto i = 1; i <= Sz; ++i)
            for (auto j = 1; j <= Sz; ++j)
                dist[i][j] = Inf;
        fill_dist(dist, x, y, map, players);

        // select target
        std::vector<Target> targets;
        for (auto& p : players)
            if (p.type != type && p.life > 0) {
                if (dist[p.y - 1][p.x] < Inf)
                    targets.emplace_back(p.x, p.y - 1, dist[p.y - 1][p.x]);
                if (dist[p.y + 1][p.x] < Inf)
                    targets.emplace_back(p.x, p.y + 1, dist[p.y + 1][p.x]);
                if (dist[p.y][p.x - 1] < Inf)
                    targets.emplace_back(p.x - 1, p.y, dist[p.y][p.x - 1]);
                if (dist[p.y][p.x + 1] < Inf)
                    targets.emplace_back(p.x + 1, p.y, dist[p.y][p.x + 1]);
            }

        if (targets.empty())
            return false;

        sort(targets.begin(), targets.end(), [](const Target& t1, const Target& t2) {
            if (t1.d == t2.d) {
                if (t1.y == t2.y)
                    return t1.x < t2.x;
                else
                    return t1.y < t2.y;
            } else {
                return t1.d < t2.d;
            }
        });
        auto near_y = targets[0].y;
        auto near_x = targets[0].x;

        // select path
        for (auto i = 1; i <= Sz; ++i)
            for (auto j = 1; j <= Sz; ++j)
                dist[i][j] = Inf;
        fill_dist(dist, near_x, near_y, map, players);

        auto min_dist = Inf;
        min_dist = std::min(min_dist, dist[y - 1][x]);
        min_dist = std::min(min_dist, dist[y + 1][x]);
        min_dist = std::min(min_dist, dist[y][x - 1]);
        min_dist = std::min(min_dist, dist[y][x + 1]);

        if (min_dist == dist[y - 1][x])
            --y;
        else if (min_dist == dist[y][x - 1])
            --x;
        else if (min_dist == dist[y][x + 1])
            ++x;
        else if (min_dist == dist[y + 1][x])
            ++y;

        return true;
    }

    bool take_turn(const Matrix& map, std::vector<Player>& players)
    {
        // check end
        auto it = find_if(players.begin(), players.end(), [this](const Player& p) {
            return ((type != p.type) && (p.life > 0));
        });
        if (it == players.end())
            return true;

        if (attack(players))
            return false;

        if (move(map, players))
            attack(players);

        return false;
    }
};

void fill_dist(int dist[][Sz + 1], int x, int y,
    const Matrix& map, const std::vector<Player>& players)
{
    std::queue<Target> q;
    q.emplace(x, y, 0);
    bool first = true;

    while (!q.empty()) {
        auto t = q.front();
        q.pop();

        // check in map
        if (t.x < 1 || t.x > Sz)
            continue;
        if (t.y < 1 || t.y > Sz)
            continue;
        // check againt walls
        if (map[t.y - 1][t.x - 1] == '#')
            continue;
        // check if already computed
        if (dist[t.y][t.x] != Inf)
            continue;
        // check if occupied
        if (!first) {
            auto it = find_if(players.begin(), players.end(), [&t](const Player& p) {
                return p.x == t.x && p.y == t.y && p.life > 0;
            });
            if (it != players.end())
                continue;
        } else
            first = false;

        dist[t.y][t.x] = t.d;

        q.emplace(t.x - 1, t.y, t.d + 1);
        q.emplace(t.x + 1, t.y, t.d + 1);
        q.emplace(t.x, t.y - 1, t.d + 1);
        q.emplace(t.x, t.y + 1, t.d + 1);
    }
}

void sort_players(std::vector<Player>& players)
{
    sort(players.begin(), players.end(), [](const Player& p1, const Player& p2) {
        if (p1.y == p2.y)
            return p1.x < p2.x;
        else
            return p1.y < p2.y;
    });
}

auto battle(const Matrix& map, std::vector<Player> players)
{
    auto i = 0;

    while (true) {
        sort_players(players);

        bool end = false;

        for (auto& p : players)
            if (p.life > 0)
                end = end || p.take_turn(map, players);

        if (end)
            break;
        ++i;
    }

    auto s = 0;
    auto dead_elfs = 0;
    for (auto& p : players)
        if (p.life > 0)
            s += p.life;
        else
            if (p.type == 'E')
                ++dead_elfs;

    return std::make_pair(i * s, dead_elfs);
}

auto battle_with_no_dead_elfs(const Matrix& map, std::vector<Player> players)
{
    for (auto power = 4; power <= 200; ++power) {
        for (auto& p : players)
            if (p.type == 'E')
                p.power = power;

        auto [res, num_dead_elfs] = battle(map, players);
        if (num_dead_elfs == 0)
            return res;
    }

    assert(false);
}

int main()
{
    Matrix map;
    std::vector<Player> players;

    auto y = 1;

    std::string line;
    while (getline(std::cin, line)) {
        Row r;
        auto x = 1;
        for (auto c : line) {
            if (c == '#' || c == '.')
                r.push_back(c);
            else if (c == 'E' || c == 'G') {
                r.push_back('.');
                if (c == 'E')
                    players.emplace_back(x, y, c, 3);
                else
                    players.emplace_back(x, y, c, 3);
            }
            ++x;
        }
        map.push_back(move(r));
        ++y;
    }

    std::cout << battle(map, players).first << "\n";
    std::cout << battle_with_no_dead_elfs(map, players) << "\n";

    return 0;
}

