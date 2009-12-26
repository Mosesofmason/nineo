#ifndef __NEXT_NINEO_FONTENCODE_DEFINE__
#define __NEXT_NINEO_FONTENCODE_DEFINE__

#include <wx/wx.h>
#include "config.hpp"
#include <wx/fontmap.h>
#include <vector>
#include <algorithm>
#include <wx/fontenum.h>

namespace NiFontEncode
{
    struct EncodeType
    {
        wxString name, desc;
        wxFontEncoding enc;
        EncodeType& operator= ( const EncodeType& e )
        {
            name    = e.name;
            desc    = e.desc;
            enc     = e.enc;
            return *this;
        }
    };
    typedef std::vector <EncodeType*> EncodeArray;
    class FontEncodeInit;
    class FontEncode
    {
        public:
            FontEncode ();
            ~FontEncode ();
        protected:
            friend class FontEncodeInit;
            static void InitEncodeList ();
            static void FreeEncodeList ();
            static EncodeArray Encodes;
        private:
            static void ClearEncodes ();
            static inline void GetEncoding ( const size_t& idx, EncodeType& e );
            static inline void FixName ( EncodeType& e );
    };
    class FontEncodeInit
    {
        public:
            FontEncodeInit ();
            ~FontEncodeInit ();
    };
}

#endif //


