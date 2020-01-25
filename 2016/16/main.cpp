#include <algorithm>
#include <iostream>
#include <string>

auto generate_extension(const std::string& data)
{
    auto extension = data;

    reverse(extension.begin(), extension.end());

    transform(extension.begin(), extension.end(), extension.begin(),
        [] (const auto& c) {
            return '0' + ('1' - c);
        });

    return extension;
}

auto generate_data(std::string data, unsigned int size)
{
    while (data.size() < size) {
        auto extension = generate_extension(data);
        data.push_back('0');
        data.append(extension);
    }

    return data.substr(0, size);
}

auto compute_csum(const std::string& data)
{
    std::string csum;
    csum.reserve(data.size() / 2);

    for (auto i = 0U; i < data.size() - 1; i += 2)
        if (data[i] == data[i + 1])
            csum.push_back('1');
        else
            csum.push_back('0');

    return csum;
}

auto compute_csum_rec(const std::string& data)
{
    auto csum = compute_csum(data);

    while (csum.size() % 2 != 1)
        csum = compute_csum(csum);

    return csum;
}

int main()
{
    std::string init_data;
    std::cin >> init_data;

    std::cout << compute_csum_rec(generate_data(init_data, 272)) << "\n";
    std::cout << compute_csum_rec(generate_data(init_data, 35651584)) << "\n";

    return 0;
}

