#include <iostream>
#include <optional>
#include <vector>

auto find_winner(int n)
{
    auto first = 1;
    auto step = 1;
    auto count = n;

    while (count > 1) {
        step *= 2;

        if (count % 2 == 1)
            first = first + step;

        count = count / 2;
    }

    return first;
}

template <typename T>
struct RedBlackTree {

    [[nodiscard]] unsigned long long size() const
    {
        return count;
    }

    void insert(T key)
    {
        root = insert(root, key);
        ++count;
    }

    void erase(T key)
    {
        if (erase(root, key))
            --count;
    }

    void erase_n_key(unsigned long long n)
    {
        if (n > 0 && n <= count) {
            erase_n_key(root, n);
            --count;
        }
    }

    [[nodiscard]] unsigned long long less_than(T key) const
    {
        return less_than(root, key);
    }

    [[nodiscard]] std::optional<T> n_key(unsigned long long n) const
    {
        if (n == 0 || n > count)
            return std::nullopt;

        return n_key(root, n);
    }

    ~RedBlackTree()
    {
        delete root;
    }

private:
    struct Node {
        T key;
        unsigned long long count {1}; // frequency of current key
        unsigned long long left_count {0}; // number of keys less that current key
        bool is_red {true};
        Node *left {nullptr};
        Node *right {nullptr};

        Node(T key)
            : key(key)
        {}

        ~Node()
        {
            delete left;
            delete right;
        }
    };

    [[nodiscard]] bool is_red(Node *root) const
    {
        return root && root->is_red;
    }

    [[nodiscard]] Node *rotate_left(Node *root)
    {
        Node *right = root->right;
        root->right = right->left;
        right->left = root;

        right->left_count += root->left_count + root->count;

        right->is_red = root->is_red;
        root->is_red = true;

        return right;
    }

    [[nodiscard]] Node *rotate_right(Node *root)
    {
        Node *left = root->left;
        root->left = left->right;
        left->right = root;

        root->left_count -= left->left_count + left->count;

        left->is_red = root->is_red;
        root->is_red = true;

        return left;
    }

    void flip_color(Node *root)
    {
        root->is_red = true;
        root->left->is_red = false;
        root->right->is_red = false;
    }

    [[nodiscard]] Node *insert(Node *root, T key)
    {
        if (!root)
            return new Node(key);

        if (key < root->key) {
            root->left = insert(root->left, key);
            ++root->left_count;
        } else if (key > root->key) {
            root->right = insert(root->right, key);
        } else {
            ++root->count;
        }

        if (is_red(root->right) && !is_red(root->left))
            root = rotate_left(root);
        if (is_red(root->left) && is_red(root->left->left))
            root = rotate_right(root);
        if (is_red(root->left) && is_red(root->right))
            flip_color(root);

        return root;
    }

    // logical deletion
    // returns true if the key was deleted, false otherwise
    bool erase(Node *root, T key)
    {
        if (!root)
            return false;

        if (key < root->key) {
            if (erase(root->left, key)) {
                --root->left_count;
                return true;
            } else
                return false;
        } else if (key > root->key) {
            return erase(root->right, key);
        } else {
            if (root->count > 0) {
                --root->count;
                return true;
            } else
                return false;
        }
    }

    // logical deletion
    void erase_n_key(Node *root, unsigned long long n)
    {
        if (root->left_count >= n) {
            erase_n_key(root->left, n);
            --root->left_count;
        } else if (root->left_count + root->count >= n) {
            --root->count;
        } else {
            erase_n_key(root->right, n - root->left_count - root->count);
        }
    }

    [[nodiscard]] unsigned long long less_than(Node *root, T key) const
    {
        if (!root)
            return 0;

        if (key < root->key)
            return less_than(root->left, key);
        else if (key > root->key)
            return root->left_count + root->count + less_than(root->right, key);
        else
            return root->left_count;
    }

    [[nodiscard]] T n_key(Node *root, unsigned long long n) const
    {
        if (root->left_count >= n)
            return n_key(root->left, n);
        else if (root->left_count + root->count >= n)
            return root->key;
        else
            return n_key(root->right, n - root->left_count - root->count);
    }

    Node *root {nullptr};
    unsigned long long count {0};
};

auto find_winner_across_circle(int n)
{
    RedBlackTree<int> circle;

    for (auto i = 1; i <= n; ++i)
        circle.insert(i);

    auto i = 0U;

    while (circle.size() > 1) {
        auto target = (i + circle.size() / 2) % circle.size();

        circle.erase_n_key(target + 1);

        if (target > i)
            ++i;

        if (i >= circle.size())
            i = 0;
    }

    return *circle.n_key(1);
}

int main()
{
    int n;
    std::cin >> n;

    std::cout << find_winner(n) << "\n";
    std::cout << find_winner_across_circle(n) << "\n";

    return 0;
}

