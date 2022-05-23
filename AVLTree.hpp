#pragma once
#include <cmath>

#include "BinarySearchTree.hpp"
/**
 * @brief AVLNode:多维护高度
 *
 * @tparam T
 */
template <typename T>
struct AVLNode : Node<T> {
    int m_height{1};
    AVLNode(T element, Node<T>* parent);
    void updateHeight();
    int balanceFactor();
    Node<T>* tallerChild();
};
template <typename T>
AVLNode<T>::AVLNode(T element, Node<T>* parent) : Node<T>(element, parent) {}

template <typename T>
void AVLNode<T>::updateHeight() {
    int leftHeight = this->m_left != nullptr
                         ? static_cast<AVLNode<T>*>(this->m_left)->m_height
                         : 0;
    int RightHeight = this->m_right != nullptr
                          ? static_cast<AVLNode<T>*>(this->m_right)->m_height
                          : 0;
    this->m_height = _MAX(leftHeight, RightHeight) + 1;
}
template <typename T>
int AVLNode<T>::balanceFactor() {
    int leftHeight = this->m_left != nullptr
                         ? static_cast<AVLNode<T>*>(this->m_left)->m_height
                         : 0;
    int RightHeight = this->m_right != nullptr
                          ? static_cast<AVLNode<T>*>(this->m_right)->m_height
                          : 0;
    return leftHeight - RightHeight;
}

template <typename T>
Node<T>* AVLNode<T>::tallerChild() {
    if (this->m_left == nullptr && this->m_right != nullptr) {
        return this->m_right;
    } else if (this->m_left != nullptr && this->m_right == nullptr) {
        return this->m_left;
    } else if (this->m_left == nullptr && this->m_right == nullptr) {
        return nullptr;
    } else {
        int rightHeight = static_cast<AVLNode<T>*>(this->m_right)->m_height;
        int leftHeight = static_cast<AVLNode<T>*>(this->m_left)->m_height;
        if (rightHeight > leftHeight) {
            return this->m_right;
        } else if (rightHeight < leftHeight) {
            return this->m_left;
        } else {
            //高度相等返回同方向
            return this->isLeftChild() ? this->m_left : this->m_right;
        }
    }
}
/**
 * @brief AVLTree
 * add和remove后恢复平衡
 * @tparam T
 */
template <typename T>
class AVLTree : public BinarySearchTree<T> {
   public:
    template <typename S>
    friend std::ostream& operator<<(std::ostream& cout, const AVLTree<S>& bst);
    explicit AVLTree(int (*compare)(const T&, const T&) = [](const T& t1,
                                                             const T& t2) {
        if (t1 < t2) return -1;
        if (t1 > t2) return 1;
        return 0;
    });
    ~AVLTree() = default;

   private:
    void afterAdd(Node<T>* node) override;
    void afterRemove(Node<T>* node) override;
    AVLNode<T>* createNewNode(T element, Node<T>* parent) override;
    bool isBalanced(Node<T>* node);
    void updateHeight(Node<T>* node);
    void rebalance(Node<T>* node);
    void rebalance2(Node<T>* node);
    void rotate(Node<T>* r,  //子树根节点
                Node<T>* b, Node<T>* c, Node<T>* d, Node<T>* e, Node<T>* f);
    void rotateRight(Node<T>* node);
    void rotateLeft(Node<T>* node);
    void afterRotate(Node<T>* grand, Node<T>* parent, Node<T>* son);
    void print(Node<T>* node, std::string str) const;
};
template <typename T>
AVLTree<T>::AVLTree(int (*compare)(const T&, const T&))
    : BinarySearchTree<T>(compare) {}
template <typename T>
bool AVLTree<T>::isBalanced(Node<T>* node) {
    return std::abs(static_cast<AVLNode<T>*>(node)->balanceFactor()) <= 1;
}
template <typename T>
void AVLTree<T>::updateHeight(Node<T>* node) {
    static_cast<AVLNode<T>*>(node)->updateHeight();
}
template <typename T>
void AVLTree<T>::afterAdd(Node<T>* node) {
    while ((node = node->m_parent) != nullptr) {
        if (this->isBalanced(node)) {
            //更新高度
            this->updateHeight(node);
        } else {
            //不平衡：恢复平衡
            this->rebalance(node);
            break;
        }
    }
}
template <typename T>
void AVLTree<T>::afterRemove(Node<T>* node) {
    while ((node = node->m_parent) != nullptr) {
        if (this->isBalanced(node)) {
            this->updateHeight(node);
        } else {
            this->rebalance(node);
        }
    }
}

template <typename T>
void AVLTree<T>::rotateRight(Node<T>* grand) {
    Node<T>* parent = grand->m_left;
    Node<T>* son = parent->m_right;
    grand->m_left = parent->m_right;
    parent->m_right = grand;
    this->afterRotate(grand, parent, son);
}
template <typename T>
void AVLTree<T>::rotateLeft(Node<T>* grand) {
    Node<T>* parent = grand->m_right;
    Node<T>* son = parent->m_left;
    grand->m_right = parent->m_left;
    parent->m_left = grand;
    this->afterRotate(grand, parent, son);
}

/**
 * @brief 旋转的公共代码
 *
 * @tparam T
 * @param grand
 * @param parent
 * @param son
 */
template <typename T>
void AVLTree<T>::afterRotate(Node<T>* grand, Node<T>* parent, Node<T>* son) {
    //让parent成为子树的根节点
    if (grand->isLeftChild()) {
        grand->m_parent->m_left = parent;
    } else if (grand->isRightChild()) {
        grand->m_parent->m_right = parent;
    } else {
        // grand是根节点
        this->m_root = parent;
    }
    //更新parent
    if (son != nullptr) {
        son->m_parent = grand;
    }
    parent->m_parent = grand->m_parent;
    grand->m_parent = parent;
    //更新g，p高度
    this->updateHeight(grand);
    this->updateHeight(parent);
}
template <typename T>
void AVLTree<T>::rebalance(Node<T>* grand) {
    Node<T>* parent = static_cast<AVLNode<T>*>(grand)->tallerChild();
    Node<T>* node = static_cast<AVLNode<T>*>(parent)->tallerChild();
    if (parent->isLeftChild() && node->isLeftChild()) {
        // LL
        this->rotateRight(grand);
    } else if (parent->isRightChild() && node->isRightChild()) {
        // RR
        this->rotateLeft(grand);
    } else if (parent->isLeftChild() && node->isRightChild()) {
        // LR
        this->rotateLeft(parent);
        this->rotateRight(grand);
    } else {
        // RL
        this->rotateRight(parent);
        this->rotateLeft(grand);
    }
}

/**
 * @brief 统一所有旋转操作：从小到大编号，旋转后结构统一
 *
 * @tparam T
 * @param grand
 */
template <typename T>
void AVLTree<T>::rebalance2(Node<T>* grand) {
    Node<T>* parent = static_cast<AVLNode<T>*>(grand)->tallerChild();
    Node<T>* node = parent->tallerChild();
    if (parent->isLeftChild() && node->isLeftChild()) {
        // LL
        this->rotate(grand, node, node->m_right, parent, parent->m_right,
                     grand);
    } else if (parent->isRightChild() && node->isRightChild()) {
        // RR
        this->rotate(grand, grand, parent->m_left, parent, node->m_left, node);
    } else if (parent->isLeftChild() && node->isRightChild()) {
        // LR
        this->rotate(grand, parent, node->m_left, node, node->m_right, grand);
    } else {
        // RL
        this->rotate(grand, grand, node->m_left, node, node->m_right, parent);
    }
}
/**
 * @brief 统一所有旋转操作：a，g不需要更改
 *         旋转之后结构统一
 * @tparam T
 * @param r 子树根节点
 * @param b
 * @param c
 * @param d
 * @param e
 * @param f
 */
template <typename T>
void AVLTree<T>::rotate(Node<T>* r, Node<T>* b, Node<T>* c, Node<T>* d,
                        Node<T>* e, Node<T>* f) {
    // 1.d成为子树根节点
    d->m_parent = r->m_parent;
    if (r->isLeftChild()) {
        d = r->m_parent->m_left;
    } else if (r->isRightChild()) {
        d = r->m_parent->m_right;
    } else {
        this->m_root = d;
    }
    // 2.b,c
    c->m_parent = b;
    b->m_left = c;
    this->updateHeight(b);
    // 3.e,f
    e->m_parent = f;
    f->m_left = e;
    this->updateHeight(f);
    // 4.b,d,f
    b->m_parent = f->m_parent = d;
    d->m_left = b;
    d->m_right = f;
    this->updateHeight(d);
}

template <typename T>
AVLNode<T>* AVLTree<T>::createNewNode(T element, Node<T>* parent) {
    return new AVLNode<T>(element, parent);
}

template <typename T>
std::ostream& operator<<(std::ostream& cout, const AVLTree<T>& avl) {
    avl.print(avl.m_root, "");
    return cout;
}

/**
 * @brief 前序遍历打印
 *
 * @tparam T
 * @param node
 */
template <typename T>
void AVLTree<T>::print(Node<T>* node, std::string str) const {
    if (node == nullptr) return;
    std::cout << str << node->m_element << std::endl;
    this->print(node->m_left, str + "[L]");
    this->print(node->m_right, str + "[R]");
}