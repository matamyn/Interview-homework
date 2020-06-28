#include <cassert>
#include <iostream>

struct Node {
  int value;
  Node *left, *right;
};
Node *tree;

bool is_bst_correct(const Node *a_tree, int *lo, int *hi) {
  assert(a_tree != nullptr && lo != nullptr && hi != nullptr);

  *lo = *hi = a_tree->value;

  if (a_tree->left != nullptr) {
    int left_lo, left_hi;
    if (!is_bst_correct(a_tree->left, &left_lo, &left_hi))
      return false;

    assert(left_lo <= left_hi);
    if (left_hi > a_tree->value)
      return false;

    *lo = left_lo;
  }

  if (a_tree->right != nullptr) {
    int right_lo, right_hi;
    if (!is_bst_correct(a_tree->right, &right_lo, &right_hi))
      return false;

    assert(right_lo <= right_hi);
    if (right_lo < a_tree->value)
      return false;

    *hi = right_hi;
  }

  return true;
}
int main() {
  tree = new Node;
  tree->value = 2;
  tree->left = new Node;
  tree->left->value = 1;
  tree->right = new Node;
  tree->right->value = 4;
  tree->right->left = new Node;
  tree->right->left->value = 3;
  int full_lo, full_hi;
  if (tree == nullptr || is_bst_correct(tree, &full_lo, &full_hi)) {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "NOT OK" << std::endl;
  }
  return 0;
}