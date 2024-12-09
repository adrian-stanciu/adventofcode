#include <bits/stdc++.h>

using namespace std;

auto solve1(const string& s)
{
    static constexpr auto EmptyId{-1};

    vector<int> entries;

    auto id = 0;
    bool is_file = true;

    for (auto c : s) {
        auto size = c - '0';

        if (is_file) {
            while (size--)
                entries.push_back(id);
            ++id;
        } else {
            while (size--)
                entries.push_back(EmptyId);
        }

        is_file = !is_file;
    }

    auto n = ssize(entries);

    auto from = n - 1;
    auto to = s[0] - '0';

    while (from > to) {
        entries[to] = exchange(entries[from], EmptyId);

        while (from > to && entries[from] == EmptyId)
            --from;

        while (from > to && entries[to] != EmptyId)
            ++to;
    }

    auto sum = 0l;

    for (auto i = 0; i < n; ++i)
        if (entries[i] != EmptyId)
            sum += 1l * i * entries[i];

    return sum;
}

auto solve2(const string& s)
{
    static constexpr auto MaxSz{9};

    struct File {
        int id;
        int from;
        int size;
    };

    vector<File> files;
    array<priority_queue<int, vector<int>, greater<>>, MaxSz + 1> free_spaces;

    auto id = 0;
    auto from = 0;
    bool is_file = true;

    for (auto c : s) {
        auto size = c - '0';

        if (is_file) {
            files.emplace_back(id, from, size);
            ++id;
        } else {
            free_spaces[size].push(from);
        }

        from += size;
        is_file = !is_file;
    }

    auto find_empty_space_before = [&](const auto& file) -> optional<int> {
        vector<pair<int, int>> candidates;

        for (auto size = file.size; size <= MaxSz; ++size)
            if (!free_spaces[size].empty())
                if (auto from = free_spaces[size].top(); from < file.from)
                    candidates.emplace_back(from, size);

        if (candidates.empty())
            return nullopt;

        sort(begin(candidates), end(candidates));

        auto [from, size] = candidates[0];

        free_spaces[size].pop();

        if (size > file.size)
            free_spaces[size - file.size].push(from + file.size);

        return from;
    };

    reverse(begin(files), end(files));

    for (auto& file : files)
        if (auto from = find_empty_space_before(file); from)
            file.from = *from;

    auto sum = 0l;

    for (const auto& file : files) {
        auto to = file.from + file.size;
        for (auto i = file.from; i < to; ++i)
            sum += 1l * i * file.id;
    }

    return sum;
}

int main()
{
    string s;
    cin >> s;

    cout << solve1(s) << '\n';
    cout << solve2(s) << '\n';

    return 0;
}

