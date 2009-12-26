#ifndef __NEXT_NINEO_NNTP_DEFINE__
#define __NEXT_NINEO_NNTP_DEFINE__
/**
 *    \file  nntp.hpp
 *   \brief  实现NNTP协议
 *
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-12
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++,borland free commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */


#include <wx/wx.h>
#include "config.hpp"
#include <wx/socket.h>
#include <wx/url.h>
#include <wx/regex.h>
#include "exception.hpp"
#include "respone.hpp"
#include "server.hpp"
#include <memory>

namespace NiNntp
{
    bool TestNntp ();
    //! NNTP协议的默认端口
    const wxString NNTP_PORT (wxT("nntp"));
    //! 换行符 
    const wxString CRLF (wxT("\r\n"));
    //! 最大缓冲大小
    const unsigned short MAX_BUFF_SIZE (5140);

    class NntpException : public NiException::NiException
    {
        public:
            NntpException ( const CODE& code, const wxString& msg )
                : NiException(code, msg)
            {}
            ~NntpException ()
            {};
    };

    /** 
     * \class Nntp
     * \brief NNTP协议类
     */
    class Nntp 
    {
        public:
            Nntp ();
            ~Nntp ();

            // Group ();
            // ListGroup ();
            // Article ();
            // Head ();
            // Body ();
            // Stat ();
            // Post ();
            // Ihave ();
            // Over ();
            // List ();
            // Hdr ();
            // ListHeaders ();
            bool Open ( const wxString& host, const wxString& port, NiResp& resp );
            bool Open ( const wxString& host, const unsigned int& port, NiResp& resp );
            bool Open ( wxIPV4address& address, NiResp& resp );
            bool Quit ();
            bool Help ( NiResp& resp );
            bool Authinfo ( const wxString& user, const wxString& pass, NiResp& resp );
            bool Authinfo ( const NiServer::Server& server, NiResp& resp );
            bool Article ( const unsigned int& num, NiResp& resp );
            bool Article ( const wxString& msgid, NiResp& resp );
            bool Group ( const wxString& group, NiResp& resp );
            bool Head ( const unsigned int& num, NiResp& resp );
            bool Head ( const wxString& msgid, NiResp& resp );
            bool List ( NiResp& resp );
            bool Post ( const wxString& message, NiResp& resp );
            bool Xover ( const unsigned int& start, const unsigned int& end, NiResp& resp );
            bool Xover ( const wxString& range, NiResp& resp );
        private: 
            bool Slave ( NiResp& resp );
            bool Stat ( NiResp& resp );
            bool Body ( const unsigned int& num, NiResp& resp );
            bool Body ( const wxString& msgid, NiResp& resp );
            bool Check ( const wxString& msgid, NiResp& resp );
            bool Date ( NiResp& resp );
            bool Xhdr ( const wxString& range, NiResp& resp );
            bool Xpat ( const wxString& range, const wxString& pat, NiResp& resp );
            bool Listgroup ( NiResp& resp );
            bool Listactive ( NiResp& resp );
            bool Mode ( NiResp& resp, const wxString& mode = wxT("Reader") );
            bool Newgroups ( const wxString& xx );
            bool Newnews ( const wxString& group, const wxString& date, const wxString& time );
            bool Next ( NiResp& resp );
            bool IHave ( const wxString& msgid, NiResp& resp );
            bool Last ( NiResp& resp );
            bool Takethis ( const wxString& msgid, NiResp& resp );


            wxSocketClient m_sock;
            bool IsOk () const;
            bool ReadLine ( wxString& line );
            bool SendLine ( const wxString& line );
    };
    typedef Nntp NiNntp;
    typedef std::auto_ptr < Nntp > NntpPtr;
}
#endif //
