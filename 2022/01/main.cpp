#include <bits/stdc++.h>

using namespace std;

int main()
{
    auto sum{0l};
    array max3sums{0l, 0l, 0l};

    string line;
    while (getline(cin, line)) {
        if (line.empty()) {
            for (auto i = 0; i < ssize(max3sums); ++i)
                if (sum >= max3sums[i]) {
                    move_backward(begin(max3sums) + i, end(max3sums) - 1, begin(max3sums) + i + 1);
                    max3sums[i] = sum;
                    break;
                }
            sum = 0;
        } else {
            sum += strtol(line.data(), nullptr, 10);
        }
    }

    cout << max3sums[0] << '\n';
    cout << accumulate(begin(max3sums), end(max3sums), 0l) << '\n';

    return 0;
}

