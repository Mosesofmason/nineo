////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////
#include "frame.hpp"
#include "grouplist.hpp"
#include "db.hpp"
#include "thread.hpp"
#include "log.hpp"
#include "vlist.hpp"
#include "group.hpp"
#include <wx/laywin.h>
#include <wx/sashwin.h>
#include <wx/fontdlg.h>

NiFrame::NiFrame ( const wxString& title )
: wxFrame (NULL, wxID_ANY, title)
{
    SetName (wxT("NiFrame"));
    m_mgr.SetManagedWindow (this);
    UI ();
    m_mgr.Update ();
    NiDB::ArticleDBManager::OpenManager ();
}

NiFrame::~NiFrame ()
{
    m_mgr.UnInit();
    NiDB::ArticleDBManager::CloseManager ();
    NiDB::ActiveDBManager::CloseManager ();
}

void NiFrame::UI ()
{
    wxAuiNotebook *nb = new wxAuiNotebook ( this, ID_NOTEBOOK, 
                                            wxDefaultPosition, wxDefaultSize,
                                            wxBORDER_NONE|wxAUI_NB_TAB_MOVE|
                                            wxAUI_NB_TAB_EXTERNAL_MOVE|
                                            wxAUI_NB_SCROLL_BUTTONS|
                                            wxAUI_NB_WINDOWLIST_BUTTON|
                                            wxAUI_NB_BOTTOM );
    wxASSERT ( nb != NULL );
    wxAuiPaneInfo info;
    info.Name(wxT("Notebook")).CenterPane().PaneBorder(false);
    m_mgr.AddPane (nb, info);

    NewsgroupUI ();

    NiThread::TaskPanel *tp = new NiThread::TaskPanel (this, wxID_ANY );
    if ( tp )
    {
        nb->AddPage (tp, _("Task Manager&Log Viewer"), true);
    }
#if __WXDEBUG__
    wxVisualList *list = new wxVisualList ( this, wxID_ANY );
    if ( list )
    {
        list->SetCount (200);
        nb->AddPage ( list, _("VisualList"), true );
    }
#endif
    nb->SetSelection (0);

    CreateStatusBar();
    BuildMenu ();
}
void NiFrame::NewsgroupUI ()
{
    wxAuiNotebook *nb = static_cast <wxAuiNotebook*> (FindWindow (ID_NOTEBOOK));
    wxASSERT ( nb != NULL );
    wxPanel *panel = new wxPanel ( this, ID_NEWSGROUPPANEL );
    wxSize size = panel->GetClientSize ();

    wxSashLayoutWindow* left    = new wxSashLayoutWindow(panel, ID_SASHLEFT,
                                            wxDefaultPosition, wxSize (200, 600),
                                            wxNO_BORDER|wxSW_BORDER|wxCLIP_CHILDREN );
    wxSashLayoutWindow* rtop    = new wxSashLayoutWindow(panel, ID_SASHRIGHTTOP,
                                            wxDefaultPosition, wxSize (800, 500),
                                            wxNO_BORDER|wxSW_BORDER|wxCLIP_CHILDREN );
    wxSashLayoutWindow* rbottom = new wxSashLayoutWindow(panel, ID_SASHRIGHTBOTTOM,
                                            wxDefaultPosition, wxSize (800, 500),
                                            wxNO_BORDER|wxSW_BORDER|wxCLIP_CHILDREN );
    left->SetDefaultSize    (wxSize(300, 1000));
    left->SetOrientation    (wxLAYOUT_VERTICAL);
    left->SetAlignment      (wxLAYOUT_LEFT);
    left->SetSashVisible    (wxSASH_RIGHT, true);
    rtop->SetDefaultSize    ( wxSize (600, 300) );
    rtop->SetOrientation    (wxLAYOUT_HORIZONTAL);
    rtop->SetAlignment      (wxLAYOUT_TOP);
    rtop->SetBackgroundColour(wxColor (55,22,33));
    rtop->SetSashVisible    (wxSASH_BOTTOM, true);
    rbottom->SetDefaultSize ( wxSize (600, 400) );
    rbottom->SetOrientation (wxLAYOUT_HORIZONTAL);
    rbottom->SetAlignment   (wxLAYOUT_TOP);
    rbottom->SetBackgroundColour(wxColor (88,33,11));

    NiGroupView     *groupview  = new NiGroupView   ( left, ID_GROUPVIEW ); 
    NiSummaryView   *summaryview= new NiSummaryView ( rtop, ID_SUMMARYVIEW );
    wxTextCtrl      *text       = new wxTextCtrl    ( rbottom, ID_MESSAGEVIEW );    

    nb->AddPage ( panel, _("Newsgroup"), true );
}

void NiFrame::Init ()
{
    NiConfig::Config *config    = NiConfig::DefaultConfig::GetConfig ();
    NiGroupView *groupview      = static_cast <NiGroupView*> (FindWindow (ID_GROUPVIEW)); 
    wxASSERT ( groupview != NULL && config != NULL );
    bool noserver = config->CountServer() == 0;

    groupview->LoadConfig ();
    if ( noserver )
    {
        wxString message = _("Are you add a new server?");
        wxMessageDialog dialog ( this, message, _("Add a new server?"), 
                                 wxYES_NO|wxICON_QUESTION );
        if ( dialog.ShowModal () == wxID_YES )
            groupview->AddNewServer ();
    }
}
void NiFrame::OnSize ( wxSizeEvent& event )
{
    Update ();
    wxWindow* newsgroup_panel = FindWindow (ID_NEWSGROUPPANEL);
    wxASSERT ( newsgroup_panel != NULL );
    wxLayoutAlgorithm layout;
    layout.LayoutWindow (newsgroup_panel);
    event.Skip ();
}

void NiFrame::OnMouseEnter ( wxMouseEvent& event )
{
    Update ();
    event.Skip ();
}
void NiFrame::OnSashDrag ( wxSashEvent& event )
{
    wxLogDebug (wxT("SashDrag"));
    wxSashLayoutWindow *left = NULL, *right = NULL,  *righttop = NULL, *rightbottom = NULL;
    left        = static_cast <wxSashLayoutWindow*> (FindWindow ( ID_SASHLEFT ));
    righttop    = static_cast <wxSashLayoutWindow*> (FindWindow ( ID_SASHRIGHTTOP ));
    rightbottom = static_cast <wxSashLayoutWindow*> (FindWindow ( ID_SASHRIGHTBOTTOM ));
    wxASSERT ( left != NULL && righttop != NULL &&  rightbottom != NULL );
    wxWindow *panel = FindWindow ( ID_NEWSGROUPPANEL );
    wxSize psize = panel->GetClientSize ();
    left->SetDefaultSize ( wxSize ( 300, psize.GetHeight () ) );
    righttop->SetDefaultSize ( wxSize ( psize.GetWidth() - 310, psize.GetHeight()/2 - 1) );
    rightbottom->SetDefaultSize ( wxSize ( psize.GetWidth() - 310, psize.GetHeight()/2 -1 ) );
    wxLayoutAlgorithm layout;
    layout.LayoutWindow (panel);
    panel->Refresh ();
}

void NiFrame::OnSelectGroup ( wxTreeEvent& event )
{
    NiGroupView*    gv  = static_cast <NiGroupView*> (FindWindow (ID_GROUPVIEW));
    NiSummaryView*  sv  = static_cast <NiSummaryView*> (FindWindow (ID_SUMMARYVIEW));
    wxASSERT ( gv != NULL && sv != NULL );

    NiGroup::SubGroup group;
    bool isok = gv->GetGroup ( event.GetItem (), group );
    if ( isok )
    {
        sv->Load (group);   
    }
}
void NiFrame::OnTest ( wxCommandEvent& event )
{
    NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger ();
    loger->Message (wxT("Test begin:"));
#if 0
    NiIdent::Ident ident;
    NiIdent::NiIdentDialog iddialog ( this, wxID_ANY, ident );
    iddialog.ShowModal ();
#endif
#if 0
    NiServer::Server server;
    NiServer::NiServerDialog server_dialog (this, wxID_ANY, server);
    server_dialog.ShowModal ();
#endif
    wxString desc;
    wxWindow* gv = FindWindow ( ID_GROUPVIEW );
    wxFont font;

    font = gv->GetFont ();
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());
    desc << wxT("|"); 
    
    font = wxSystemSettings::GetFont (wxSYS_OEM_FIXED_FONT);
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());
    desc << wxT("|"); 

    font = wxSystemSettings::GetFont (wxSYS_ANSI_FIXED_FONT);
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());
    desc << wxT("|"); 

    font = wxSystemSettings::GetFont (wxSYS_ANSI_VAR_FONT);
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());
    desc << wxT("|"); 

    font = wxSystemSettings::GetFont (wxSYS_DEVICE_DEFAULT_FONT);
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());

#if 0
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());
    desc += wxString::From8BitData (font.GetNativeFontInfoUserDesc ().ToUTF8 ().data());
#endif
    wxLogDebug (wxT("GetFont: %s"), desc.c_str());
    loger->Message (wxT("Test end."));
}
void NiFrame::BuildMenu ()
{
    wxMenu *kineo, *help; 
    wxMenuBar *bar;
    bar = new wxMenuBar;
    kineo = new wxMenu;
    wxASSERT ( bar != NULL && kineo != NULL );
    kineo->Append ( ID_ADD_NEW_SERVER, _("Add new server"), _("Add a new server") );
    kineo->Append ( ID_UPDATE_GROUPLIST, _("Update grouplist"), 
                    _("Update grouplist of all server.") );
    kineo->Append ( ID_SHOW_GROUPLIST, _("Show grouplist"),
                    _("Show all grouplist") );
    kineo->Append ( ID_SYNC, _("Sync from newsgroup server"),
                             _("Sync all newsgroup from server"));
    kineo->AppendSeparator ();
    kineo->Append ( ID_EXIT, _("Exit"), _("Exit KiNeo"));
    bar->Append ( kineo, _("KiNeo"));
#ifdef __WXDEBUG__
    wxMenu *debug;
    debug =  new wxMenu ();
    wxASSERT ( debug != NULL );
    debug->Append ( ID_TEST, wxT("Test"), _("Test"));
    bar->Append (debug, wxT("Debug"));
#endif
    SetMenuBar (bar);
}
void NiFrame::OnAddNewServer ( wxCommandEvent& event )
{
    NiGroupView* groupview = dynamic_cast <NiGroupView*> ( FindWindow (ID_GROUPVIEW) );
    if ( groupview )
    {
        groupview->AddNewServer ();
    }
}
void NiFrame::OnUpdateGrouplist ( wxCommandEvent& event )
{
    NiGroupView* groupview = dynamic_cast <NiGroupView*> ( FindWindow (ID_GROUPVIEW) );
    if ( groupview )
    {
        groupview->UpdateGrouplist ();
    }
}
void NiFrame::OnShowGrouplist ( wxCommandEvent& event )
{
    NiGroup::GrouplistDialog dialog ( this, wxID_ANY );
    dialog.ShowModal ();
}
void NiFrame::OnSync ( wxCommandEvent& event )
{
    NiGroupView *gv = static_cast <NiGroupView*> (FindWindow (ID_GROUPVIEW));
    wxASSERT ( gv != NULL );
    gv->SyncAll ();
}
void NiFrame::OnGroupviewMenu ( wxTreeEvent& event )
{
    NiGroupView *gv = static_cast <NiGroupView*> (FindWindow (ID_GROUPVIEW));
    wxASSERT ( gv != NULL );
    gv->ItemMenu ();
}
void NiFrame::OnExit ( wxCommandEvent& event )
{
    Close(true);
}
BEGIN_EVENT_TABLE (NiFrame, wxFrame)
    EVT_SIZE ( NiFrame::OnSize )
    EVT_ENTER_WINDOW ( NiFrame::OnMouseEnter )
    EVT_MENU ( ID_EXIT,             NiFrame::OnExit )
    EVT_MENU ( ID_ADD_NEW_SERVER,   NiFrame::OnAddNewServer )
    EVT_MENU ( ID_UPDATE_GROUPLIST, NiFrame::OnUpdateGrouplist ) 
    EVT_MENU ( ID_SHOW_GROUPLIST, NiFrame::OnShowGrouplist )
    EVT_MENU ( ID_SYNC, NiFrame::OnSync )
    EVT_SASH_DRAGGED_RANGE ( ID_SASHLEFT, ID_SASHRIGHTBOTTOM, NiFrame::OnSashDrag )
    EVT_TREE_ITEM_MENU ( ID_GROUPVIEW, NiFrame::OnGroupviewMenu )
    EVT_TREE_SEL_CHANGED ( ID_GROUPVIEW, NiFrame::OnSelectGroup )
#ifdef __WXDEBUG__
EVT_MENU ( ID_TEST, NiFrame::OnTest )
#endif 
END_EVENT_TABLE();

