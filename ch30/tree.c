#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct tree {
  struct node *root;
};

struct node {
  struct node *left;
  struct node *right;
  int key;
  void *value;
  int depth;
};

void print_tree(struct tree *tree) {
  struct node *node = tree->root;
  if (!node) {
    return;
  }
  int depth = 0;

  struct queue queue;
  init_queue(&queue, 512);
  node->depth = depth;
  enqueue(&queue, node);
  printf("%d: ", depth);
  while ((node = dequeue(&queue))) {
    if (depth != node->depth) {
      depth = node->depth;
      printf("\n%d: ", depth);
    }
    printf("%d ", node->key);
    if (node->left) {
      node->left->depth = node->depth + 1;
      enqueue(&queue, node->left);
    }
    if (node->right) {
      node->right->depth = node->depth + 1;
      enqueue(&queue, node->right);
    }
  }
  printf("\n");
  destroy_queue(&queue);
}

struct node *new_node(int key, char *value) {
  struct node *new = malloc(sizeof(struct node));
  *new = (struct node){.key = key, .value = value};
  return new;
}

// Find the inorder successor
struct node *inorder_successor(struct node *node) {
  struct node *current = node;
  // Find the leftmost leaf
  while (current->left) {
    current = current->left;
  }
  return current;
}

void add(struct tree *tree, int key, void *value) {

  if (tree->root == NULL) {
    tree->root = new_node(key, value);
    return;
  }

  struct node *node = tree->root;
  while (node) {
    if (key > node->key) {
      if (node->right == NULL) {
        node->right = new_node(key, value);
        return;
      } else {
        node = node->right;
      }
    } else if (key < node->key) {
      if (node->left == NULL) {
        node->left = new_node(key, value);
        return;
      } else {
        node = node->left;
      }
    } else {
      node->value = value;
    }
  }
}

void *lookup(struct tree *tree, int key) {
  struct node *node = tree->root;
  while (node) {
    if (key > node->key) {
      node = node->right;
    } else if (key < node->key) {
      node = node->left;
    } else {
      return node->value;
    }
  }
  return NULL;
}

void del(struct tree *tree, int key) {
  struct node *prev = NULL;
  struct node *node = tree->root;
  while (node) {
    if (key > node->key) {
      prev = node;
      node = node->right;
    } else if (key < node->key) {
      prev = node;
      node = node->left;
    } else {
      // Both leafs are empty
      if (node->left == NULL && node->right == NULL) {
        if (node == tree->root) {
          tree->root = NULL;
        } else if (prev->left == node) {
          prev->left = NULL;
        } else {
          prev->right = NULL;
        }
        free(node);
        // One of the leafs are empty
      } else if (node->left == NULL || node->right == NULL) {
        struct node *child = node->left == NULL ? node->right : node->left;
        if (tree->root == node) {
          tree->root = child;
        } else if (prev->left == node) {
          prev->left = child;
        } else {
          prev->right = child;
        }
        free(node);
        // Both leafs are populated
      } else {
        struct node *succ = inorder_successor(node->right);
        int succ_key = succ->key;
        void *succ_value = succ->value;
        del(tree, succ->key);
        node->key = succ_key;
        node->value = succ_value;
      }
      node = NULL;
    }
  }
}

int main() {
  struct tree tree = {};

  add(&tree, 15, "value");
  add(&tree, 10, "value");
  add(&tree, 20, "value");
  add(&tree, 8, "value");
  add(&tree, 12, "value");
  add(&tree, 18, "value");
  add(&tree, 25, "value");

  print_tree(&tree);

  del(&tree, 15);
  printf("After deleting key\n");

  print_tree(&tree);
}
