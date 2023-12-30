#include <bitset>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "knot_hash.hpp"

static constexpr auto NumRounds = 64;
static constexpr auto NumElements = 256;
static constexpr auto GroupSz = 16;
static constexpr auto NumGroups = (NumElements / GroupSz);
static constexpr auto BitsetSz = (NumGroups * 8);

auto read_ascii_lengths(const std::string& s)
{
    std::vector<unsigned int> lengths;

    std::stringstream ss(s);
    char c;
    while (ss >> c)
        lengths.push_back(c);

    return lengths;
}

unsigned int count_set_bits(unsigned int n)
{
    unsigned int count = 0;

    while (n) {
        n = n & (n - 1);
        ++count;
    }

    return count;
}

unsigned int count_set_bits(const std::vector<unsigned int>& dense_hash)
{
    unsigned int count = 0;

    for (auto dh_elem : dense_hash)
        count += count_set_bits(dh_elem);

    return count;
}

unsigned int count_used_squares(const std::string& key, size_t size)
{
    unsigned int count = 0;

    for (size_t i = 0; i < size; ++i) {
        auto s = key + "-" + std::to_string(i);
        auto ascii_lengths = read_ascii_lengths(s);

        for (auto extra_len : {17, 31, 73, 47, 23})
            ascii_lengths.push_back(extra_len);

        auto sparse_hash = compute_sparse_hash(NumElements, NumRounds, ascii_lengths);
        auto dense_hash = compute_dense_hash(sparse_hash, GroupSz);

        count += count_set_bits(dense_hash);
    }

    return count;
}

auto build_bitset(const std::vector<unsigned int>& dense_hash)
{
    std::bitset<BitsetSz> bs;
    size_t pos = 0;

    for (auto dh_elem : dense_hash) {
        size_t i = 8;
        do {
            --i;
            bs.set(pos, (dh_elem >> i) & 1);
            ++pos;
        } while (i != 0);
    }

    return bs;
}

void dfs(size_t r, size_t c, std::vector<std::bitset<BitsetSz>>& matrix)
{
    matrix[r].reset(c);

    if ((r != 0) && matrix[r - 1].test(c))
        dfs(r - 1, c, matrix);
    if ((r != matrix.size() - 1) && matrix[r + 1].test(c))
        dfs(r + 1, c, matrix);
    if ((c != 0) && matrix[r].test(c - 1))
        dfs(r, c - 1, matrix);
    if ((c != matrix[r].size() - 1) && matrix[r].test(c + 1))
        dfs(r, c + 1, matrix);
}

unsigned int count_regions(std::vector<std::bitset<BitsetSz>>& matrix)
{
    unsigned int count = 0;

    for (size_t i = 0; i <  matrix.size(); ++i)
        for (size_t j = 0; j < matrix[i].size(); ++j)
            if (matrix[i].test(j)) {
                ++count;
                dfs(i, j, matrix);
            }

    return count;
}

unsigned int count_regions(const std::string& key, size_t size)
{
    std::vector<std::bitset<BitsetSz>> matrix;

    for (size_t i = 0; i < size; ++i) {
        auto s = key + "-" + std::to_string(i);
        auto ascii_lengths = read_ascii_lengths(s);

        for (auto extra_len : {17, 31, 73, 47, 23})
            ascii_lengths.push_back(extra_len);

        auto sparse_hash = compute_sparse_hash(NumElements, NumRounds, ascii_lengths);
        auto dense_hash = compute_dense_hash(sparse_hash, GroupSz);

        matrix.push_back(build_bitset(dense_hash));
    }

    return count_regions(matrix);
}

int main()
{
    std::string key;
    getline(std::cin, key);

    std::cout << count_used_squares(key, 128) << "\n";
    std::cout << count_regions(key, 128) << "\n";

    return 0;
}

