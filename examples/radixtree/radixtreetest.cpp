#include "radixtree/radixtree.h"

int main(int argc, char **argv)
{
  htl::RadixTree t;
  t.insert("abhörmaßnahme");
  t.insert("feuchtwedel");
  t.insert("abhöreinheit");
  return 0;
}
