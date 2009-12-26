////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "group.hpp"
#include <wx/dynarray.h>

namespace NiGroup
{
    NiGroup::NiGroup ()
    {}

    NiGroup::~NiGroup ()
    {}

    wxString NiGroup::Name () const
    {
        return m_name;
    }

    void NiGroup::SetName ( const wxString& name )
    {
        m_name = name;
    }

    wxString NiGroup::Prefix () const
    {
        return m_name.Prefix ();
    }

    NiUtils::NiHASH NiGroup::PrefixHash () const
    {
        return m_name.PrefixHash ();
    }
    NiUtils::NiHASH NiGroup::NameHash () const
    {
        return m_name.Hash();
    }
    NiUnsubGroup::NiUnsubGroup ()
        : m_last(0), m_first(0), m_post(false)
    {}

    NiUnsubGroup::NiUnsubGroup ( const wxString& name, 
                                 const unsigned int& first,
                                 const unsigned int& last,
                                 const bool& post )
    {
        SetName (name);
        First (first);
        Last (last);
        Post (post);
    }
    NiUnsubGroup::~NiUnsubGroup ()
    {}

    bool NiUnsubGroup::IsValid () const
    {
        return ( !m_name.IsEmpty() && m_last != 0 && m_first != 0 );
    }
    unsigned int NiUnsubGroup::Last () const
    {
        return m_last;
    }
    unsigned int NiUnsubGroup::First () const
    {
        return m_first;
    }
    bool NiUnsubGroup::Post () const
    {
        return m_post;
    }
    void NiUnsubGroup::Last ( const unsigned int& last )
    {
        m_last = last;
    }
    void NiUnsubGroup::First ( const unsigned int& first )
    {
        m_first = first;
    }
    void NiUnsubGroup::Post ( const bool& post )
    {
        m_post = post;
    }

    NiUnsubGroup& NiUnsubGroup::operator () ( const wxString& line )
    {
        m_name.Clear();
        m_first = 0;
        m_last = 0;
        m_post = false;
        const wxRegEx re ( NiRegEx::MatchListGroup );
        if (re.Matches (line) && re.GetMatchCount() == 5 )
        {
            wxString name, first, last, post;
            name = re.GetMatch (line, 1);
            first = re.GetMatch (line, 3);
            last = re.GetMatch (line, 2);
            post = re.GetMatch (line,4);
            if ( !name.IsEmpty() && first.IsNumber() && 
                    last.IsNumber() && !post.IsEmpty() )
            {
                m_name = name;
                m_first = wxAtoi(first);
                m_last = wxAtoi (last);
                m_post = ( post == wxT("y") ? true : false );
            }
        }
        return *this;
    }
    
    SubGroup::SubGroup ( const wxString& server, const wxString& name )
        : m_server (server)
    {
        SetName (name); 
    }
    SubGroup::SubGroup ()
    {}
    SubGroup::~SubGroup ()
    {}
    wxString SubGroup::GetServer () const
    {
        return m_server;
    }
    void SubGroup::SetServer ( const wxString& server )
    {
        m_server = server;
    }
    bool SubGroup::operator== ( const SubGroup& t )
    {
        return ( m_server == t.GetServer() && m_name == t.Name() );
    }
};
