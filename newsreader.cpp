////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "newsreader.hpp"
#include <wx/splitter.h>
#include <wx/textctrl.h>

BEGIN_EVENT_TABLE ( NiNewsReader, wxSplitterWindow )

END_EVENT_TABLE ()


NiNewsReader::NiNewsReader ( wxWindow* parent, wxWindowID id, const wxString& name )
: wxSplitterWindow( parent, id, wxDefaultPosition, wxDefaultSize, NStyle )
{
  wxSplitterWindow *split = new wxSplitterWindow ( this, ID_SPLIT, 
                                                   wxDefaultPosition, wxDefaultSize, 
                                                   NStyle );
  NiGroupView *groupview =  new NiGroupView (this, wxID_ANY );
  if ( split )
    {
      split->SplitHorizontally ( new NiTreeListCtrl(split, wxID_ANY),
				                 new wxTextCtrl (split, wxID_ANY, wxT(""), 
                                                 wxDefaultPosition, wxDefaultSize, 
                                                 NStyle ),
                                200 );
      groupview->LoadConfig ();
      SplitVertically ( groupview, split, 200);
    }
}

NiNewsReader::~NiNewsReader ()
{}
