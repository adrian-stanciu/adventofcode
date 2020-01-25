#include <iostream>
#include <vector>

size_t compute_value_after_last_insertion(size_t steps, size_t iters)
{
    std::vector<size_t> cbuf;
    size_t pos = 0;

    cbuf.reserve(iters + 1);
    cbuf.push_back(0);

    for (size_t i = 1; i <= iters; ++i) {
        pos = (pos + steps) % cbuf.size();
        ++pos; // advance where new value will be inserted
        cbuf.insert(cbuf.begin() + pos, i);
    }

    return cbuf[(pos + 1) % cbuf.size()];
}

size_t compute_value_after_0(size_t steps, size_t iters)
{
    size_t value_after_0 = 0;
    size_t pos = 0;

    for (size_t i = 1; i <= iters; ++i) {
        pos = (pos + steps) % i;
        ++pos; // advance where new value will be inserted
        if (pos == 1)
            value_after_0 = i;
    }

    return value_after_0;
}

int main()
{
    size_t steps;
    std::cin >> steps;

    std::cout << compute_value_after_last_insertion(steps, 2017) << "\n";
    std::cout << compute_value_after_0(steps, 50000000) << "\n";

    return 0;
}

