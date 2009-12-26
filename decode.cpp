////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "decode.hpp"
#include "base64.hpp"
#include "qoupri.hpp"
#include <string>

namespace NiDecode
{
    wxString Base64Decode ( const wxString& data )
    {
        std::string in, out;
        in = data.To8BitData().data();
        Mime::base64_decode (out, in);
        return wxString::From8BitData ( out.c_str() );
    }
    wxString QPDecode ( const wxString& data )
    {
        std::string in, out;
        in = data.To8BitData().data();
        Mime::qp_decode (out, in);
        return wxString::From8BitData ( out.c_str() );
    }
}

