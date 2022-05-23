#pragma once
#include <queue>
using std::queue;

#define _MAX(a, b) ((a) > (b) ? (a) : (b))

template <typename T>
struct Node {
    T m_element;
    Node* m_parent;
    Node* m_left{nullptr};
    Node* m_right{nullptr};
    Node(T element, Node* parent);
    bool hasTwoChildren();
    bool isLeaf();
    bool isLeftChild();
    bool isRightChild();
    Node* sibling();  //返回兄弟节点
};
template <typename T>
Node<T>::Node(T element, Node<T>* parent)
    : m_element(element), m_parent(parent) {}
template <typename T>
bool Node<T>::hasTwoChildren() {
    return this->m_left != nullptr && this->m_right != nullptr;
}
template <typename T>
bool Node<T>::isLeaf() {
    return this->m_left == nullptr && this->m_right == nullptr;
}

template <typename T>
bool Node<T>::isLeftChild() {
    return this->m_parent != nullptr && this == this->m_parent->m_left;
}
template <typename T>
bool Node<T>::isRightChild() {
    return this->m_parent != nullptr && this == this->m_parent->m_right;
}
/**
 * @brief 返回兄弟节点
 *
 * @tparam T
 * @return Node<T>*
 */
template <typename T>
Node<T>* Node<T>::sibling() {
    if (this->isLeftChild()) {
    }
    return this->m_parent -
}

template <typename T>
class BinaryTree {
   protected:
    int m_size{0};
    Node<T>* m_root{nullptr};

   public:
    using TraversalFunc_t = bool (*)(T&);
    virtual ~BinaryTree() { this->clear(); };
    inline int size() const { return this->m_size; }
    inline bool isEmpty() const { return this->m_size == 0; }
    void clear();
    void preOrderTraversal(TraversalFunc_t tf);
    void inOrderTraversal(TraversalFunc_t tf);
    void postOrderTraversal(TraversalFunc_t tf);
    void levelOrderTraversal(TraversalFunc_t tf);
    bool isComplete();
    int height();
    int height2();
    virtual bool isContain(T element) const = 0;
    virtual void add(T element) = 0;
    virtual void remove(T element) = 0;

   protected:
    Node<T>* predecessor(Node<T>* node);
    Node<T>* successor(Node<T>* node);

   private:
    void clear(Node<T>* node);
    void preOrderTraversal(Node<T>* node, TraversalFunc_t tf);
    void inOrderTraversal(Node<T>* node, TraversalFunc_t tf);
    void postOrderTraversal(Node<T>* node, TraversalFunc_t tf);
    int height(Node<T>* node);
};

template <typename T>
void BinaryTree<T>::clear(Node<T>* node) {
    //后续遍历
    if (node == nullptr) return;
    this->clear(node->m_left);
    this->clear(node->m_right);
    delete node;
}

template <typename T>
void BinaryTree<T>::clear() {
    this->clear(this->m_root);
    this->m_size = 0;
    this->m_root = nullptr;
}

template <typename T>
void BinaryTree<T>::preOrderTraversal(Node<T>* node, TraversalFunc_t tf) {
    static bool ret = false;
    if (node == nullptr || ret) return;
    ret = tf(node->m_element);
    this->preOrderTraversal(node->m_left, tf);
    this->preOrderTraversal(node->m_right, tf);
}
template <typename T>
void BinaryTree<T>::inOrderTraversal(Node<T>* node, TraversalFunc_t tf) {
    static bool ret = false;
    if (node == nullptr || ret) return;
    this->inOrderTraversal(node->m_left, tf);
    if (ret) return;
    ret = tf(node->m_element);
    this->inOrderTraversal(node->m_right, tf);
}
template <typename T>
void BinaryTree<T>::postOrderTraversal(Node<T>* node, TraversalFunc_t tf) {
    static bool ret = false;
    if (node == nullptr || ret) return;
    this->postOrderTraversal(node->m_left, tf);
    this->postOrderTraversal(node->m_right, tf);
    if (ret) return;
    ret = tf(node->m_element);
}
template <typename T>
void BinaryTree<T>::preOrderTraversal(TraversalFunc_t tf) {
    if (tf == nullptr) return;
    this->preOrderTraversal(this->m_root, tf);
}
template <typename T>
void BinaryTree<T>::inOrderTraversal(TraversalFunc_t tf) {
    if (tf == nullptr) return;
    this->inOrderTraversal(this->m_root, tf);
}
template <typename T>
void BinaryTree<T>::postOrderTraversal(TraversalFunc_t tf) {
    if (tf == nullptr) return;
    this->postOrderTraversal(this->m_root, tf);
}

template <typename T>
void BinaryTree<T>::levelOrderTraversal(TraversalFunc_t tf) {
    if (this->m_root == nullptr || tf == nullptr) return;
    queue<Node<T>*> q;
    q.push(this->m_root);
    while (!q.empty()) {
        Node<T>* top = q.front();
        q.pop();
        if (tf(top->m_element)) return;
        if (top->m_left != nullptr) {
            q.push(top->m_left);
        }
        if (top->m_right != nullptr) {
            q.push(top->m_right);
        }
    }
}
/**
 * @brief 判断是否是完全二叉树
 * 层序遍历：queue
 *
 * @tparam T
 * @return true
 * @return false
 */
template <typename T>
bool BinaryTree<T>::isComplete() {
    if (this->m_root == nullptr) return false;
    queue<Node<T>*> q;
    q.push(this->m_root);
    bool isLeaf = false;
    while (!q.empty()) {
        Node<T>* front = q.front();
        q.pop();
        if (isLeaf && !front->isLeaf()) return false;
        if (front->m_left == nullptr && front->m_right != nullptr) {
            return false;
        }
        if (front->m_left != nullptr && front->m_right != nullptr) {
            q.push(front->m_left);
            q.push(front->m_right);
        } else if (front->m_left != nullptr && front->m_right == nullptr) {
            isLeaf = true;
            q.push(front->m_left);
        } else {
            isLeaf = true;
        }
    }
    return true;
}
/**
 * @brief 二叉树高度：递归
 *
 * @tparam T
 * @return int
 */

template <typename T>
int BinaryTree<T>::height() {
    return this->height(this->m_root);
}
template <typename T>
int BinaryTree<T>::height(Node<T>* node) {
    if (node == nullptr) return 0;
    return _MAX(this->height(node->m_left), this->height(node->m_right)) + 1;
}

/**
 * @brief 二叉树高度：层序遍历
 *
 * @tparam T
 * @return int
 */
template <typename T>
int BinaryTree<T>::height2() {
    if (this->m_root == nullptr) return 0;
    queue<Node<T>*> q;
    int height = 0;
    q.push(this->m_root);
    int rowLength = q.size();
    while (!q.empty()) {
        Node<T>* front = q.front();
        q.pop();
        --rowLength;
        if (front->m_left != nullptr) {
            q.push(front->m_left);
        }
        if (front->m_right != nullptr) {
            q.push(front->m_right);
        }
        if (rowLength == 0) {
            rowLength = q.size();
            ++height;
        }
    }
    return height;
}
/**
 * @brief 前驱节点
 * 1.node->m_left!=nullptr 左子树中最右的节点
 * 2.node->m_left==nullptr(该节点是这颗子树
 * 中序遍历时最先被访问的节点)往上找父节点
 * 2.1node->m_parent==nullptr:没有前驱节点
 * 2.2node->m_parent!=nullptr:
 * 2.2.1父节点左子树： 该节点是新子树(包括父节点)
 * 中序遍历时最先被访问的节点，继续找父节点
 * 2.2.2父节点右子树: 前驱节点=父节点
 * @tparam T
 * @param node
 * @return Node<T>*
 */
template <typename T>
Node<T>* BinaryTree<T>::predecessor(Node<T>* node) {
    if (node->m_left != nullptr) {
        Node<T>* preNode = node->m_left;
        while (preNode->m_right != nullptr) {
            preNode = preNode->m_right;
        }
        return preNode;
    } else {
        while (node->m_parent != nullptr && node == node->m_parent->m_left) {
            node = node->m_parent;
        }
        return node->m_parent;
    }
}
/**
 * @brief 后继节点
 *
 * @tparam T
 * @param node
 * @return Node<T>*
 */
template <typename T>
Node<T>* BinaryTree<T>::successor(Node<T>* node) {
    if (node->m_right != nullptr) {
        Node<T>* sucNode = node->m_right;
        while (sucNode->m_left != nullptr) {
            sucNode = sucNode->m_left;
        }
        return sucNode;
    } else {
        while (node->m_parent != nullptr && node == node->m_parent->m_right) {
            node = node->m_parent;
        }
        return node->m_parent;
    }
}