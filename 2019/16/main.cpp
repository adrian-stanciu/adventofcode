#include <iostream>
#include <vector>

void fft(std::vector<int> values, unsigned int iters,
    unsigned int offset, unsigned int count)
{
    for (auto i = 1U; i <= iters; ++i) {
        std::vector<int> new_values;
        new_values.reserve(values.size());

        std::vector<int> prefix_sum(values.size() + 1, 0);

        for (auto j = 1U; j <= values.size(); ++j)
            prefix_sum[j] = prefix_sum[j - 1] + values[j - 1];

        for (auto j = 0U; j < values.size(); ++j) {
            auto sum = 0LL;

            auto k = j;
            auto chunk_sz = j + 1;

            while (true) {
                if (k + chunk_sz < values.size()) {
                    sum += prefix_sum[k + chunk_sz] - prefix_sum[k];

                    k += 2 * chunk_sz;
                    if (k >= values.size())
                        break;
                } else {
                    sum += prefix_sum[values.size()] - prefix_sum[k];
                    break;
                }

                if (k + chunk_sz < values.size()) {
                    sum -= prefix_sum[k + chunk_sz] - prefix_sum[k];

                    k += 2 * chunk_sz;
                    if (k >= values.size())
                        break;
                } else {
                    sum -= prefix_sum[values.size()] - prefix_sum[k];
                    break;
                }
            }

            new_values.push_back(abs(sum) % 10);
        }

        swap(values, new_values);
    }

    for (auto i = offset; i < offset + count; ++i)
        std::cout << values[i];
    std::cout << "\n";
}

void fft_repeated(std::vector<int> values,
    unsigned int repeated_x_times, unsigned int iters,
    unsigned int offset, unsigned int count)
{
    const auto sz = values.size();

    if (offset > sz * repeated_x_times - count) {
        std::cout << "N/A\n";
        return;
    }

    for (auto i = 1U; i < repeated_x_times; ++i)
        for (auto j = 0U; j < sz; ++j)
            values.push_back(values[j]);

    fft(std::move(values), iters, offset, count);
}

int main()
{
    std::vector<int> values;

    char d;
    while (std::cin >> d)
        values.push_back(d - '0');

    fft(values, 100, 0, 8);

    auto offset = 0;
    for (auto i = 0; i < 7; ++i)
        offset = 10 * offset + values[i];

    fft_repeated(std::move(values), 10000, 100, offset, 8);

    return 0;
}

