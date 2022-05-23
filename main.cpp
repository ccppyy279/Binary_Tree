#include <iostream>

#include "AVLTree.hpp"
#include "BinarySearchTree.hpp"
using std::cout;
int main() {
    AVLTree<int> bst;
    for (auto i : {44, 49, 2, 8, 26, 32, 12, 33, 4, 42, 36, 14, 11, 10, 15}) {
        bst.add(i);
    }
    cout << bst;
    cout << bst.height() << std::endl;
    cout << bst.height2() << std::endl;
    cout << bst.isComplete() << std::endl;
    cout << bst.isContain(15) << std::endl;
    cout << bst.size() << std::endl;
    bst.preOrderTraversal([](int& t) {
        std::cout << t << " ";
        return false;
    });
    cout << std::endl;
    bst.inOrderTraversal([](int& t) {
        std::cout << t << " ";
        return false;
    });
    cout << std::endl;
    bst.postOrderTraversal([](int& t) {
        std::cout << t << " ";
        return false;
    });
    cout << std::endl;
    bst.levelOrderTraversal([](int& t) {
        std::cout << t << " ";
        return false;
    });
    cout << std::endl;
    for (auto i : {44, 49, 2, 8, 26, 32, 12, 33, 4, 42, 36, 14, 11, 10, 15}) {
        bst.remove(i);
        cout << bst;
    }
    bst.clear();
    cout << bst;
    cout << bst.isEmpty();
}