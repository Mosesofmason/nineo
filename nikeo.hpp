#ifndef __NEXT_NINEO_APPLICATION_DEFINE__
#define __NEXT_NINEO_APPLICATION_DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "frame.hpp"
#include <wx/ffile.h>

class NiApp : public wxApp
{
public:
  bool OnInit ();
  virtual int OnExit ();
private:
  void InitDebug ();
  bool InitConfig ();
  NiFrame *m_frame;
#ifdef __WXDEBUG__
  wxFFile *m_file;
#endif
};

IMPLEMENT_APP(NiApp);
DECLARE_APP (NiApp);
#endif //
