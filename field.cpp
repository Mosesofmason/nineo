////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "field.hpp"
#include <wx/regex.h>
#include "decode.hpp"

namespace NiField
{
    Field::Field ()
    {}
    Field::~Field ()
    {}

    bool Field::operator () ( const wxString& data )
    {
        return Parser (data); 
    }

    bool Field::Parser ( const wxString& data )
    {
        m_rawdata = data;
        m_data.Clear ();
        m_data.Alloc (data.size ());
        m_charset = wxT("Raw");
        wxRegEx re (wxT("=\\?([^\\?]*)\\?(.)\\?([^\\?]*)\\?="));
        unsigned int start = 0, len = 0;
        wxString tdata;
        
        const wxChar *bp, *ep, *dp;
        dp = bp = m_rawdata.c_str();
        ep = bp + m_rawdata.Len ();
        while ( dp < ep )
        {
            if ( *dp == wxT('=') && re.Matches (dp) && re.GetMatchCount() == 4 )
            {
                if ( re.GetMatch ( &start, &len, 1 ) )
                {
                    m_charset = wxString ( (dp + start), len ).Upper();
                }

                if ( re.GetMatch ( &start, &len, 2 ) )
                {
                    m_encodetype = *(dp+start);
                }

                if ( re.GetMatch ( &start, &len, 3 ) )
                {
                    tdata = wxString ((dp + start), len);
                    if ( m_encodetype == wxT('q') || m_encodetype == wxT('Q') )
                    {
                        m_data += NiDecode::QPDecode(tdata);
                    }
                    else 
                        if ( m_encodetype == wxT('B') || m_encodetype == wxT('b') )
                        {
                            m_data += NiDecode::Base64Decode (tdata);
                        }
                }
                if ( re.GetMatch ( &start, &len, 0 ) )
                {
                    dp += len;
                }
            }
            else
            {
                m_data += *dp;
            }
            ++dp;
        }
        return true;
    }
    wxString Field::Data () const
    {
        return m_data;
    }
    wxString Field::DataStripRe () const
    {
        const wxChar *bp, *ep, *dp;
        unsigned int start, len;
        dp = bp = m_data.c_str();
        ep = bp + m_data.Len ();
        wxRegEx re (wxT("^([rR]e|[Rr]e \\[\\d+\\]): "), wxRE_DEFAULT|wxRE_ICASE);
        while ( dp < ep && re.Matches (dp) )
        {
            if ( re.GetMatch ( &start, &len, 0 ) )
            {
                dp += len;
            }
        }
        return wxString ( dp );
    }
    NiUtils::HASH Field::DataStripReHash () const
    {
        return NiUtils::NiHash (DataStripRe());
    }
    wxString Field::Charset () const
    {
        return m_charset;
    }
}
