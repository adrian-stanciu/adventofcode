#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

auto read_pipes()
{
    std::vector<std::vector<unsigned long>> pipes;

    std::string line;
    while (getline(std::cin, line)) {
        // it assumes that the left ends are sorted in ascending order
        static const std::regex re{"[0-9]+ <-> (.*)"};

        std::smatch matched;
        regex_match(line, matched, re);
        std::stringstream ss(matched[1].str());

        std::vector<unsigned long> right_ends;

        unsigned long right_end;
        while (ss >> right_end) {
            right_ends.push_back(right_end);
            ss.ignore(); // skip ','
        }

        pipes.push_back(std::move(right_ends));
    }

    return pipes;
}

unsigned long compute_subtree_size(unsigned long root,
    const std::vector<std::vector<unsigned long>>& pipes,
    std::vector<bool>& visited)
{
    unsigned long size = 0;

    std::queue<unsigned long> q;
    q.push(root);

    while (!q.empty()) {
        unsigned long current = q.front();
        q.pop();

        visited[current] = true;
        ++size;

        for (auto pipe_end : pipes[current])
            if (!visited[pipe_end])
                q.push(pipe_end);
    }

    return size;
}

unsigned long count_groups(const std::vector<std::vector<unsigned long>>& pipes)
{
    unsigned long count = 0;

    std::vector<bool> visited;
    visited.resize(pipes.size());

    for (size_t i = 0; i < pipes.size(); ++i)
        if (!visited[i]) {
            ++count;
            compute_subtree_size(i, pipes, visited);
        }

    return count;
}

int main()
{
    auto pipes = read_pipes();

    std::vector<bool> visited;
    visited.resize(pipes.size());
    std::cout << compute_subtree_size(0, pipes, visited) << "\n";

    std::cout << count_groups(pipes) << "\n";

    return 0;
}

