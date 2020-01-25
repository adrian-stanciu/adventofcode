#include <deque>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Tree = std::unordered_map<std::string, std::vector<std::string>>;

auto dfs_count(const Tree& tree, const std::string& node, int level,
    std::unordered_set<std::string>& visited, int& cnt)
{
    if (visited.find(node) != visited.end())
        return;

    visited.insert(node);

    cnt += level;

    if (tree.count(node))
        for (const auto& child : tree.at(node))
            if (visited.find(child) == visited.end())
                dfs_count(tree, child, level + 1, visited, cnt);
}

auto count_orbits(const Tree& tree)
{
    std::unordered_set<std::string> visited;
    auto cnt = 0;

    dfs_count(tree, "COM", 0, visited, cnt);

    return cnt;
}

auto dfs_path(const Tree& tree, const std::string& node, const std::string& target,
    std::unordered_set<std::string>& visited, std::deque<std::string>& path, bool& found)
{
    if (found)
        return;

    if (visited.find(node) != visited.end())
        return;

    visited.insert(node);

    if (node == target) {
        found = true;
        return;
    }

    path.push_back(node);

    if (tree.count(node))
        for (const auto& child : tree.at(node))
            if (visited.find(child) == visited.end())
                dfs_path(tree, child, target, visited, path, found);

    if (!found)
        path.pop_back();
}

auto get_path_to(const Tree& tree, const std::string& target)
{
    std::unordered_set<std::string> visited;
    std::deque<std::string> path;
    bool found = false;

    dfs_path(tree, "COM", target, visited, path, found);

    return path;
}

auto count_dist(const Tree& tree)
{
    auto you_path = get_path_to(tree, "YOU");
    auto san_path = get_path_to(tree, "SAN");

    for (auto i = 0U; i < std::min(you_path.size(), san_path.size()); ++i)
        if (you_path[i] != san_path[i])
            return you_path.size() - i + san_path.size() - i;

    return 0UL;
}

int main()
{
    Tree tree;

    std::string line;
    while (getline(std::cin, line)) {
        std::stringstream ss(line);

        std::string parent;
        getline(ss, parent, ')');
        std::string child;
        getline(ss, child);

        tree[std::move(parent)].push_back(std::move(child));
    }

    std::cout << count_orbits(tree) << "\n";
    std::cout << count_dist(tree) << "\n";

    return 0;
}

