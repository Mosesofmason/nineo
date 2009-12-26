////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "groupview.hpp"
#include "server.hpp"
#include "group.hpp"
#include <algorithm>
#include "thread.hpp"
#include "log.hpp"

BEGIN_EVENT_TABLE ( NiGroupView, wxTreeCtrl )

END_EVENT_TABLE ()

class NiTreeItemServer : public wxTreeItemData, public NiServer::Server
{
    public:
        NiTreeItemServer () {}
        ~NiTreeItemServer () {};
};
class NiTreeItemGroup : public wxTreeItemData, public NiGroup::SubGroup
{
    public:
        NiTreeItemGroup () {}
        ~NiTreeItemGroup () {}
};

NiGroupView::NiGroupView ( wxWindow* parent, wxWindowID id, const long& style, 
                           const wxString& name )
  : wxTreeCtrl ( parent, id, wxDefaultPosition, wxDefaultSize, style, 
                 wxDefaultValidator, name)
{
    AddRoot (_("Welcome! Nice to see you."));
}
bool NiGroupView::LoadConfig ()
{
    NiConfig::Config* config = NiConfig::DefaultConfig::GetConfig ();
    wxTreeItemId root_id = GetRootItem ();
    NiTreeItemServer* serverdata = NULL;
    NiServer::Server* server = NULL;

    if ( !root_id.IsOk () ) return false; 

    for ( size_t pos = 0, count = config->CountServer (); pos < count; ++pos )
    {
        AddServer ( config->GetServerAt (pos) );
    }
    if ( config->CountServer () == 0 )
    {
#ifdef __BETA__ 
        wxString message;
        message << wxT("Is add default server host?\n")
                << wxT("List:\n\t news.cn99.com\n")
                << wxT("\tnews.yaako.com\n")
                << wxT("\tnews.newsfan.net\n")
                << wxT("\tnews.aioe.org\n")
                << wxT("\tfreenews.netfront.net\n");
        wxMessageDialog dialog (this, message, wxT(""), wxYES_NO|wxICON_QUESTION );
        if ( dialog.ShowModal () == wxID_YES )
        {
            NiServer::Server    cn99   (wxT("CN99"),wxT("news.cn99.com"), 119),
                                yaako  (wxT("Yaako"), wxT("news.yaako.com"), 119),
                                newsfan(wxT("NewsFan"), wxT("news.newsfan.net"), 119),
                                aioe   (wxT("AiOE.org"), wxT("news.aioe.org"), 119),
                                nf     (wxT("NetFront"),wxT("freenews.netfront.net"),119);
            AddNewServer (cn99);
            AddNewServer (yaako);
            AddNewServer (newsfan);
            AddNewServer (aioe);
            AddNewServer (nf);
        }
#elif __WXDEBUG__
        NiServer::Server local (wxT("LocalHost(hamster)"),wxT("localhost"), 119, 
                                wxT("admin"), wxT("123"));
        AddNewServer (local);
#endif
    }

    for ( size_t gpos = 0, gcount = config->CountGroup (); gpos < gcount; ++gpos )
    {
        AddGroup ( config->GetGroupAt (gpos) );
    }
    ExpandAllChildren (root_id);
    return true;
}
bool NiGroupView::AddServer ( const NiServer::Server& server )
{
    NiTreeItemServer* serverdata = NULL;
    NiServer::Server* ptrserver = NULL;
    wxTreeItemId root_id = GetRootItem ();
    if ( !root_id.IsOk () ) return false;

    serverdata  = new NiTreeItemServer;
    ptrserver   = dynamic_cast <NiServer::Server*> ( serverdata );
    wxASSERT ( serverdata != NULL && ptrserver != NULL );
    *ptrserver = server;
    AppendItem ( root_id, serverdata->GetLabel (), -1, -1, serverdata );
    ExpandAllChildren (root_id);
    m_serverlist.push_back (ptrserver);
    return true;
}
bool NiGroupView::HasGroup ( const NiGroup::SubGroup* group ) const
{
    NiGroup::SubGroupPtrList::const_iterator itr;
    itr = std::find ( m_grouplist.begin (), m_grouplist.end(), group );
    return ( itr != m_grouplist.end () );
}
bool NiGroupView::GetGroup ( const wxTreeItemId& id, NiGroup::SubGroup& group )
{
    wxTreeItemData *data = GetItemData ( id );
    NiTreeItemGroup *groupdata = static_cast <NiTreeItemGroup*> (data);
    if ( groupdata != NULL && HasGroup (groupdata) )
    {
        group = *groupdata;
        return true;
    }else 
    {
        return false;
    }
}
bool NiGroupView::AddGroup ( const NiGroup::SubGroup& group )
{
    if ( HasGroup (group) ) return true;
    NiServer::ServerPtrList::iterator itr;
    wxTreeItemId serverid;
    for ( itr = m_serverlist.begin(); itr < m_serverlist.end(); ++itr )
    { 
        if ( (*itr)->GetAddress () == group.GetServer ()
                || 
             (*itr)->GetLabel () == group.GetServer() )
        {
            NiTreeItemServer *server = static_cast <NiTreeItemServer*>(*itr);
            serverid = server->GetId ();
            break;
        }
    }

    if ( serverid.IsOk () )
    {
        NiConfig::Config *config = NiConfig::DefaultConfig::GetConfig ();
        NiTreeItemGroup *itemgroup = new NiTreeItemGroup;
        NiGroup::SubGroup *ptrgroup = dynamic_cast <NiGroup::SubGroup*> (itemgroup);
        wxASSERT ( itemgroup != NULL && ptrgroup != NULL && config != NULL );
        *ptrgroup = group;
        config->AddGroup (group);
        config->Save ();
        AppendItem ( serverid, group.Name(), -1, -1, itemgroup );
        ExpandAllChildren ( serverid );
        m_grouplist.push_back (ptrgroup);
        return true;
    }
    return false;
}
struct OF_MATCH_SERVER_PTR
{
    public:
        OF_MATCH_SERVER_PTR ( const NiServer::Server& server )
            : m_server (server)
        {}
        bool operator () ( NiServer::ServerPtrList::value_type& data )
        {
            return ( m_server.GetLabel () == data->GetLabel () 
                     ||
                     m_server.GetAddress () == data->GetAddress () );
        }
    private:
        const NiServer::Server& m_server;
};
bool NiGroupView::HasServer ( const NiServer::Server& server )
{
    if ( m_serverlist.empty () ) return false;
    NiServer::ServerPtrList::iterator itr;
    itr = std::find_if ( m_serverlist.begin(), m_serverlist.end(),
                         OF_MATCH_SERVER_PTR ( server ) );
    return ( itr != m_serverlist.end () );
}
struct OF_MATCH_GROUP_PTR
{
    public:
        OF_MATCH_GROUP_PTR ( const NiGroup::SubGroup& group )
            : m_group(group)
        {}
        bool operator () ( NiGroup::SubGroupPtrList::value_type& data )
        {
            return ( m_group.GetServer () == data->GetServer ()
                     &&
                     m_group.Name () == data->Name () );
        }
    private:
        const NiGroup::SubGroup& m_group;
};
bool NiGroupView::HasGroup ( const NiGroup::SubGroup& group )
{
    if ( m_grouplist.empty () ) return false;
    return ( std::find_if ( m_grouplist.begin(), m_grouplist.end(),
                            OF_MATCH_GROUP_PTR (group) ) != m_grouplist.end() );
}
bool NiGroupView::AddNewServer ( const NiServer::Server& server )
{
    bool isok = false;
    if ( !HasServer ( server ) )
    {
        NiConfig::Config* config = NiConfig::DefaultConfig::GetConfig ();
        AddServer (server);
        config->AddServer (server);
        config->Save ();
        isok = true;
    }
    else
    {
        wxString message;
        message = wxString::Format ( _("Server %s [%s] was exits!\n"),
                server.GetLabel ().c_str(),
                server.GetAddress ().c_str() );
        wxMessageDialog dialog ( this, message, _("Message"), 
                wxOK|wxSTAY_ON_TOP|wxICON_INFORMATION );
        dialog.Centre ();
        dialog.ShowModal ();
        isok = true;
    }
    return isok;
}
bool NiGroupView::AddNewServer ()
{
    NiServer::Server server;
    NiServer::NiServerDialog dialog ( this, wxID_ANY, server, _("Add a new server"));
    if ( dialog.ShowModal () == wxOK )
    {
        AddNewServer ( server );
    }
    return true;
}
bool NiGroupView::UpdateGrouplist ( const int& pos )
{
    NiThread::ThreadManager* manager = NiThread::DefaultManager::Manager ();
    bool isok = false;
    if ( pos >= 0 && pos < m_serverlist.size ())
    {
        wxString message;
        message << wxT("Update grouplist of ")
                << m_serverlist[pos]->GetAddress ();
        NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger ();
        loger->Message (message);
        manager->AddTaskUpdateGrouplist ( *m_serverlist[pos] );
        manager->AddThreadNntp ( *m_serverlist[pos] );
        isok = true;
    }
    else
    {
        NiServer::ServerPtrList::iterator bitr, eitr;
        bitr = m_serverlist.begin ();
        eitr = m_serverlist.end ();
        NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger ();
        loger->Message(_("Update grouplist of all server."));
        for ( ; bitr < eitr; ++bitr )
        {
           manager->AddTaskUpdateGrouplist ( **bitr );
           manager->AddThreadNntp ( **bitr );
        }
        isok = true;
    }
    if ( isok )
    {
        wxMessageDialog dialog ( this, 
                _("Are you want to update grouplist now?\nNote: Your computer must online."),
                _("Update grouplist now?"), wxYES_NO|wxICON_QUESTION );
        
        if ( dialog.ShowModal () == wxID_YES )
        {
            manager->Online ();
            manager->Run ();
        }
    }
    return isok;
}
void NiGroupView::ItemMenu ()
{
    wxLogDebug (wxT("ItemMenu"));
}
void NiGroupView::SyncAll ()
{
    wxLogDebug (wxT("SyncAll"));
    NiGroup::SubGroupPtrList::iterator gbi, gei;
    gbi = m_grouplist.begin ();
    gei = m_grouplist.end ();
    NiThread::ThreadManager *manager = NiThread::DefaultManager::Manager ();
    for ( ; gbi < gei; ++gbi )
    {
        manager->AddTaskUpdateGroup ( (**gbi) ); 
    }
    NiServer::ServerPtrList::iterator sbi, sei;
    sbi = m_serverlist.begin ();
    sei = m_serverlist.end ();
    manager->Offline ();
    for ( ; sbi < sei; ++sbi )
    {
        manager->AddThreadNntp ( (**sbi) );
    }
    manager->Online ();
    manager->Run ();
}
NiGroupView::~NiGroupView ()
{};
