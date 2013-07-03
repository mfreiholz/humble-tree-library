#ifndef _RADIXTREE_HEADER_
#define _RADIXTREE_HEADER_

#include <string>
#include <vector>

#include "api/defines.h"

NAMESPACE_BEGIN

class RadixTreeNode;
class RadixTreeNodeTraversalHelper;


class HTL_EXPORT_API RadixTree
{
public:
  enum ErrorType
  {
    NO_ERROR,
    DUPLICATE_KEY_ERROR,
    NODE_NOT_FOUND_ERROR
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
  
  /*
    Inserts a new <code>value</code> into the tree and associates it with the <code>key</code>.

    \param[in] key
      The key which is used to find the saved <code>value</code>.
    \param[in] value
      The data which should be saved in the tree.

    \return Returns <code>NO_ERROR</code>, if the value has been inserted.
      If the tree already contains a value saved by <code>key</code>, the method
      returns a <code>DUPLICATE_KEY_ERROR</code> code.
  */
  int insert(const std::string &key, const std::string &value = std::string());

  /*
    For internal use only!
  */
  int insert(RadixTreeNode &node, const std::string &key, const std::string &value);
  
  /*
    Removes a node from tree.
    
    \param[in] key
      The key of the node, which should be removed.
      
    \return Returns NO_ERROR, if the node has been removed.
      If there is no node which matches the <code>key</code>, the function returns NODE_NOT_FOUND_ERROR.
  */
  int remove(const std::string &key);
  
  /*
    Searches for a node by its key.

    \param[in] key
      The key of the node.

    \return Returns the RadixTreeNode object. If the node could not be found, the method
      will return the root-node of the tree. Use RadixTree::isRoot() to prove that.
  */
  const RadixTreeNode& find(const std::string &key);
  
  /*
    For internal use only!
  */
  void find(const std::string &key, RadixTreeNode &parent, RadixTreeNode &node, RadixTreeNodeTraversalHelper &helper);

  /*
    Deletes a RadixTreeNode completely.
    If <code>recursive</code> is <code>true</code>, the method also deletes all
    child nodes of the <code>node</code>.

    \param[in] node
    \param[in] recursive
  */
  static void FreeData(RadixTreeNode *node, bool recursive);
  
private:
  RadixTreeNode *_root;
  unsigned long long _size;
};


class HTL_EXPORT_API RadixTreeNode
{
  friend class RadixTree;
  friend class RadixTreeNodeTraversalFindHelper;
  friend class RadixTreeNodeTraversalRemoveHelper;
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


class HTL_EXPORT_API RadixTreeNodeTraversalHelper
{
public:
  /*
    Gets called whenever node matches the key/prefix.
  */
  virtual void match(const std::string &key, RadixTreeNode &parent, RadixTreeNode &node) = 0;
};


class HTL_EXPORT_API RadixTreeNodeTraversalFindHelper
  : public RadixTreeNodeTraversalHelper
{
public:
  inline RadixTreeNodeTraversalFindHelper()
    : _result(NULL)
  {
  }

  inline bool hasFound() const
  {
    return (_result != NULL);
  }

  inline RadixTreeNode& getResult() const
  {
    return *_result;
  }

  virtual void match(const std::string &key, RadixTreeNode &parent, RadixTreeNode &node);

protected:
  RadixTreeNode *_result;
};


class HTL_EXPORT_API RadixTreeNodeTraversalRemoveHelper
  : public RadixTreeNodeTraversalHelper
{
public:
  inline RadixTreeNodeTraversalRemoveHelper(RadixTree *tree)
    : _result(RadixTree::NODE_NOT_FOUND_ERROR),
      _tree(tree)
  {
  }

  inline int getResult() const
  {
    return _result;
  }

  // Pre-condition: node != "root-node"
  virtual void match(const std::string &key, RadixTreeNode &parent, RadixTreeNode &node);

protected:
  int _result;
  RadixTree *_tree;
};

NAMESPACE_END
#endif
