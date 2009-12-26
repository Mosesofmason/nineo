////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "threadtree.hpp"
#include <algorithm>

namespace NiTree
{
    struct OF_CLEAR_CHILDS
    {
        void operator () ( NodePtrVector::value_type& node )
        {
            if ( node != NULL )
            {
                delete node;
                node = NULL;
            }
        }
    };
    int Node::print_num = -1;
    Node::Node ()
     : m_parent(NULL), m_nodecount (0), m_depth (0)
    {}
    Node::~Node ()
    {
#if 0
        if ( !m_childs.empty () )
        {
            std::for_each ( m_childs.begin(), m_childs.end(), OF_CLEAR_CHILDS () );
            m_childs.clear ();
        }
#endif
    }
    void Node::Reset ()
    {
        m_parent = NULL;
        m_childs.clear ();
        m_depth = m_nodecount = 0;
    }
    size_t Node::Depth () const
    {
#if 0
        Node* cur = m_parent;
        size_t depth = 0;
        while ( cur )
        {
            ++depth;
            cur = cur->m_parent;
        }
#endif
        return m_depth;
    }
    size_t Node::NodeCount () const 
    {
        return m_nodecount;
    }
    bool Node::IsDummy () const
    {
       return ( m_parent != NULL ); 
    }
    void Node::AddChild ( Node* node )
    {
        wxASSERT ( node != this );
#if 0
        size_t childcount = node->m_nodecount + 1;
        Node* cur = node->m_parent;
        while ( cur )
        {
            cur->m_nodecount -= childcount;
            cur = cur->m_parent;
        }
        cur = this;
        while ( cur )
        {
            cur->m_nodecount += childcount;
            cur = cur->m_parent;
        }
#endif
        if ( node->m_parent != NULL )
        {
            NodePtrVector::iterator itr = std::find ( node->m_parent->m_childs.begin(),
                                                      node->m_parent->m_childs.end (),
                                                      node );
            if ( itr != node->m_parent->m_childs.end () )
            {
                node->m_parent->m_childs.erase (itr);
            }
        }
        node->m_parent = this;
        m_childs.push_back ( node );
    }
    wxString Node::Label () const
    {
        return wxString::Format (wxT("%d"), this);
    }
    void Node::Print () const
    {
        size_t depth = Depth ();
        wxString space;
        while ( depth-- != 0 ) space += wxT(" ");
        if ( print_num < 0 )
        {
            wxLogDebug (wxT("%s Node count: %d, %d"),
                        space.c_str(), NodeCount(), Label().c_str());
        }
        else
        {
            wxLogDebug (wxT("%s[%6d] Node: %d, %d"),
                        space.c_str(), print_num, NodeCount(), Label().c_str());
            ++print_num;
        }
        Node* node = NULL;
        for ( size_t pos = 0, count = m_childs.size(); pos < count ; ++pos )
        {
            node = m_childs[pos];
            if ( node ) node->Print ();
        }
    }
    void Node::DelNode ( Node* node )
    {
    }
    size_t Node::Sum ( const size_t& depth )
    {
        m_nodecount = 0;
        m_depth = depth + 1;
        if ( !m_childs.empty () )
        {
            for ( size_t pos = 0, count = m_childs.size(); pos < count; ++pos )
            {
                m_nodecount += m_childs [ pos ]->Sum (m_depth) + 1;
            }
        }
        return m_nodecount;
    }
    
    void Node::DelAll ()
    {
        m_nodecount = 0;
        if ( !m_childs.empty() )
        {
            Node* node = NULL;
            for ( size_t pos = 0, count = m_childs.size(); pos < count; ++pos )
            {
                node = m_childs [pos];
                if ( node != NULL )
                {
                    node->DelAll ();
                }
            }
        }
    }
    Tree::Tree ()
        : m_nodecount(0)
    {
        InitRoot ();
    }
    Tree::~Tree ()
    {}
    void Tree::Clean ()
    {
        if ( m_root.get() != NULL )
            m_root->DelAll ();
    }
    void Tree::NodeReset ( Node* node )
    {
        if ( node != NULL )
            node->Reset ();
    }
    void Tree::Reset ()
    {
        if ( m_root.get() != NULL )
        {
            m_root->Reset ();
        }
        m_nodecount = 0;
    }
    void Tree::InitRoot ()
    {
        if ( m_root.get() == NULL )
        {
            m_root = NodePtr ( new Node );
        }
        else
        {
            // m_root->DelAll ();
        }
    }
    Node* Tree::Root () const
    {
        return m_root.get();
    }
    size_t Tree::NodeCount () 
    {
        if ( m_nodecount == 0 && m_root.get() != NULL )
        {
            m_nodecount = m_root->Sum (); 
        }
        return m_nodecount;
    }
    Node* Tree::NodeLink ( Node* parent, Node* node )
    {
        wxASSERT ( parent != NULL && node != NULL );
        parent->AddChild ( node );
        m_nodecount = 0;
        return node;
    }
    Node* Tree::NodeLink ( Node* node )
    {
        return NodeLink ( m_root.get(), node );
    }
    void Tree::Print () const
    {
        Node::print_num = 0;
        if ( m_root.get () != NULL )
        {
            m_root->Print ();
        }
        Node::print_num = -1;
    }
    Node* Tree::At ( const size_t& idx ) const
    {
        wxASSERT ( idx < m_nodecount && idx >=  0 );
        Node* node = NULL;
        if ( m_nodecount )
        {
            size_t count = idx + 1;
            node = Find ( count, m_root.get() );
        }
        else 
        {
           node = m_root.get();
        }
        return node;
    }
    Node* Tree::Find ( size_t& count, Node* node )
    {
        Node* result = NULL, *tnode = NULL;
        if ( count == 0 )
        {
            result = node;
        }
        else if ( node != NULL )
        {
            --count;
            size_t tcc = 0;
            for ( size_t pos = 0, xc = node->m_childs.size(); pos < xc; ++pos )
            {
                tnode = node->m_childs [ pos ];
                tcc = tnode->NodeCount ();
                if ( tcc >= count )
                {
                    result = Find ( count, tnode);
                    break;
                }
                else
                {
                    count -= tcc + 1;
                }
            }
        }
        return result;
    }
};
