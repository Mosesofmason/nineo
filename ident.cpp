////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "ident.hpp"
#include "config.hpp"
#include <wx/textctrl.h>
#include "regex.hpp"

namespace NiIdent
{
    NiIdent::NiIdent ( const wxString& label )
        : m_re (NULL), m_label (label)
    {}
    NiIdent::~NiIdent ()
    {
        if ( m_re != NULL ) delete m_re;
    }
    wxString NiIdent::Label () const
    {
        return m_label;
    }
    void NiIdent::Label ( const wxString& label )
    {
        m_label = label;
    }
    bool NiIdent::operator () ( const wxString& server, const wxString& group )
    {
        const wxString m = wxString::Format (wxT("%s:%s"), server.c_str(), group.c_str());
        return ( IsMatch(m) || IsRegex (m) );
    }
    bool NiIdent::operator () ( const wxString& m )
    {
        return ( IsMatch(m) || IsRegex (m) );
    }

    bool NiIdent::operator %  ( const wxString& m )
    {
        return IsMatch (m);
    }

    bool NiIdent::operator|  ( const wxString& m )
    {
        return IsRegex (m);
    }

    bool NiIdent::operator == ( const wxString& m )
    {
        return ( IsMatch (m) || IsRegex (m) );
    }

    NiIdent& NiIdent::operator =  ( const NiIdent& id )
    {
        m_name = id.Name ();
        m_email = id.Email ();
        m_signature = id.Signature ();
        Regex (id.Regex());
        Match (id.Match());
        return *this;
    }
    wxString NiIdent::Match () const
    {
        return m_match;
    }
    wxString NiIdent::Regex () const
    {
        return m_regex;
    }
    void NiIdent::Match ( const wxString& match )
    {
        m_match = match;
    }
    void NiIdent::Regex ( const wxString& re )
    {
        m_regex = re;
        if ( !m_regex.IsEmpty() )
        {
            if ( m_re )
            {   
                m_re->Compile ( m_regex );
            }
            else
            {
                m_re = new wxRegEx;
                if ( m_re ) m_re->Compile ( m_regex );
            }
        }
        else 
        {
            delete m_re;
            m_re = NULL;
        }
    }
    bool NiIdent::IsMatch ( const wxString& m )
    {
        return ( !m_match.IsEmpty() && m_match.Matches (m) );
    }
    bool NiIdent::IsRegex ( const wxString& re )
    {
        return ( m_re && !m_regex.IsEmpty() && m_re->IsValid() && m_re->Matches (re) );
    }
    void NiIdent::Name ( const wxString& name )
    {
        m_name = name;
    }
    void NiIdent::Email ( const wxString& email )
    {
        m_email = email;
    }

    void NiIdent::Signature ( const wxString& signature )
    {
        m_signature = signature;
    }

    wxString NiIdent::Name () const
    {
        return m_name;
    }

    wxString NiIdent::Email () const
    {
        return m_email;
    }

    wxString NiIdent::Signature () const
    {
        return m_signature;
    }

    ////////////////////////////////////////////////////////////////////////////////
    BEGIN_EVENT_TABLE ( NiIdentDialog, wxDialog )
        EVT_BUTTON  ( wxID_OK,      NiIdentDialog::OnOk )
        EVT_TEXT    ( ID_USERNAME,  NiIdentDialog::OnEntry )
        EVT_TEXT    ( ID_EMAIL,     NiIdentDialog::OnEntry )
        EVT_TEXT    ( ID_SIGNATURE, NiIdentDialog::OnEntry )
    END_EVENT_TABLE ()
    NiIdentDialog::NiIdentDialog ( wxWindow* parent, wxWindowID id, Ident& ident,
                                   const wxString& title, long style )
    : m_ident (ident), wxDialog ( parent, id, title ) 
    {
        DoUI ( style );
        if ( !( style & wxCANCEL ) )
        {
            SetWindowStyleFlag (wxCAPTION|wxSTAY_ON_TOP);
        }
        Centre ();
    }
    
    void NiIdentDialog::DoUI ( long style )
    {
        wxString help_text = _("Ident");

        wxString tips_username, tips_email, tips_signature;
        tips_username   = _("What's you name?\neg: Hero");
        tips_email      = _("Give me your email address!\neg: hero@game.com");
        tips_signature  = _("What about you?\neg: I love PPMM, ^o^\n May I make friend with you?");

        wxBoxSizer *main, *button;
        main    = new wxBoxSizer (wxVERTICAL);
        button  = new wxBoxSizer (wxHORIZONTAL);

        wxFlexGridSizer* form = new wxFlexGridSizer ( 2, 0, 0 );
    
        wxStaticText *luser, *lemail, *lsignature, *help;
        wxTextCtrl *tuser, *temail, *tsignature;

        help        = new wxStaticText ( this, wxID_ANY, help_text);
        luser       = new wxStaticText ( this, wxID_ANY, _("Username: "));
        lemail      = new wxStaticText ( this, wxID_ANY, _("Email: "));
        lsignature  = new wxStaticText ( this, wxID_ANY, _("Signature: "));

        tuser       = new wxTextCtrl ( this, ID_USERNAME, wxEmptyString,
                                       wxDefaultPosition, wxSize (480, -1),
                                       wxBORDER_SIMPLE );

        temail      = new wxTextCtrl ( this, ID_EMAIL, wxEmptyString,
                                       wxDefaultPosition, wxSize (480, -1),
                                       wxBORDER_SIMPLE );

        tsignature  = new wxTextCtrl ( this, ID_SIGNATURE, wxEmptyString,
                                       wxDefaultPosition, wxSize (480, 100), 
                                       wxTE_MULTILINE|wxBORDER_SIMPLE );

        luser->SetToolTip (tips_username);
        lemail->SetToolTip (tips_email);
        lsignature->SetToolTip (tips_signature);
        
        tuser->SetToolTip (tips_username);
        temail->SetToolTip (tips_email);
        tsignature->SetToolTip (tips_signature);

        main->Add (help, 0, wxALL|wxEXPAND, 12 );
        main->Add (form, 0, wxALL, 8 );
        main->Add (button, 0, wxALL, 8 );
        form->AddGrowableCol (1);

        form->Add (luser,   0,  wxALL,  2);
        form->Add (tuser,   0,  wxALL|wxEXPAND,  2);
        form->Add (lemail,  0,  wxALL,  2);
        form->Add (temail,  0,  wxALL|wxEXPAND,  2);
        form->Add (lsignature,  0,  wxALL,  2);
        form->Add (tsignature,  0,  wxALL|wxEXPAND,  2);
    
        wxButton *b_ok, *b_cancel;
        b_ok    =  new wxButton ( this, wxID_OK, _("OK") );
        button->Add ( b_ok, 0, wxALL|wxALIGN_RIGHT , 5 );
        if ( style & wxCANCEL )
        {
            b_cancel=  new wxButton ( this, wxID_CANCEL, _("Cancel") );
            button->Add ( b_cancel, 0, wxALL|wxALIGN_RIGHT, 5 );
        }
        if ( !CheckInput () )
        {
            b_ok->Enable (false);
        }

        tuser->SetValue ( m_ident.Name () );
        temail->SetValue ( m_ident.Email () );
        tsignature->SetValue ( m_ident.Signature () );

        this->SetSizer (main);
        main->Fit (this);
        main->SetSizeHints (this);
    }     
    
    void NiIdentDialog::OnOk ( wxCommandEvent& event )
    {
        if ( IsModal () )
        {
            EndModal (wxOK);
        }
    }
    void NiIdentDialog::OnEntry ( wxCommandEvent& event )
    {
        wxButton *button = dynamic_cast <wxButton*> (FindWindow (wxID_OK));
        wxASSERT ( button != NULL );
#if 0
        if ( button != NULL )
        {
#endif
            if ( CheckInput () )
            {
                button->Enable (true);
            }
            else
            {
                button->Enable (false);
            }
#if 0
        }
#endif
    }
    bool NiIdentDialog::CheckInput ()
    {
        bool isok = true;
        wxTextCtrl *username, *email, *signature;
        username    = dynamic_cast <wxTextCtrl*> (FindWindow (ID_USERNAME));
        email       = dynamic_cast <wxTextCtrl*> (FindWindow (ID_EMAIL));
        signature   = dynamic_cast <wxTextCtrl*> (FindWindow (ID_SIGNATURE));
        wxASSERT ( username != NULL && email != NULL && signature != NULL );
#if 0
        if ( username != NULL && email != NULL && signature != NULL )
        {
#endif
            if ( username && username->GetValue().IsEmpty() ) 
            {
                isok = false;
            }
            if ( email && !IsEmail (email->GetValue()) ) 
            {
                isok = false;
            }
            if ( signature && signature->GetValue().Freq (wxT('\n')) > 5 ) 
            {
                isok = false;
            }
            if ( isok )
            {
                m_ident.Name ( username->GetValue () );
                m_ident.Email ( email->GetValue () );
                m_ident.Signature ( signature->GetValue () );
            }
#if 0
        }
        else
        {
            isok = false;
        }
#endif
        return isok;
    }
    bool NiIdentDialog::IsEmail ( const wxString& email )
    {
        wxRegEx re ( NiRegEx::MatchEmail );
        return re.Matches (email); 
    }
    NiIdentDialog::~NiIdentDialog ()
    {}

    BEGIN_EVENT_TABLE ( NiIdentListCtrl, wxListCtrl )
        END_EVENT_TABLE ()

    NiIdentListCtrl::NiIdentListCtrl ( wxWindow *parent, wxWindowID id, NiIdentArray&
            idents, const wxString& name )
        : m_idents (idents)
    {
        wxSize size ( 778, 600 );
        if ( parent )
        {
            wxSize psize = parent->GetClientSize ();
            size.SetWidth ( psize.GetWidth() );
            size.SetHeight ( psize.GetHeight () * 0.9 );
        }
        Create ( parent, id, wxDefaultPosition, size, 
                wxLC_REPORT|wxLC_VIRTUAL|NStyle, 
                wxDefaultValidator, name );

        SetItemCount (m_idents.size());
        size_t col_width = size.GetWidth()/6;
        InsertColumn ( 0, _("Label:"), wxLIST_FORMAT_LEFT, col_width);
        InsertColumn ( 1, _("Name:"), wxLIST_FORMAT_LEFT, col_width);
        InsertColumn ( 2, _("Email:"), wxLIST_FORMAT_LEFT, col_width);
        InsertColumn ( 3, _("Match:"), wxLIST_FORMAT_LEFT, col_width);
        InsertColumn ( 4, _("Regex:"), wxLIST_FORMAT_LEFT, col_width);
        InsertColumn ( 5, _("Signature:"), wxLIST_FORMAT_LEFT, 
                        size.GetWidth() -  ( col_width * 5 ));
    }
    NiIdentListCtrl::~NiIdentListCtrl ()
    {}
    wxListItemAttr* NiIdentListCtrl::OnGetItemAttr ( long item ) const
    {
        return NULL;
    }
    int NiIdentListCtrl::OnGetItemImage ( long item ) const
    {
        return -1;
    }
    int NiIdentListCtrl::OnGetItemColumnImage ( long item, long column ) const
    {
        return -1;
    }
    wxString NiIdentListCtrl::OnGetItemText ( long item, long column ) const
    {
        if ( item < 0 || item > m_idents.size() ) return wxEmptyString;
        size_t idx = (size_t) item, col = (size_t) column;
        wxString text;
        const NiIdent& ident= m_idents.at(idx);

        switch (col)
        {
            case 0: text = ident.Label();
                    break;
            case 1: text = ident.Name ();
                    break;
            case 2: text = ident.Email ();
                    break;
            case 3: text = ident.Match ();
                    break;
            case 4: text = ident.Regex ();
                    break;
            case 5: text = ident.Signature ();
                    break;
            default:break;
        };
        return text;
    }
    BEGIN_EVENT_TABLE ( NiIdentManager, wxDialog )
    END_EVENT_TABLE ()

    NiIdentManager::NiIdentManager ( wxWindow* parent, wxWindowID id, const wxString& title )
    {
        Create (parent, id, title, wxDefaultPosition, wxSize(780,620), 
                wxDEFAULT_DIALOG_STYLE, title );
        Centre ();
        wxBoxSizer *main_sizer = new wxBoxSizer ( wxVERTICAL );
        wxFlexGridSizer *button_sizer = new wxFlexGridSizer ( 1, 12, 0, 5 );
        NiIdentListCtrl *list = new NiIdentListCtrl (this, wxID_ANY, m_idents );
        if ( button_sizer )
        {
            wxButton *bview, *bnew, *bdel, *bmodif, *bok, *bcancel;
            bview = bnew = bdel = bmodif = bok = bcancel = NULL;
            bview = new wxButton (this, ID_VIEW, _("View ident"));
            bnew = new wxButton (this, ID_NEW, _("New ident"));
            bdel = new wxButton (this, ID_DEL, _("Delete"));
            bmodif = new wxButton (this, ID_MODIF, _("Modif ident"));
            bok = new wxButton (this, wxID_OK, _("OK"));
            bcancel = new wxButton (this, wxID_CANCEL, _("Cancel"));
            button_sizer->AddSpacer (1);
            button_sizer->Add (bview, wxRIGHT);
            button_sizer->AddSpacer (1);
            button_sizer->Add (bnew, wxRIGHT);
            button_sizer->AddSpacer (1);
            button_sizer->Add (bdel, wxRIGHT);
            button_sizer->AddSpacer (1);
            button_sizer->Add (bmodif, wxRIGHT);
            button_sizer->AddSpacer (1);
            button_sizer->Add (bok, wxRIGHT);
            button_sizer->AddSpacer (1);
            button_sizer->Add (bcancel, wxRIGHT);
        }
        main_sizer->Add ( list, wxEXPAND );
        main_sizer->AddSpacer (2);
        main_sizer->Add (button_sizer );
        SetSizer (main_sizer);
        main_sizer->Fit(this);
        main_sizer->SetSizeHints(this);
    }
    NiIdentManager::~NiIdentManager ()
    {}
}
