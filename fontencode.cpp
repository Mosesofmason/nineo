
////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "fontencode.hpp"

namespace NiFontEncode
{
    EncodeArray FontEncode::Encodes;

    void FontEncode::InitEncodeList ()
    {
        ClearEncodes ();
        size_t count = wxFontMapper::GetSupportedEncodingsCount ();
        EncodeType e;
        Encodes.reserve (count);

        for ( size_t pos = 0; pos < count; pos++ )
        {
            GetEncoding ( pos, e );
            if ( e.enc != 0 )
            {
                EncodeType* ep = new EncodeType;
                if ( ep )
                {
                    FixName (e);
                    *ep = e;
                    Encodes.push_back (ep);
#if 0
                    wxLogDebug (wxT("%2d, %2d %20s"), pos + 1, e.enc, e.name.c_str());
#endif
                }
            }
        }
    }
    void FontEncode::FreeEncodeList ()
    {
        ClearEncodes ();
    }
    inline void FontEncode::GetEncoding ( const size_t& idx, EncodeType& e )
    {
        e.enc   = wxFontMapper::GetEncoding (idx);
        e.name  = wxFontMapper::GetEncodingName (e.enc);
        e.desc  = wxFontMapper::GetEncodingDescription (e.enc);
    }
    struct OF_ClearEncodes 
    {
        OF_ClearEncodes& operator () ( EncodeArray::value_type& e )
        {
            if ( e != NULL )
            {
                delete e;
                e = NULL;
            }
            return *this;
        }
    };
    void FontEncode::ClearEncodes ()
    {
        if ( !Encodes.empty() )
        {
            std::for_each ( Encodes.begin(), Encodes.end(), OF_ClearEncodes ());
            Encodes.clear();
        }
    }
    inline void FontEncode::FixName ( EncodeType& e )
    {
        switch(e.enc)
        {
            case wxFONTENCODING_CP932:          // Japanese (shift-JIS)
                {
                    e.name = _("SHIFT-JIS [932]");
                    break;
                }
            case wxFONTENCODING_CP936:          // Chinese simplified (GB)
                {
                    e.name  = _("GB2312/GBK [936]");
                    break;
                }
            case wxFONTENCODING_CP949:          // Korean (Hangul charset)
                {
                    e.name  = _("EUC-KR [949]");
                    break;
                }
            case wxFONTENCODING_CP950:          // Chinese (traditional - Big5)
                {
                    e.name = _("BIG5 [950]");
                    break;
                }
            case wxFONTENCODING_EUC_JP:         // Extended Unix Codepage for Japanese
                {
                    e.name  = _("EUC-JP [51932]");
                    break;
                }
            case wxFONTENCODING_CP437:
                {
                    e.name  = _("CP437");
                    break;
                }
                //case wxFONTENCODING_UTF7:    // not support
                //break;
                //case wxFONTENCODING_UTF8:
                //break;
            case wxFONTENCODING_UTF16BE:
                {
                    e.name  = _("UTF-16BE");
                    e.desc  = _("Unicode 16 bit Big Endian (UTF-16BE)");
                    break;
                }
            case wxFONTENCODING_UTF16LE:
                {
                    e. name = _("UTF-16LE");
                    e.desc  = _("Unicode 16 bit Little Endian (UTF-16LE)");
                    break;
                }
            case wxFONTENCODING_UTF32BE:
                {
                    e.name  = _("UTF-32BE");
                    e.desc  = _("Unicode 32 bit Big Endian (UTF-32BE)");
                    break;
                }
            case wxFONTENCODING_UTF32LE:
                {
                    e.name  = _("UTF-32LE");
                    e.desc  = _("Unicode 32 bit Little Endian (UTF-32LE)");
                    break;
                }
            default:
                break;
        };
    }

    FontEncode::FontEncode ()
    {
    }
    
    FontEncode::~FontEncode ()
    {}

    FontEncodeInit::FontEncodeInit ()
    {
        FontEncode::InitEncodeList ();
    }
    FontEncodeInit::~FontEncodeInit ()
    {
        FontEncode::FreeEncodeList ();
    }
}
