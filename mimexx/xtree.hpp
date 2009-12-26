#ifndef __XTREE__
#define __XTREE__
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

/////////////////////////////// Define TreeNode /////////////////////
template <typename TKey, typename TData> class TreeNode
{
  TKey  m_key;
  TData m_data;
  TreeNode* m_parent;
  std::map<TreeNode*, TreeNode*> m_childs;
  std::vector<TreeNode*> m_ch;
  typename std::map<TreeNode*, TreeNode*>::iterator m_pos;
  
public:
  /**
   * \brief constructor, construct a tree node with key and data.
   * If parent is left NULL, 
   * node is assumed to be root
   * \param key node key, must have operator =, <
   * \param data node data, this class must have operator=
   * \param parent parent node 
   */
  TreeNode(const TKey& key, const TData& data, TreeNode* parent = NULL);

  /**
   * \brief destructor, deletes this node and all its children
   */
  virtual ~TreeNode();

  //---------------------------------------------------
  // Setters / Getters
  //---------------------------------------------------

  /**
   * \brief get node data
   * \return node data
   */
  TData& GetData()  
  { 
    return m_data; 
  };

  /**
   * \brief get node key
   * \return key
   */
  TKey& GetKey()  
  { 
    return m_key; 
  };

  /**
   * \brief set node data
   * \param data data
   */
  void SetData(const TData& data) 
  { 
    m_data = data;
  };

  /**
   * \brief set node key
   * \param key key
   */
  void SetKey(const TKey& key) 
  { 
    m_key = key; 
  };

  /**
   * \brief return the parent node of this node (or null if root)
   * \return pointer to parent node
   */
  TreeNode* GetParent()
  {
    return m_parent;
  }

  
  //---------------------------------------------
  // Misc
  //---------------------------------------------

  /**
   * \brief print the tree to stream (default is stdout)
   */
  void Print(std::ostream& stream = std::cout , int depth = 0);

  /**
   * \brief check if this node is root
   * \return true if node is root
   */
  bool IsRoot() const 
  {
    return m_parent == NULL;
  };

  //----------------------------------------------
  // Operations
  //----------------------------------------------
  /**
   * \brief add new child node to this node. Duplicate nodes are allowed.
   * However, Remove() will remove 
   * the first occurance of a node by a given key
   * \param key node key
   * \param data node data
   * \return newly added node
   */
  TreeNode* Insert(const TKey& key, const TData& data) ;

  /**
   * \brief remove first occurance of node with a given key.
   * If the node to be removed
   * is the root a std::exception* is thrown, which must be deleted by caller. 
   * Remove does not delete the memory allocated for the node.
   * user must delete the removed node
   * 
   * \param key node key
   * \return removed node
   */
  TreeNode* Remove(const TKey& key);

  /**
   * \brief Find a node by a key
   * \param key node key
   * \return node, or NULL if non exist
   */
  TreeNode* Find(const TKey& key) ;

  /**
   * \brief return the first child of this node 
   * \return 
   */
  TreeNode* First();

  /**
   * \brief return the next child of this node
   * \return 
   */
  TreeNode* Next();

  std::map<TreeNode*, TreeNode*>& GetChilds() { return m_childs; }
  std::vector<TreeNode*>& GetChilds__ () { return m_ch; }
};
///////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------
// TreeWalker class
//----------------------------------------------------------------

template <typename TKey, typename TData>
class TreeWalker
{
  std::vector<TreeNode<TKey, TData>* > m_children;
  size_t m_pos;

private:
  void GetChildren(TreeNode<TKey, TData>* node);

public:
  TreeWalker(TreeNode<TKey, TData>* node);
  virtual ~TreeWalker();
  bool End();
  void operator++(int);
  TreeNode<TKey, TData>* GetNode() 
  { 
    if(m_pos < m_children.size())
      return m_children[m_pos];
    return NULL; 
  }
};

//////////////////////////////// Tree ////////////////////////////////
template <typename TKey, typename TData>
class Tree
{
  std::map<TKey, TreeNode<TKey, TData>*> m_nodes;
  TreeNode<TKey, TData>* m_root;
public:
  /**
   * \brief construct a tree with root node with Key and data
   * \param key root node key
   * \param data root node data
   */
  Tree(const TKey& key, const TData& data);

  /**
   * \brief destructor, deletes all tree and its sub nodes
   */
  virtual ~Tree();

  /*
   * \brief find node in the tree with Data.
   * \param data search key
   * \return node
   */
  TreeNode<TKey, TData>* FindData ( const TData& data );
  
  /**
   * \brief find node in the tree with Key.
   * \param key search key
   * \return node
   */
  TreeNode<TKey, TData>* Find(const TKey& key) ;
  
  /**
   * \brief remove node (and all its sub nodes) from the tree using key
   * as the criterion. this function does not delete the removed node
   * \param key search key
   * \return remove node
   */
  TreeNode<TKey, TData>* Remove(const TKey& key);

  /**
   * \brief Insert node to the tree
   * \param key new node key
   * \param data new node data 
   * \param parent new node parent, if left NULL parent is set to root
   * \return newly added node
   */
  TreeNode<TKey, TData>* Insert(const TKey& key, const TData& data, TreeNode<TKey, TData>* parent = NULL) ;
	
  /**
   * \brief return tree root
   * \return root node
   */
  TreeNode<TKey, TData>* GetRoot() { return m_root;}

  /**
   * \brief prints the tree in tabulated format to stream
   * \param stream output stream (default is set to stdout)
   * \param depth tab depth (for internal use)
   */
  void Print(std::ostream& stream = std::cout , int depth = 0);
  
  /**
   * \brief Print the tree in stack.
   * \param stream output stream ( default is set to stdout )
   * \param depth tab depth ( for internal use )
   */
  void Print2 (std::ostream& stream = std::cout, int depth = 0 );
  /**
   * \brief compare this tree against another tree
   * \param targetTree target tree to compare with
   * \param deletedItems array of pairs of items
   *        which exist in this tree and not in target tree
   * \param modifiedItems array of pairs of items which have
   *        the same key but differnt data 
   * \param newItems array of pairs of items which exist in
   *        the target tree but not in this tree
   */
  void Compare(Tree* targetTree, std::vector<std::pair<TKey, TData> >& deletedItems, std::vector<std::pair<TKey, TData> >& modifiedItems, std::vector<std::pair<TKey, TData> >& newItems);
};
///////////////////////////// End Tree

////////////////////////////////////////////////// 
//////////////////////////////////////////////////

template <typename TKey, typename TData>
TreeNode<TKey, TData>::TreeNode(const TKey& key, 
				const TData& data, 
				TreeNode* parent)
  : m_parent(parent), m_key(key), m_data(data)
{}

template <typename TKey, typename TData>
TreeNode<TKey, TData>::~TreeNode()
{
  std::map<TreeNode*, TreeNode*>::iterator iter = m_childs.begin();
  for(; iter != m_childs.end(); iter++)
    {
      delete iter->second;
    }
  m_childs.clear();
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* TreeNode<TKey, TData>::Insert(const TKey& key, 
						     const TData& data) 
{
  TreeNode* newNode = new TreeNode(key, data, this);
  m_childs[newNode] = newNode;
  m_ch.push_back(newNode);
  return newNode;
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* TreeNode<TKey, TData>::Remove(const TKey& key) 
{
  TreeNode* node = Find(key);
  if( node )
    {
      if(NULL == node->m_parent) return NULL;
	// Cant remove root
	//throw new std::exception("can't remove root node!");

      std::map<TreeNode*, TreeNode*>::iterator iter = node->m_parent->m_childs.find(node);
      if(iter != node->m_parent->m_childs.end())
	node->m_parent->m_childs.erase(iter);
      
      std::vector<TreeNode*>::iterator c_iter;
      c_iter = find(node->m_parent->m_ch.begin(),
		    node->m_parent->m_ch.end(), node);
      if ( c_iter != node->m_parent->m_ch.end())
	   node->m_parent->m_ch.erase (c_iter);
	   
      return node;
    }
  return NULL;
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* TreeNode<TKey, TData>::Find(const TKey& key) 
{
  if(m_key == key)
    return this;

  std::map<TreeNode*, TreeNode*>::iterator iter;

  // Scan first the childs of this node
  for(iter = m_childs.begin(); iter != m_childs.end(); iter++)
    {
      if(((TreeNode*)iter->second)->GetKey() == key)
	return iter->second;
    }

  // Scan level below
  for(iter = m_childs.begin(); iter != m_childs.end(); iter++)
    {
      TreeNode* node = NULL;
      node = iter->second->Find(key);
      if(node) return node;
    }
  return NULL;
}

template <typename TKey, typename TData>
void TreeNode<TKey, TData>::Print(std::ostream& stream, int depth) 
{
  std::string tab = " ";
  for(int i=0; i<depth; i++)
    tab += "    ";

  stream << tab.c_str() << m_key << std::endl;
  depth++;

  std::map<TreeNode*, TreeNode*>::iterator iter = m_childs.begin();
  for(; iter != m_childs.end(); iter++)
    iter->second->Print(stream, depth);
}

// Prepare for tree iteration in the current node
template <typename TKey, typename TData>
TreeNode<TKey, TData>* TreeNode<TKey, TData>::First()
{
  m_pos = m_childs.begin();
  return Next();
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* TreeNode<TKey, TData>::Next()
{
  if(m_pos == m_childs.end())
    return NULL;
  TreeNode* nextElem = m_pos->second;
  m_pos++;
  return nextElem;
}

template <typename TKey, typename TData>
TreeWalker<TKey, TData>::TreeWalker(TreeNode<TKey, TData>* node)
  : m_pos(0)
{
  m_children.push_back(node);
  GetChildren(node);
}

template <typename TKey, typename TData>
TreeWalker<TKey, TData>::~TreeWalker()
{
}

template <typename TKey, typename TData>
void TreeWalker<TKey, TData>::GetChildren(TreeNode<TKey, TData>* node)
{
  if(node == NULL)
    return;
  std::map<TreeNode<TKey, TData>*, TreeNode<TKey, TData>*>::iterator iter = node->GetChilds().begin();
  for(; iter != node->GetChilds().end(); iter++)
    {
      m_children.push_back(iter->second);
      GetChildren(iter->second);
    }
}

template <typename TKey, typename TData>
bool TreeWalker<TKey, TData>::End()
{
  return m_pos == m_children.size();
}

template <typename TKey, typename TData>
void TreeWalker<TKey, TData>::operator++(int)
{
  m_pos++;
}

////////////////////////////////////////////////////////////////////////

template <typename TKey, typename TData>
Tree<TKey, TData>::Tree(const TKey& key, const TData& data)
{
  m_root = new TreeNode<TKey, TData>(key, data);
}

template <typename TKey, typename TData>
Tree<TKey, TData>::~Tree()
{
  delete m_root;
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* Tree<TKey, TData>::FindData ( const TData& data )
{
  std::map <TKey, TreeNode<TKey, TData>* >::const_iterator iter;
  for ( iter =  m_nodes.begin (); iter < m_nodes.end (); ++iter )
    if ( iter->second == data ) return iter->second;
  return NULL;
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* Tree<TKey, TData>::Find(const TKey& key)
{
  std::map<TKey, TreeNode<TKey, TData>*>::const_iterator iter = m_nodes.find(key);
  if(iter == m_nodes.end())
    return NULL;
  return iter->second;
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* Tree<TKey, TData>::Insert(const TKey& key, const TData& data, TreeNode<TKey, TData>* parent /*NULL*/)
{
  TreeNode<TKey, TData>* parentNode;
  (parent == NULL) ? parentNode = m_root : parentNode = parent;
  TreeNode<TKey, TData>* node = parentNode->Insert(key, data);
  m_nodes[key] = node;
  return node;
}

template <typename TKey, typename TData>
TreeNode<TKey, TData>* Tree<TKey, TData>::Remove(const TKey& key)
{
  std::map<TKey, TreeNode<TKey, TData>*>::const_iterator iter = m_nodes.find(key);
  if(iter == m_nodes.end())
    return NULL;

  // Remove from the map all sub-nodes of the tree
  TreeWalker<TKey, TData> walker(iter->second);

  for(; !walker.End(); walker++)
    {
      std::map<TKey, TreeNode<TKey, TData>*>::iterator it = 
	m_nodes.find(walker.GetNode()->GetKey());
      if(it != m_nodes.end())
	m_nodes.erase(it);
    }
  return m_root->Remove(key);
}

template <typename TKey, typename TData>
void Tree<TKey, TData>::Print2(std::ostream& stream = std::cout, int depth = 0)
{
  std::stack< TreeNode<TKey, TData>* > Stack;
  std::stack <int> iStack;
  TreeNode<TKey, TData>* tp = NULL;
  //std::map<TreeNode<TKey,TData>*, TreeNode<TKey,TData>*> childs;
  std::vector<TreeNode<TKey,TData>*> childs;
  //std::map<TreeNode<TKey,TData>*, TreeNode<TKey,TData>*>::iterator cp;
  std::vector<TreeNode<TKey,TData>*>::iterator cp;
  int itab; std::string tab;
  Stack.push (m_root);
  iStack.push (0);
  while ( !Stack.empty () )
    {
      tp = Stack.top (); Stack.pop ();
      itab = iStack.top(); iStack.pop ();
      tab = " ";  for ( int i = 0; i < itab; i++ ) tab += "   ";
      stream << tab << tp->GetKey () << endl;
      childs = tp->GetChilds__ ();
      if ( !childs.empty () )
	{
	  ++itab;
	  std::sort (childs.begin(), childs.end(),
		     greater<TreeNode<TKey,TData>*>());
	  for ( cp = childs.begin() ; cp != childs.end(); ++cp )
	    {
	      Stack.push (*cp); iStack.push (itab);
	    }
	}
    }
}

template <typename TKey, typename TData>
void Tree<TKey, TData>::Print(std::ostream& stream = std::cout , int depth = 0)
{
  m_root->Print(stream, depth);
}

template <typename TKey, typename TData>
void Tree<TKey, TData>
::Compare(Tree* targetTree, 
	  std::vector<std::pair<TKey, TData> >& deletedItems, 
	  std::vector<std::pair<TKey, TData> >& modifiedItems, 
	  std::vector<std::pair<TKey, TData> >& newItems)
{
  deletedItems.clear(); newItems.clear(); modifiedItems.clear();
  TreeWalker<TKey, TData> sourceTreeWalker(this->GetRoot());
  TreeWalker<TKey, TData> targetTreeWalker(targetTree->GetRoot());
	
  for(; !sourceTreeWalker.End(); sourceTreeWalker++)
    {
      TreeNode<TKey, TData>* node = targetTree->Find(sourceTreeWalker.GetNode()->GetKey());
      if(node == NULL)
	{
	  // Item does not exist in target tree which means it must been deleted
	  std::pair<TKey, TData> itemPair;
	  itemPair.first = sourceTreeWalker.GetNode()->GetKey();
	  itemPair.second = sourceTreeWalker.GetNode()->GetData();
	  deletedItems.push_back( itemPair );
	}
      else 
	{
	  // Compare data
	  if(node->GetData() == sourceTreeWalker.GetNode()->GetData())
	    continue;

	  // Data was modified
	  std::pair<TKey, TData> itemPair;
	  itemPair.first = sourceTreeWalker.GetNode()->GetKey();
	  itemPair.second = sourceTreeWalker.GetNode()->GetData();
	  modifiedItems.push_back( itemPair );
	}
    }
	
  for(; !targetTreeWalker.End(); targetTreeWalker++)
    {
      if(Find(targetTreeWalker.GetNode()->GetKey()) == NULL)
	{
	  // Node from target tree does not exist in this tree
	  // which means that this node is new
	  // Data was modified
	  std::pair<TKey, TData> itemPair;
	  itemPair.first = targetTreeWalker.GetNode()->GetKey();
	  itemPair.second = targetTreeWalker.GetNode()->GetData();
	  newItems.push_back( itemPair );
	}
    }
}
#endif
