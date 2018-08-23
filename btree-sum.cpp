#include <iostream>
#include <stdexcept>
#include <stack>
#include <deque>

struct Node {
    Node(int v):
        value(v),
        left(nullptr),
        right(nullptr)
    {
    }

    int value = 0;
    Node *left = nullptr, *right = nullptr;
};

#if 0
          5
        /   \
      12     9
      /\
    -3   8
#endif

// O(n) n: nodes
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

    int sum = 0;

    std::stack<Node*> stk;

    stk.push(root);

    while (!stk.empty()) {
        Node* n = stk.top();
        stk.pop();

        if (n->right)
            stk.push(n->right);
        if (n->left)
            stk.push(n->left);

        sum += n->value;
    }

    std::cout << "Loop result (depth first): " << sum << std::endl;

    sum = 0;

    std::deque<Node*> dq;

    dq.push_back(root);

    while (!dq.empty()) {
        Node *n = dq.front();
        dq.pop_front();

        if (n->right)
            dq.push_back(n->right);
        if (n->left)
            dq.push_back(n->left);

        sum += n->value;
    }

    std::cout << "Loop result (breadth-first): " << sum << std::endl;
    return 0;
}
