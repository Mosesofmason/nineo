////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "safe.hpp"

namespace NiDB
{
    Safe::Safe ()
        : m_count (0)
    {}

    Safe::~Safe ()
    {}

    void Safe::Use ()
    {
        ++m_count;
    }

    void Safe::Release ()
    {
        if ( m_count > 0 ) --m_count;
    }

    bool Safe::IsFree ()
    {
        return m_count == 0;
    }

    SafeLocker::SafeLocker ( Safe& o )
        : m_safe (o), wxMutexLocker (o)
    {}

    SafeLocker::~SafeLocker ()
    {}

    SafeLocker& SafeLocker::operator++ ()
    {
        m_safe.Use ();
        return *this;
    }

    SafeLocker& SafeLocker::operator-- ()
    {
        m_safe.Release ();
        return *this;
    }

    SafeLiveLocker::SafeLiveLocker ( Safe& o )
        : m_safe (o)
    {
        SafeLocker locker (m_safe);
        ++locker;
    }

    SafeLiveLocker::~SafeLiveLocker ()
    {
        SafeLocker locker(m_safe);
        --locker;
    }
};

