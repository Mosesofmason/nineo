////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "regex.hpp"

namespace NiRegEx
{
    size_t NiPcre::MAX_VECTOR_SIZE = 100;

    NiPcre::NiPcre ( const size_t& vc_size )
        : m_re (NULL)
    {
       InitVector ( vc_size );
    }
    NiPcre::NiPcre ( const wxString& pattern, const size_t& vc_size )
        : m_re (NULL)
    {
        m_options = PCRE_NO_UTF8_CHECK;
        Compile ( pattern );
        InitVector ( vc_size );
    }
    void NiPcre::InitVector ( const size_t& vc_size )
    {
        m_vector.reserve ( ( vc_size + 1 ) * 3 );
        m_vecpairs.reserve (vc_size);
    }
    NiPcre::~NiPcre ()
    {
        if ( IsValid () )
        {
            pcre_free (m_re);
            m_re = (pcre*)NULL;
        }
    }
    bool NiPcre::Compile ( const wxString& pattern )
    {
        wxCharBuffer buff = pattern.To8BitData ();
        return __Compile__ ( buff.data(), pattern.Len() );
        return false;
    }
    bool NiPcre::UCompile ( const wxString& pattern )
    {
        wxCharBuffer buff = pattern.ToUTF8 ();
        return __Compile__ ( buff.data(), pattern.Len () );
    }
    bool NiPcre::__Compile__ ( const char* cpattern, const size_t& len )
    {
        m_vecpairs.clear ();
        if ( IsValid () ) 
        {
            pcre_free ( m_re );
            m_re = (pcre*)NULL;
        }
        const char *error = (const char*) NULL; 
        int erroffset;
        m_re = pcre_compile ( cpattern, 0, &error, &erroffset, NULL );
        if ( m_re == NULL )
        {
            m_error = wxString::Format (wxT("PCRE compilation faild at offset %d: %s\n"),
                                        erroffset, wxString::From8BitData (error));
            if ( error != NULL )
            {   
                delete [] const_cast <char*> (error);
            }
            wxLogDebug ( m_error );
            return false;
        }
        return true;
    }
    bool NiPcre::Matches ( const wxString& data )
    {
        wxCharBuffer buff = data.To8BitData ();
        return __Matches__ ( buff.data(), strlen (buff.data()) );
        return false;
    }
    bool NiPcre::UMatches ( const wxString& data )
    {
        return false;
    }
    bool NiPcre::__Matches__ ( const char* buf, const size_t& len )
    {
        m_vecpairs.clear ();
        int rc, subject_length;
        rc = pcre_exec ( m_re, NULL, buf, len, 
                         0, m_options, &m_vector[0], m_vector.capacity() );
        if ( rc < 0 )
        {
            switch ( rc )
            {
                case PCRE_ERROR_NOMATCH: m_error = _("PCRE No match"); break;
                default :
                         m_error = wxString::Format (_("PCRE Mathing error: %d"), rc);
                         break;
            };
            return false;
        }
        else
        {
            const char* substring = NULL;
            int x = 0, start = 0, end = 0;
            for ( size_t pos = 0; pos < rc; ++pos )
            {
                start   = m_vector [ pos * 2 ];
                end     = m_vector [ pos * 2 + 1 ];
                m_vecpairs.push_back ( std::make_pair ( start, end ) );
#if 0
                x = pcre_get_substring ( buf,  &m_vector[0], m_vector.max_size (),
                                         pos, &substring );
                if ( x != PCRE_ERROR_NOMEMORY && x != PCRE_ERROR_NOSUBSTRING )
                {
                    wxLogDebug (wxT("PCRE Match (%3d) [%5d | %5d] %s"), 
                                m_vecpairs.size(), start, end,
                                wxString::From8BitData (substring).c_str());
                    pcre_free_substring ( substring );
                }
#endif
            }
            return true;
        }
    }
    bool NiPcre::FindAll ( const wxString& data )
    {
        if ( data == wxEmptyString ) return false;
        wxCharBuffer buf = data.To8BitData ();
        return __FindAll__ ( buf.data(), data.Len() );
    }
    bool NiPcre::__FindAll__ ( const char* buf, const size_t& len )
    {
        const char *bptr = NULL, *ptr = NULL, *eptr = NULL, *substring = NULL;
        bptr = ptr = buf;
        eptr = buf + len;
        int rc, x, start, end, offset;
        m_vecpairs.clear ();
        while ( ptr < eptr )
        {
            rc = pcre_exec ( m_re, NULL, ptr, eptr - ptr, 0, 0, 
                            &m_vector[0], m_vector.capacity());
            if ( rc < 0 )
            {
                break;
            }
            else
            {
                offset = ( ptr - bptr );
                for ( size_t pos = 1; pos < rc; ++pos )
                {
                    start   = m_vector[ pos * 2 ] + offset;
                    end     = m_vector[ pos * 2 + 1 ] + offset;
                    m_vecpairs.push_back ( std::make_pair ( start, end ) );
#if 0 
                    substring = NULL;
                    x = pcre_get_substring ( ptr, &m_vector[0], m_vector.max_size(), 
                                             pos, &substring );

                    if ( x >= 0 )
                    {
                        wxLogDebug (wxT("PCRE FindAll (%3d) [%5d | %5d] %s"), 
                                    m_vecpairs.size(), start, end, 
                                    wxString::From8BitData (substring, end-start).c_str());
                    }
                    if ( substring != NULL )
                        pcre_free_substring (substring);
#endif
                }
                ptr =  ptr + m_vector [ 1 ] + 1;
            }
        }
        return m_vecpairs.size() != 0;
    }
    size_t NiPcre::GetMatchCount () const
    {
        return m_vecpairs.size ();
    }
    void NiPcre::GetMatch ( size_t& start, size_t& end, const size_t& match ) const
    {
        start   = GetMatchStart ( match );
        end     = GetMatchEnd   ( match );
    }
    wxString NiPcre::GetSubString ( const wxString& data, const size_t& match ) const 
    {
        wxASSERT ( match < m_vecpairs.size () );
        wxCharBuffer buf = data.To8BitData ();
        int start, end, len;
        
        start   = m_vecpairs [ match ].first;
        end     = m_vecpairs [ match ].second;
        len     = data.Len ();

        if ( len == 0 || len < end || match > m_vecpairs.size ())
        {
            return wxEmptyString;
        }
        return wxString::From8BitData ( buf.data() + start, end - start );
    }
    size_t NiPcre::GetMatchStart ( const size_t& match ) const
    {
        wxASSERT ( match < m_vecpairs.size () );
        return m_vecpairs [ match ].first;
    }
    size_t NiPcre::GetMatchEnd ( const size_t& match ) const
    {
        wxASSERT ( match < m_vecpairs.size () );
        return m_vecpairs [ match ].second;
    }
    size_t NiPcre::GetMatchLen ( const size_t& match ) const
    {
        return ( GetMatchEnd ( match ) - GetMatchStart ( match ) );
    }
    bool NiPcre::IsValid ()
    {
        return m_re != NULL;
    }
    wxString NiPcre::Version ()
    {
        return wxString::Format(wxT("PCRE Version: %s"),
                                wxString::From8BitData ( pcre_version () ).c_str() );
    }
    RegEx::RegEx ()
    {}
    RegEx::~RegEx ()
    {}
    RegEx::RegEx ( const wxString& expr, int flags )
        : wxRegEx ( expr, flags )
    {}

    bool RegEx::Compile ( const wxString& pattern, int flags )
    {
       return wxRegEx::Compile ( pattern, flags );
    }
    bool RegEx::IsValid ()
    {
        return wxRegEx::IsValid ();
    }
    bool RegEx::GetMatch ( size_t& start, size_t& len, size_t index ) const
    {
        return wxRegEx::GetMatch (&start, &len, index );
    }
    wxString RegEx::GetMatch ( const wxString& text, size_t index ) const
    {
        return wxRegEx::GetMatch ( text, index );
    }
    size_t RegEx::GetMatchCount () const
    {
        return wxRegEx::GetMatchCount ();
    }
    bool RegEx::Matches ( const wxChar* text, int flags ) const
    {
        return wxRegEx::Matches ( text, flags );
    }
    bool RegEx::Matches ( const wxChar* text, int flags, size_t len ) const
    {
        return wxRegEx::Matches ( text, flags, len );
    }
    bool RegEx::Matches ( const wxString& text, int flags ) const
    {
        return wxRegEx::Matches ( text, flags );
    }
    bool RegEx::MatchesAll ( const wxString& text, int flags ) const
    {
        bool ismatch = false;
        const wxChar *bptr = NULL, *ptr = NULL, *eptr = NULL;
        unsigned int start, end, count, len;
        bptr = ptr = text.c_str();
        eptr = bptr + text.Len ();
        size_t freq = text.Freq (wxT('<')), mcount = 0;
        //wxLogDebug (wxT("%d >> %s"), text.Len(), bptr ); 
        while ( ptr < eptr )
        {
            if ( Matches ( ptr ) )
            {
                ismatch = true;
                count = GetMatchCount ();
                for ( size_t pos = 1; pos < count; ++pos )
                {
                    if ( GetMatch ( start, end, pos ) )
                    {
                        start   += ptr - bptr;
                        end     += ptr - bptr;
                        if ( start > text.Len () || end > text.Len () || start > end ) 
                            continue;
                        ++mcount;
                        //wxLogDebug (wxT("(%3d) [ %5d, %5d ] %s"), pos, start, end);
                        //wxLogDebug (wxT("%s"),text.Mid( start, end - start ).c_str());
                    }
                }
                if ( GetMatch ( start, end, 0 ) )
                {
                    // wxLogDebug (wxT("%d - %d"), start, end );
                    ptr = ptr + end + 1;
                }
            }
            else
            {
                break;
            }
        }
        if ( freq != mcount )
            wxLogDebug (wxT("%d %d, %s"), freq, mcount, text.c_str());
        return ismatch;
    }
    int RegEx::Replace ( wxString* text, const wxString& replacement, size_t maxMatches ) const 
    {
        return wxRegEx::Replace ( text, replacement, maxMatches );
    }
    int RegEx::ReplaceAll ( wxString* text, const wxString& replacement ) const
    {
        return wxRegEx::ReplaceAll ( text, replacement );
    }
    int RegEx::ReplaceFirst ( wxString* text, const wxString& replacement ) const
    {
        return wxRegEx::ReplaceFirst ( text, replacement );
    }
};
