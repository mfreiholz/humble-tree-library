#include "radixtree/radixtree.h"

int main(int argc, char **argv)
{
  htl::RadixTree t;
  t.insert("abh�rma�nahme");
  t.insert("feuchtwedel");
  t.insert("abh�reinheit");

  t.find("lang");
  t.find("feuchtlampe");
  t.find("abh�reinheit");
  return 0;
}
