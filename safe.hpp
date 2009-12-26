#ifndef __NEXT_NINEO_SAFE_DEFINE__
#define __NEXT_NINEO_SAFE_DEFINE__

#include <wx/wx.h>
#include <wx/thread.h>
#include "config.hpp"

namespace NiDB
{
    class Safe;
    class SafeLocker;
    class SafeLiveLocker;

    class Safe : public wxMutex
    {
        public:
            Safe ();
            ~Safe ();
            bool IsFree ();
        protected:
            friend class SafeLocker;
            friend class SafeLiveLocker;
            void Use ();
            void Release ();
        private:
            wxMutex m_lock;
            size_t m_count;
    };

    class SafeLocker : public wxMutexLocker
    {
        public: 
            SafeLocker ( Safe& o );
            ~SafeLocker ();
            SafeLocker& operator++ ();
            SafeLocker& operator-- ();
        private:
            Safe& m_safe;

    };

    class SafeLiveLocker
    {
        public:
            SafeLiveLocker ( Safe& o );
            ~SafeLiveLocker ();
        private:
            Safe& m_safe;
    };
}
#endif //


