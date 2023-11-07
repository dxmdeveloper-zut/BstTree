#pragma once

#include <functional>
#include <string>
#include <sstream>
#include <valarray>

template<class T>
struct _tree_node {
    size_t uniq_id;

    _tree_node<T> *parent = nullptr;
    _tree_node<T> *left_child = nullptr;
    _tree_node<T> *right_child = nullptr;

    T data;

    _tree_node(T data, size_t uniq_id)
            : data(data), uniq_id(uniq_id) {};
};

template<class T>
struct node_with_depth {
    _tree_node<T> *node;
    size_t depth;
};

template<class T>
class bst_tree {
private:
    _tree_node<T> *root = nullptr;
    size_t tree_size = 0;
    size_t uniq_id_cnt = 0;

public:
    ///@param cmpr - compare function, should return negative value if a < b, positive if a > b, 0 if a == b. Use a - b if possible.
    void add(T data, std::function<int(const T &, const T &)> cmpr);

    T *find(const T &data, std::function<bool(const T &, const T &)> cmpr) const;

    size_t get_height() const;

    bool remove(const T &data, std::function<bool(const T &, const T &)> cmp);

    void remove(size_t uniq_id);

    void clear();

    std::string to_string() const;

    std::string to_html() const;

    ~bst_tree() { clear(); }

private:

    bool remove_node(_tree_node<T> *node);

    _tree_node<T> *next_preorder(_tree_node<T> *node) const;

    node_with_depth<T> next_preorder(node_with_depth<T> node_w_depth) const;

    _tree_node<T> *next_inorder(_tree_node<T> *node) const;
};

template<class T>
void bst_tree<T>::clear() {
    _tree_node<T> *node = nullptr;
    while ((node = next_inorder(nullptr))) {
        remove_node(node);
    }

}

template<class T>
void bst_tree<T>::remove(size_t uniq_id) {
    _tree_node<T> node = nullptr;
    while ((node = next_preorder(node))) {
        if (node->uniq_id == uniq_id) {
            remove_node(node);
            return;
        }
    }
}

template<class T>
bool bst_tree<T>::remove(const T &data, std::function<bool(const T &, const T &)> cmp) {
    _tree_node<T> *node = find(data, cmp);
    if (node == nullptr) return false;
    return remove_node(node);
}

template<class T>
bool bst_tree<T>::remove_node(_tree_node<T> *node) {
    if (root == nullptr) return false;

    if (node == root) {
        // when node is a root
        if (node->left_child) {
            root = node->left_child;
            root->right_child = node->right_child;
            root->parent = nullptr;
            if (node->right_child)
                node->right_child->parent = root; // new root pointer assignment
        } else if (node->right_child) {
            root = node->right_child;

            root->left_child = nullptr;
            root->parent = nullptr;
        } else root = nullptr;

    } else {
        // node is not a root case
        _tree_node<T> *replnode = nullptr;
        if (node->left_child) {
            replnode = node->left_child;
            while (replnode->right_child) {
                replnode = replnode->right_child;
            }
        } else if (node->right_child) {
            replnode = node->right_child;
            while (replnode->left_child) {
                replnode = replnode->left_child;
            }
        }
        if (replnode) {

            if (replnode->parent->right_child == replnode)
                replnode->parent->right_child = replnode->left_child ? replnode->left_child : nullptr;
            else replnode->parent->left_child = replnode->right_child ? replnode->right_child : nullptr;

            node->data = replnode->data;
            node->uniq_id = replnode->uniq_id;

            node = replnode;
            // replnode->left_child = node->left_child;
            // replnode->right_child = node->right_child;
            // replnode->parent = node->parent;
        } else {
            if (node->parent->left_child == node) node->parent->left_child = nullptr;
            else node->parent->right_child = nullptr;
        }
    }
    delete node;
    tree_size--;
    return true;
}

template<class T>
T *bst_tree<T>::find(const T &data, std::function<bool(const T &, const T &)> cmpr) const {
    if (root == nullptr) return nullptr;
    auto *node = root;
    int res;

    while (node && (res = cmpr(data, node->data))) {
        if (res <= -1) node = node->left_child;
        else node = node->right_child;
    }
    return node;
}

template<class T>
std::string bst_tree<T>::to_string() const {
    std::stringstream ss{};
    ss << "{\n\theight: " << get_height() << ",\n\tsize: " << tree_size << ",\n\troot: " << root;

    ss << ",\n\tvalues inorder: [";

    _tree_node<T> *node = nullptr;
    while ((node = next_inorder(node)) != nullptr) {
        ss << node->data << ", ";
    }
    ss.seekp(-2, std::ios_base::end);

    ss << "],\n\tvalues preorder: [";
    node = nullptr;
    while ((node = next_preorder(node)) != nullptr) {
        ss << node->data << ", ";
    }
    ss.seekp(-2, std::ios_base::end);
    ss << "]\n}";

    return ss.str();
}

template<class T>
size_t bst_tree<T>::get_height() const {
    if (root == nullptr) return 0;

    node_with_depth<T> node_w_depth{nullptr, 0};
    size_t max_depth = 0;
    while ((node_w_depth = next_preorder(node_w_depth)).node != nullptr) {
        max_depth = std::max(node_w_depth.depth, max_depth);
    }
    return max_depth + 1;
}

template<class T>
std::string bst_tree<T>::to_html() const {
    auto *root = this->root;
    auto h = this->get_height();
    auto get_node_of_lvl = [root](size_t level, size_t ind) -> _tree_node<T> * {
        auto *node = root;
        for (size_t i = 0; i < level; i++) {
            node = (ind >> (level - i - 1)) & 1 ? node->right_child : node->left_child;
            if (node == nullptr) return nullptr;
        }
        return node;
    };

    if (root == nullptr) return "";

    std::stringstream ss{};
    ss << "<table><tr><td colspan=\"8098\">";
    ss << root->data << "</tr></td>\n";


    for (size_t i = 1; i < h; i++) {
        ss << "<tr>";

        for (size_t ii = 0; ii < (1 << i); ii++) {
            auto *node = get_node_of_lvl(i, ii);
            if (i != h - 1)
                ss << "<td colspan=\"" << (1 << (h - i - 1)) << "\">";
            else ss << "<td>";

            if (node != nullptr)
                ss << "<sub>[" << node->uniq_id << "]</sub> " << node->data;

            ss << "</td>";
        }
        ss << "</td>\n";
    }
    ss << "</table>";
    return ss.str();
}

template<class T>
void bst_tree<T>::add(T data, std::function<int(const T &, const T &)> cmpr) {
    auto *node = new _tree_node<T>(data, uniq_id_cnt++);
    tree_size++;
    if (root == nullptr) {
        root = node;
        return;
    }

    for (auto *cur_node = root; true;) {
        if (cmpr(data, cur_node->data) <= -1) {
            if (!cur_node->left_child) {
                cur_node->left_child = node;
                node->parent = cur_node;
                return;
            }
            cur_node = cur_node->left_child;
        } else {
            if (!cur_node->right_child) {
                cur_node->right_child = node;
                node->parent = cur_node;
                return;
            }
            cur_node = cur_node->right_child;
        }
    }
}

template<class T>
_tree_node<T> *bst_tree<T>::next_inorder(_tree_node<T> *node) const {
    if (root == nullptr) return nullptr;

    if (node == nullptr || node->right_child) {
        node = node ? node->right_child : root;
        while (node->left_child) {
            node = node->left_child;
        }
        return node;
    }

    while (node->parent && node->parent->right_child == node) {
        node = node->parent;
    }
    if (!node->parent) return nullptr;


    return node->parent;
}

template<class T>
node_with_depth<T> bst_tree<T>::next_preorder(node_with_depth<T> node_w_depth) const {
    if (root == nullptr) return {nullptr, 0};

    auto *node = node_w_depth.node;
    auto depth = node_w_depth.depth;

    if (node == nullptr)
        return {root, 0};
    if (node->left_child)
        return {node->left_child, depth + 1};
    if (node->right_child)
        return {node->right_child, depth + 1};
    if (node == root)
        return {nullptr, 0};

    while (!node->parent->right_child || node->parent->right_child == node) {
        node = node->parent;
        depth--;
        if (node == root)
            return {nullptr, 0};
    }

    return {node->parent->right_child, depth};
}

template<class T>
_tree_node<T> *bst_tree<T>::next_preorder(_tree_node<T> *node) const {
    return next_preorder({node, 0}).node;
}
