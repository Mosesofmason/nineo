///////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "db.hpp"
#include "nntp.hpp"
#include <iostream>
#include <string>

namespace NiNntp
{
    bool TestNntp ()
    {

        NiNntp nntp;
        NiRespDebug resp;
        if ( nntp.Open (wxT("localhost"), wxEmptyString, resp))
        {
#if 0

            NiDB::NiActiveDB activedb;
            try
            {
                activedb.Open (wxT("cn.fan.gl"));
            }
            catch ( wxSQLite3Exception& e )
            {
                wxLogDebug (e.GetMessage ());
                throw;
            }
#endif
            nntp.Authinfo ( wxT("admin"), wxT("123"), resp);
            
            NiRespGroup gresp;
            nntp.Group ( wxT("cn.fan"), gresp);

            wxLogDebug (wxT("Group: %s Count: %d, First: %d, Last: %d"),
                        gresp.Group().c_str(),
                        gresp.Count(),
                        gresp.First(),
                        gresp.Last());
#if 1
            NiDB::ArticleDB db; 
            db.Open (wxT("cn.fan.db"));  
            unsigned int start, end, first, last;
            first   = gresp.First ();
            last    = gresp.Last ();
            start   = db.LastId ( first, last );
            end     = last;
            if ( end > start )
            {
                wxLogDebug (wxT("Fetch overview from %d to %d."), start, end );
                NiRespOverview respov (db);
                nntp.Xover (start, end, respov);
            }
#endif
//!             db.Close ();
        }
        return false;
    }
};
