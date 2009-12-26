#ifndef __NEXT_NINEO_FRAME_DEFINE__
#define __NEXT_NINEO_FRAME_DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "config.hpp"
#include <wx/aui/auibook.h>
#include <wx/aui/aui.h>
#include <wx/html/htmlwin.h>
#include "newsreader.hpp"
#include "groupview.hpp"
#include "ident.hpp"

class NiFrame: public wxFrame
{
    enum
    {
        ID_NOTEBOOK,
        ID_WELCOMEPAGE,
        ID_SASHLEFT,
        ID_SASHRIGHT,
        ID_SASHRIGHTTOP,
        ID_SASHRIGHTBOTTOM,
        ID_NEWSGROUPPANEL,
        ID_GROUPVIEW,
        ID_SUMMARYVIEW,
        ID_MESSAGEVIEW,
        ID_EXIT,
        ID_ADD_NEW_SERVER,
        ID_UPDATE_GROUPLIST,
        ID_SHOW_GROUPLIST,
        ID_SYNC,
#ifdef __WXDEBUG__
        ID_TEST
#endif
    };
    public:
    NiFrame ( const wxString& title = TITLE);
    ~NiFrame ();
    void Init ();
    private:
    void UI ();
    void NewsgroupUI ();
    void OnSashDrag ( wxSashEvent& event );
    void OnAddNewServer ( wxCommandEvent& event );
    void OnUpdateGrouplist ( wxCommandEvent& event );
    void OnShowGrouplist ( wxCommandEvent& event );
    void OnSize ( wxSizeEvent& event );
    void OnMouseEnter ( wxMouseEvent& event );
    void OnSelectGroup ( wxTreeEvent& event );
    void OnTest ( wxCommandEvent& event );
    void BuildMenu ();
    void OnExit ( wxCommandEvent& event );
    void OnGroupviewMenu ( wxTreeEvent& event );
    void OnSync ( wxCommandEvent& event );
    private:
    wxAuiManager m_mgr;
    DECLARE_EVENT_TABLE ()
};
#endif //
