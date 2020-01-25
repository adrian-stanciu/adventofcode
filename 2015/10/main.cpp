#include <iostream>
#include <string>

auto next_seq(const std::string& seq)
{
    std::string next_seq;

    auto pos = 0;
    while (true) {
        auto mismatch_pos = seq.find_first_not_of(seq[pos], pos);
        if (mismatch_pos == std::string::npos) {
            next_seq.append(std::to_string(seq.size() - pos));
            next_seq.push_back(seq[pos]);
            break;
        }

        next_seq.append(std::to_string(mismatch_pos - pos));
        next_seq.push_back(seq[pos]);

        pos = mismatch_pos;
    }

    return next_seq;
}

auto next_seq(const std::string& seq, int iters)
{
    auto next_seq{seq};

    for (auto i = 1; i <= iters; ++i)
        next_seq = ::next_seq(next_seq);

    return next_seq;
}

int main()
{
    std::string seq;
    std::cin >> seq;

    std::cout << next_seq(seq, 40).size() << "\n";
    std::cout << next_seq(seq, 50).size() << "\n";

    return 0;
}

