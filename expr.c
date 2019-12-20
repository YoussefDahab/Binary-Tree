/* Author : Youssef Dahab
File name: expr.c */

#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *makeString(char *s1, char *s2, char *s3) {
  char *concat_string;

  concat_string = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);
  if (concat_string == NULL)
    return NULL;

  strcpy(concat_string, s1);
  strcat(concat_string, s2);
  strcat(concat_string, s3);

  return concat_string;
}

Node *createNode(char *s, double val) {
  /* create node */
  Node *node_1 = malloc(sizeof(Node));
  if (node_1 == NULL)
    return NULL;

  node_1->expr_string = s;

  node_1->left = NULL;
  node_1->right = NULL;

  node_1->num_parents = 0;
  node_1->value = val;

  return node_1;
}

Node *binop(Operation op, Node *a, Node *b) {
  if (a->num_parents == 1 || b->num_parents == 1)
    return NULL;

  /* create a node */
  Node *node_2 = malloc(sizeof(Node));
  if (node_2 == NULL)
    return NULL;

  node_2->left = a;
  node_2->right = b;

  node_2->operation = op;

  node_2->value = 0;
  node_2->num_parents = 0;

  a->num_parents += 1;
  b->num_parents += 1;

  char *left_string = makeString("(", a->expr_string, ")");
  char *right_string = makeString("(", b->expr_string, ")");

  switch (op) {
  case addop:
    node_2->expr_string = makeString(a->expr_string, "+", b->expr_string);
    break;
  case subop:
    node_2->expr_string = makeString(a->expr_string, "-", b->expr_string);
    break;
  case mulop:
    node_2->expr_string = makeString(left_string, "*", right_string);
    break;
  case divop:
    node_2->expr_string = makeString(left_string, "/", right_string);
    break;
  }

  free(left_string);
  free(right_string);

  return node_2;
}

double evalTree(Node *root) {
  /* if pointer to root is NULL return 0 */
  if (root == NULL)
    return 0;

  if (root->left == NULL && root->right == NULL)
    return root->value;

  double left_value = evalTree(root->left);
  double right_value = evalTree(root->right);

  switch (root->operation) {
  case addop:
    root->value = left_value + right_value;
    break;
  case subop:
    root->value = left_value - right_value;
    break;
  case mulop:
    root->value = left_value * right_value;
    break;
  case divop:
    root->value = left_value / right_value;
    break;
  }

  return root->value;
}

void freeTree(Node *root) {
  if (root == NULL)
    return;

  freeTree(root->left);
  freeTree(root->right);

  /* free root->expr_string since I allocated memory to it through calling
  makeString
  leaf nodes have been assigned expressions so no need to free them. they
  weren't allocated memory for */
  if (root->left != NULL && root->right != NULL)
    free(root->expr_string);
  free(root);
}

Node *duplicateTree(Node *root) {
  if (root == NULL)
    return NULL;

  /* create duplicate node */
  Node *duplicate_node = malloc(sizeof(Node));
  if (duplicate_node == NULL)
    return NULL;

  duplicate_node->operation = root->operation;
  duplicate_node->value = root->value;
  duplicate_node->num_parents = root->num_parents;

  /* Allocate memory for non-leaf nodes expressions through makeString*/
  if (root->left == NULL && root->right == NULL)
    duplicate_node->expr_string = root->expr_string;
  else
    duplicate_node->expr_string = makeString(root->expr_string, "", "");

  duplicate_node->left = duplicateTree(root->left);
  duplicate_node->right = duplicateTree(root->right);

  return duplicate_node;
}

void printTree(Node *root) {
  if (root == NULL)
    return;

  printf(" Node\n");
  printf("    expr_string = %s\n", root->expr_string);
  printf("    value       = %g\n", root->value);
  printf("    num_parents = %d\n", root->num_parents);

  printTree(root->left);
  printTree(root->right);
}
