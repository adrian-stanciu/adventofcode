#include <bits/stdc++.h>

using namespace std;

struct Interval {
    int from;
    int to;

    auto size() const
    {
        return to - from + 1;
    }

    auto is_intersecting(const Interval& other) const
    {
        return max(from, other.from) <= min(to, other.to);
    }

    auto intersection_size(const Interval& other) const
    {
        return max(min(to, other.to) - max(from, other.from) + 1, 0);
    }
};

struct Cuboid {
    Interval x;
    Interval y;
    Interval z;

    auto size() const
    {
        return 1l * x.size() * y.size() * z.size();
    }

    auto is_intersecting(const Cuboid& other) const
    {
        return x.is_intersecting(other.x) &&
               y.is_intersecting(other.y) &&
               z.is_intersecting(other.z);
    }

    auto intersection_size(const Cuboid& other) const
    {
        return 1l * x.intersection_size(other.x) *
                    y.intersection_size(other.y) *
                    z.intersection_size(other.z);
    }
};

struct Cmd {
    bool turn_on;
    Cuboid cuboid;
};

auto split(const Cuboid& on, const Cuboid& off)
{
    vector<Cuboid> pieces;

    Interval x_common{on.x};
    if (on.x.from <= off.x.from) {
        if (on.x.from < off.x.from)
            pieces.push_back(Cuboid{Interval{on.x.from, off.x.from - 1}, on.y, on.z});
        x_common.from = off.x.from;
    }
    if (off.x.to <= on.x.to) {
        if (off.x.to < on.x.to)
            pieces.push_back(Cuboid{Interval{off.x.to + 1, on.x.to}, on.y, on.z});
        x_common.to = off.x.to;
    }

    Interval y_common{on.y};
    if (on.y.from <= off.y.from) {
        if (on.y.from < off.y.from)
            pieces.push_back(Cuboid{x_common, Interval{on.y.from, off.y.from - 1}, on.z});
        y_common.from = off.y.from;
    }
    if (off.y.to <= on.y.to) {
        if (off.y.to < on.y.to)
            pieces.push_back(Cuboid{x_common, Interval{off.y.to + 1, on.y.to}, on.z});
        y_common.to = off.y.to;
    }

    if (on.z.from < off.z.from)
        pieces.push_back(Cuboid{x_common, y_common, Interval{on.z.from, off.z.from - 1}});
    if (off.z.to < on.z.to)
        pieces.push_back(Cuboid{x_common, y_common, Interval{off.z.to + 1, on.z.to}});

    return pieces;
}

auto solve(const vector<Cmd>& cmds)
{
    vector<Cuboid> on_cuboids;

    for (const auto& cmd : cmds) {
        vector<Cuboid> new_on_cuboids;

        for (const auto& on_cuboid : on_cuboids)
            if (on_cuboid.is_intersecting(cmd.cuboid)) {
                for (const auto& new_on_cuboid : split(on_cuboid, cmd.cuboid))
                    new_on_cuboids.push_back(new_on_cuboid);
            } else {
                new_on_cuboids.push_back(on_cuboid);
            }

        if (cmd.turn_on)
            new_on_cuboids.push_back(cmd.cuboid);

        on_cuboids.swap(new_on_cuboids);
    }

    return on_cuboids;
}

int main()
{
    static const regex r{R"((on|off) x=(-?[0-9]+)..(-?[0-9]+),y=(-?[0-9]+)..(-?[0-9]+),z=(-?[0-9]+)..(-?[0-9]+))"};

    auto to_num = [](const string& s) {
        return strtol(s.data(), nullptr, 10);
    };

    vector<Cmd> cmds;

    string line;
    while(getline(cin, line)) {
        smatch matched;
        regex_match(line, matched, r);

        Cmd cmd;
        cmd.turn_on = matched[1].str() == "on";
        cmd.cuboid.x.from = to_num(matched[2].str());
        cmd.cuboid.x.to = to_num(matched[3].str());
        cmd.cuboid.y.from = to_num(matched[4].str());
        cmd.cuboid.y.to = to_num(matched[5].str());
        cmd.cuboid.z.from = to_num(matched[6].str());
        cmd.cuboid.z.to = to_num(matched[7].str());

        cmds.push_back(cmd);
    }

    auto on_cuboids = solve(cmds);

    cout << accumulate(begin(on_cuboids), end(on_cuboids), 0l, [](auto acc, const auto& cuboid) {
        static const Cuboid target{Interval{-50, 50}, Interval{-50, 50}, Interval{-50, 50}};

        return acc + cuboid.intersection_size(target);
    }) << '\n';

    cout << accumulate(begin(on_cuboids), end(on_cuboids), 0l, [](auto acc, const auto& cuboid) {
        return acc + cuboid.size();
    }) << '\n';

    return 0;
}

