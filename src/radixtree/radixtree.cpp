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
  FreeData(_root, true);
  _root = NULL;
}

bool RadixTree::isRoot(const RadixTreeNode &node) const
{
  return (&node == _root);
}

int RadixTree::insert(const std::string &key, const std::string &value)
{
  if (value.empty())
    return insert(*_root, key, key);
  else
    return insert(*_root, key, value);
}

int RadixTree::insert(RadixTreeNode &node, const std::string &key, const std::string &value)
{
  const unsigned int match_count = node.getNumberOfMatchingBytes(key);
  
  // We are either at the root node or we need to go down the tree.
  if (&node == _root || match_count == 0 || (match_count < key.length() && match_count >= node._key.length())) {
    const std::string new_key = key.substr(match_count, std::string::npos);
    for (size_t i = 0; i < node._children.size(); ++i) {
      if (node._children[i]->_key.find(new_key[0]) == 0) {
        return insert(*node._children[i], new_key, value);
      }
    }
    
    // No node with prefix "key" found, create a new one.
    RadixTreeNode *n = new RadixTreeNode();
    n->_key = new_key;
    n->_real = true;
    n->_value = value;
    node._children.push_back(n);
    return NO_ERROR;
  }
  // There is an exact match. Make the current node as data node.
  else if (match_count == key.length() && match_count == node._key.length()) {
    if (node._real) {
      return DUPLICATE_KEY_ERROR;
    }
    node._real = true;
    node._value = value;
    return NO_ERROR;
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
    node._value.clear();  // Fix.
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
    return NO_ERROR;
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
    return NO_ERROR;
  }
}

int RadixTree::remove(const std::string &key)
{
  RadixTreeNodeTraversalRemoveHelper helper(this);
  find(key, *_root, *_root, helper);
  return helper.getResult();
}

const RadixTreeNode& RadixTree::find(const std::string &key)
{
  RadixTreeNodeTraversalFindHelper helper;
  find(key, *_root, *_root, helper);

  if (helper.hasFound())
    return helper.getResult();
  return *_root;
}

void RadixTree::find(const std::string &key, RadixTreeNode &parent, RadixTreeNode &node, RadixTreeNodeTraversalHelper &helper)
{
  const unsigned int match_count = node.getNumberOfMatchingBytes(key);
  
  // If the key matches the node's key, we have a match.
  if (match_count == key.length() && match_count == node._key.length()) {
    // Found: "node"
    helper.match(key, parent, node);
  }
  // Either we are at the ROOT node or we need to traverse the children.
  else if (&node == _root || (match_count < key.length() && match_count >= node._key.length())) {
    const std::string new_key = key.substr(match_count, std::string::npos);
    for (size_t i = 0; i < node._children.size(); ++i) {
      if (node._children[i]->_key.find(new_key[0]) == 0) {
        find(new_key, node, *node._children[i], helper);
        break;
      }
    }
  }
}

void RadixTree::FreeData(RadixTreeNode *node, bool recursive)
{
  std::vector<RadixTreeNode*> vec;
  vec.push_back(node);
  while (!vec.empty()) {
    RadixTreeNode *n = vec.front();
    vec.erase(vec.begin());
    if (recursive) {
      for (size_t i = 0; i < n->_children.size(); ++i)
        vec.push_back(n->_children[i]);
    }
    delete n;
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

const std::string& RadixTreeNode::getKey() const
{
  return _key;
}

const std::string& RadixTreeNode::getValue() const
{
  return _value;
}

bool RadixTreeNode::isReal() const
{
  return _real;
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

///////////////////////////////////////////////////////////////////////////////
// Traversal helpers.
///////////////////////////////////////////////////////////////////////////////

void RadixTreeNodeTraversalFindHelper::match(const std::string &key, RadixTreeNode &parent, RadixTreeNode &node)
{
  if (node.isReal()) {
    _result = &node;
  }
}

void RadixTreeNodeTraversalRemoveHelper::match(const std::string &key, RadixTreeNode &parent, RadixTreeNode &node)
{
  if (_tree && _tree->isRoot(node)) {
    return;
  }

  // If the node does not have any childs,
  // we need to remove it from it's parent child list.
  // Note: As soon as we removed the "node" from parent's node
  // child list, the "node" variable is invalid!  
  if (node._children.size() == 0) {

    for (size_t i = 0; i < parent._children.size(); ++i) {
      if (parent._children[i] == &node) {
        RadixTreeNode *child = parent._children[i];
        parent._children.erase(parent._children.begin() + i);
        delete child;
        break;
      }
    }

    // If the parent node is not a real node and does only have
    // one child after we removed "node", we need to merge it.
    if (parent._children.size() == 1 && !parent._real && !_tree->isRoot(parent)) {
      RadixTreeNode *child = parent._children[0];
      parent._key     += child->_key;
      parent._value    = child->_value;
      parent._real     = child->_real;
      parent._children = child->_children;  // TODO(mfreiholz) This can be a big copy process? We need to move it.
      delete child;
    }
  }
  // We need to merge the single child of the node with
  // itself, and remove the child nodes.
  else if (node._children.size() == 1) {
    RadixTreeNode *child = node._children[0];
    node._key      += child->_key;
    node._value     = child->_value;
    node._real      = child->_real;
    node._children  = child->_children;  // TODO(mfreiholz) same here...
    delete child;
  }
  // The node does have more than only one child.
  // We can simply mark the found node as NOT real.
  else {
    node._real = false;
  }
  _result = RadixTree::NO_ERROR;
}

NAMESPACE_END
