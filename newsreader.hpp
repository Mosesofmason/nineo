#ifndef __NEXT_NINEO_NEWSREADERS_DEFINE__
#define __NEXT_NINEO_NEWSREADERS_DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "config.hpp"
#include <wx/panel.h>
#include <wx/splitter.h>
#include "groupview.hpp"
#include "summaryview.hpp"


class NiNewsReader : public wxSplitterWindow
{
  DECLARE_EVENT_TABLE ()
public:
  NiNewsReader ( wxWindow *parent, wxWindowID id, const wxString& name = wxT("NewsReaders") );
  ~NiNewsReader ();
private:
  enum
    {
      ID_SPLIT = wxID_HIGHEST + 1,
    };
};

#endif //
