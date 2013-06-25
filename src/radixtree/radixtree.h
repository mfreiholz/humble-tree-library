#ifndef _RADIXTREE_HEADER_
#define _RADIXTREE_HEADER_

#include <string>
#include <vector>

#include "api/defines.h"

NAMESPACE_BEGIN

class RadixTreeNode;

class HTL_EXPORT_API RadixTree
{
public:
  RadixTree();
  ~RadixTree();
  
  void insert(const std::string &key, const std::string &value = std::string());
  void insert(RadixTreeNode &node, const std::string &key, const std::string &value);
  
  void find(const std::string &key);
  void find(const std::string &key, const RadixTreeNode &parent, const RadixTreeNode &node);
  
private:
  RadixTreeNode *_root;
  unsigned long long _size;
};


class HTL_EXPORT_API RadixTreeNode
{
  friend class RadixTree;
public:
  RadixTreeNode();
  ~RadixTreeNode();
  
  const std::string& getKey() const;
  const std::string& getValue() const;
  bool isReal() const;
  
  unsigned int getNumberOfMatchingBytes(const std::string &key) const;

private:
  std::string _key;
  std::string _value;
  bool _real;
  std::vector<RadixTreeNode*> _children;
};

NAMESPACE_END
#endif
