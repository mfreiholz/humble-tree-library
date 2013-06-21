#include "ternarytree/stringsearchtree.h"

#include <stdio.h>

int main(int argc, char **argv)
{
  htl::StringSearchTree tree;
  tree.insert("anton");
  tree.insert("berta");
  tree.insert("kosten");
  tree.insert("authors");
  tree.insert("proceeds");
  tree.insert("comparison");
  tree.insert("randomized");
  tree.insert("version", std::string("Version"));
  tree.insert("computer");
  tree.insert("ternary");
  tree.insert("theoretical");
  tree.insert("become");
  tree.insert("fahrplan");

  if (tree.find_exact("kosten"))
    fprintf(stdout, "OK\n");
  else
    fprintf(stderr, "ERROR\n");

  if (!tree.find_exact("not-in-tree"))
    fprintf(stdout, "OK\n");
  else
    fprintf(stderr, "ERROR\n");

  if (tree.find("compar"))
    fprintf(stdout, "OK\n");
  else
    fprintf(stderr, "ERROR\n");

  std::vector<std::string> vec = tree.find_prefix("be", 10);
  if (vec.size() == 2)
    fprintf(stdout, "OK\n");
  else
    fprintf(stderr, "ERROR\n");

  // Endless running loop as memory check.
  while (true) {
    htl::StringSearchTree t;
    t.insert("foo");
    t.insert("bar");
  }

  return 0;
}
