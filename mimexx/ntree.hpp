#ifndef __NTREE____
#define __NTREE____
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
namespace Beyooo
{
  namespace Tree
  {
    template <typename TKey, typename TData>
    class TreeNode
    {
    private:
      TData m_data;
      TKey m_key;
      TreeNode<TKey,TData>* m_parent;
      std::map<TKey, TreeNode*> m_childs;
      typename std::map<TKey, TreeNode*>::iterator m_pos;
      
    public:
      TreeNode ( const TKey& key, const TData& data, TreeNode<TKey, TData>* parent = NULL );
      virtual ~TreeNode ();
      TData& get_data () { return m_data; };
      TKey& get_key () { return m_key; };
      void set_data ( const TData& data ) { m_data = data;};
      TreeNode* get_parent () { return m_parent; };
      bool is_root () const { return m_parent == NULL; };
      TreeNode* insert ( const TKey& key, const TData& data );
      //void insert ( const TData& data );
      TreeNode* remove ( const TKey& key );
      TreeNode* find ( const TKey& key );
      TreeNode* first ();
      TreeNode* next ();
      std::map <TKey, TreeNode<TKey,TData>*>& get_childs ()
      { return m_childs; }
      //      TreeNode& operator= ( const TType& data );
      bool operator < ( const TData& data );
      bool operator > ( const TData& data );
      bool operator == ( const TData& data );
      //operator TType () const;
      //operator TreeNode () const;
//       bool operator < ( const TreeNode<TType>& data );
//       bool operator > ( const TreeNode<TType>& data );
//       bool operator= ( const TreeNode<TType>& data );
      
    };
    //////////////////////////////////////////////////////////////
    template <typename TKey, typename TData> class Tree
    {
    private:
      std::map<TKey, TreeNode<TKey, TData>*> m_nodes;
      TreeNode<TKey, TData>* m_root;
    public:
      Tree ( const TKey& key, const TData& data );
      virtual ~Tree ();
      TreeNode<TKey, TData>* find ( const TKey& key );
      TreeNode<TKey, TData>* remove ( const TKey& key );
      TreeNode<TKey, TData>* insert ( const TKey& key, const TData& data,
				      TreeNode<TKey,TData>* parent = NULL);
      TreeNode<TKey, TData>* get_root () { return m_root; }
      void print ();
    };

    template <typename TKey, typename TData>
    TreeNode<TKey, TData>::TreeNode ( const TKey& key, const TData& data,
				      TreeNode<TKey,TData>* parent)
      : m_parent(parent), m_key(key), m_data(data)
    {}
    template <typename TKey, typename TData>
    TreeNode<TKey, TData>::~TreeNode()
    {
      std::map<TKey, TreeNode<TKey,TData>*>::iterator i;
      for (i = m_childs.begin (); i != m_childs.end(); ++i )
	{
	  delete i->second;
	  m_childs.erase (i);
	}
    }
//     template <typename TKey, typename TData>
//     TreeNode<TKey, TData>::operator TType () const
//     {
//       return m_data;
//     }
    template <typename TKey, typename TData>
    bool TreeNode<TKey, TData>::operator < (const TData& data )
    {
      return m_data < data;
    }

    template <typename TKey, typename TData>
    bool TreeNode<TKey, TData>::operator > (const TData& data )
    {
      return m_data > data;
    }

    template <typename TKey, typename TData>
    bool TreeNode<TKey, TData>::operator== ( const TData& data)
    {
      return m_data == data;
    }

    template <typename TKey, typename TData>
    TreeNode<TKey, TData>* TreeNode<TKey, TData>::insert(const TKey& key,
							 const TData& data )
    {
      TreeNode<TKey, TData>* nnode = new TreeNode(key, data, this);
      m_childs[key] = nnode;
      //m_childs.push_back(nnode);
      return nnode;
    }
    template <typename TKey, typename TData>
    TreeNode<TKey, TData>* TreeNode<TKey, TData>::remove ( const TKey& key )
    {
      TreeNode* node = find (key);
      TreeNode* node2 = NULL;
      if ( node )
	{
	  if ( node->m_parent == NULL ) return NULL;
	  
	  std::map<TKey, TreeNode*>::iterator iter;
	  for ( iter = node->m_parent->m_childs.begin ();
		iter != node->m_parent->m_childs.end(); ++iter)
	    {
	      if ( *iter == key )
		{
		  iter.second->m_parent->m_childs.erase (iter);
		  return iter.second;
		}
	    }
	}
      return NULL;
    }
    template <typename TKey, typename TData>
    TreeNode<TKey, TData>* TreeNode<TKey, TData>::find ( const TKey& key)
    {
      if ( m_key == key )
	return this;
      std::stack<TreeNode<TKey,TData>*> Stack;
      TreeNode<TKey,TData>* tp = NULL;
      Stack.push (this);
      while ( !Stack.empty () )
	{
	  tp = Stack.top (); Stack.pop ();
	  if ( tp->get_key() == key ) return tp;
	  std::map<TKey,TreeNode<TKey,TData>*> childs = tp->get_childs ();
	  if ( !childs.empty ())
	    {
	      std::map<TKey,TreeNode<TKey,TData>*>::iterator ci;
	      for ( ci = childs.begin(); ci != childs.end(); ++ci)
		Stack.push (ci.second);
	    }
	}
      return NULL;
    }
    //////////////////////////////////////////////////////////////////////////
    template <typename TKey, typename TData>
    TreeNode<TKey,TData>* TreeNode<TKey, TData>::first()
    {
      m_pos = m_childs.begin ();
      return next ();
    }

    template <typename TKey, typename TData>
    TreeNode<TKey, TData>* TreeNode<TKey, TData>::next ()
    {
      if ( m_pos == m_childs.end () ) return NULL;
      TreeNode<TKey, TData>* elem = m_pos.second;
      m_pos++;
      return elem;
    }
    /////////////////////////////////////////////////////////////////////////
    template<typename TKey, typename TData>
    Tree<TKey, TData>::Tree ( const TKey& key, const TData& data)
    {
      m_root = new TreeNode<TKey, TData>(key, data);
    }
    template<typename TKey, typename TData>
    Tree<TKey, TData>::~Tree ()
    {
      delete m_root;
    }
    template<typename TKey, typename TData>
    void Tree<TKey, TData>::print ()
    {
      std::stack<TreeNode<TKey,TData>*> Stack;
      std::stack<int> istack;
      TreeNode<TKey,TData>* sp;
      Stack.push (m_root);
      istack.push (0);
      while ( !Stack.empty () )
	{
	  int is = istack.top (); istack.pop ();
	  sp = Stack.top (); Stack.pop ();
	  std::string tab = "", str;
	  for ( int i = 0; i < is-1 ; ++i)
	    tab += "  ";
// 	 //  std::cout << tab << sp << endl;
// // 	  if ( sp == NULL ) continue;
	  if ( sp->is_root ())
	    std::cout << tab << "Root" << endl;
	  else 
	    std::cout << tab << sp->get_data() << endl;
	  std::map<TKey,TreeNode<TKey,TData>*> childs = sp->get_childs ();
	  std::map<TKey,TreeNode<TKey,TData>*>::iterator ci;
	  if ( !childs.empty ())
	    {
	      ++is;
	      for (ci = childs.begin(); ci != childs.end(); ++ci)
		{
		  Stack.push (ci->second);
		  istack.push (is);
		}
 	    }
 	  sp = NULL;
 	}
    }
    template<typename TKey, typename TData>
    TreeNode<TKey, TData>* Tree<TKey, TData>::find ( const TKey& key )
    {
//       std::vector<TreeNode<TType>*> childs;
      std::map<TKey, TreeNode<TKey,TData>*>::iterator iter;
//       std::stack<TreeNode<TType>*> Stack;
//       TreeNode<TType>* sp;

      //iter = std::find (m_nodes.begin(), m_nodes.end(), key);
      //if ( iter != m_nodes.end() ) return iter.second;
      iter = m_nodes.find (key);
      if ( iter != m_nodes.end()) return iter->second;
//       for ( iter = m_nodes.begin (); iter < m_nodes.end(); ++iter)
// 	if ( **iter == data ) return *iter;
//       Stack.push (m_root);
//       for ( iter = m_nodes.begin (); iter != m_nodes.end(); iter++)
// 	Stack.push ( *iter );
//       while ( !Stack.empty () )
// 	{
// 	  sp = Stack.top (); Stack.pop ();
// 	  if ( sp->get_data() == data ) return sp;
// 	  childs = sp->get_childs ();
// 	  if ( !childs.empty () )
// 	    for ( iter = childs.begin (); iter != childs.end(); iter++)
// 	      Stack.push(*iter);
// 	}
      return NULL;
    }

    template <typename TKey, typename TData>
    TreeNode<TKey, TData>* Tree<TKey, TData>::insert( const TKey& key,
						      const TData& data,
						      TreeNode<TKey,TData>* parent)
    {
      TreeNode<TKey,TData>* parentnode;
      TreeNode<TKey,TData>* node = NULL;
//       if ( parent == NULL )
// 	{
// 	  parentnode = m_root;
// 	  node = parentnode->insert(data);
// 	  m_nodes.push_back(node);
// 	}
//       else
// 	{
// 	  parentnode = parent;
// 	  node = parentnode->insert(data);
// 	}
      if ( parent == NULL )
	  parentnode = m_root;
      else
	  parentnode = parent;
      node = parentnode->insert(key, data);
      if (node) m_nodes[key] = node;
      return node;
    }
    template <typename TKey, typename TData>
    TreeNode<TKey, TData>* Tree<TKey,TData>::remove ( const TKey& key)
    {
      TreeNode<TKey,TData>* node = find ( key );
      if ( node == NULL ) return NULL;
      return node->remove (key);
    }
  }
}
#endif
