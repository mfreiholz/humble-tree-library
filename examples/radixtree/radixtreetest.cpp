#include "radixtree/radixtree.h"

int main(int argc, char **argv)
{
  htl::RadixTree t;
  for (size_t i = 0; i < 999999999; ++i) {
    t.insert("abh�rma�nahme");
    t.insert("feuchtwedel");
    t.insert("abh�reinheit");

    const htl::RadixTreeNode &n1 = t.find("lang");
    if (t.isRoot(n1)) fprintf(stdout, "OK #1 (%s)\n", n1.getValue().c_str());
  
    const htl::RadixTreeNode &n2 = t.find("feuchtlampe");
    if (t.isRoot(n2)) fprintf(stdout, "OK #2 (%s)\n", n2.getValue().c_str());
  
    const htl::RadixTreeNode &n3 = t.find("abh�reinheit");
    if (!t.isRoot(n3)) fprintf(stdout, "OK #3 (%s)\n", n3.getValue().c_str());

    t.remove("abh�rma�nahme");
    t.remove("feuchtwedel");
    t.remove("abh�reinheit");
  }
  return 0;
}
