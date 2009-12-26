#ifndef __NEXT_NINEO_UTILS_DEFINE__
#define __NEXT_NINEO_UTILS_DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////
#include <wx/longlong.h>

class wxString;
namespace NiUtils
{
    typedef unsigned long NiHASH;
    typedef unsigned long HASH;
    HASH NiHash ( const wxString& str );
    class Hash
    {
        private:
            HASH m_hash;
        public:
            Hash ();
            ~Hash ();
            void MakeHash ( const wxString& data );
            HASH GetHash () const;
    };
    class TimeReport
    {
        public:
            TimeReport ();
            ~TimeReport ();
            void Start ();
            void End ();
            TimeReport& operator () ( const wxString& topic = wxT("") );
        private:
            wxLongLong m_start, m_end;
    };
#if 0
    inline void NiAssert ( const bool& expr, const wxString& msg = wxT("") )
    {
#ifdef __BETA__
        wxLogMessage ( wxT("%s [%d] #s"), __FILE__, __LINE__, msg.c_str() );
#else
        NULL;
#endif
    }
#endif
};

#endif //
