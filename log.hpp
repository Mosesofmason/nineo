#ifndef __NEXT_NINEO_LOG_DEFINE__
#define __NEXT_NINEO_LOG_DEFINE__
#include <wx/wx.h>
#include "config.hpp"
#include <vector>
#include <wx/thread.h>

namespace NiUtils
{
    class Log
    {
        public:
            enum Type
            {
                EMPTY,
                DEBUG,
                WARR,
                MESSAGE
            };
            Log ();
            ~Log ();
            Type GetType () const;
            wxString GetLog () const;
        protected:
            friend class Loger;
            void Debug ( const wxString& debug );
            void Warr ( const wxString& warr );
            void Message ( const wxString& message );
            void SetLog ( const Type& type, const wxString& log );
            void Reset ();
        private:
            wxString m_log;
            Type m_type;
    };
    class Loger
    {
        public:
            void Debug ( const wxString& debug );
            void Warr ( const wxString& warr );
            void Message ( const wxString& message );
            size_t Size () const;
            wxString GetLog ( const size_t& idx );
        protected:
            friend class DefaultLoger;
            Loger ( const size_t& max_size = 200 );
            ~Loger ();
        private:
            wxCSConv m_conv;
            bool AddLog ( const Log& log );
            typedef std::vector <Log> LogList;
            typedef std::vector <Log*> LogPtrList;
            LogList m_buff;
            LogPtrList m_log, m_free;
            wxMutex m_lock;
    };

    class DefaultLoger
    {
        private:
            static Loger* LOGER;
            DefaultLoger ();
            ~DefaultLoger ();
        public:
            static Loger* GetLoger ();
    };
    inline Loger* GetLoger ()
    {
        return DefaultLoger::GetLoger ();
    }
}
#endif //


