////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "utils.hpp"
#include <wx/wx.h>
#include "log.hpp"

namespace NiUtils
{
    NiHASH NiHash ( const wxString& str )
    {
        NiHASH hash = 5381;
        int c = 0;
        const wxCharBuffer buf = str.To8BitData ();
        const char *bp = buf.data(), *ep = bp + strlen(buf.data());
        while ( ep > bp )
        {
            c = (int)(unsigned char)*bp;
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
            ++bp;
        }
        return hash;
    }
    Hash::Hash ()
        : m_hash (0)
    {}

    Hash::~Hash ()
    {}

    void Hash::MakeHash ( const wxString& data )
    {
        m_hash = NiHash (data);
    }
    HASH Hash::GetHash () const
    {
        return m_hash;
    }
    TimeReport::TimeReport ()
        : m_start (0,0), m_end (0,0)
    {}
    TimeReport::~TimeReport ()
    {}
    void TimeReport::Start ()
    {
        m_start = wxGetLocalTimeMillis ();
    }
    void TimeReport::End ()
    {
        m_end = wxGetLocalTimeMillis ();
    }
    TimeReport& TimeReport::operator () ( const wxString& topic )
    {
        wxLongLong time = m_end - m_start;
        wxString message = wxString::Format (wxT("Time: %3.20lf (%10ld/1000) #%s"), 
                                             time.ToDouble() * 0.001, time.ToLong (),
                                             topic.c_str());
        NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
        loger->Debug (message);
        return *this;
    }
};


