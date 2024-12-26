#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "knot_hash.hpp"

static constexpr auto NumRounds = 64;
static constexpr auto NumElements = 256;
static constexpr auto GroupSz = 16;

auto read_lengths(const std::string& s)
{
    std::vector<unsigned int> lengths;

    std::stringstream ss(s);
    std::string val;
    while (getline(ss, val, ','))
        lengths.push_back(strtoul(val.data(), nullptr, 10));

    return lengths;
}

auto read_ascii_lengths(const std::string& s)
{
    std::vector<unsigned int> lengths;

    std::stringstream ss(s);
    char c;
    while (ss >> c)
        lengths.push_back(c);

    return lengths;
}

void print_dense_hash(const std::vector<unsigned int>& dense_hash)
{
    // save old flags
    const std::ios::fmtflags old_flags(std::cout.flags());

    // change flags
    std::cout << std::setfill('0') << std::hex;

    for (auto dh_elem : dense_hash)
        std::cout << std::setw(2) << dh_elem;
    std::cout << "\n";

    // restore old flags
    std::cout.flags(old_flags);
}

int main()
{
    std::string input;
    getline(std::cin, input);

    auto lengths = read_lengths(input);
    auto ascii_lengths = read_ascii_lengths(input);

    auto hash_code = compute_sparse_hash(NumElements, 1, lengths);
    std::cout << hash_code[0] * hash_code[1] << "\n";

    for (auto extra_len : {17, 31, 73, 47, 23})
        ascii_lengths.push_back(extra_len);

    auto sparse_hash = compute_sparse_hash(NumElements, NumRounds, ascii_lengths);
    auto dense_hash = compute_dense_hash(sparse_hash, GroupSz);

    print_dense_hash(dense_hash);

    return 0;
}

