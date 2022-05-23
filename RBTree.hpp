#include "BinarySearchTree.hpp"

template <typename T>
struct RBNode : Node<T> {
    static const bool RED = false;
    static const bool BLACK = true;
    bool m_color{BLACK};
    RBNode(T element, Node<T>* parent);
};
template <typename T>
RBNode<T>::RBNode(T element, Node<T>* parent) : Node<T>(element, parent) {}

template <typename T>
class RBTree : public BinarySearchTree<T> {
    static const bool RED = false;
    static const bool BLACK = true;

   public:
    explicit RBTree(int (*compare)(const T&, const T&) = [](const T& t1,
                                                            const T& t2) {
        if (t1 < t2) return -1;
        if (t1 > t2) return 1;
        return 0;
    });
    ~RBTree() = default;

   private:
    /**
     * @brief 染色
     */
    Node<T>* color(Node<T>* node, bool color);
    Node<T>* red(Node<T>* node);
    Node<T>* black(Node<T>* node);
    bool colorOf(Node<T>* node);
    bool isBlack(Node<T>* node);
    bool isRed(Node<T>* node);
};

template <typename T>
RBTree<T>::RBTree(int (*compare)(const T&, const T&))
    : BinarySearchTree<T>(compare) {}

template <typename T>
Node<T>* RBTree<T>::color(Node<T>* node, bool color) {
    if (node == nullptr) return node;
    static_cast<RBNode<T>*>(node)->m_color = color;
    return node;
}

template <typename T>
Node<T>* RBTree<T>::red(Node<T>* node) {
    return this->color(node, RED);
}
template <typename T>
Node<T>* RBTree<T>::black(Node<T>* node) {
    return this->color(node, BLACK);
}

template <typename T>
bool RBTree<T>::colorOf(Node<T>* node) {
    return node == nullptr ? BLACK : static_cast<RBNode<T>*>(node)->m_color;
}

template <typename T>
bool RBTree<T>::isBlack(Node<T>* node) {
    return this->colorOf(node) == BLACK;
}
template <typename T>
bool RBTree<T>::isRed(Node<T>* node) {
    return this->colorOf(node) == RED;
}