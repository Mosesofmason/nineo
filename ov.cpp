////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "ov.hpp"
#include "regex.hpp"
#include <wx/wx.h>

namespace NiNntp
{
    Id::Id ()
        : m_id (0)
    {}
    Id::~Id ()
    {}

    unsigned int Id::GetId () const
    {
        return m_id;
    }
    void Id::SetId ( const unsigned int& id )
    {
        m_id = id;
    }

    Lines::Lines ()
        : m_lines (0)
    {}
    Lines::~Lines ()
    {}
    size_t Lines::GetLines () const
    {
        return m_lines;
    }
    void Lines::SetLines ( const size_t& lines )
    {
        m_lines = lines;
    }

    Bytes::Bytes ()
        : m_bytes (0)
    {}
    Bytes::~Bytes ()
    {}
    size_t Bytes::GetBytes () const
    {
        return m_bytes;
    }
    void Bytes::SetBytes ( const size_t& bytes )
    {
        m_bytes = bytes;
    }
    From::From ()
    {}
    From::~From ()
    {}
    wxString From::GetFrom () const
    {
        if ( m_from.get() != NULL )
        {
            return *m_from;
        }
        return wxEmptyString;
    }
    void From::SetFrom ( const wxString& from )
    {
        if ( m_from.get() == NULL )
        {
            m_from = wxStringPtr ( new wxString );
        }
        if ( m_from.get() != NULL )
        {
            *m_from = from;
        }
    }

    Subject::Subject ()
    {}
    Subject::~Subject ()
    {}
    wxString Subject::GetSubject () const
    {
        if ( m_subject.get() != NULL )
        {
            return *m_subject;
        }
       return wxEmptyString; 
    }
    void Subject::SetSubject ( const wxString& subject )
    {
        if ( m_subject.get() == NULL )
        {
            m_subject = wxStringPtr ( new wxString );
        }
        if ( m_subject.get() != NULL )
        {
            *m_subject = subject;
        }
    }
    Date::Date ()
    {}
    Date::~Date ()
    {}
    wxString Date::GetDate () const
    {
        if ( m_date.get() != NULL )
        {
            return *m_date;
        }
        return wxEmptyString;
    }
    void Date::SetDate ( const wxString& date )
    {
        if ( m_date.get() == NULL )
        {
            m_date = wxStringPtr ( new wxString );    
        }
        if ( m_date.get() != NULL )
        {
            *m_date = date;
        }
    }

    MessageID::MessageID ()
    {}
    MessageID::~MessageID ()
    {}
    wxString MessageID::GetMessageID () const
    {
        if ( m_msgid.get() != NULL )
        {
            return *m_msgid;
        }
        return wxEmptyString;
    }
    void MessageID::SetMessageID ( const wxString& msgid )
    {
        if ( m_msgid.get() == NULL )
        {
            m_msgid = wxStringPtr ( new wxString );
        }
        if ( m_msgid.get() != NULL )
        {
            *m_msgid = msgid;
        }
    }

    References::References ()
    {}
    References::~References ()
    {}
    wxString References::GetReferences () const
    {
        if ( m_refs.get() != NULL )
        {
            return *m_refs;
        }
        return wxEmptyString;
    }
    void References::SetReferences ( const wxString& refs )
    {
        if ( m_refs.get() == NULL )
        {
            m_refs = wxStringPtr ( new wxString );
        }
        if ( m_refs.get() != NULL )
        {
            *m_refs = refs;
        }
    }
    XRef::XRef ()
    {}
    XRef::~XRef ()
    {}
    wxString XRef::GetXRef () const
    {
        if ( m_xref.get() != NULL )
        {
            return *m_xref;
        }
        return wxEmptyString;
    }
    void XRef::SetXRef ( const wxString& xref )
    {
        if ( m_xref.get() == NULL )
        {
            m_xref = wxStringPtr ( new wxString );
        }
        if ( m_xref.get() != NULL )
        {
            *m_xref = xref;
        }
    }
    
    Overview::Overview ()
    {}
    Overview::~Overview ()
    {}

    bool Overview::ParserOverview ( const wxString& ov )
    {
#if 1
        NiRegEx::NiPcre re ( NiRegEx::MatchOverview );
#else
        wxRegEx re ( NiRegEx::MatchOverview );
#endif
        if ( re.Matches ( ov ) )
        {
            wxString id, subject, from, msgid, date, refs, xrefs, bytes, lines;
            id      = re.GetMatch(ov, 1);
            subject = re.GetMatch(ov, 2);
            from    = re.GetMatch(ov, 3);
            date    = re.GetMatch(ov, 4);
            msgid   = re.GetMatch(ov, 5);
            refs    = re.GetMatch(ov, 6);
            bytes   = re.GetMatch(ov, 7);
            lines   = re.GetMatch(ov, 8);
            xrefs   = re.GetMatch(ov, 9);
#if 0
                wxLogDebug (wxT("%s|%s|%s|%s|%s|%s|%s|%s|%s"),
                        id.c_str(),
                        subject.c_str(),
                        from.c_str(),
                        date.c_str(),
                        msgid.c_str(),
                        refs.c_str(),
                        bytes.c_str(),
                        lines.c_str(),
                        xrefs.c_str());
#endif

            if ( id.IsNumber () && lines.IsNumber() && bytes.IsNumber() )
            {
                SetId           (wxAtoi (id));
                SetSubject      (subject);
                SetFrom         (from);
                SetDate         (date);
                SetMessageID    (msgid);
                SetReferences   (refs);
                SetBytes        (wxAtoi (bytes));
                SetLines        (wxAtoi (lines));
                SetXRef         (xrefs);
                return true;
            }
        }
        return false;
    }
    Overview& Overview::operator () ( const wxString& ov )
    {
        ParserOverview (ov);
        return *this;
    }
    Overview& Overview::operator  = ( const Overview& ov )
    {
        SetId           ( ov.GetId () );
        SetLines        ( ov.GetLines () );
        SetBytes        ( ov.GetBytes () );
        SetSubject      ( ov.GetSubject () );
        SetFrom         ( ov.GetFrom () );
        SetDate         ( ov.GetDate () );
        SetMessageID    ( ov.GetMessageID () );
        SetReferences   ( ov.GetReferences () );
        SetXRef         ( ov.GetXRef () );
        return *this;
    }
    Overview& Overview::operator << ( Overview& ov )
    {
        m_id        = ov.m_id;
        m_lines     = ov.m_lines;
        m_bytes     = ov.m_bytes;
        m_subject   = ov.m_subject;
        m_from      = ov.m_from;
        m_date      = ov.m_date;
        m_msgid     = ov.m_msgid;
        m_refs      = ov.m_refs;
        m_xref      = ov.m_xref;
        m_from = ov.m_from;
        return *this;
    }

#if 0
    Overview::Overview ()
        : m_id (0), m_bytes(0), m_lines(0)
    {
    }
    Overview::~Overview ()
    {}
    void Overview::Id ( const unsigned int& id ) 
    {
        m_id = id;
    }
    unsigned int Overview::Id () const
    {
        return m_id;
    }
    void Overview::Bytes ( const size_t& bytes )
    {
        m_bytes = bytes;
    }
    size_t Overview::Bytes () const
    {
        return m_bytes;
    }
    void Overview::Lines ( const size_t& lines )
    {
        m_lines = lines;
    }
    size_t Overview::Lines () const
    {
        return m_lines;
    }
    void Overview::Subject ( const wxString& subject )
    {
        if ( m_subject.get () == NULL )
        {
            m_subject = wxStringPtr ( new wxString );
        }

        if ( m_subject.get() != NULL )
        {
            *m_subject = subject;
        }
    }
    wxString Overview::Subject () const
    {
        if ( m_subject.get() != NULL )
        {
            return *m_subject;
        }
        return wxEmptyString;

    }
    void Overview::From ( const wxString& from )
    {
        if ( m_from.get () == NULL )
        {
            m_from = wxStringPtr ( new wxString );
        }
        
        if ( m_from.get() != NULL )
        {
            *m_from = from;
        }
    }
    wxString Overview::From () const
    {
        if ( m_from.get() != NULL )
        {
            return *m_from;
        }
        return wxEmptyString;
     }
    void Overview::Date ( const wxString& date )
    {
        if ( m_date.get () == NULL )
        {
            m_date = wxStringPtr ( new wxString );
        }

        if ( m_date.get() != NULL )
        {
           *m_date = date;
        }
    }
    wxString Overview::Date () const
    {
        if ( m_date.get() != NULL )
        {
            return *m_date;
        }
        return wxEmptyString;
    }
    void Overview::MessageID ( const wxString& msgid )
    {
        if ( m_msgid.get () == NULL )
        {
            m_msgid = wxStringPtr ( new wxString );
        }
        if ( m_msgid.get() != NULL )
        {
            *m_msgid = msgid;
        }
    }
    wxString Overview::MessageID () const
    {
        if ( m_msgid.get() != NULL )
        {
            return *m_msgid;
        }
        return wxEmptyString;
    }
    void Overview::References ( const wxString& refs )
    {
        if ( m_refs.get () == NULL )
        {
            m_refs = wxStringPtr ( new wxString );
        }

        if ( m_refs.get() != NULL )
        {
            *m_refs = refs;
        }
    }
    wxString Overview::References () const
    {
        if ( m_refs.get () != NULL )
        {
            return *m_refs;
        }
        return wxEmptyString;
    }
    void Overview::XRef ( const wxString& xref )
    {
        if ( m_xref.get () == NULL )
        {
            m_xref = wxStringPtr ( new wxString );
        }

        if ( m_xref.get() != NULL )
        {
            *m_xref = xref;
        }
    }
    wxString Overview::XRef () const
    {
        if ( m_xref.get () != NULL )
        {
            return *m_xref;
        }
        return wxEmptyString;
    }
    Overview& Overview::operator() ( const wxString& ov )
    {
        ParserOV ( ov );
        return *this;
    }
    inline bool Overview::ParserOV ( const wxString& ov )
    {
        wxRegEx re ( NiRegEx::MatchOverview );
        if ( re.Matches ( ov ) )
        {
            wxString id, subject, from, msgid, date, refs, xrefs, bytes, lines;
            id      = re.GetMatch(ov, 1);
            subject = re.GetMatch(ov, 2);
            from    = re.GetMatch(ov, 3);
            date    = re.GetMatch(ov, 4);
            msgid   = re.GetMatch(ov, 5);
            refs    = re.GetMatch(ov, 6);
            bytes   = re.GetMatch(ov, 7);
            lines   = re.GetMatch(ov, 8);
            xrefs   = re.GetMatch(ov, 9);
            if ( id.IsNumber () && lines.IsNumber() && bytes.IsNumber() )
            {
#if 0
                wxLogDebug (wxT("%s|%s|%s|%s|%s|%s|%s|%s|%s"),
                        id.c_str(),
                        subject.c_str(),
                        from.c_str(),
                        date.c_str(),
                        msgid.c_str(),
                        refs.c_str(),
                        bytes.c_str(),
                        lines.c_str(),
                        xrefs.c_str());
#endif
                m_id        = wxAtoi (id);
                Subject     (subject);
                From        (from);
                Date        (date);
                MessageID   (msgid);
                References  (refs);
                m_bytes     = wxAtoi (bytes);
                m_lines     = wxAtoi (lines);
                XRef        (xrefs);
                return true;
            }
        }
        return false;
    }

    Overview& Overview::operator =  ( const Overview& ov )
    {
        Id (ov.Id ());
        Subject (ov.Subject());
        From (ov.From());
        Date (ov.Date());
        MessageID (ov.MessageID());
        References (ov.References());
        Bytes (ov.Bytes());
        Lines (ov.Lines());
        XRef (ov.XRef());
        return *this;
    }
#endif
}
