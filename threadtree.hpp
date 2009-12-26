#ifndef __NEXT_NINEO_TREE_DEFINE__
#define __NEXT_NINEO_TREE_DEFINE__
/**
 *    \file  tree.hpp
 *   \brief  树状数据结构
 *
 * 实现树状结构，同时实现下标式随机树结构节点的功能。
 *
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-25
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++,borland free commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */
#include <wx/wx.h>
#include "config.hpp"
#include <vector>
#include <memory>
#include <algorithm>

namespace NiTree
{
    class Node;
    class Tree;
    typedef std::vector < Node* > NodePtrVector;
    class Node
    {
        friend class Tree;
        public:
            Node ();
            ~Node ();
            size_t NodeCount () const;
            bool IsDummy () const;
            size_t Depth () const;
        protected:
            virtual wxString Label () const;
        private:
            void Reset ();
            static int print_num;
            void DelAll ();
            void DelNode ( Node* node );
            size_t Sum ( const size_t& depth = 0 );
            void AddChild ( Node* node );
            void Print () const;
            Node *m_parent; 
            NodePtrVector m_childs;
            size_t m_nodecount, m_depth;
    };
    typedef std::auto_ptr < Node > NodePtr;
    class Tree
    {
        public:
            Tree ();
            ~Tree ();
            Node* Root () const;
            size_t NodeCount ();
            Node* NodeLink ( Node* node );
            Node* NodeLink ( Node* parent, Node* node );
            void Print () const;
            Node* At ( const size_t& idx ) const;
            void Clean ();
            void NodeReset ( Node* node );
            void Reset ();
        private:
            static Node* Find ( size_t& count, Node* node );
            void InitRoot ();
            NodePtr m_root;
            size_t m_nodecount;
    };
};
#endif //


