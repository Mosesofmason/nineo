////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "server.hpp"

namespace NiServer
{
    Server::Server ()
        : m_port (119)
    {}
    Server::Server ( const wxString& label,     const wxString& address,
                     const wxString& port,      const wxString& user,
                     const wxString& pass )
        : m_label ( label ), m_address ( address ), m_user (user), m_pass ( pass )
    {
        SetPort (port);
    }
    Server::Server ( const wxString& label,     
                     const wxString& address,   const unsigned int& port,
                     const wxString& user,      const wxString& pass )
        : m_label (label), m_address (address), m_port (port), m_user (user), m_pass (pass)
    {}
    Server::~Server ()
    {}
    void Server::SetLabel ( const wxString& label )
    {
        m_label = label;
    }
    wxString Server::GetLabel () const
    {
        return m_label;
    }
    void Server::SetAddress ( const wxString& address )
    {
        m_address = address;
    }
    wxString Server::GetAddress () const
    {
        return m_address;
    }
    void Server::SetPort ( const wxString& port )
    {
        if ( port.IsNumber () )
            SetPort ( wxAtoi (port) );
    }
    void Server::SetPort ( const unsigned int& port )
    {
        m_port = port;
    }
    unsigned int Server::GetPort () const
    {
        return m_port;
    }
    void Server::SetUsername ( const wxString& user )
    {
        m_user = user;
    }
    wxString Server::GetUsername () const
    {
        return m_user;
    }
    void Server::SetPassword ( const wxString& pass )
    {
        m_pass = pass;
    }
    wxString Server::GetPassword () const
    {
        return m_pass;
    }
    NiUtils::HASH Server::Hash () const
    {
        return NiUtils::NiHash ( m_address );
    }
#if 0
    bool Server::operator== ( const Server& server )
    {
        return ( m_label == server.GetLabel () || m_address == server.GetAddress());
    }
#endif
#if 0
    bool Server::operator==  ( const NiGroup::SubGroup& group ) const
    {
        wxString server = group.GetServer ();
        return ( 
                 GetAddress () == server
                       || 
                 GetAddress () == server 
               );
    }
#endif
    wxIPV4address Server::GetIPV4 () const 
    {
        wxIPV4address address;
        address.Hostname ( m_address );
        address.Service ( m_port );
        return address;
    }
    bool Server::RequireAuthinfo () const
    {
        return ( !m_user.IsEmpty() && !m_pass.IsEmpty() );
    }
    BEGIN_EVENT_TABLE ( NiServerDialog, wxDialog )
        EVT_CHECKBOX    ( ID_LOGIN,     NiServerDialog::OnCheckbox )
        EVT_TEXT        ( ID_LABEL,     NiServerDialog::OnEnter )
        EVT_TEXT        ( ID_ADDRESS,   NiServerDialog::OnEnter )
        EVT_TEXT        ( ID_PORT,      NiServerDialog::OnEnter )
        EVT_TEXT        ( ID_USER,      NiServerDialog::OnEnter )
        EVT_TEXT        ( ID_PASS,      NiServerDialog::OnEnter )
        EVT_BUTTON      ( wxID_OK,      NiServerDialog::OnOk )
    END_EVENT_TABLE ()
    NiServerDialog::NiServerDialog ( wxWindow* parent, wxWindowID id, Server& server, 
                                     const wxString& title )
    : m_server (server), wxDialog ( parent, id, title )
    {
        DoUI ();
        Centre ();
    }
    NiServerDialog::~NiServerDialog ()
    {
    }
    void NiServerDialog::OnOk ( wxCommandEvent& event )
    {
        if ( IsModal () )
        {
            wxTextCtrl *name, *address, *port, *user, *pass;
            wxCheckBox *login;
            name        = dynamic_cast <wxTextCtrl*> ( FindWindow(ID_LABEL) );
            address     = dynamic_cast <wxTextCtrl*> ( FindWindow(ID_ADDRESS) );
            port        = dynamic_cast <wxTextCtrl*> ( FindWindow(ID_PORT) );
            user        = dynamic_cast <wxTextCtrl*> ( FindWindow(ID_USER) );
            pass        = dynamic_cast <wxTextCtrl*> ( FindWindow(ID_PASS) );
            login       = dynamic_cast <wxCheckBox*> ( FindWindow(ID_LOGIN) );
            wxASSERT ( name != NULL && address != NULL && port != NULL );
            wxASSERT ( user != NULL && pass != NULL && login != NULL );
            m_server.SetLabel ( name->GetValue () );
            m_server.SetAddress ( address->GetValue () );
            m_server.SetPort ( port->GetValue () );
            m_server.SetUsername ( user->GetValue () );
            m_server.SetPassword ( pass->GetValue () );
            EndModal (wxOK);
        }
    }
    void NiServerDialog::OnCheckbox ( wxCommandEvent& event )
    {
        Checkbox (event.IsChecked ());
        wxButton *ok        = dynamic_cast <wxButton*> ( FindWindow (wxID_OK) );
        wxTextCtrl *user    = dynamic_cast <wxTextCtrl*> ( FindWindow (ID_USER) );
        wxASSERT ( ok != NULL && user != NULL );
        ok->Enable ( CheckInput () );
        user->SetFocus ();
    }
    void NiServerDialog::OnEnter ( wxCommandEvent& event )
    {
        wxButton *ok = dynamic_cast <wxButton*> ( FindWindow (wxID_OK) );
        wxASSERT ( ok != NULL );
        ok->Enable (CheckInput ());
    }
    void NiServerDialog::Checkbox ( const bool& check )
    {
        wxStaticText *luser, *lpass;
        wxTextCtrl *tuser, *tpass;
        luser = dynamic_cast <wxStaticText*> ( FindWindow ( ID_LABEL_USER ) );
        lpass = dynamic_cast <wxStaticText*> ( FindWindow ( ID_LABEL_PASS ) );
        tuser = dynamic_cast <wxTextCtrl*> ( FindWindow ( ID_USER ) );
        tpass = dynamic_cast <wxTextCtrl*> ( FindWindow ( ID_PASS ) );
        wxASSERT ( luser != NULL && lpass != NULL && tuser != NULL && tpass != NULL );
        luser->Enable (check);
        lpass->Enable (check);
        tuser->Enable (check);
        tpass->Enable (check);
    }
    bool NiServerDialog::CheckInput ()
    {
        wxTextCtrl *tname, *taddress, *tport, *tuser, *tpass;
        wxCheckBox *login;
        tname       = dynamic_cast <wxTextCtrl*> ( FindWindow (ID_LABEL) );
        taddress    = dynamic_cast <wxTextCtrl*> ( FindWindow (ID_ADDRESS) );
        tport       = dynamic_cast <wxTextCtrl*> ( FindWindow (ID_PORT) );
        tuser       = dynamic_cast <wxTextCtrl*> ( FindWindow (ID_USER) );
        tpass       = dynamic_cast <wxTextCtrl*> ( FindWindow (ID_PASS) );
        login       = dynamic_cast <wxCheckBox*> ( FindWindow (ID_LOGIN) );

        wxASSERT ( tname != NULL && taddress != NULL && tport != NULL
                   && 
                   tuser != NULL && tpass != NULL && login != NULL );
        bool isok = true;
        if ( tname != NULL && tname->GetValue ().IsEmpty () ) isok = false;
        if ( tport != NULL && 
                (tport->GetValue ().IsEmpty () || !tport->GetValue().IsNumber ()) ) 
            isok = false;
        if ( taddress != NULL && taddress->GetValue ().IsEmpty () ) isok = false;
        if ( login->GetValue () 
                && ( tuser->GetValue().IsEmpty () || tpass->GetValue().IsEmpty () ) )
        {
            isok = false;
        }
        return isok;
    }
    void NiServerDialog::DoUI ()
    {
        wxString name, address, port, user, pass;
        name    = m_server.GetLabel ();
        address = m_server.GetAddress ();
        port    << m_server.GetPort ();
        user    = m_server.GetUsername ();
        pass    = m_server.GetPassword ();


        wxStaticText *llabel, *laddress, *lport, *luser, *lpass;
        llabel      = new wxStaticText ( this, wxID_ANY, _("Name: ") );
        laddress    = new wxStaticText ( this, wxID_ANY, _("Address: "));
        lport       = new wxStaticText ( this, wxID_ANY, _("Port: "));
        luser       = new wxStaticText ( this, ID_LABEL_USER, _("User: "));
        lpass       = new wxStaticText ( this, ID_LABEL_PASS, _("Password:"));
        wxASSERT ( llabel != NULL && laddress != NULL && lport != NULL 
                   &&
                   luser != NULL && lpass != NULL );
        
        wxTextCtrl *tlabel, *taddress, *tport, *tuser, *tpass;
        tlabel      = new wxTextCtrl ( this, ID_LABEL, wxEmptyString,
                                       wxDefaultPosition, wxSize (200, -1),
                                       wxBORDER_SIMPLE );
        taddress    = new wxTextCtrl ( this, ID_ADDRESS, wxEmptyString,
                                       wxDefaultPosition, wxSize (200, -1),
                                       wxBORDER_SIMPLE );
        tport       = new wxTextCtrl ( this, ID_PORT, wxEmptyString, 
                                       wxDefaultPosition, wxSize (60, -1),
                                       wxBORDER_SIMPLE );
        tuser       = new wxTextCtrl ( this, ID_USER, wxEmptyString,
                                       wxDefaultPosition, wxSize (200, -1),
                                       wxBORDER_SIMPLE );
        tpass       = new wxTextCtrl ( this, ID_PASS, wxEmptyString,
                                       wxDefaultPosition, wxSize (200, -1),
                                       wxBORDER_SIMPLE|wxTE_PASSWORD );
        wxASSERT ( tlabel != NULL && taddress != NULL && tport != NULL 
                   && 
                   tuser != NULL && tpass != NULL );

        wxCheckBox *login;
        login = new wxCheckBox ( this, ID_LOGIN, _("This server need login!" ) );
        wxButton *button_ok = NULL, *button_cancel = NULL;
        button_ok       = new wxButton ( this, wxID_OK, _("OK") );
        button_cancel   = new wxButton ( this, wxID_CANCEL, _("Cancel") );
        wxASSERT ( login != NULL && button_ok != NULL && button_cancel != NULL );
        login->SetValue ( !user.IsEmpty () && !pass.IsEmpty() );

        wxString tip_name, tip_address, tip_port, tip_user, tip_pass;
        tip_name = _("What is server name?\neg: newsgroup No.xx");
        tip_address << _("What is server address?\neg:\n")
                    << wxT("\tnews.yaako.com\n")
                    << wxT("\tnews.cn99.com\n")
                    << wxT("\tnews.newsfan.com");
        tip_port = _("Which port to use?\ndefault newsgroup server port is 119");
        tip_user = _("What is username to authinfo in this server?");
        tip_pass = _("What is password to authinfo in this server?");

        tlabel->SetValue ( name );
        tlabel->SetToolTip (tip_name);
        taddress->SetValue ( address );
        taddress->SetToolTip (tip_address);
        tport->SetValue ( port );
        tport->SetToolTip (tip_port);
        tuser->SetValue ( user );
        tuser->SetToolTip (tip_user);
        tpass->SetValue ( pass );
        tpass->SetToolTip (tip_pass);

        wxBoxSizer *main = new wxBoxSizer ( wxVERTICAL );
        wxFlexGridSizer *form, *login_form, *button_sizer;
        wxStaticBoxSizer *login_sizer;

        form = new wxFlexGridSizer ( 2, 0, 0 );
        login_form = new wxFlexGridSizer ( 2, 0, 0 );
        login_sizer = new wxStaticBoxSizer ( wxVERTICAL, this, _("Require login: ") );
        button_sizer = new wxFlexGridSizer ( 1, 5, 0, 0 );

        this->SetSizer (main);

        form->AddGrowableCol (1);
        form->Add ( llabel, 0, wxALL, 2 );
        form->Add ( tlabel, 0, wxALL, 2 );
        form->Add ( laddress, 0, wxALL, 2 );
        form->Add ( taddress, 0, wxALL, 2 );
        form->Add ( lport, 0, wxALL, 2 );
        form->Add ( tport, 0, wxALL, 2 );

        login_form->Add ( luser, 0, wxALL, 2 );
        login_form->Add ( tuser, 0, wxALL, 2 );
        login_form->Add ( lpass, 0, wxALL, 2 );
        login_form->Add ( tpass, 0, wxALL, 2 );

        login_sizer->Add ( login, 0, wxALL, 2 );
        login_sizer->Add ( login_form, 0, wxALL, 2 );
        
        button_sizer->Add ( button_ok, 0, wxALL, 2 );
        button_sizer->Add ( button_cancel, 0, wxALL, 2 );

        main->Add ( form, 0, wxALL, 5 );
        main->Add ( login_sizer, 0, wxALL, 5 );
        main->Add ( button_sizer, 0, wxALL, 5 );
        main->Fit (this);
        main->SetSizeHints (this);

        Checkbox ( login->GetValue () );
        button_ok->Enable ( CheckInput () );
        tlabel->SetFocus ();
    }
}
