////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "observer.hpp"
#include <algorithm>

namespace NiUtils
{
    ObClient::ObClient ()
        : m_server (NULL)
    {}
    ObClient::~ObClient ()
    {
        if ( m_server != NULL )
        {
            (*m_server) -= this;
        }
    }

    ObServer::ObServer ()
    {
        m_clientlist.reserve (50);
    }
    ObServer::~ObServer ()
    {
        ObClientList::iterator bi, ei;
        bi = m_clientlist.begin ();
        ei = m_clientlist.end ();
        for ( ; bi < ei ; ++bi )
        {
            (*bi)->m_server = NULL;
        }
    }

    void ObServer::ObUpdate ( const int& uid )
    {
        ObClientList::iterator bi, ei;
        bi = m_clientlist.begin ();
        ei = m_clientlist.end ();
        for ( ; bi < ei; ++bi )
        {
            (*bi)->ObUpdate ( *this, uid );
        }
    }
    bool ObServer::HasClient ( ObClient* client )
    {
        ObClientList::iterator itr;
        itr = std::find (m_clientlist.begin(), m_clientlist.end(), client);
        return ( itr != m_clientlist.end() );
    }
    ObServer& ObServer::operator+= ( ObClient* client )
    {
        if ( !HasClient (client) )
        {
            m_clientlist.push_back (client);
        }
        return *this;
    }
    
    ObServer& ObServer::operator-= ( ObClient* client )
    {
        ObClientList::iterator itr;
        itr = std::find ( m_clientlist.begin(), m_clientlist.end(), client);
        if ( itr != m_clientlist.end() )
        {
            m_clientlist.erase ( itr );
        }
        return *this;
    }
}
