////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "nntp.hpp"
#include "db.hpp"

namespace NiNntp
{
    bool Nntp::ReadLine ( wxString& line )
    {
        line.Clear ();
        wxCharBuffer buf (MAX_BUFF_SIZE);
        char *pBuf = buf.data();
        size_t nread = 0;

        if ( !IsOk () ) return false;

        while ( m_sock.WaitForRead() )
        {
            // peek at the socket to see if there is a CRLF

            m_sock.Peek(pBuf, MAX_BUFF_SIZE);
            nread = m_sock.LastCount();

            if ( nread == 0 || m_sock.Error() ) 
            {
                return false;
            }
            // look for "\r\n" paying attention to a special case: "\r\n" could
            // have been split by buffer boundary, so check also for \r at the end
            // of the last chunk and \n at the beginning of this one
            pBuf[nread] = '\0';
            const char *eol = strchr(pBuf, '\n');

            // if we found '\n', is there a '\r' as well?
            if ( eol )
            {
                if ( eol == pBuf )
                {
                    // check for case of "\r\n" being split
                    if ( line.IsEmpty() || line.Last() != wxT('\r') )
                    {
                        // ignore the stray '\n'
                        eol = NULL;
                    }
                    //else: ok, got real EOL
                    // read just this '\n' and restart
                    nread = 1;
                }
                else // '\n' in the middle of the buffer
                {
                    // in any case, read everything up to and including '\n'
                    nread = eol - pBuf + 1;

                    if ( eol[-1] != '\r' )
                    {
                        // as above, simply ignore stray '\n'
                        eol = NULL;
                    }
                }
            }
            //if ( m_thread && m_thread->TestDestroy () ) return false;
            m_sock.Read(pBuf, nread);

            if ( m_sock.LastCount() != nread )return false;

            pBuf[nread] = '\0';
            line += wxString::From8BitData (pBuf, nread);
            if ( eol )
            {
                // remove trailing "\r\n"
                line.RemoveLast(2);
                return true;
            }
        }
        return false;
    }

    bool Nntp::SendLine ( const wxString& line )
    {
        if ( !IsOk () ) return false;
        wxCharBuffer buff = line.To8BitData ();
        size_t size = 0, len = strlen (buff.data());
        m_sock.Write ( buff.data(), len );
        size = m_sock.LastCount ();
        return ( size == len );
    }
    bool Nntp::IsOk () const 
    {
        return ( m_sock.IsConnected () && m_sock.IsOk () );
    }
    Nntp::Nntp ()
    {
        m_sock.SetTimeout (120);
        m_sock.SetFlags (wxSOCKET_NOWAIT);
    }

    Nntp::~Nntp ()
    {
        Quit ();
    }

    bool Nntp::Open ( const wxString& host, const wxString& port, NiResp& resp  )
    {
        return Open ( host, wxAtoi (port), resp );
    }
    bool Nntp::Open ( wxIPV4address& address, NiNntp::NiResp& resp )
    {
        bool ret = false;
        wxString ip = address.IPAddress ();
        if ( ip == wxT("255.255.255.255") ) return false;
        if ( m_sock.Connect (address) )
        {
            wxString line;
            if ( ReadLine (line) )
            {
                resp (line);
                if ( resp.Code () == 200 )
                    ret = true;
            }
        }
        return ret;
    }
    bool Nntp::Open ( const wxString& host, const unsigned int& port, NiResp& resp )
    {
        wxIPV4address address;
        address.Hostname ( host );
        if ( port )
        {
            address.Service ( port );
        }
        else
        {
            address.Service ( NNTP_PORT );
        }
        return Open ( address, resp );
    }
    bool Nntp::Quit ()
    {
        if ( m_sock.IsConnected () )
        {
            wxString cmd = wxT("QUIT") + CRLF, line;
            if ( SendLine ( cmd ) )
            {
                if ( ReadLine (line) )
                {
                    NiResp resp;
                    resp (line);
                    if ( resp.Code () == 205 )
                        return true;
                }
            }
        }
        return false;
    }
    bool Nntp::Help ( NiResp& resp )
    {
        if ( IsOk () )
        {
            if ( SendLine (wxT("HELP") + CRLF) )
            {
                wxString line;
                if ( !ReadLine (line) ) return false;
                resp (line);

                if ( resp.Code () != 100 ) return false;
                while (1)
                {
                    line.Clear ();
                    if (!ReadLine (line)) return false;
                    if ( line.IsEmpty() || line == wxT(".") ) break;
                    resp << line;
                }
                return true;
            }
        }
        return false;
    }
    bool Nntp::Authinfo ( const NiServer::Server& server, NiResp& resp )
    {
        return ( server.RequireAuthinfo () 
                 &&
                 Authinfo ( server.GetUsername (), server.GetPassword (), resp )
               );
    }
    bool Nntp::Authinfo ( const wxString& user, const wxString& pass, NiResp& resp )
    {
        if ( !IsOk () ) return false;
        if ( !SendLine (wxString::Format (wxT("AUTHINFO USER %s%s"), user.c_str(), CRLF.c_str())) ) return false;
        wxString line;
        if ( !ReadLine ( line ) ) return false;
        resp (line);
        if ( resp.Code () != 381 ) return false;
        if ( !SendLine (wxString::Format (wxT("AUTHINFO PASS %s%s"), pass.c_str(), CRLF.c_str())) ) return false;
        if ( !ReadLine ( line ) ) return false;
        resp (line);
        if ( resp.Code () != 281 ) return false;
        return true;
    }

    bool Nntp::Article ( const unsigned int& num, NiResp& resp )
    {
        if ( !IsOk () ) { return false; }

        if ( !SendLine ( wxString::Format ( wxT("ARTICLE %d\r\n"), num ) ) ) 
        {
            return false;;
        }

        wxString line;
        if ( !ReadLine (line) ) 
        {
            return false;
        }
        resp (line);
        if ( resp.Code () != 220 ) 
        {
            return false;
        }
        while (1)
        {
            if ( !ReadLine (line) ) return false;
            if ( line == wxT(".") )
            {
                return true;
            }
            resp << line;
        }
        return false;
    }

    bool Nntp::Article ( const wxString& msgid, NiResp& resp )
    {
        if ( msgid.IsNumber () && Article ( wxAtoi (msgid), resp ) ) 
            return true;
        else
            return false;
    }

    bool Nntp::Body ( const unsigned int& num, NiResp& resp )
    {
        return false;
    }

    bool Nntp::Body ( const wxString& msgid, NiResp& resp )
    {
        return false;
    }

    bool Nntp::Check ( const wxString& msgid, NiResp& resp )
    {
        return false;
    }

    bool Nntp::Date ( NiResp& resp )
    {
        return false;
    }

    bool Nntp::Group ( const wxString& group, NiResp& resp )
    {
        if ( !IsOk() ) return false;
        wxASSERT ( !group.IsEmpty() );
        if ( !SendLine (wxString::Format (wxT("GROUP %s%s"), group.c_str(), CRLF.c_str())) ) return false;
        wxString line;
        if ( !ReadLine (line) ) return false;
        resp (line);
        if ( resp.Code () == 211 )
            return true;
        return false;
    }

    bool Nntp::Head ( const unsigned int& num, NiResp& resp )
    {
        return false;
    }

    bool Nntp::Head ( const wxString& msgid, NiResp& resp )
    {
        return false;
    }

    bool Nntp::IHave ( const wxString& msgid, NiResp& resp )
    {
        return false;
    }

    bool Nntp::Last ( NiResp& resp )
    {
        return false;
    }

    bool Nntp::List ( NiResp& resp )
    {
        if ( !IsOk () ) return false;
        if ( !SendLine (wxT("LIST") + CRLF) ) return false;

        wxString line;
        if ( !ReadLine (line) ) return false;
        resp ( line );
        if ( resp.Code () != 215 )
            return false;
        while (1)
        {
            line.Clear ();
            if ( !ReadLine (line) ) return false;
            if ( line == wxT(".") || line.IsEmpty() ) break;
            resp << line;
        }
        return true;
    }


    bool Nntp::Listgroup ( NiResp& resp )
    {
        if ( IsOk () )
        {
            wxString line;
            if ( !SendLine (wxT("LISTGROUP" ) + CRLF )) return false;
            if ( !ReadLine (line) ) return false;
            resp(line);
            if ( resp.Code () != 215 ) return false;
            while (1)
            {
                line.Clear ();
                if ( !ReadLine (line) ) return false;
                if ( line == wxT(".") ) break;
                resp << line;
            }
            return true;
        }
        return false;
    }
    bool Nntp::Listactive ( NiResp& resp )
    {
        if ( IsOk() )
        {
            wxString line;
            if ( !SendLine ( wxT("LIST ACTIVE") + CRLF ) ) return false;
            if ( !ReadLine (line) ) return false;
            resp(line);
            if ( resp.Code ()!= 215 ) return false;

        }
        return false;
    }

    bool Nntp::Mode ( NiResp& resp, const wxString& mode )
    {
        return false;
    }

    bool Nntp::Newgroups ( const wxString& xx )
    {
        return false;
    }

    bool Nntp::Newnews ( const wxString& group, const wxString& date, const wxString& time )
    {
        return false;
    }

    bool Nntp::Next ( NiResp& resp )
    {
        return false;
    }

    bool Nntp::Post ( const wxString& message, NiResp& resp )
    {
        if ( !IsOk () || !SendLine (wxT("POST\r\n")) ) return false;
        wxString line;
        if ( !ReadLine (line) ) return false;
        resp (line);
        if ( resp.Code () != 340 ) return false;

        wxString data;
        size_t len = message.Len(), pos = 0;
        bool last_cr = false;

        data.Alloc ( len + 100 );
        wxLogDebug (wxT("%d\n%s"), len, message.c_str() );
        for ( pos = 0; pos < len; ++pos )
        {
            if ( message [pos] == wxT('\r') || message [pos] == wxT('\0') )
            {
                last_cr = false;
                continue;
            }
            if ( last_cr && message [pos] == wxT('.') )
            {
                data << wxT("..");
                last_cr = false;
            }
            else if ( message [pos] == wxT('\n') )
            {
                data << wxT("\r\n");
                last_cr = true;
            }
            else
            {
                data << message [pos];
                last_cr = false;
            }
        }
        data << CRLF << wxT('.') << CRLF;
        if ( !IsOk () || !SendLine (data) ) return false;
        if ( !ReadLine ( line ) ) return false;
        resp (line);
        if ( resp.Code () != 240 ) return false;
        return true;
    }

    bool Nntp::Slave ( NiResp& resp )
    {
        return false;
    }

    bool Nntp::Stat ( NiResp& resp )
    {
        return false;
    }

    bool Nntp::Takethis ( const wxString& msgid, NiResp& resp )
    {
        return false;
    }

    bool Nntp::Xhdr ( const wxString& range, NiResp& resp )
    {
        return false;
    }

    bool Nntp::Xpat ( const wxString& range, const wxString& pat, NiResp& resp )
    {
        return false;
    }
    bool Nntp::Xover ( const unsigned int& start, const unsigned int& end,
                       NiResp& resp )
    {
        if ( start == 0 || end == 0  ) return false;
        return Xover ( wxString::Format (wxT("%d-%d"), start, end), resp);
    }
    bool Nntp::Xover ( const wxString& range, NiResp& resp )
    {
        if ( !IsOk() ) return false;
        wxString line;
        if ( !SendLine (wxString::Format ( wxT("XOVER %s%s"), 
                                           range.c_str(), CRLF.c_str())) ) 
                                                                return false;
        if ( !ReadLine (line) ) return false;
        resp (line);
        if ( resp.Code() != 224 ) return false;
        while (1)
        {
            line.Clear ();
            if ( !ReadLine (line) ) return false;
            if ( line == wxT(".") ) break;
            resp << line;
        }
        return true;
    }

}

