////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "db.hpp"

namespace NiDB
{
    bool TestDB ()
    {
        NiActiveDB adb;
        try
        {
            adb.Open (wxT("cn.fan.gl"));
            wxLogDebug (wxT("Open db is OK."));
            adb.Close ();
        }
        catch (wxSQLite3Exception& except)
        {
            wxLogDebug (except.GetMessage());
        }
        return false;
    }
};
