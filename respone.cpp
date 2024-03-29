////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "respone.hpp"
#include <wx/datetime.h>
#include "log.hpp"

namespace NiNntp
{
    wxString NiNntpCodeToMessage ( const CODE& code )
    {
        switch (code)
        {
            //    Response code 100 (multi-line)
            //       Generated by: HELP
            //       Meaning: help text follows.
            case 100: return _("help text follows.");

                      //    Response code 101 (multi-line)
                      //       Generated by: CAPABILITIES
                      //       Meaning: capabilities list follows.
            case 101: return _("capabilities list follows.");

                      //    Response code 111
                      //       Generated by: DATE
                      //       1 argument: yyyymmddhhmmss
                      //       Meaning: server date and time.
            case 111: return _("server date and time.");

                      //    Response code 200
                      //       Generated by: initial connection, MODE READER
                      //       Meaning: service available, posting allowed.
            case 200: return _("service available, posting allowed.");


                      //    Response code 201
                      //       Generated by: initial connection, MODE READER
                      //       Meaning: service available, posting prohibited.
            case 201: return _("service available, posting prohibited.");

                      //    Response code 205
                      //       Generated by: QUIT
                      //       Meaning: connection closing (the server immediately closes the
                      //       connection).
            case 205: return _("connection closing (the server immediately closes the connection).");

                      //    Response code 211
                      //       The 211 response code has two completely different forms,
                      //       depending on which command generated it:

                      //          (not multi-line)
                      //          Generated by: GROUP
                      //          4 arguments: number low high group
                      //          Meaning: group selected.

                      //          (multi-line)
                      //          Generated by: LISTGROUP
                      //          4 arguments: number low high group
                      //          Meaning: article numbers follow.
            case 211: return _("group selected.");

                      //    Response code 215 (multi-line)
                      //       Generated by: LIST
                      //       Meaning: information follows.
            case 215: return _("information follows.");

                      //    Response code 220 (multi-line)
                      //       Generated by: ARTICLE
                      //       2 arguments: n message-id
                      //       Meaning: article follows.
            case 220: return _("article follows.");

                      //    Response code 221 (multi-line)
                      //       Generated by: HEAD
                      //       2 arguments: n message-id
                      //       Meaning: article headers follow.
            case 221: return _("article headers follow.");

                      //    Response code 222 (multi-line)
                      //       Generated by: BODY
                      //       2 arguments: n message-id
                      //       Meaning: article body follows.
            case 222: return _("article body follows.");

                      //    Response code 223
                      //       Generated by: LAST, NEXT, STAT
                      //       2 arguments: n message-id
                      //       Meaning: article exists and selected.
            case 223: return _("article exists and selected.");

                      //    Response code 224 (multi-line)
                      //       Generated by: OVER
                      //       Meaning: overview information follows.
            case 224: return _("overview information follows.");

                      //    Response code 225 (multi-line)
                      //       Generated by: HDR
                      //       Meaning: headers follow.
            case 225: return _("headers follow.");

                      //    Response code 230 (multi-line)
                      //       Generated by: NEWNEWS
                      //       Meaning: list of new articles follows.
            case 230: return _("list of new articles follows.");

                      //    Response code 231 (multi-line)
                      //       Generated by: NEWGROUPS
                      //       Meaning: list of new newsgroups follows.
            case 231: return _("list of new newsgroups follows.");

                      //    Response code 235
                      //       Generated by: IHAVE (second stage)
                      //       Meaning: article transferred OK.
            case 235: return _("article transferred OK.");

                      //    Response code 240
                      //       Generated by: POST (second stage)
                      //       Meaning: article received OK.
            case 240: return _("article received OK.");

            case 281: return _("Authentication accepted");

                      //    Response code 335
                      //       Generated by: IHAVE (first stage)
                      //       Meaning: send article to be transferred.
            case 335: return _("send article to be transferred.");

                      //    Response code 340
                      //       Generated by: POST (first stage)
                      //       Meaning: send article to be posted.
            case 340: return _("send article to be posted.");

            case 381: return _("More authentication information required");

                      //    Response code 400
                      //       Generic response and generated by initial connection
                      //       Meaning: service not available or no longer available (the server
                      //       immediately closes the connection).
            case 400: return _("service not available or no longer available (the server immediately closes the connection).");

                      //    Response code 401
                      //       Generic response
                      //       1 argument: capability-label
                      //       Meaning: the server is in the wrong mode; the indicated capability
                      //       should be used to change the mode.
            case 401: return _("the server is in the wrong mode; the indicated capability should be used to change the mode.");

                      //    Response code 403
                      //       Generic response
                      //       Meaning: internal fault or problem preventing action being taken.
            case 403: return _("internal fault or problem preventing action being taken.");

                      //    Response code 411
                      //       Generated by: GROUP, LISTGROUP
                      //       Meaning: no such newsgroup.
            case 411: return _("no such newsgroup.");

                      //    Response code 412
                      //       Generated by: ARTICLE, BODY, GROUP, HDR, HEAD, LAST, LISTGROUP,
                      //       NEXT, OVER, STAT
                      //       Meaning: no newsgroup selected.
            case 412: return _("no newsgroup selected.");

                      //    Response code 420
                      //       Generated by: ARTICLE, BODY, HDR, HEAD, LAST, NEXT, OVER, STAT
                      //       Meaning: current article number is invalid.
            case 420: return _("current article number is invalid.");

                      //    Response code 421
                      //       Generated by: NEXT
                      //       Meaning: no next article in this group.
            case 421: return _("no next article in this group.");

                      //    Response code 422
                      //       Generated by: LAST
                      //       Meaning: no previous article in this group.
            case 422: return _("no previous article in this group.");

                      //    Response code 423
                      //       Generated by: ARTICLE, BODY, HDR, HEAD, OVER, STAT
                      //       Meaning: no article with that number or in that range.
            case 423: return _("no article with that number or in that range.");

                      //    Response code 430
                      //       Generated by: ARTICLE, BODY, HDR, HEAD, OVER, STAT
                      //       Meaning: no article with that message-id.
            case 430: return _("no article with that message-id.");
                      //    Response code 435
                      //       Generated by: IHAVE (first stage)
                      //       Meaning: article not wanted.
            case 435: return _("article not wanted.");

                      //    Response code 436
                      //       Generated by: IHAVE (either stage)
                      //       Meaning: transfer not possible (first stage) or failed (second
                      //       stage); try again later.
            case 436: return _("transfer not possible (first stage) or failed (second stage); try again later.");

                      //    Response code 437
                      //       Generated by: IHAVE (second stage)
                      //       Meaning: transfer rejected; do not retry.
            case 437: return _("transfer rejected; do not retry.");

                      //    Response code 440
                      //       Generated by: POST (first stage)
                      //       Meaning: posting not permitted.
            case 440: return _("posting not permitted.");

                      //    Response code 441
                      //       Generated by: POST (second stage)
                      //       Meaning: posting failed.
            case 441: return _("posting failed.");

                      //    Response code 480
                      //       Generic response
                      //       Meaning: command unavailable until the client has authenticated
                      //       itself.
            case 480: return _("command unavailable until the client has authenticated itself.");

                      //    Response code 483
                      //       Generic response
                      //       Meaning: command unavailable until suitable privacy has been
                      //       arranged.
            case 483: return _("command unavailable until suitable privacy has been arranged.");

                      //    Response code 500
                      //       Generic response
                      //       Meaning: unknown command.
            case 500: return _("unknown command.");

                      //    Response code 501
                      //       Generic response
                      //       Meaning: syntax error in command.
            case 501: return _("syntax error in command.");

                      //    Response code 502
                      //       Generic response and generated by initial connection

                      //       Meaning for the initial connection and the MODE READER command:
                      //       service permanently unavailable (the server immediately closes the
                      //       connection).

                      //       Meaning for all other commands: command not permitted (and there
                      //       is no way for the client to change this).
            case 502: return _("command not permitted (and there is no way for the client to change this).");

                      //    Response code 503
                      //       Generic response
                      //       Meaning: feature not supported.
            case 503: return _("feature not supported.");

                      //    Response code 504
                      //       Generic response
                      //       Meaning: error in base64-encoding [RFC4648] of an argument.
            case 504: return _("error in base64-encoding [RFC4648] of an argument.");
            default:
                      return _("Unkonw respone.");
        };
    };
    ////////////////////////////////////////////////////////////////////////////////
    //////////////////// NiResp

    NiResp::NiResp ()
        : m_code (0)
    {}
    NiResp::~NiResp ()
    {}

    wxString NiResp::Status ()
    {
        return m_status;
    }

    CODE NiResp::Code ()
    {
        return m_code;
    }
    wxString NiResp::Message ()
    {
        return NiNntpCodeToMessage (m_code);
    }

    bool NiResp::CheckCode ( const CODE& code )
    {
        switch ( code/100 )
        {
            case 1 : return true;
            case 2 : return true;
            case 3 : return false;
            case 4 : return false;
            case 5 : return false;
        }
    }

    void NiResp::ClearStatus ()
    {
        m_status.Clear ();
        m_code = 0;
    }
    inline bool NiResp::PushStatus ( const wxString& line )
    {
        return Status (line);
    }
    bool NiResp::Status ( const wxString& line )
    {
        m_status = line;

        wxRegEx re;
        re.Compile (wxT("^([[:digit:]]{3}).*$"));
        if ( re.IsValid () && re.Matches (line) && re.GetMatchCount () == 2 )
        {
            wxString code = re.GetMatch (line, 1);
            if ( code.IsNumber () )
            {
                unsigned long val = 0;
                code.ToULong (&val);
                m_code = val;
                return true;
            }
        }
        return false;
    }

    NiResp& NiResp::operator () ( const wxString& line )
    {
        PushStatus (line);
        return *this;
    }

    NiResp& NiResp::operator << ( const wxString& line )
    {
        return *this;
    }
    ////////////////////////////////////////////////////////////////////////////////
    //  NiRespDataCount 
    // 
    
    NiRespDataCount::NiRespDataCount ()
        : m_datacount (0), m_linecount(0)
    {}
    NiRespDataCount::~NiRespDataCount ()
    {}
    size_t NiRespDataCount::DataCount () const
    {
        return m_datacount;
    }
    size_t NiRespDataCount::LineCount () const
    {
        return m_linecount;
    }
    NiResp& NiRespDataCount::operator () ( const wxString& line )
    {
        Status (line);
        m_datacount = 0;
        m_linecount = 0;
        return *this;
    }
    NiResp& NiRespDataCount::operator << ( const wxString& line )
    {
        m_datacount += line.Len ();
        ++m_linecount;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // NiRespOverview 
    //
    NiRespOverview::NiRespOverview ( NiDB::ArticleDB& db )
        : m_db (db), m_ov ( new Overview ), SafeLiveLocker (db)
    {}
    NiRespOverview::~NiRespOverview ()
    {
        m_db.Commit ();
    }
    NiResp& NiRespOverview::operator () ( const wxString& line )
    {
        PushStatus ( line );
        return *this;
    }
    NiResp& NiRespOverview::operator << ( const wxString& line )
    {
        if ( m_ov.get () != NULL )
        {
            (*m_ov) (line);
            m_db.AddOV (*m_ov);
        }
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //////////////////// NiRespListGroup

    NiRespListGroup::NiRespListGroup ( NiDB::NiActiveDB& db )
        : m_db(db), m_count(0)
    {
        m_db.BeginUpdate (500);
    }

    NiRespListGroup::~NiRespListGroup ()
    {
        {
            wxMutexLocker lock (m_db.lock);
            m_db.EndUpdate ();
        }
    }
    NiResp& NiRespListGroup::operator () ( const wxString& line )
    {
        PushStatus (line);
        return *this;
    }

    NiResp& NiRespListGroup::operator << ( const wxString& line )
    {
        NiGroup::NiUnsubGroup group;
        group(line);
        m_db.Add (group);
        ++m_count;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //////////////////// NiRespML

    NiRespML::NiRespML ()
    {}

    NiRespML::~NiRespML ()
    {}

    NiResp& NiRespML::operator () ( const wxString& line )
    {
        PushStatus (line);
        return *this;
    }

    NiResp& NiRespML::operator << ( const wxString& line )
    {
        m_data << line << wxT("\n");
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //////////////////// NiRespDebug


    NiRespDebug::NiRespDebug ()
    {}

    NiRespDebug::~NiRespDebug ()
    {}
    NiResp& NiRespDebug::operator () ( const wxString& line )
    {
        if ( line != wxT("\n") )
        {
            NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger ();
            wxString message = wxString::Format (wxT("NNTP>> %s"), line.c_str());
            loger->Debug (message);
        }
        PushStatus (line);
        return *this;
    }

    NiResp& NiRespDebug::operator << ( const wxString& line )
    {
        NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger();
        loger->Debug ( line );
        return *this;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////// NiRespGroup

    NiRespGroup::NiRespGroup ()
        : m_first(0), m_last(0), m_count(0)
    {
        m_re.Compile (
        wxT("^[[:digit:]]{3} ([[:digit:]]*) ([[:digit:]]*) ([[:digit:]]*) (.*)$")
                      );
    }

    NiRespGroup::~NiRespGroup ()
    {
    }

    unsigned int NiRespGroup::First ()
    {
        return m_first;
    }

    unsigned int NiRespGroup::Last ()
    {
        return m_last;
    }

    unsigned int NiRespGroup::Count ()
    {
        return m_count;
    }

    wxString NiRespGroup::Group ()
    {
        return m_group;
    }
    NiResp& NiRespGroup::operator () ( const wxString& line )
    {
        if ( m_re.IsValid () )
        {
            if ( m_re.Matches (line) && m_re.GetMatchCount() == 5 )
            {
                wxString first, last, count, group;
                count   = m_re.GetMatch (line, 1);
                first   = m_re.GetMatch (line, 2);
                last    = m_re.GetMatch (line,  3);
                group   = m_re.GetMatch (line, 4);
                if ( count.IsNumber ()   && first.IsNumber() 
                        && 
                     last.IsNumber()     && !group.IsEmpty() )
                {
                    m_count = wxAtoi (count);
                    m_first = wxAtoi (first);
                    m_last = wxAtoi (last);
                    m_group = group;
                }
            }
        }
        PushStatus (line);
        return *this;
    }

    NiResp& NiRespGroup::operator << ( const wxString& line )
    {
        return *this;
    }
};
