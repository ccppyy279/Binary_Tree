#pragma once
#include <iostream>
#include <string>

#include "BinaryTree.hpp"

template <typename T>
class BinarySearchTree : public BinaryTree<T> {
   protected:
    //用于比较元素大小的函数指针
    int (*m_compare)(const T&, const T&);

   public:
    template <typename S>
    friend std::ostream& operator<<(std::ostream& cout,
                                    const BinarySearchTree<S>& bst);
    explicit BinarySearchTree(int (*compare)(const T&, const T&) =
                                  [](const T& t1, const T& t2) {
                                      if (t1 < t2) return -1;
                                      if (t1 > t2) return 1;
                                      return 0;
                                  });
    ~BinarySearchTree() = default;
    void add(T element) override;
    void remove(T elment) override;
    bool isContain(T element) const override;

   protected:
    virtual void afterAdd(Node<T>* node) {}
    virtual void afterRemove(Node<T>* node) {}
    virtual Node<T>* createNewNode(T element, Node<T>* parent);

   private:
    Node<T>* getNode(T element) const;
    void print(Node<T>* node, std::string str) const;
};
template <typename T>
BinarySearchTree<T>::BinarySearchTree(int (*compare)(const T&, const T&))
    : m_compare(compare) {}

template <typename T>
Node<T>* BinarySearchTree<T>::getNode(T element) const {
    Node<T>* node = this->m_root;
    while (node != nullptr) {
        int cmp = this->m_compare(element, node->m_element);
        if (cmp == 0) {
            return node;
        }
        if (cmp > 0) {
            node = node->m_right;
        } else {
            node = node->m_left;
        }
    }
    return nullptr;
}

template <typename T>
void BinarySearchTree<T>::add(T element) {
    if (this->m_root == nullptr) {
        this->m_root = this->createNewNode(element, nullptr);
        ++this->m_size;
        return;
    }
    Node<T>* node = this->m_root;
    Node<T>* parent = this->m_root;
    int cmp;
    while (node != nullptr) {
        parent = node;
        cmp = this->m_compare(element, node->m_element);
        if (cmp < 0) {
            node = node->m_left;
        } else if (cmp > 0) {
            node = node->m_right;
        } else {
            //相等 覆盖
            node->m_element = element;
            return;
        }
    }
    Node<T>* newNode = this->createNewNode(element, parent);
    if (cmp > 0) {
        parent->m_right = newNode;
    } else if (cmp < 0) {
        parent->m_left = newNode;
    }
    ++this->m_size;
    this->afterAdd(newNode);
}

template <typename T>
void BinarySearchTree<T>::remove(T element) {
    Node<T>* node = this->getNode(element);
    if (node == nullptr) return;

    if (node->hasTwoChildren()) {
        /**
         * @brief
         * 删除度=2的节点，为了保证删除后仍为BST，用前驱节点/后继节点代替原节点，
         * 转换为删除前驱节点/后继节点(度=0/1)
         */
        Node<T>* sucNode = this->successor(node);
        node->m_element = sucNode->m_element;
        node = sucNode;
        //统一逻辑转换为删除node
    }
    Node<T>* child = node->m_left != nullptr ? node->m_left : node->m_right;

    if (child == nullptr) {
        //叶子节点
        if (node == this->m_root) {
            //根节点
            this->m_root = nullptr;
        } else {
            if (node == node->m_parent->m_left) {
                node->m_parent->m_left = nullptr;
            } else {
                node->m_parent->m_right = nullptr;
            }
        }
    } else {
        //度=1的节点
        child->m_parent = node->m_parent;
        if (node == this->m_root) {
            //根节点
            this->m_root = child;
        } else {
            if (node == node->m_parent->m_left) {
                node->m_parent->m_left = child;
            } else {
                node->m_parent->m_right = child;
            }
        }
    }
    this->afterRemove(node);
    delete node;
    --this->m_size;
}

template <typename T>
bool BinarySearchTree<T>::isContain(T element) const {
    return this->getNode(element) != nullptr;
}
template <typename T>
std::ostream& operator<<(std::ostream& cout, const BinarySearchTree<T>& bst) {
    bst.print(bst.m_root, "");
    return cout;
}

/**
 * @brief 前序遍历打印
 *
 * @tparam T
 * @param node
 */
template <typename T>
void BinarySearchTree<T>::print(Node<T>* node, std::string str) const {
    if (node == nullptr) return;
    std::cout << str << node->m_element << std::endl;
    this->print(node->m_left, str + "[L]");
    this->print(node->m_right, str + "[R]");
}

template <typename T>
Node<T>* BinarySearchTree<T>::createNewNode(T element, Node<T>* parent) {
    return new Node<T>(element, parent);
}