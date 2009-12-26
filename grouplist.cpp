////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "grouplist.hpp"
#include "activedb.hpp"
#include "groupview.hpp"
namespace NiGroup
{
    class Grouplist : public wxListCtrl
    {
        public:
            Grouplist ( wxWindow* parent, wxWindowID id );
            ~Grouplist ();
            void SetServer ( const NiServer::Server& server );
            void SetClass ( const NiDB::NiClass& oclass );
            NiServer::Server GetServer () const;
            size_t GetCount () const;
            virtual wxString OnGetItemText ( long item, long column ) const;
        private:
            void LoadDB ();
            void LoadData ( const size_t& count = 100 ) const;
            void OnScrollPageDown ( wxScrollWinEvent& event );
            NiServer::Server m_server;
            NiDB::NiClass m_class;
            UnsubGroupList m_grouplist;
            wxCSConv m_conv;

            DECLARE_EVENT_TABLE ()
    };

    BEGIN_EVENT_TABLE ( Grouplist, wxListCtrl )
        EVT_SCROLLWIN_PAGEDOWN ( Grouplist::OnScrollPageDown )
    END_EVENT_TABLE ()
    Grouplist::Grouplist ( wxWindow* parent, wxWindowID id )
    : wxListCtrl ( parent, id, wxDefaultPosition, wxSize(800, 400), 
                   NStyle|wxLC_REPORT|wxLC_VIRTUAL|wxLC_SINGLE_SEL ),
      m_conv (wxT("GB2312"))
    {
        InsertColumn ( 0, _("Group name: "), wxLIST_FORMAT_CENTER, 300);
        InsertColumn ( 1, _("Descption: "), wxLIST_FORMAT_LEFT, 200);
    }
    Grouplist::~Grouplist ()
    {}
    void Grouplist::SetServer ( const NiServer::Server& server )
    {
        m_server = server;
    }   
    NiServer::Server Grouplist::GetServer () const
    {
        return m_server;
    }
    size_t Grouplist::GetCount () const
    {
        return m_grouplist.size ();
    }
    void Grouplist::SetClass ( const NiDB::NiClass& oclass )
    {
        m_grouplist.clear ();
        m_grouplist.reserve (oclass.Num ());
        m_class = oclass;   
        DeleteAllItems ();
        if ( !m_server.GetLabel ().IsEmpty() && !m_server.GetAddress ().IsEmpty () )
        {
            LoadDB ();
        }
        SetItemCount ( m_class.Num() );
    }
    void Grouplist::LoadDB ()
    {
        NiDB::ActiveDB *db = NiDB::ActiveDBManager::GetDB (m_server);
        wxASSERT ( db != NULL );
        db->ListGroup ( m_class, m_grouplist, 100);
    }
    void Grouplist::LoadData ( const size_t& count ) const
    {
        Grouplist *uthis = const_cast <Grouplist*> (this);
        NiDB::ActiveDB *db = NiDB::ActiveDBManager::GetDB (m_server);
        wxASSERT ( db != NULL );
        db->ListGroup ( m_class, uthis->m_grouplist, count);
    }
    void Grouplist::OnScrollPageDown ( wxScrollWinEvent& event )
    {
        if ( m_grouplist.size () != m_class.Num () )
        {
            LoadData (100);
        }
        event.Skip ();
    }
    wxString Grouplist::OnGetItemText ( long item, long column ) const
    {
        if ( m_grouplist.size () < item )
        {
           LoadData ( item );    
        }
        else if ( m_grouplist.size () != m_class.Num () && (item % 100) == 0 )
        {
            LoadData (100);
        }
        if ( column == 0 && m_grouplist.size () >= item )
        {
            return wxString(m_grouplist [ item ].Name ().To8BitData(), m_conv);
        }
        return wxEmptyString;
    }
    BEGIN_EVENT_TABLE ( GrouplistDialog, wxDialog )
        EVT_CHOICE              ( ID_SERVER_CHOICE, GrouplistDialog::OnServer )
        EVT_CHOICE              ( ID_CLASS_CHOICE, GrouplistDialog::OnClass )
        EVT_LIST_ITEM_SELECTED  ( ID_GROUP_LIST, GrouplistDialog::OnGroupSel )
        EVT_BUTTON              ( ID_SUBJECT, GrouplistDialog::OnSubject )
    END_EVENT_TABLE ()
    GrouplistDialog::GrouplistDialog ( wxWindow* parent, wxWindowID id,
                                       const wxString& title )
    : wxDialog ( parent, id, title ), m_index (-1)
    {
        DoUI ();
        Centre ();
        LoadServer ();
    }
    void GrouplistDialog::DoUI ()
    {
        wxChoice *c_server, *c_class;
        wxStaticBox *sb_server, *sb_class;
        wxStaticBoxSizer *sbs_server, *sbs_class;
        c_server    = c_class   = NULL;
        sb_server   = sb_class  = NULL;
        sbs_server  = sbs_class = NULL;
        wxBoxSizer *choice_sizer = NULL;
    
        c_server    = new wxChoice ( this, ID_SERVER_CHOICE, 
                                     wxDefaultPosition, wxSize (400, -1));
        c_class     = new wxChoice ( this, ID_CLASS_CHOICE,
                                     wxDefaultPosition, wxSize (400, -1));
        choice_sizer= new wxBoxSizer (wxHORIZONTAL);
        wxASSERT ( c_server != NULL && c_class != NULL && choice_sizer != NULL );
        sb_server   = new wxStaticBox (this, wxID_ANY, _("Server list:"));
        sb_class    = new wxStaticBox (this, wxID_ANY, _("Group classify:"));
        sbs_server  = new wxStaticBoxSizer (sb_server, wxVERTICAL);
        sbs_class   = new wxStaticBoxSizer (sb_class, wxVERTICAL);
        wxASSERT ( sb_server != NULL && sb_class != NULL 
                   && 
                   sbs_server != NULL && sbs_class != NULL );

        sbs_server->Add (c_server, 0, wxALL, 2 );
        sbs_class->Add (c_class, 0, wxALL, 2 );
        choice_sizer->Add ( sbs_server, 0, wxALL, 5 );
        choice_sizer->Add ( sbs_class, 0, wxALL, 5 );

        Grouplist *group = new Grouplist ( this, ID_GROUP_LIST );
        wxBoxSizer *button_sizer = NULL;
        wxButton *button_subject = NULL, *button_close = NULL;
        button_subject  = new wxButton (this, ID_SUBJECT, _("Subject"));
        button_close    = new wxButton (this, wxID_CANCEL, _("Close"));
        button_sizer    = new wxBoxSizer (wxHORIZONTAL);
        wxASSERT (button_subject != NULL && button_close != NULL && button_sizer != NULL);
        button_sizer->Add (button_subject, 0, wxALL, 5);
        button_sizer->Add (button_close, 0, wxALL, 5);

        wxBoxSizer *main_sizer = NULL;
        main_sizer  = new wxBoxSizer (wxVERTICAL);
        main_sizer->Add (choice_sizer, 0, wxALL, 5 );
        main_sizer->Add (group, 0, wxALL|wxEXPAND, 2);
        main_sizer->Add (button_sizer, 0, wxALL, 5);
        this->SetSizer (main_sizer);

        main_sizer->Fit (this);
        main_sizer->SetSizeHints (this);

    }
    void GrouplistDialog::LoadServer ()
    {
        NiConfig::Config *config = NiConfig::DefaultConfig::GetConfig ();
        wxChoice *c_server, *c_class;
        c_server    = dynamic_cast <wxChoice*> (FindWindow (ID_SERVER_CHOICE));
        c_class     = dynamic_cast <wxChoice*> (FindWindow (ID_CLASS_CHOICE));

        wxASSERT ( config != NULL && c_server != NULL && c_class != NULL );
        c_server->Clear ();
        size_t pos = 0, count = config->CountServer ();
        NiServer::Server server;
        for ( ; pos < count; ++pos )
        {
            server = config->GetServerAt (pos);
            c_server->Append (wxString::Format (wxT("%s [%s]"), 
                                                server.GetLabel ().c_str(),
                                                server.GetAddress ().c_str()));
        }
        if ( count >= 1 )
        {
            c_server->SetSelection (0);
            LoadClass (0);
        }
        else
        {
            c_server->Enable (0);
        }
    }
    void GrouplistDialog::OnGroupSel ( wxListEvent& event )
    {
        m_index = event.GetIndex ();
    }
    void GrouplistDialog::OnSubject ( wxCommandEvent& event )
    {
        Grouplist* gl = dynamic_cast <Grouplist*>(FindWindow(ID_GROUP_LIST));
        NiGroupView* gv  = dynamic_cast <NiGroupView*>(FindWindowByName(wxT("GroupView")));
        wxASSERT ( gl != NULL && gv != NULL );
        gl->SetFocus ();
        NiServer::Server server = gl->GetServer ();
        if ( m_index >= 0 )
        {
            wxString group = gl->GetItemText (m_index);
            SubGroup new_group ( server.GetAddress(), group );
            if ( ! gv->HasGroup (new_group) )
                gv->AddGroup ( new_group );
        }
    }
    void GrouplistDialog::OnServer ( wxCommandEvent& event )
    {
        LoadClass (event.GetSelection ());
    }
    void GrouplistDialog::LoadClass ( const size_t& idx )
    {
        size_t count = 0;
        NiConfig::Config *config = NiConfig::DefaultConfig::GetConfig ();
        wxChoice *c_class = dynamic_cast <wxChoice*>(FindWindow (ID_CLASS_CHOICE));
        NiDB::ActiveDB* db = NULL;
        wxASSERT ( config != NULL && c_class != NULL );

        c_class->Clear ();
        count = config->CountServer ();
        if ( idx < count )
        {
            m_classlist.clear ();
            NiServer::Server server = config->GetServerAt ( idx );
            db = NiDB::ActiveDBManager::GetDB (server);
            wxASSERT ( db != NULL );

            db->ListClass ( m_classlist );
            NiDB::ClassList::iterator itr;
            wxCSConv conv (wxT("GB2312"));
            for ( itr = m_classlist.begin(); itr < m_classlist.end(); ++itr )
            {
                c_class->Append (wxString::Format (wxT(" %s (%d)"),
                            conv.cMB2WC(itr->Name ().To8BitData()).data (),
                            itr->Num ()
                            ));
            }
            if ( !m_classlist.empty() )
            {
                c_class->SetSelection(0);
                LoadGroup ();
            }
            else
            {
                Grouplist *group = static_cast <Grouplist*> (FindWindow (ID_GROUP_LIST));
                wxASSERT ( group != NULL );
                c_class->Enable (false);
                group->Enable (false);
            }
        }
    }
    void GrouplistDialog::OnClass ( wxCommandEvent& event )
    {
        m_index = -1;
        LoadGroup ();
    }
    void GrouplistDialog::LoadGroup ()
    {
        Grouplist *group = dynamic_cast <Grouplist*> (FindWindow (ID_GROUP_LIST));
        wxChoice *c_server, *c_class;
        c_server    = dynamic_cast <wxChoice*> (FindWindow (ID_SERVER_CHOICE));
        c_class     = dynamic_cast <wxChoice*> (FindWindow (ID_CLASS_CHOICE));
        NiConfig::Config* config = NiConfig::DefaultConfig::GetConfig ();
        wxASSERT ( group != NULL && c_server != NULL && c_class != NULL && config != NULL);
        NiServer::Server server;
        NiDB::NiClass nclass;
        size_t server_sel, class_sel;
        server_sel      = c_server->GetSelection ();
        class_sel       = c_class->GetSelection ();
        if ( server_sel < config->CountServer () && class_sel < m_classlist.size() )
        {
            server = config->GetServerAt (server_sel);
            nclass = m_classlist.at (class_sel);
            group->SetServer (server);
            group->SetClass (nclass);
            group->SetFocus ();
        }
    }
    GrouplistDialog::~GrouplistDialog ()
    {}
}
