#ifndef __NEXT_NINEO_CONFIG__DEFINE__
#define __NEXT_NINEO_CONFIG__DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/debug.h>
#include <tinyxml/tinyxml.h>
#include <map>
#include "ident.hpp"
#include "server.hpp"
#include "group.hpp"

const unsigned int ver_major (0), ver_minor (0), ver_dev (1);
#ifdef __WXDEBUG__
const wxString NINEO_NAME (wxT("NiNeo"));
const wxString NINEO_VERSION (wxString::Format(wxT("[DEBUG] %d.%d.%d"), ver_major, ver_minor, ver_dev));
const wxString DEBUG_FILENAME ( wxString::Format (wxT("%s-%d.%d.%d.log"),
                                NINEO_NAME.Lower ().c_str(),
                                ver_major, ver_minor, ver_dev));
#elif __BETA__
const wxString NINEO_NAME (wxT("KiNeo"));
const short YEAR = __BETAYEAR__, MONTH = __BETAMONTH__, DAY = __BETADAY__;
const wxString BETA_DATE        = wxString::Format (wxT("%d-%d-%d"),
                                                        YEAR,
                                                        MONTH,
                                                        DAY);
const wxString NINEO_VERSION    = wxString::Format ( wxT("[BETA] %s"), 
                                                     BETA_DATE.c_str() );
const wxString DEBUG_FILENAME   = wxString::Format ( wxT("kineo.beta.%s.log"), 
                                                     BETA_DATE.c_str());
#else
const wxString NINEO_NAME (wxT("NiNeo"));
const wxString NINEO_VERSION (wxString::Format(wxT("[RELEASE] %d.%d.%d"), ver_major, ver_minor, ver_dev));
const wxString DEBUG_FILENAME (wxT("NiNeo.log"));
#endif

const wxString TITLE = wxString::Format (wxT("%s %s"),
					 NINEO_NAME.c_str(),
					 NINEO_VERSION.c_str());

const long NStyle = wxBORDER_SIMPLE;
const wxString CONFIGFILENAME = wxT("config.xml");

namespace NiConfig
{
    using namespace NiGroup;
    using namespace NiServer;
    using namespace NiIdent;

    static size_t MaxFetchCount = 300;
    
    inline wxString WorkDir ()
    {
        return wxGetCwd ();
    }
    inline wxString DataDir ()
    {
        return wxString::Format (wxT("%s/Data"), wxGetCwd().c_str());
    }
    inline wxString ConfigFile ()
    {
        return CONFIGFILENAME;
    }
    inline wxString ServerActiveDataFile ( const Server& server )
    {
        return wxString::Format ( wxT("%s/%s.adb"), 
                                  DataDir().c_str(),
                                  server.GetAddress().c_str() );
    }
    inline wxString GroupArticleDataFile ( const SubGroup& group )
    {
        return wxString::Format ( wxT("%s/%s.mdb"),
                                  DataDir().c_str(),
                                  group.Name().c_str());
    }
    inline bool HasConfigFile () 
    {
        return wxFileExists (ConfigFile());
    }
    inline bool MakeDataDir ()
    {
        if ( !wxDirExists ( DataDir () ) )
        {
           return wxMkDir ( DataDir () );
        }
        return false;
    }
    typedef std::map < wxString, wxString > SetupTable;
    class Config
    {
        public:
            bool InitConfig ();
            bool Del ( const wxString& key );
            bool Load ();
            bool Save ();
            bool HasConfigFile () const;
            bool Set ( const wxString& key, const wxString& value );
            bool Set ( const wxString& key, const double& d );
            bool Set ( const wxString& key, const long& l );
            bool Set ( const wxString& key, const int& i );
            bool Get ( const wxString& key, wxString& value );
            bool Get ( const wxString& key, double& d );
            bool Get ( const wxString& key, long& l );
            bool Get ( const wxString& key, int& i );
            void SetIdentName ( const wxString& name );
            void SetIdentEmail ( const wxString& email );
            void SetIdentSignature ( const wxString& signature );
            void SetIdent ( const Ident& id );
            wxString GetIdentName () const;
            wxString GetIdentEmail () const;
            wxString GetIdentSignature () const;
            Ident GetIdent () const;
            
            size_t CountServer () const;
            void AddServer ( const Server& server );
            Server GetServer ( const wxString& label ) const;
            Server GetServerAt ( const size_t& idx ) const;
            void GetServerAt ( const size_t& idx, Server& server ) const;
            bool DelServer ( const wxString& label );
            bool DelServer ( const Server& server );
            bool DelServerAt ( const size_t& idx );
            
            size_t CountGroup () const;
            void AddGroup ( const SubGroup& group );
            SubGroup GetGroupAt ( const size_t& idx ) const;
        protected:
            friend class DefaultConfig;
            Config ( const wxString& filename );
            ~Config ();
        private:
            void SaveTable ( TiXmlElement* root );
            void LoadTable ( TiXmlElement* root );
            void LoadIdent ( TiXmlElement* root );
            void SaveIdent ( TiXmlElement* root );
            void LoadServer ( TiXmlElement* root );
            void SaveServer ( TiXmlElement* root );
            void LoadGroup ( TiXmlElement* root );
            void SaveGroup ( TiXmlElement* root );
            void Clear ();
            wxString m_filename;
            SetupTable m_setuptable;
            Ident m_ident;
            SubGroupList m_grouplist;
            ServerList m_serverlist;
    };
    class DefaultConfig 
    {
        public:
            static Config* GetConfig ();
        private:
            DefaultConfig ();
            ~DefaultConfig ();
            static Config *m_config;
    };
};
#endif //
