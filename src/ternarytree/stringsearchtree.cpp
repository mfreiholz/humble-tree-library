#include "stringsearchtree.h"

NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
// StringSearchTree
///////////////////////////////////////////////////////////////////////////////

StringSearchTree::StringSearchTree()
  : _root(0)
{
}

StringSearchTree::~StringSearchTree()
{
  delete_node(_root);
  _root = 0;
}

void StringSearchTree::insert(char *value)
{
  _root = insert(value, std::string(value), _root);
}

void StringSearchTree::insert(char *key, const std::string &value)
{
  _root = insert(key, value, _root);
}

bool StringSearchTree::find(char *value)
{
  StringSearchTreeNode *n = find_recursive(value, _root);
  return (n != 0);
}

bool StringSearchTree::find_exact(char *value)
{
  StringSearchTreeNode *n = find_recursive(value, _root);
  return (n != 0 && n->_end);
}

std::vector<std::string> StringSearchTree::find_prefix(char *value, unsigned int max, prefix_check_callback_t cb) const
{
  StringSearchTreeNode *n = find_recursive(value, _root);
  if (!n)
    return std::vector<std::string>();

  find_prefix_data_t data;
  data.prefix = std::string(value);

  find_prefix_traverse(n, max, data, cb);

  return data.found_words;
}

// Private ////////////////////////////////////////////////////////////////////

StringSearchTreeNode* StringSearchTree::insert(char *value, const std::string &word, StringSearchTreeNode *node)
{
  if (!node)
  {
    const char c = *value;
    node = new StringSearchTreeNode(c, false);
  }

  if (*value < node->_value)
  {
    node->_low = insert(value, word, node->_low);
  }
  else if (*value == node->_value)
  {
    if (*++value == 0)
    {
      node->_end = true;
      node->_word = word;
    }
    else
    {
      node->_equal = insert(value, word, node->_equal);
    }
  }
  else
  {
    node->_high = insert(value, word, node->_high);
  }
  return node;
}

void StringSearchTree::delete_node(StringSearchTreeNode *begin)
{
  std::vector<StringSearchTreeNode*> vec;
  vec.push_back(begin);
  while (!vec.empty()) {
    StringSearchTreeNode *node = vec.at(vec.size() - 1);
    vec.pop_back();
    if (node->_low)   vec.push_back(node->_low);
    if (node->_equal) vec.push_back(node->_equal);
    if (node->_high)  vec.push_back(node->_high);
    delete node;
  }
}

StringSearchTreeNode* StringSearchTree::find_recursive(char *value, StringSearchTreeNode *node) const
{
  if (!node)
    return 0;

  if (*value < node->_value)
  {
    return find_recursive(value, node->_low);
  }
  else if (*value == node->_value)
  {
    if (*++value == 0)
      return node;
    else
      return find_recursive(value, node->_equal);
  }
  else
  {
    return find_recursive(value, node->_high);
  }
}

void StringSearchTree::find_prefix_traverse(StringSearchTreeNode *node, unsigned int max, find_prefix_data_t &data, prefix_check_callback_t cb) const
{
  if (data.found_words.size() >= max)
    return;

  if (node->_end)
  {
    if (cb(data.prefix, node->_word))
      data.found_words.push_back(node->_word);
  }

  if (node->_low)   find_prefix_traverse(node->_low,   max, data, cb);
  if (node->_equal) find_prefix_traverse(node->_equal, max, data, cb);
  if (node->_high)  find_prefix_traverse(node->_high,  max, data, cb);
}

///////////////////////////////////////////////////////////////////////////////
// MF_TernaryTreeNode
///////////////////////////////////////////////////////////////////////////////

StringSearchTreeNode::StringSearchTreeNode(char c, bool end)
  : _value(c),
    _end(end),
    _low(0),
    _equal(0),
    _high(0)
{
}

StringSearchTreeNode::~StringSearchTreeNode()
{
}

///////////////////////////////////////////////////////////////////////////////
// Global funtion implementations.
///////////////////////////////////////////////////////////////////////////////

bool prefix_check_default_impl(const std::string &prefix, const std::string &word)
{
  std::size_t pos = word.find(prefix);
  return (pos != std::string::npos && pos == 0);
}

NAMESPACE_END
