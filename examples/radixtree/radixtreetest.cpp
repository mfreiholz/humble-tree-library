#include "radixtree/radixtree.h"

#include <cstdio>
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#endif

unsigned long long get_current_timestamp_ms()
{
#ifdef _WIN32
  ULONGLONG ts = GetTickCount64();
#endif
  return ts;
}

int load_words(std::vector<std::string> &words)
{
  unsigned long long ts = 0;
  unsigned long long elapsed = 0;

  fprintf(stdout, "Loading words... ");
  ts = get_current_timestamp_ms();
  FILE *fh = fopen("E:\\Temp\\german.dict", "r");
  if (!fh) {
    fprintf(stdout, "FAILED!\n");
    return 1;
  }
  char buff[255];
  while (fgets(buff, 255, fh) != NULL) {
    words.push_back(std::string(buff));
  }
  fclose(fh);
  elapsed = get_current_timestamp_ms() - ts;
  fprintf(stdout, "Done %d words.\n", words.size());
  return 0;
}

int search_benchmark(htl::RadixTree &tree, const std::vector<std::string> &words, size_t mod = 1000000, size_t max = 10000000)
{
  unsigned long long ts = get_current_timestamp_ms();
  srand(time(NULL));

  for (size_t i = 1; i <= max; ++i) {
    const int random_index = rand() % words.size();
    const htl::RadixTreeNode& n = tree.find(words[random_index]);

    if (i % mod == 0) {
      unsigned long long now     = get_current_timestamp_ms();
      unsigned long long elapsed = now - ts;
      ts = now;
      fprintf(stdout, "Searched %d times in %d ms.\n", mod, elapsed);
    }
  }

  return 0;
}

int fill_tree_benchmark(htl::RadixTree &tree, const std::vector<std::string> &words)
{
  unsigned long long ts = 0;
  unsigned long long elapsed = 0;

  fprintf(stdout, "Fill tree with words... ");
  ts = get_current_timestamp_ms();
  std::vector<std::string>::const_iterator i_end = words.end();
  for (std::vector<std::string>::const_iterator i = words.begin(); i != i_end; ++i) {
    tree.insert(*i);
  }
  elapsed = get_current_timestamp_ms() - ts;
  fprintf(stdout, "Done in %d ms!\n", elapsed);

  return 0;
}

int empty_tree_benchmark(htl::RadixTree &tree, const std::vector<std::string> &words)
{
  unsigned long long ts = 0;
  unsigned long long elapsed = 0;

  fprintf(stdout, "Remove words from tree... ");
  ts = get_current_timestamp_ms();
  std::vector<std::string>::const_iterator i_end = words.end();
  for (std::vector<std::string>::const_iterator i = words.begin(); i != i_end; ++i) {
    tree.remove(*i);
  }
  elapsed = get_current_timestamp_ms() - ts;
  fprintf(stdout, "Done in %d ms!\n", elapsed);

  return 0;
}

int main(int argc, char **argv)
{
  // Unit test.
  for (size_t i = 0; i < 0; ++i)
  {
    htl::RadixTree t;
    for (size_t i = 0; i < 1; ++i) {
      t.insert("abhörmaßnahme");
      t.insert("feuchtwedel");
      t.insert("abhöreinheit");

      const htl::RadixTreeNode &n1 = t.find("lang");
      if (t.isRoot(n1)) fprintf(stdout, "OK #1 (%s)\n", n1.getValue().c_str());
  
      const htl::RadixTreeNode &n2 = t.find("feuchtlampe");
      if (t.isRoot(n2)) fprintf(stdout, "OK #2 (%s)\n", n2.getValue().c_str());
  
      const htl::RadixTreeNode &n3 = t.find("abhöreinheit");
      if (!t.isRoot(n3)) fprintf(stdout, "OK #3 (%s)\n", n3.getValue().c_str());

      std::vector<const htl::RadixTreeNode*> nv4;
      if (t.findPrefix("abh", 10, nv4) == htl::RadixTree::ET_NO_ERROR && nv4.size() == 2) fprintf(stdout, "OK #4 (%d)\n", nv4.size());

      t.remove("abhörmaßnahme");
      t.remove("feuchtwedel");
      t.remove("abhöreinheit");
    }
  }

  // Benchmark.
  {
    std::vector<std::string> words;
    load_words(words);
    for (size_t i = 0; i < 5; ++i)
    {
      htl::RadixTree bench_tree;
      fill_tree_benchmark(bench_tree, words);
      search_benchmark(bench_tree, words);
      empty_tree_benchmark(bench_tree, words);
    }
    words.clear();
  }

  fprintf(stdout, "END (Press any key to end)\n\n");
  fgetc(stdin);
  return 0;
}
