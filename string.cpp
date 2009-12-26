////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "string.hpp"
#include "regex.hpp"
#include <wx/wx.h>

namespace NiString
{
    HashString::HashString ( const wxString& o )
        : wxString (o)
    {}

    HashString::HashString ()
    {}

    HashString::~HashString ()
    {}

    NiUtils::NiHASH HashString::Hash () const
    {
        return NiUtils::NiHash (*this);
    }

    GroupString::GroupString ( const wxString& o )
        : HashString(o)
    {}
    
    GroupString::GroupString ()
    {}
    GroupString::~GroupString ()
    {}

    NiUtils::NiHASH GroupString::PrefixHash () const
    {
        return NiUtils::NiHash (Prefix());
    }

    wxString GroupString::Prefix () const
    {
        const wxRegEx re (NiRegEx::CatchGroupnamePrefix);
        if ( re.Matches (*this) && re.GetMatchCount() == 2 )
        {
            return re.GetMatch (*this, 1);
        }
        return wxEmptyString;
    }
}

