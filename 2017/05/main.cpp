#include <iostream>
#include <vector>

auto read_vector()
{
    std::vector<int> vec;

    int n;
    while (std::cin >> n)
        vec.push_back(n);

    return vec;
}

auto count1(std::vector<int> vec)
{
    auto count = 0;
    auto index = 0;

    while ((0 <= index) && (index < static_cast<int>(vec.size()))) {
        ++count;

        auto jump = vec[index];
        ++vec[index];
        index += jump;
    }

    return count;
}

auto count2(std::vector<int> vec, int threshold)
{
    auto count = 0;
    auto index = 0;

    while ((0 <= index) && (index < static_cast<int>(vec.size()))) {
        ++count;

        auto jump = vec[index];
        if (jump >= threshold)
            --vec[index];
        else
            ++vec[index];
        index += jump;
    }

    return count;
}

int main()
{
    auto vec = read_vector();

    std::cout << count1(vec) << "\n";
    std::cout << count2(vec, 3) << "\n";

    return 0;
}

