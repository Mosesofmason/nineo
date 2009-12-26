#ifndef __NEXT_NINEO_FIELD_DEFINE__
#define __NEXT_NINEO_FIELD_DEFINE__

#include <wx/wx.h>
#include "regex.hpp"
#include "utils.hpp"

namespace NiField
{
    class Field
    {
        public:
            Field ();
            ~Field ();
            bool operator () ( const wxString& data );
            bool Parser ( const wxString& data );
            wxString Data () const;
            wxString DataStripRe () const;
            wxString Charset () const;
            NiUtils::HASH DataStripReHash () const;
        private:
            wxString m_data, m_charset, m_rawdata;
            wxChar m_encodetype;
    };
};
#endif //


