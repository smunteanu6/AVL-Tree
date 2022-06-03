
/*
  MIT License
  Copyright (c) 2022 Ștefan Munteanu
  https://github.com/smunteanu6/AVL-Tree/
*/

#include <bits/stl_algobase.h>
#include <cstddef>


namespace avl {

  template<class Node> constexpr int getHeight(Node* node) {
    return node ? node->height : 0;
  }

  template<class Node> constexpr int getBalanceFactor(Node* node) {
    return avl::getHeight(node->left) - avl::getHeight(node->right);
  }

  template<class Node> inline void appendChilden(Node* dest, const Node* src) {
    dest->left = src->left;
    dest->right = src->right;
  }

  template<class Node> inline Node* rotateLeft(Node* node) {
    Node* p = node->right;
    node->right = p->left;
    p->left = node;
    node->height = std::max(avl::getHeight(node->left), avl::getHeight(node->right)) + 1;
    p->height = std::max(avl::getHeight(p->left), avl::getHeight(p->right)) + 1;
    return p;
  }

  template<class Node> inline Node* rotateRight(Node* node) {
    Node *p = node->left;
    node->left = p->right;
    p->right = node;
    node->height = std::max(avl::getHeight(node->left), avl::getHeight(node->right)) + 1;
    p->height = std::max(avl::getHeight(p->left), avl::getHeight(p->right)) + 1;
    return p;
  }

  template<class Node, class Key> inline Node* fixRotation(Node* node, const Key& key) {
    node->height = std::max(avl::getHeight(node->left), avl::getHeight(node->right)) + 1;
    int balanceFactor = avl::getBalanceFactor(node);
    if (balanceFactor > 1) {
      if (key < *node->left->key) {
        return avl::rotateRight(node);
      } else if (key > *node->left->key) {
        node->left = avl::rotateLeft(node->left);
        return avl::rotateRight(node);
      }
    } else if (balanceFactor < -1) {
      if (key > *node->right->key) {
        return avl::rotateLeft(node);
      } else if (key < *node->right->key) {
        node->right = avl::rotateRight(node->right);
        return avl::rotateLeft(node);
      }
    }
    return node;
  }

  template<class Node> Node* removeMin(Node*& node) {
    if (node->left) {
      Node* min = avl::removeMin(node->left);
      node = avl::fixRotation(node, *min->key);
      return min;
    } else {
      Node* min = node;
      node = NULL;
      return min;
    }
  }

  template<class Node, class Key> Node* remove(Node* node, const Key& key) {

    if (!node) return NULL;
    if (key == *node->key) {
      if (node->left && node->right) {
        Node* p = avl::removeMin(node->right);
        avl::appendChilden(p, node);
        delete node;
        return p;
      } else if (node->left) {
        Node* p = node->left;
        delete node;
        return p;
      } else if (node->right) {
        Node* p = node->right;
        delete node;
        return p;
      } else {
        delete node;
        return NULL;
      }
    }

    if (key < *node->key) node->left = avl::remove(node->left, key);
    else node->right = avl::remove(node->right, key);
    return avl::fixRotation(node, key);
  }

  template<class Node, class Key> Node* find(Node* node, const Key& key) {
    while (node)
      if (key < node->key) node = node->left;
      else if (key > node->key) node = node->right;
      else return node;
    return NULL;
  }
}

namespace avl {

  template<class Node, typename F> static void dfs(const Node* node, F callback) {
    if (!node) return;
    dfs(node->left, callback);
    callback(node);
    dfs(node->right, callback);
  }

  template<class Key> class set {

    struct Node {

      const Key* key;
      Node* left;
      Node* right;
      int height;

      Node(const Key& key) {
        this->key = new Key(key);
        this->left = NULL;
        this->right = NULL;
        this->height = 1;
      }

      void operator delete(void* ptr) {
        delete static_cast<Key*>(ptr);
      }

    } *root = NULL;
    size_t nodes = 0ull;

    static Node* insert(Node* node, const Key& key) {
      if (!node) return new Node(key);
      if (key == *node->key) return node;
      if (key < *node->key) node->left = insert(node->left, key);
      else node->right = insert(node->right, key);
      return fixRotation(node, key);
    }

    public:

    template<typename F> inline void dfs(F callback) {
      avl::dfs(this->root, callback);
    }

    inline void insert(const Key& key) {
      this->root = this->insert(this->root, key);
    }

    inline void remove(const Key& key) {
      this->root = avl::remove(this->root, key);
    }

    inline const Node* find(const Key& key) {
      return avl::find(this->root, key);
    }

    inline const bool has(const Key& key) {
      return avl::find(this->root, key);
    }

  };

  template<class Key, class Value> class map {

    typedef std::pair<const Key&,Value&> Kvpair;

    struct Node {
      const Key* key;
      Value* value;
      Node* left;
      Node* right;
      int height;

      Node(const Kvpair& pair) {
        this->key = new Key(pair.first);
        this->value = new Value(pair.second);
        this->left = NULL;
        this->right = NULL;
        this->height = 1;
      }

      void operator delete(void* ptr) {
        delete static_cast<Key*>(ptr);
        delete static_cast<Value*>(ptr + sizeof(Key*));
      }

    } *root = NULL;
    size_t nodes = 0ull;

    static Node* insert(Node* node, const Kvpair& pair) {
      if (!node) return new Node(pair);
      if (pair.first == node->key) return node;
      if (pair.first < node->key) node->left = insert(node->left, pair);
      else node->right = insert(node->right, pair);
      return fixRotation(node, pair.first);
    }

    public:

    inline void insert(const Kvpair& pair) {
      this->root = this->insert(this->root, pair);
    }

    inline void remove(const Key& key) {
      this->root = avl::remove(this->root, key);
    }

    inline Kvpair find(const Key& key) {
      return *static_cast<Kvpair*>(avl::find(this->root, key));
    }

    inline Value& operator[](const Key& key) {
      return avl::find(this->root, key)->value;
    }

  };

}