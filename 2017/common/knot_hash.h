#pragma once

#include <algorithm>
#include <vector>

namespace {
    void circular_reverse(std::vector<unsigned int>& code, size_t first, size_t len)
    {
        size_t size = code.size();
        size_t last = first + len;

        if (last < size)
            reverse(code.begin() + first, code.begin() + last);
        else {
            auto off = last - size;

            rotate(code.begin(), code.begin() + off, code.end());
            reverse(code.begin() + first - off, code.begin() + last - off);
            rotate(code.rbegin(), code.rbegin() + off, code.rend());
        }
    }

    auto compute_sparse_hash(size_t size, size_t rounds,
        const std::vector<unsigned int>& lengths)
    {
        std::vector<unsigned int> code;

        code.reserve(size);
        for (size_t i = 0; i < size; ++i)
            code.push_back(i);

        size_t pos = 0;
        size_t skip = 0;

        for (size_t i = 0; i < rounds; ++i)
            for (auto len : lengths) {
                circular_reverse(code, pos, len);

                pos += len + skip;
                pos = pos % size;

                ++skip;
            }

        return code;
    }

    auto compute_dense_hash(const std::vector<unsigned int>& sparse_hash, size_t group_size)
    {
        std::vector<unsigned int> dense_hash;
        size_t current_group_size = 0;
        size_t dh_elem = 0;

        for (auto sh_elem : sparse_hash) {
            dh_elem ^= sh_elem;

            ++current_group_size;
            if (current_group_size == group_size) {
                dense_hash.push_back(dh_elem);
                current_group_size = 0;
                dh_elem = 0;
            }
        }

        return dense_hash;
    }
}

