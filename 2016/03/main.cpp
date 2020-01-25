#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using Sides = std::tuple<int, int, int>;

bool is_valid_triangle(const Sides& sides)
{
    const auto& [l1, l2, l3] = sides;
    return l1 + l2 > l3 && l2 + l3 > l1 && l3 + l1 > l2;
}

auto count_triangles(const std::vector<Sides>& sides_vec)
{
    auto cnt = 0;

    for (const auto& sides : sides_vec)
        if (is_valid_triangle(sides))
            ++cnt;

    return cnt;
}

int main()
{
    std::vector<Sides> sides_vec;

    int l1, l2, l3;
    while (std::cin >> l1 >> l2 >> l3)
        sides_vec.push_back({l1, l2, l3});

    std::cout << count_triangles(sides_vec) << "\n";

    std::vector<Sides> sides_vec2;

    for (auto i = 0U; i < sides_vec.size() - 2; i += 3) {
        sides_vec2.push_back({std::get<0>(sides_vec[i]),
            std::get<0>(sides_vec[i + 1]),
            std::get<0>(sides_vec[i + 2])});
        sides_vec2.push_back({std::get<1>(sides_vec[i]),
            std::get<1>(sides_vec[i + 1]),
            std::get<1>(sides_vec[i + 2])});
        sides_vec2.push_back({std::get<2>(sides_vec[i]),
            std::get<2>(sides_vec[i + 1]),
            std::get<2>(sides_vec[i + 2])});
    }

    std::cout << count_triangles(sides_vec2) << "\n";

    return 0;
}

