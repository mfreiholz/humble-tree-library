#include "radixtree/radixtree.h"

int main(int argc, char **argv)
{
  htl::RadixTree t;
  t.insert("abhörmaßnahme");
  t.insert("feuchtwedel");
  t.insert("abhöreinheit");

  t.find("lang");
  t.find("feuchtlampe");
  t.find("abhöreinheit");
  return 0;
}
