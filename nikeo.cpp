////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "config.hpp"
#include "nikeo.hpp"
#include "ident.hpp"
#include "utils.hpp"
#include "log.hpp"

void NiApp::InitDebug ()
{
    wxFFile* file = new wxFFile ( DEBUG_FILENAME, wxT("w+"));
    if ( file )
    {
        wxLog *olog = wxLog::SetActiveTarget ( new wxLogStderr ( file->fp () ) );
        if ( olog )
            delete olog;
    }
    NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
    loger->Message (wxString::Format(wxT("Welcome to %s."), TITLE.c_str()));
}
bool NiApp::InitConfig ()
{   
    NiConfig::Config* config = NiConfig::DefaultConfig::GetConfig (); 
    wxASSERT ( config != NULL );
    if ( !NiConfig::HasConfigFile () )
    {
        NiIdent::Ident ident;
#if __BETA__ 
        ident.Name (NINEO_NAME);
        ident.Email(wxT("xxx@xxx.xx"));
        ident.Signature (wxT("I love cn.fan."));
#elif __WXDEBUG__
        ident.Name (wxT("newblue"));
        ident.Email (wxT("newblue@localhost.cn"));
        ident.Signature (wxT("Good Good Programming!!"));
#endif
        NiIdent::NiIdentDialog dialog ( NULL, wxID_ANY, ident, 
                                        _("Setup default ident"), wxOK|wxCANCEL );
        int ret = dialog.ShowModal ();
        if ( ret == wxOK )
        {
            config->SetIdent (ident);
            config->Save ();
        }
        else if ( ret == wxID_CANCEL )
        {
            return false;
        }
    }
    else
    {
        config->Load ();
    }
    return true;
}
bool NiApp::OnInit ()
{
    bool isok = InitConfig ();
    if ( !isok ) return false;

    InitDebug (); 
    wxSocketBase::Initialize();
    NiConfig::MakeDataDir ();
 
    m_frame = new NiFrame;
    NiConfig::Config* config = NiConfig::DefaultConfig::GetConfig ();
    wxASSERT ( config != NULL );

    if ( m_frame )
    {
        SetTopWindow (m_frame);
        m_frame->Maximize(true);
        m_frame->Centre();
        wxSleep (1);
        m_frame->Show(true);
        m_frame->Init ();
    }
    return true;
}

int NiApp::OnExit ()
{
    return 0;
}
