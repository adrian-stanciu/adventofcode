#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
    long y;
    long x;

    Point() = default;

    Point(long y, long x)
    : y(y), x(x)
    {}
};

bool operator==(const Point& p1, const Point& p2)
{
    return p1.y == p2.y && p1.x == p2.x;
}

struct PointHasher {
    auto operator()(const Point& p) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= p.y + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= p.x + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

struct FileSystem {
    long y;
    long x;
    long size;
    long used;
    long avail;

    FileSystem(long y, long x, long size, long used, long avail)
    : y(y), x(x), size(size), used(used), avail(avail)
    {}
};

void parse(const std::string& line, std::vector<FileSystem>& fs_vec)
{
    static const std::regex fs_re{".*-x([0-9]+)-y([0-9]+)\\s+([0-9]+)T\\s+([0-9]+)T\\s+([0-9]+)T\\s+.*"};

    std::smatch matched;
    if (regex_match(line, matched, fs_re)) {
        auto x = strtol(matched[1].str().data(), nullptr, 10);
        auto y = strtol(matched[2].str().data(), nullptr, 10);
        auto size = strtol(matched[3].str().data(), nullptr, 10);
        auto used = strtol(matched[4].str().data(), nullptr, 10);
        auto avail = strtol(matched[5].str().data(), nullptr, 10);

        fs_vec.emplace_back(y, x, size, used, avail);
    }
}

auto count_pairs(const std::vector<FileSystem>& fs_vec)
{
    auto cnt = 0;

    for (auto i = 0U; i < fs_vec.size(); ++i)
        for (auto j = 0U; j < fs_vec.size(); ++j)
            if (i != j && fs_vec[i].used > 0 && fs_vec[i].used <= fs_vec[j].avail)
                ++cnt;

    return cnt;
}

constexpr auto Inf = std::numeric_limits<long>::max();

using Map = std::vector<std::vector<char>>;

auto a_star_dist(const Map& map, const Point& src, const Point& dst, const Point& skip)
{
    static const std::array<int, 4> dy{-1, 1, 0, 0};
    static const std::array<int, 4> dx{0, 0, -1, 1};

    auto dist = [&] (const Point& p) {
        return abs(p.y - dst.y) + abs(p.x - dst.x);
    };

    struct State {
        Point p;

        long d;
        long score;

        bool operator==(const State& s) const
        {
            return p == s.p;
        }

        bool operator<(const State& s) const
        {
            if (score == s.score) {
                if (d == s.d)
                    return !(*this == s);
                else
                    return d < s.d;
            } else
                return score < s.score;
        }
    };

    struct StateHasher {
        auto operator()(const State& s) const
        {
            // based on boost's hash_combine()
            size_t h = 0;
            h ^= PointHasher{}(s.p) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };

    std::set<State> sorted_set;
    std::unordered_set<State, StateHasher> open_set;
    std::unordered_set<State, StateHasher> closed_set;

    State start{src, 0, dist(src)};
    sorted_set.emplace(start);
    open_set.emplace(std::move(start));

    while (!open_set.empty()) {
        auto s = *sorted_set.begin();
        sorted_set.erase(sorted_set.begin());

        if (s.p == dst)
            return s.d;

        open_set.erase(s);
        closed_set.emplace(s);

        for (auto i = 0; i < 4; ++i) {
            auto y = s.p.y + dy[i];
            if (y < 0 || y >= static_cast<long>(map.size()))
                continue;
            auto x = s.p.x + dx[i];
            if (x < 0 || x >= static_cast<long>(map[0].size()))
                continue;

            if (map[y][x] == '#')
                continue;

            Point next_p{y, x};

            if (next_p == skip)
                continue;

            auto next_s_d = s.d + 1;
            State next_s{next_p, next_s_d, next_s_d + dist(next_p)};

            if (closed_set.find(next_s) == closed_set.end()) {
                auto it = open_set.find(next_s);
                if (it == open_set.end()) {
                    sorted_set.emplace(next_s);
                    open_set.emplace(std::move(next_s));
                } else if (next_s.score < it->score ||
                    (next_s.score == it->score && next_s.d < it->d)) {
                    sorted_set.erase(*it);
                    sorted_set.emplace(std::move(next_s));
                }
            }
        }
    }

    return Inf;
}

auto a_star(const Map& map, const Point& from, const Point& to, const Point& empty)
{
    static const std::array<int, 4> dy{-1, 1, 0, 0};
    static const std::array<int, 4> dx{0, 0, -1, 1};

    auto dist = [&] (const Point& p) {
        return abs(p.y - to.y) + abs(p.x - to.x);
    };

    struct State {
        Point obj;
        Point empty;

        long d;
        long score;

        bool operator==(const State& s) const
        {
            return obj == s.obj && empty == s.empty;
        }

        bool operator<(const State& s) const
        {
            if (score == s.score) {
                if (d == s.d)
                    return !(*this == s);
                else
                    return d < s.d;
            } else
                return score < s.score;
        }
    };

    struct StateHasher {
        auto operator()(const State& s) const
        {
            // based on boost's hash_combine()
            size_t h = 0;
            h ^= PointHasher{}(s.obj) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= PointHasher{}(s.empty) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };

    std::set<State> sorted_set;
    std::unordered_set<State, StateHasher> open_set;
    std::unordered_set<State, StateHasher> closed_set;

    State start{from, empty, 0, dist(from)};
    sorted_set.emplace(start);
    open_set.emplace(std::move(start));

    while (!sorted_set.empty()) {
        auto s = *sorted_set.begin();
        sorted_set.erase(sorted_set.begin());

        if (s.obj == to)
            return s.d;

        open_set.erase(s);
        closed_set.emplace(s);

        for (auto i = 0; i < 4; ++i) {
            auto y = s.obj.y + dy[i];
            if (y < 0 || y >= static_cast<long>(map.size()))
                continue;
            auto x = s.obj.x + dx[i];
            if (x < 0 || x >= static_cast<long>(map[0].size()))
                continue;

            if (map[y][x] == '#')
                continue;

            Point next_obj{y, x};

            auto next_obj_d = a_star_dist(map, s.empty, next_obj, s.obj);
            if (next_obj_d == Inf)
                continue;

            auto next_s_d = s.d + next_obj_d + 1;
            State next_s{next_obj, s.obj, next_s_d, next_s_d + dist(next_obj)};

            if (closed_set.find(next_s) == closed_set.end()) {
                auto it = open_set.find(next_s);
                if (it == open_set.end()) {
                    sorted_set.emplace(next_s);
                    open_set.emplace(std::move(next_s));
                } else if (next_s.score < it->score ||
                    (next_s.score == it->score && next_s.d < it->d)) {
                    sorted_set.erase(*it);
                    sorted_set.emplace(next_s);
                }
            }
        }
    }

    return -1L;
}

int main()
{
    std::vector<FileSystem> fs_vec;

    std::string line;
    while (getline(std::cin, line))
        parse(line, fs_vec);

    std::optional<Point> empty;
    std::optional<long> empty_size;

    auto max_y = 0L;
    auto max_x = 0L;
    for (const auto& fs : fs_vec) {
        max_y = std::max(max_y, fs.y);
        max_x = std::max(max_x, fs.x);

        if (fs.used == 0) {
            empty = {fs.y, fs.x};
            empty_size = fs.size;
        }
    }

    std::cout << count_pairs(fs_vec) << "\n";

    // solve a simpler problem:
    // * a single slot in grid is empty
    // * all other slots in grid are fixed ('#') or movable ('.')
    // * we need to find the minimum steps to move a slot from its initial position to a final one

    // reduce the initial problem to the simpler one:
    // * the empty slot is the one having a used value equal to zero
    // * the fixed slots are the ones having a used value greater than the size of the empty slot
    // * all other slots are movable

    if (empty.has_value()) {
        Map map(max_y + 1, std::vector<char>(max_x + 1, '.'));

        for (const auto& fs : fs_vec)
            if (fs.used > empty_size.value())
                map[fs.y][fs.x] = '#';

        std::cout << a_star(map, Point{0, max_x}, Point{0, 0}, empty.value()) << "\n";
    } else
        std::cout << "N/A\n";

    return 0;
}

