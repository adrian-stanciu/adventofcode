#include <limits>
#include <iostream>
#include <vector>

using Image = std::vector<std::vector<char>>;

enum Color : char {
    Black = '0',
    White = '1',
    Transparent = '2',
};

constexpr auto LayerW = 25;
constexpr auto LayerH = 6;
constexpr auto LayerSz = LayerW * LayerH;

auto compute_csum(const Image& img)
{
    auto csum = 0;

    auto min_0 = std::numeric_limits<int>::max();

    for (auto layer = 0U; layer < img.size(); ++layer) {
        auto c0 = 0;
        auto c1 = 0;
        auto c2 = 0;

        for (auto pixel = 0; pixel < LayerSz; ++pixel)
            if (img[layer][pixel] == '0')
                ++c0;
            else if (img[layer][pixel] == '1')
                ++c1;
            else if (img[layer][pixel] == '2')
                ++c2;

        if (c0 < min_0) {
            min_0 = c0;
            csum = c1 * c2;
        }
    }

    return csum;
}

auto decode(const Image& img)
{
    std::vector<char> dec_img(LayerSz);

    for (auto pixel = 0; pixel < LayerSz; ++pixel)
        for (auto layer = 0U; layer < img.size(); ++layer)
            if (img[layer][pixel] == Color::Transparent) {
                continue;
            } else if (img[layer][pixel] == Color::Black) {
                dec_img[pixel] = ' ';
                break;
            } else if (img[layer][pixel] == Color::White) {
                dec_img[pixel] = '#';
                break;
            }

    for (auto pixel = 0; pixel < LayerSz; ++pixel) {
        std::cout << dec_img[pixel];
        if ((pixel + 1) % LayerW == 0)
            std::cout << "\n";
    }
}

int main()
{
    Image img;

    char c;
    std::vector<char> layer;
    while (std::cin >> c) {
        layer.push_back(c);

        if (layer.size() % LayerSz == 0)
            img.push_back(std::move(layer));
    }

    std::cout << compute_csum(img) << "\n";

    decode(img);

    return 0;
}

