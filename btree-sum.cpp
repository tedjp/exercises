#include <iostream>
#include <stdexcept>

struct Node {
    Node(int v):
        value(v),
//        sum(0),
        left(nullptr),
        right(nullptr)
    {
    }

    int value = 0;
//    int sum;
    Node *left = nullptr, *right = nullptr;
};

#if 0
          5
        /   \
      12     9
      /\
    -3   8
#endif

static int sum_recursive(const Node* node) {
    if (node == nullptr)
        return 0;

    return node->value + sum_recursive(node->left) + sum_recursive(node->right);
}

int main(void) {
    Node five(5);
    Node twelve(12);
    Node minus_three(-3);
    Node eight(8);
    Node nine(9);

    Node *root = &five;
    five.left = &twelve;
    five.right = &nine;
    twelve.left = &minus_three;
    twelve.right = &eight;

    std::cout << "recursive result: " << sum_recursive(root) << std::endl;
}
