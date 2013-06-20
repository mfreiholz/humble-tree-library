#ifndef _MF_TERNARYTREEIMPL_HEADER_
#define _MF_TERNARYTREEIMPL_HEADER_

#include <string>
#include <vector>

#include "api/defines.h"

NAMESPACE_BEGIN

class StringSearchTreeNode;

/*!
  Checks whether the "word" begins with the given "prefix".
  Case sensitive!
*/
bool prefix_check_default_impl(const std::string &prefix, const std::string &word);

/*!
  This tree provides fast checks whether a string exists
  and supports prefix search.

  \author Manuel Freiholz
*/
class HTL_EXPORT_API StringSearchTree
{
public:
  /*!
    Defines a callback which check whether the seconds parameter (word) begins
    with the first parameter (prefix).

    \param[in] prefix
      The prefix used for search.
    \param[in] word
      The found word.

    \return Returns <code>true</code>, if the <code>word</code> begins with the given <code>prefix</code>.
  */
  typedef bool (*prefix_check_callback_t)(const std::string &, const std::string &);

  StringSearchTree();
  ~StringSearchTree();

  /*!
    Inserts a new value into the tree. Uses "value" as key and value.

    \param[in] value
      Key/value to insert into tree.
  */
  void insert(char *value);

  /*!
  Inserts a new key/value pair into the tree.
  This function provides the possibility to store a different string as value
  for the key.

    \param[in] key
      The key which should be used to find the value.
    \param[in value
      The value of the "key".
  */
  void insert(char *key, const std::string &value);

  /*!
  Searches for a specific string, which does not have to be a complete word.
  
  \param[in] value
    Prefix of a word or a complete word.

  \return Returns "true", if the exact string is in the tree.
  */
  bool find(char *value);

  /*!
  Searches for a specific value, which must have a final word associated.

  \param[in] value
    The exact key which has a word associated.
  */
  bool find_exact(char *value);

  /*!
    Searches for words which begins with the given "prefix".

  \param[in] prefix
    The prefix of a word.
  \param[in] max
    The maximum number of words, which the returning vector will contain.
  \param[in,optional] cb
    Callback function which is used to check validate a word against "prefix".
  */
  std::vector<std::string> find_prefix(char *prefix, unsigned int max, prefix_check_callback_t cb = prefix_check_default_impl) const;

private:
  /* Helper struct, used by find_prefix_traverse(). */
  struct find_prefix_data_t
  {
    std::vector<std::string> found_words;
    std::string prefix;
  };

  StringSearchTreeNode* insert(char *value, const std::string &word, StringSearchTreeNode *node);
  void delete_node(StringSearchTreeNode *begin);
  StringSearchTreeNode* find_recursive(char *value, StringSearchTreeNode *node) const;
  void find_prefix_traverse(StringSearchTreeNode *node, unsigned int max, find_prefix_data_t &data, prefix_check_callback_t cb) const;

private:
  StringSearchTreeNode *_root;
};


class HTL_EXPORT_API StringSearchTreeNode
{
  friend class StringSearchTree;
public:
  StringSearchTreeNode(char c, bool end);
  ~StringSearchTreeNode();

private:
  std::string _word;
  char _value;
  bool _end;
  StringSearchTreeNode *_low, *_equal, *_high;
};

NAMESPACE_END
#endif
