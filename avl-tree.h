


#include <bits/stl_algobase.h>

template<class T> class avl {
  
  struct Node {
    T key;
    Node* left;
    Node* right;
    int height;

    constexpr const int getHeight() const {
      return this ? height : 0;
    }

    constexpr const int getBalanceFactor() const {
      return this ? left->getHeight() - right->getHeight() : 0;
    }

    Node* const rotateLeft() {
      Node* p = right;
      right = p->left;
      p->left = this;
      height = max(left->getHeight(), right->getHeight()) + 1;
      p->height = max(p->left->getHeight(), p->right->getHeight()) + 1;
      return p;
    }

    Node* const rotateRight() {
      Node* p = left;
      left = p->right;
      p->right = this;
      height = max(left->getHeight(), right->getHeight()) + 1;
      p->height = max(p->left->getHeight(), p->right->getHeight()) + 1;
      return p;
    }

    Node* const rotateLeftRight() {
      left = left->rotateLeft();
      return rotateRight();
    }

    Node* const rotateRightLeft() {
      right = right->rotateRight();
      return rotateLeft();
    }

    Node*& findMin() {
      Node* p = this;
      while (p->left->left)
        p = p->left;
      return p->left;
    }

    Node*& findMax() {
      Node* p = this;
      while (p->right->right)
        p = p->right;
      return p->right;
    }

    Node* insertNode(const T& __key) {
      if (!this) return new Node(__key);
      else if (__key < key) left = left->insertNode(__key);
      else if (__key > key) right = right->insertNode(__key);
      else return this;

      height = max(left->getHeight(), right->getHeight()) + 1;
      int bf = getBalanceFactor();

      if (getBalanceFactor() > 1) {
        if (__key < left->key) return rotateRight();
        else if (__key > left->key) return rotateLeftRight();
      } else if (getBalanceFactor() < -1) {
        if (__key > right->key) return rotateLeft();
        else if (__key < right->key) return rotateRightLeft();
      }
      return this;
    }

    Node* removeNode(const T& __key) {
      if (!this) return NULL;
      else if (__key < key) left = left->removeNode(__key);
      else if (__key > key) right = right->removeNode(__key);
      else if (left && right) {
        Node *&p = right->findMin();
        key = p->key;
        free(p), p = NULL;
        return this;
      } else if (left != right) {
        Node *p = left ? left : right;
        *this = *p, free(p);
        return this;
      } else return NULL;

      height = max(left->getHeight(), right->getHeight()) + 1;
      if (getBalanceFactor() > 1) {
        if (__key < left->key) return rotateRight();
        else if (__key > left->key) return rotateLeftRight();
      } else if (getBalanceFactor() < -1) {
        if (__key < right->key) return rotateLeft();
        else if (__key > right->key) return rotateRightLeft();
      }
      return this;
    }

    template<typename F> void dfs(F callback) const {
      if (!this) return;
      left->dfs(callback);
      callback(key);
      right->dfs(callback);
    }

    Node(T __key) {
      key = __key;
      left = NULL;
      right = NULL;
      height = 1;
    }

  } *root = NULL;

  size_t nodes_count = 0ull;

  public:

  const bool empty() const {
    return !nodes_count;
  }

  const size_t size() const {
    return nodes_count;
  }

  void insert(T key) {
    root = root->insertNode(key);
  }

  void remove(T key) {
    root = root->removeNode(key);
  }

  const Node* const find(T key) const {
    const Node* node = root;
    while (node)
      if (key < node->key) node = node->left;
      else if (key > node->key) node = node->right;
      else return node;
    return NULL;
  }

  template<typename F> void forEach(F callback) {
    root->dfs(callback);
  }

};