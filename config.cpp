
////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////
#include "group.hpp"
#include "server.hpp"
#include "config.hpp"
#include <wx/file.h>
#include <algorithm>
#include "log.hpp"

namespace NiConfig
{
    using namespace NiServer;
    using namespace NiGroup;
    using namespace NiIdent;
    Config::Config ( const wxString& filename )
        : m_filename ( filename )
    {}
    Config::~Config ()
    {}
    bool Config::InitConfig ()
    {
        if ( m_filename.IsEmpty () ) return false;
        TiXmlDocument document ( m_filename.To8BitData () );
        TiXmlDeclaration* decl = new TiXmlDeclaration ("1.0", "utf-8","");
        TiXmlElement* root = new TiXmlElement ("NiNeo");
        TiXmlComment* comment = new TiXmlComment;

        comment->SetValue ("Dont edit this file!");
        root->LinkEndChild (comment);
        document.LinkEndChild (decl);
        document.LinkEndChild (root);

        return document.SaveFile (m_filename.To8BitData ());
    }

    bool Config::HasConfigFile () const
    {
        return ( !m_filename.IsEmpty () && wxFile::Exists (m_filename) );
    }
    bool Config::Load ()
    {
        if ( !HasConfigFile () ) return false;

        TiXmlDocument document ( m_filename.To8BitData () ); 
        bool isok = document.LoadFile ();
        if ( !isok )
        {
            NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger ();
            wxString message = wxString::Format (wxT("Load config file error [ %s ]."),
                                      wxString::From8BitData ( document.ErrorDesc() ));
            return false;
        }
        document.Print ();
        TiXmlElement* root = document.RootElement ();
        if ( root )
        {
            LoadIdent   (root);
            LoadTable   (root);
            LoadServer  (root);
            LoadGroup   (root);
        }
        return false;
    }
    void Config::LoadTable ( TiXmlElement* root )
    {
        if ( root != NULL )
        {
            TiXmlHandle hroot (root);
            TiXmlElement *set;
            set = hroot.FirstChildElement ("Setup").FirstChildElement("Set").Element();
            TiXmlAttribute *attr = NULL; 
            wxString key, value;
            for ( ; set != NULL ; set = set->NextSiblingElement () )
            {
                attr = set->FirstAttribute ();
                if ( attr ) 
                {
                    key = wxString::FromUTF8 (attr->Name ());
                    value = wxString::FromUTF8 (attr->Value ());
                    m_setuptable.insert ( std::make_pair ( key, value ) );
                    wxLogDebug (wxT("set %s=%s"), key.c_str(), value.c_str());
                }
            }
        }
    }
    void Config::LoadIdent ( TiXmlElement* root )
    {
        if ( root != NULL )
        {
            TiXmlHandle hroot (root);
            TiXmlElement* ident;
            ident = hroot.FirstChildElement("Ident").Element ();
            if ( ident )
            {
                m_ident.Name (wxString::FromUTF8 ( ident->Attribute("Username")));
                m_ident.Email (wxString::FromUTF8 ( ident->Attribute("Email")));
                m_ident.Signature (wxString::FromUTF8 ( ident->Attribute("Signature")));
            }
        }
    }
    void Config::LoadServer ( TiXmlElement* root )
    {
        if ( root != NULL )
        {
            TiXmlHandle hroot ( root );
            TiXmlElement *server;
            server = hroot.FirstChild ("ServerList").FirstChild ("Server").Element();

            while ( server )
            {
                m_serverlist.push_back (
                        Server ( wxString::FromUTF8(server->Attribute ("Label")),
                                 wxString::FromUTF8(server->Attribute ("Address")),
                                 wxString::FromUTF8(server->Attribute ("Port")),
                                 wxString::FromUTF8(server->Attribute ("Username")),
                                 wxString::FromUTF8(server->Attribute ("Password"))
                               )
                                        );
                wxLogDebug ( wxT("Load %s"),
                             wxString::FromUTF8(server->Attribute("Label")).c_str());
                server = server->NextSiblingElement ();
            }
        }
    }
    void Config::LoadGroup ( TiXmlElement* root )
    {
        wxASSERT ( root != NULL );
        TiXmlHandle hroot (root);
        TiXmlElement* group;
        group = hroot.FirstChild ("GroupList").FirstChild ("Group").Element ();
        SubGroup subgroup;
        while ( group )
        {
            m_grouplist.push_back ( 
                    SubGroup ( wxString::FromUTF8 ( group->Attribute ("Server") ),
                               wxString::FromUTF8 ( group->Attribute ("Name") )
                               )
                    );
            group = group->NextSiblingElement ();
        }
    }
    bool Config::Save ()
    {
        if ( m_filename.IsEmpty () ) return false;
        TiXmlDocument doc ( m_filename.To8BitData () );
        TiXmlDeclaration* decl = new TiXmlDeclaration ("1.0", "utf-8", "");
        TiXmlElement* root = new TiXmlElement ("NiNeo");
        TiXmlComment* comment = new TiXmlComment;
        comment->SetValue ("Dont edit this file!");

        root->LinkEndChild (comment);
        doc.LinkEndChild (decl);
        doc.LinkEndChild (root);
        SaveIdent   (root);
        SaveServer  (root);
        SaveGroup   (root);
        SaveTable   (root);
        return doc.SaveFile ();
    }
    void Config::SaveTable ( TiXmlElement* root )
    {
        if ( root )
        {
            TiXmlElement* setup = new TiXmlElement ("Setup");
            root->LinkEndChild (setup);
            SetupTable::iterator itr;
            for ( itr = m_setuptable.begin(); itr != m_setuptable.end (); ++itr )
            {
                TiXmlElement* node = new TiXmlElement ("Set");
                if ( node )
                {
                    setup->LinkEndChild (node);
                    node->SetAttribute (itr->first.ToUTF8 ().data(),
                                        itr->second.ToUTF8 ().data());
                }
            }
        }
    }
    void Config::SaveIdent ( TiXmlElement* root )
    {
        if ( root )
        {
            TiXmlElement* ident = new TiXmlElement ("Ident");
            if ( ident )
            {
                ident->SetAttribute ("Username", m_ident.Name().ToUTF8().data ());
                ident->SetAttribute ("Email", m_ident.Email ().ToUTF8().data ());
                ident->SetAttribute ("Signautre", m_ident.Signature().ToUTF8().data());
                root->LinkEndChild (ident);
            }
        }
    }
    void Config::SaveServer ( TiXmlElement* root )
    {
        TiXmlElement* serverlist = new TiXmlElement("ServerList");
        wxASSERT ( root != NULL && serverlist != NULL );

        root->LinkEndChild ( serverlist );
        ServerList::iterator itr = m_serverlist.begin ();
        for ( ; itr < m_serverlist.end (); ++itr )
        {
            TiXmlElement* server = new TiXmlElement("Server");
            wxASSERT ( server != NULL );
            serverlist->LinkEndChild (server);
            server->SetAttribute ("Label",      itr->GetLabel ().ToUTF8 ().data());
            server->SetAttribute ("Address",    itr->GetAddress ().ToUTF8 ().data());
            server->SetAttribute ("Port",       itr->GetPort());
            server->SetAttribute ("Username",   itr->GetUsername ().ToUTF8 ().data());
            server->SetAttribute ("Password",   itr->GetPassword ().ToUTF8 ().data());
        }
    }
    void Config::SaveGroup ( TiXmlElement* root )
    {
        TiXmlElement *grouplist = new TiXmlElement ("GroupList"), *group;
        wxASSERT ( root != NULL );

        root->LinkEndChild (grouplist);
        SubGroupList::iterator itr;

        for ( itr = m_grouplist.begin (); itr < m_grouplist.end (); ++itr )
        {
            group = new TiXmlElement ("Group");
            grouplist->LinkEndChild (group);
            group->SetAttribute ("Server",  itr->GetServer ().ToUTF8().data());
            group->SetAttribute ("Name",    itr->Name ().ToUTF8().data());
        }
    }
    void Config::Clear ()
    {
        m_setuptable.clear ();
    }
    bool Config::Set ( const wxString& key, const wxString& value )
    {
        SetupTable::iterator itr = m_setuptable.find (key);

        if ( itr != m_setuptable.end () )
        {
            itr->second = value;
            return true;
        }
        else
        {
            m_setuptable.insert ( std::make_pair (key, value) );
            return true;
        }
    }
    bool Config::Set ( const wxString& key, const double& d )
    {
        wxString value;
        value << d;
        return Set ( key, value );
    }
    bool Config::Set ( const wxString& key, const long& l )
    {
        wxString value;
        value << l;
        return Set ( key, value );
    }
    bool Config::Set ( const wxString& key, const int& i )
    {
        wxString value;
        value << i;
        return Set ( key, value );
    }
    bool Config::Get ( const wxString& key, wxString& value )
    {
        SetupTable::iterator itr = m_setuptable.find (key);
        if ( itr != m_setuptable.end () )
        {
            value = itr->second;
            return true;
        }
        else
        {
            return false;
        }

    }
    bool Config::Get ( const wxString& key, double& d )
    {
        wxString value;
        bool isok = Get ( key, value );
        if ( !isok ) return false;
        return value.ToDouble (&d);
    }
    bool Config::Get ( const wxString& key, long& l )
    {
        wxString value;
        bool isok = Get ( key, value );
        if ( !isok ) return false;
        return value.ToLong (&l);
    }
    bool Config::Get ( const wxString& key, int& i )
    {
        wxString value;
        bool isok = Get (key, value);
        if ( !isok ) return false;
        return value.ToLong ((long*)&i);
    }
    bool Config::Del ( const wxString& key )
    {
        SetupTable::iterator itr = m_setuptable.find ( key );
        if ( itr != m_setuptable.end () )
        {
            m_setuptable.erase (itr);
            return true;
        }
        else
        {
            return false;
        }
    }
    void Config::SetIdentName ( const wxString& name )
    {
        m_ident.Name (name);
    }
    void Config::SetIdentEmail ( const wxString& email )
    {
        m_ident.Email (email);
    }
    void Config::SetIdentSignature ( const wxString& signature )
    {
        m_ident.Signature (signature);
    }
    void Config::SetIdent ( const Ident& id )
    {
        m_ident = id;
    }
    wxString Config::GetIdentName () const
    {
        return m_ident.Name ();
    }
    wxString Config::GetIdentEmail () const
    {
        return m_ident.Email ();
    }
    wxString Config::GetIdentSignature () const
    {
        return m_ident.Signature ();
    }
    Ident Config::GetIdent () const
    {
        return m_ident;
    }

    size_t Config::CountServer () const
    {
        return m_serverlist.size ();
    }
    void Config::AddServer ( const Server& server )
    {
        ServerList::iterator itr;
        itr = std::find_if ( m_serverlist.begin(), m_serverlist.end(), 
                          OF_MATCH_SERVER (server.GetAddress ()));

        if ( itr == m_serverlist.end() )
        {
            m_serverlist.push_back ( server );
        }
        else
        {
            *itr = server;
        }
    }
    Server Config::GetServer ( const wxString& label ) const
    {
        return Server ();
    }
    Server Config::GetServerAt ( const size_t& idx ) const
    {
        return m_serverlist [idx];
    }
    void Config::GetServerAt ( const size_t& idx, Server& server ) const
    {
        server = m_serverlist [idx];
    }
    bool Config::DelServer ( const wxString& label )
    {
        return false;
    }
    bool Config::DelServer ( const Server& server )
    {
        return false;
    }
    bool Config::DelServerAt ( const size_t& idx )
    {
        ServerList::iterator itr = m_serverlist.begin () + idx;
        if ( itr < m_serverlist.end () )
        {
            m_serverlist.erase (itr);
            return true;
        }
        else
        {
            return false;
        }
    }
    
    size_t Config::CountGroup () const
    {
        return m_grouplist.size ();
    }
    void Config::AddGroup ( const SubGroup& group )
    {
        SubGroupList::iterator itr;
        itr = std::find ( m_grouplist.begin(), m_grouplist.end(), group );
        if ( itr == m_grouplist.end() )
        {
            m_grouplist.push_back ( group );
        }
        else
        {
            *itr = group;
        }
    }
    SubGroup Config::GetGroupAt ( const size_t& idx ) const
    {
        return m_grouplist [ idx ];
    }
    Config* DefaultConfig::m_config = NULL;
    DefaultConfig::DefaultConfig ()
    {}
    DefaultConfig::~DefaultConfig ()
    {}
    Config* DefaultConfig::GetConfig ()
    {
        if ( m_config == NULL )
        {
            m_config = new Config ( ConfigFile () );
        }
        return m_config;
    }
};

