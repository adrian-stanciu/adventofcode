#include <cmath>
#include <iostream>

auto sum_of_divisors(int n)
{
    auto sum = 0;

    auto n_sqrt = static_cast<int>(sqrt(n));

    for (auto i = 1; i < n_sqrt; ++i)
        if (n % i == 0) {
            sum += i;
            sum += n / i;
        }

    if (n_sqrt * n_sqrt == n)
        sum += n_sqrt;

    return sum;
}

auto find_lowest1(int n)
{
    auto i = 0;

    while (10 * sum_of_divisors(++i) < n);

    return i;
}

auto sum_of_divisors_gte(int n, int limit)
{
    auto sum = 0;

    auto n_sqrt = static_cast<int>(sqrt(n));

    for (auto i = 1; i < n_sqrt; ++i)
        if (n % i == 0) {
            auto big_divisor = n / i;
            if (big_divisor >= limit)
                sum += big_divisor;
            else
                break;

            if (i >= limit)
                sum += i;
        }

    if (n_sqrt * n_sqrt == n && n_sqrt >= limit)
        sum += n_sqrt;

    return sum;
}

auto find_lowest2(int n)
{
    auto i = 0;

    while (true) {
        ++i;
        // sum of divisors which are not less than i / 50
        auto limit = i / 50 + (i % 50 ? 1 : 0);
        if (11 * sum_of_divisors_gte(i, limit) >= n)
            break;
    }

    return i;
}

int main()
{
    int n;
    std::cin >> n;

    std::cout << find_lowest1(n) << "\n";
    std::cout << find_lowest2(n) << "\n";

    return 0;
}

