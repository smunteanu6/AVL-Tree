
/*
  MIT License
  Copyright (c) 2022 Ștefan Munteanu
  https://github.com/smunteanu6/AVL-Tree/
*/

#include <bits/stl_algobase.h>
#include <cstddef>

template<class Node> static constexpr int getHeight(Node* node) {
  return node ? node->height : 0;
}

template<class Node> static constexpr int getBalanceFactor(Node* node) {
  return getHeight(node->left) - getHeight(node->right);
}

template<class Node> static inline Node* rotateLeft(Node* node) {
  Node* p = node->right;
  node->right = p->left;
  p->left = node;
  node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
  p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
  return p;
}

template<class Node> static inline Node* rotateRight(Node* node) {
  Node *p = node->left;
  node->left = p->right;
  p->right = node;
  node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
  p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
  return p;
}

template<class Node, class Key> static inline Node* fixRotation(Node* node, const Key& key) {
  node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
  int balanceFactor = getBalanceFactor(node);
  if (balanceFactor > 1) {
    if (key < node->left->key) {
      return rotateRight(node);
    } else if (key > node->left->key) {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }
  } else if (balanceFactor < -1) {
    if (key > node->right->key) {
      return rotateLeft(node);
    } else if (key < node->right->key) {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }
  }
  return node;
}

template<class Node, class Key> static Node* findNode(Node* node, const Key& key) {
  while (node) {
    if (key < node->key) node = node->left;
    else if (key > node->key) node = node->right;
    else return node;
  }
  return NULL;
}

namespace avlset {

  template<class Node, class Key> static Node* insert(Node* node, const Key& key) {
    if (!node) return new Node(key);
    if (key == node->key) return node;
    if (key < node->key) node->left = insert(node->left, key);
    else node->right = insert(node->right, key);
    return fixRotation(node, key);
  }

  template<class Node, class Key> static Node* remove(Node* node, const Key& key) {
    if (!node) return NULL;
    if (key == node->key) {
      if (node->left && node->right) {
        Node** p = &node->right;
        while ((**p).left) p = &(**p).left;
        node->key = (**p).key;
        free(*p), *p = NULL;
      } else if (node->left) {
        Node* p = node->left;
        *node = *p;
        free(p);
      } else if (node->right) {
        Node* p = node->right;
        *node = *p;
        free(p);
      } else {
        free(node);
        node = NULL;
      }
      return node;
    }

    if (key < node->key) node->left = remove(node->left);
    else node->right = remove(node->right);
    return fixRotation(node);

  }
}

namespace avlmap {

  template<class Node, class Key, class Value> static Node* insert(Node* node, const std::pair<Key,Value>& pair) {
    if (!node) return new Node(pair);
    if (pair.first == node->key) return node;
    if (pair.first < node->key) node->left = insert(node->left, pair);
    else node->right = insert(node->right, pair);
    return fixRotation(node, pair.first);
  }

  template<class Node, class Key, class Value> static Node* remove(Node* node, const Key& key) {
    if (!node) return NULL;
    if (key == node->key) {
      if (node->left && node->right) {
        Node** p = &node->right;
        while ((**p).left) p = &(**p).left;
        node->key = (**p).key;
        node->value = (**p).value;
        free(*p), *p = NULL;
      } else if (node->left) {
        Node* p = node->left;
        *node = *p;
        free(p);
      } else if (node->right) {
        Node* p = node->right;
        *node = *p;
        free(p);
      } else {
        free(node);
        node = NULL;
      }
      return node;
    }

    if (key < node->key) node->left = remove(node->left);
    else node->right = remove(node->right);
    return fixRotation(node);

  }
}

namespace avl {

  template<class Node, typename F> void dfs(const Node* node, F callback) {
    if (!node) return;
    dfs(node->left, callback);
    callback(node);
    dfs(node->right, callback);
  }

  template<class Key> class set {

    struct Node {
      Key key;
      Node* left;
      Node* right;
      int height;

      Node(const Key& key) {
        this->key = key;
        this->left = NULL;
        this->right = NULL;
        this->height = 1;
      }

    } *root = NULL;
    size_t nodes = 0ull;

    public:

    inline void insert(const Key& key) {
      this->root = avlset::insert(this->root, key);
    }

    inline void remove(const Key& key) {
      this->root = avlset::remove(this->root, key);
    }

    inline const Node* find(const Key& key) {
      return findNode(this->root, key);
    }

    inline const bool has(const Key& key) {
      return findNode(this->root, key);
    }

  };

  template<class Key, class Value> class map {

    struct Node {
      Key key;
      Value value;
      Node* left;
      Node* right;
      int height;

      Node(const std::pair<Key,Value>& pair) {
        this->key = pair.first;
        this->value = pair.second;
        this->left = NULL;
        this->right = NULL;
        this->height = 1;
      }

    } *root = NULL;
    size_t nodes = 0ull;

    public:

    inline void insert(const std::pair<Key,Value>& pair) {
      this->root = avlmap::insert(this->root, pair);
    }

    inline void remove(const Key& key) {
      this->root = avlmap::remove(this->root, key);
    }

    inline const Node* find(const Key& key) {
      return findNode(this->root, key);
    }

    inline Value& operator[](const Key& key) {
      return findNode(this->root, key)->value;
    }

  };

}