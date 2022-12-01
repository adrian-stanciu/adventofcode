#include <bits/stdc++.h>

using namespace std;

int main()
{
    auto sum{0l};
    array max3sums{0l, 0l, 0l};

    auto update_max3sums = [&](auto sum) {
        for (auto i = 0; i < ssize(max3sums); ++i)
            if (sum >= max3sums[i]) {
                move_backward(begin(max3sums) + i, end(max3sums) - 1, end(max3sums));
                max3sums[i] = sum;
                break;
            }
    };

    string line;
    while (getline(cin, line)) {
        if (line.empty()) {
            update_max3sums(sum);
            sum = 0;
        } else {
            sum += strtol(line.data(), nullptr, 10);
        }
    }
    update_max3sums(sum);

    cout << max3sums[0] << '\n';
    cout << accumulate(begin(max3sums), end(max3sums), 0l) << '\n';

    return 0;
}

