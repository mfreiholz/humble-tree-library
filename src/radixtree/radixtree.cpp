#include "radixtree.h"

NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
// RadixTree
///////////////////////////////////////////////////////////////////////////////

RadixTree::RadixTree()
  : _root(new RadixTreeNode()),
    _size(0)
{
  _root->_key = std::string("");
}

RadixTree::~RadixTree()
{
}

void RadixTree::insert(const std::string &key, const std::string &value)
{
  insert(*_root, key, value);
}

void RadixTree::insert(RadixTreeNode &node, const std::string &key, const std::string &value)
{
  const unsigned int match_count = node.getNumberOfMatchingBytes(key);
  
  // We are either at the root node or we need to go down the tree.
  if (&node == _root || match_count == 0 || (match_count < key.length() && match_count >= node._key.length())) {
    bool found = false;
    const std::string new_key = key.substr(match_count, std::string::npos);
    for (size_t i = 0; i < node._children.size(); ++i) {
      if (node._children[i]->_key.find(new_key[0]) == 0) {
        found = true;
        insert(*node._children[i], new_key, value);
        break;
      }
    }
    
    if (!found) {
      RadixTreeNode *n = new RadixTreeNode();
      n->_key = new_key;
      n->_real = true;
      n->_value = value;
      node._children.push_back(n);
    }
  }
  // There is an exact match. Make the current node as data node.
  else if (match_count == key.length() && match_count == node._key.length()) {
    if (node._real) {
      // Duplicate key.
      return;
    }
    node._real = true;
    node._value = value;
  }
  // We need to split this node. Because the key to be inserted is a prefix of the current node's key.
  else if (match_count > 0 && match_count < node._key.length()) {
    RadixTreeNode *n1 = new RadixTreeNode();
    n1->_key      = node._key.substr(match_count, std::string::npos);
    n1->_real     = node._real;
    n1->_value    = node._value;
    n1->_children = node._children;
    
    node._key = key.substr(0, match_count);
    node._real = false;
    node._children.clear();
    node._children.push_back(n1);
    
    if (match_count < key.length()) {
      RadixTreeNode *n2 = new RadixTreeNode();
      n2->_key = key.substr(match_count, std::string::npos);
      n2->_real = true;
      n2->_value = value;
      
      node._children.push_back(n2);
    }
    else {
      node._real = true;
      node._value = value;
    }
  }
  // This key needs to be added as the child of the current "node".
  else {
    RadixTreeNode *n = new RadixTreeNode();
    n->_key = node._key.substr(match_count, std::string::npos);
    n->_value    = node._value;
    n->_real     = node._real;
    n->_children = node._children;
    
    node._key = key;
    node._real = true;
    node._value = value;
    
    node._children.push_back(n);
  }
}

///////////////////////////////////////////////////////////////////////////////
// RadixTreeNode
///////////////////////////////////////////////////////////////////////////////

RadixTreeNode::RadixTreeNode()
  : _key(),
    _value(),
    _real(false),
    _children()
{
}

RadixTreeNode::~RadixTreeNode()
{
}

unsigned int RadixTreeNode::getNumberOfMatchingBytes(const std::string &key) const
{
  unsigned int match_count = 0;
  while (match_count < key.length() && match_count < _key.length()) {
    if (key[match_count] != _key[match_count])
      break;
    ++match_count;
  }
  return match_count;
}

NAMESPACE_END
