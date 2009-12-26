////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "log.hpp"
#include <algorithm>

namespace NiUtils
{
    Log::Log ()
        : m_type (EMPTY)
    {}
    Log::~Log ()
    {}
    void Log::SetLog ( const Type& type, const wxString& log )
    {
        m_type  = type;
        m_log   = log;
    }
    Log::Type Log::GetType () const
    {
        return m_type;
    }
    wxString Log::GetLog () const
    {
        return m_log;
    }

    void Log::Reset ()
    {
        m_type      = EMPTY;
        m_log.Clear ();
    }
    void Log::Debug ( const wxString& debug )
    { 
        SetLog ( DEBUG, debug );
    }
    void Log::Warr ( const wxString& warr )
    {
        SetLog ( WARR, warr);
    }
    void Log::Message ( const wxString& message )
    {
        SetLog ( MESSAGE, message );
    }

    Loger::Loger ( const size_t& max_size )
        : m_conv (wxT("GB2312"))
    {
        m_buff.reserve   (max_size);
        m_log.reserve    (max_size);
        m_free.reserve   (max_size);
        Log* log = &m_buff[0];

        for ( size_t pos = 0, count = max_size; pos < count; ++pos )
        {
            m_buff.push_back (Log());
            m_free.push_back ((log+pos));
        }
    }
    Loger::~Loger ()
    {}
    bool Loger::AddLog ( const Log& log )
    {
        Log* tl = NULL;
        if ( m_log.size () == m_log.capacity () )
        {
            tl = *m_log.begin ();
            m_log.erase ( m_log.begin() );
        }
        else if ( !m_free.empty () )
        {
            tl = m_free.back ();
            m_free.pop_back ();
        }
        wxASSERT ( tl != NULL );
        *tl = log;
        m_log.push_back (tl);
        return true;
    }
    size_t Loger::Size () const
    {
        return m_log.size ();
    }
    wxString Loger::GetLog ( const size_t& idx )
    {
        wxMutexLocker locker ( m_lock );
        wxString log;
        if ( idx < m_log.size () )
        {
            Log *lg = m_log[idx];
            log = wxString(lg->GetLog ().To8BitData(), m_conv);
            if ( log.IsEmpty () )
            {
                log = lg->GetLog ();
            }
        }
        return log;
    }
    void Loger::Debug ( const wxString& debug )
    {
#ifdef __WXDEBUG__
        wxMutexLocker locker ( m_lock );
        wxLogMessage (wxT("DEBUG: %s"), debug.c_str());
        Log log;
        log.Debug (debug);
        AddLog (log);
#endif
    }
    void Loger::Warr ( const wxString& warr )
    {
        wxMutexLocker locker ( m_lock );
        wxLogMessage (wxT("WARR: %s"), warr.c_str());
        Log log;
        log.Warr (warr);
        AddLog (log);
    }
    void Loger::Message ( const wxString& message )
    {
        wxMutexLocker locker ( m_lock );
        wxLogMessage (wxT("MESSAGE: %s"), message.c_str());
        Log log;
        log.Message (message);
        AddLog (log);
    }

    Loger* DefaultLoger::LOGER = NULL;
    Loger* DefaultLoger::GetLoger ()
    {
        if ( LOGER == NULL )
        {
            LOGER = new Loger;
        }
        return LOGER;
    }
}
