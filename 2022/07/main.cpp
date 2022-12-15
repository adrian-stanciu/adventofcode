#include <bits/stdc++.h>

using namespace std;

const auto Limit{100'000l};
const auto Total{70'000'000l};
const auto Needed{30'000'000l};

// unchecked assumptions:
// 1. each directory is listed only once
// 2. "cd /" is not used to jump to root directory;
//    thus, "cd .." can be used to update ancestors' sizes
auto compute_dir_sizes()
{
    vector<long> dir_sizes;
    stack<long> cwd_sizes;

    auto pop_dir = [&]() {
        if (cwd_sizes.empty())
            return false;

        dir_sizes.push_back(cwd_sizes.top());
        cwd_sizes.pop();

        if (cwd_sizes.empty())
            return false;

        cwd_sizes.top() += dir_sizes.back();
        return true;
    };

    string line;
    while (getline(cin, line))
        if (line == "$ cd ..")
            pop_dir();
        else if (line.starts_with("$ cd"))
            cwd_sizes.push(0);
        else if (line == "$ ls" || line.starts_with("dir"))
            continue;
        else // file
            cwd_sizes.top() += strtol(line.data(), nullptr, 10);

    while (pop_dir());

    return dir_sizes;
}

int main()
{
    auto dir_sizes = compute_dir_sizes();

    cout << accumulate(begin(dir_sizes), end(dir_sizes), 0l, [](auto acc, auto dir_size) {
        return acc + (dir_size <= Limit ? dir_size : 0);
    }) << '\n';

    sort(begin(dir_sizes), end(dir_sizes));
    auto used = dir_sizes.back();
    auto to_free = used - (Total - Needed);
    cout << *lower_bound(begin(dir_sizes), end(dir_sizes), to_free) << '\n';

    return 0;
}

