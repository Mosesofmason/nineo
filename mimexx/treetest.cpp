#include "xtree.hpp"
#include <iostream>
#include <string>

typedef Tree<std::string, std::string> MyTree;
typedef TreeNode<std::string, std::string> MyNode;

int main(int argc, char **argv)
{
  MyTree* m_tree;
  // Construct a tree with ROOT node
  m_tree = new MyTree("ROOT", "ROOT");

  MyNode* node = NULL;
      
  // Add first level nodes to the tree, using NULL will add
  // the node as the child of the root
  m_tree->Insert("Node1_1", "Data0", NULL);
  node = m_tree->Insert("Node1_2", "Data1");
      
  // Add second level nodes to 'node'
  m_tree->Insert("Node2_1", "data0", node);
  m_tree->Insert("Node2_1", "data0", node);
  m_tree->Insert("Node2_2", "data0", node);
  m_tree->Insert("Node2_3", "data0", node);

  // Add another level 0 node
  node = m_tree->Insert("Node1_3", "Data3", NULL);

  m_tree->Insert("Node3_1", "data0", node);
  m_tree->Insert("Node3_2", "data0", node);
  m_tree->Insert("Node3_3", "data0", node);
  m_tree->Print ();
  m_tree->Print2 ();
  // Demonstrate removal of items,
  // Remove() function returns the node that was just removed 
  // note that it does not delete it
  MyNode* node1 = m_tree->Remove("Node1_1");
  MyNode* node2 = m_tree->Remove("Node1_2");

  // Delete the remove nodes
  delete node1;
  delete node2;
  
  //m_tree->Print2 ();
  // Test tree walker
  // Construct a tree walker from root 
//   TreeWalker<std::string, std::string> walker(m_tree->GetRoot());

//   for(; !walker.End(); walker++)
//     {
//       MyNode *node = walker.GetNode();
//       std::cout << node->GetKey()<< std::endl;
//     }

  delete m_tree;
  return 0;
}
