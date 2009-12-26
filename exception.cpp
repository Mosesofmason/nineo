////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "exception.hpp"


namespace NiException
{
  NiException::NiException ( const CODE& code, const wxString& msg )
    : m_code(code), m_msg (msg)
  {}
  NiException::~NiException ()
  {}

  CODE NiException::Code ()
  {
    return m_code;
  }
  wxString NiException::Message ()
  {
    return m_msg;
  }
};
