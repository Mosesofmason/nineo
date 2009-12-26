#ifndef __NEXT_NINEO_EXCEPTION_DEFINE__
#define __NEXT_NINEO_EXCEPTION_DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "config.hpp"

namespace NiException
{
 typedef unsigned int CODE;
  class NiException
  {
  public:
   
    NiException ( const CODE& code, const wxString& msg );
    ~NiException ();
    wxString Message ();
    CODE Code ();
  private:
    wxString m_msg;
    CODE m_code;
  };
};
#endif //
