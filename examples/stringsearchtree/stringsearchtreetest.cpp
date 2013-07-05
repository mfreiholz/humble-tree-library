#include "ternarytree/stringsearchtree.h"

#include <cstdio>
#include <ctime>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#endif

unsigned long long get_current_timestamp_ms()
{
#ifdef _WIN32
  ULONGLONG ts = GetTickCount64();
#else
  unsigned long long ts = 0;
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

int search_benchmark(htl::StringSearchTree &tree, const std::vector<std::string> &words, size_t mod = 1000000, size_t max = 10000000)
{
  unsigned long long ts = get_current_timestamp_ms();
  srand(time(NULL));

  for (size_t i = 1; i <= max; ++i) {
    const int random_index = rand() % words.size();
    const bool found = tree.find(const_cast<char*>(words[random_index].c_str()));

    if (i % mod == 0) {
      unsigned long long now     = get_current_timestamp_ms();
      unsigned long long elapsed = now - ts;
      ts = now;
      fprintf(stdout, "Searched %d times in %d ms.\n", mod, elapsed);
    }
  }

  return 0;
}

int fill_tree_benchmark(htl::StringSearchTree &tree, const std::vector<std::string> &words)
{
  unsigned long long ts = 0;
  unsigned long long elapsed = 0;

  fprintf(stdout, "Fill tree with words... ");
  ts = get_current_timestamp_ms();
  std::vector<std::string>::const_iterator i_end = words.end();
  for (std::vector<std::string>::const_iterator i = words.begin(); i != i_end; ++i) {
    char *c = const_cast<char*>(i->c_str());
    tree.insert(c);
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
  }

  // Benchmark.
  {
    std::vector<std::string> words;
    load_words(words);
    for (size_t i = 0; i < 5; ++i)
    {
      htl::StringSearchTree bench_tree;
      fill_tree_benchmark(bench_tree, words);
      search_benchmark(bench_tree, words);
    }
  }

  fprintf(stdout, "END\n\n");
  fgetc(stdin);
  return 0;
}
