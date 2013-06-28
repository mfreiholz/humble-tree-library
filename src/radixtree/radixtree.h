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
  enum Errors
  {
    NO_ERROR,
    DUPLICATE_KEY_ERROR
  };

  RadixTree();
  ~RadixTree();
  
  /*
    Compares the given specified tree-node with the internal root node of the tree.
    
    \param[in] node
      The node which should be compared with the root-node.
      
    \return Returns <code>true</code>, if the given <code>node</code> refers
      to the tree's internal root-node.
  */
  bool isRoot(const RadixTreeNode &node) const;
  
  int insert(const std::string &key, const std::string &value = std::string());
  int insert(RadixTreeNode &node, const std::string &key, const std::string &value);
  
  const RadixTreeNode& find(const std::string &key);
  
  /*
    Searches for the node with the given <code>key</code>.
    
    \param[in] key
      The exact key of the node.
    \param[in] parent
      The parent node of the <code>node</code>.
    \param[in] node
      The node object where the search starts.
      
    \return Returns the node which matches the <code>key</code>. If no node has been found,
      the root node will return.
  */
  const RadixTreeNode& find(const std::string &key, const RadixTreeNode &parent, const RadixTreeNode &node);
  
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
