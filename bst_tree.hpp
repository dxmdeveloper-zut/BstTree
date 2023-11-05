#pragma once

#include <string>
#include <functional>

template<class T>
struct _tree_node {
    size_t uniq_id;

    _tree_node<T> *parent = nullptr;
    _tree_node<T> *left_child = nullptr;
    _tree_node<T> *right_child = nullptr;

    T data;
};

template<class T>
class bst_tree {
private:
    _tree_node<T> *root = nullptr;
    size_t tree_size = 0;
public:
    void add(T data, std::function<int < const T &, const T &>>cmpr);

    T *find(std::function<bool(const T &)> cmpr);

    size_t getHeight();

    void remove_one_if(std::function<bool(const T &)> cmpr);

    void remove(size_t uniq_id);

    void clear();

    std::string to_string();

    ~bst_tree() { clear(); }

private:
    void remove_node(_tree_node<T> *node);

    _tree_node<T> next_preorder(_tree_node<T> *node);

    _tree_node<T> next_inorder(_tree_node<T> *node);
};

template<class T>
_tree_node<T> bst_tree<T>::next_inorder(_tree_node<T> *node) {
    if (node == nullptr) node = root;

    if(!node->right_child) {
        while (!node->parent->right_child || node->parent->right_child == node) {
            node = node->parent;
            if (node == root) return nullptr;
        }
        node = node->parent->right_child;
    } else {
        node->right_child;
    }

    while (node->left_child){
        node = node->left_child;
    }

    return node;
}

template<class T>
_tree_node<T> bst_tree<T>::next_preorder(_tree_node<T> *node) {
    if (node == nullptr) node = root;
    if (node->left_child) return node->left_child;
    if (node->right_child) return node->right_child;
    if (node == root) return nullptr;

    while (!node->parent->right_child || node->parent->right_child == node) {
        node = node->parent;
        if (node == root) return nullptr;
    }

    return node->parent->right_child;
}
