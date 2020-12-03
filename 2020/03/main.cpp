#include <bits/stdc++.h>

using namespace std;

int main()
{
    vector<string> map;

    string line;
    while (getline(cin, line))
        map.push_back(move(line));

    auto n = map.size();
    auto m = map[0].size();

    auto count = [&] (auto dx, auto dy, auto what) {
        auto cnt = 0;
        for (size_t i = 0, j = 0; i < n; i += dy, j += dx, j %= m)
            cnt += map[i][j] == what;
        return cnt;
    };

    cout << count(3, 1, '#') << '\n';

    auto res = 1l;
    const pair<int, int> slopes[] = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
    for (auto [dx, dy] : slopes)
        res *= count(dx, dy, '#');
    cout << res << '\n';

    return 0;
}

