#ifndef __NEXT_NINEO_DECODE_DEFINE__
#define __NEXT_NINEO_DECODE_DEFINE__
#include <wx/wx.h>
#include "config.hpp"
#include <string>

namespace NiDecode
{
    wxString Base64Decode ( const wxString& data );
    wxString QPDecode ( const wxString& data );
}
#endif //

