#ifndef __NEXT_NINEO_SERVER_DEFINE__
#define __NEXT_NINEO_SERVER_DEFINE__


#include <wx/wx.h>
#include <wx/dialog.h>

#include <wx/socket.h>
#include "group.hpp"
#include "utils.hpp"
#include <vector>

namespace NiServer
{
    class Server
    {
        private:
            wxString m_label, m_address, m_user, m_pass;
            unsigned int m_port;
        public:
            Server ();
            Server ( const wxString& label, 
                     const wxString& address,
                     const wxString& port = wxT("119"),  
                     const wxString& user = wxEmptyString,
                     const wxString& pass = wxEmptyString );

            Server ( const wxString& label,     
                     const wxString& address,
                     const unsigned int& port = 119,   
                     const wxString& user = wxEmptyString,
                     const wxString& pass = wxEmptyString  );
            ~Server ();
            void SetLabel ( const wxString& label );
            wxString GetLabel () const;
            void SetAddress ( const wxString& address );
            wxString GetAddress () const;
            void SetPort ( const wxString& port );
            void SetPort ( const unsigned int& port );
            unsigned int GetPort () const;
            void SetUsername ( const wxString& user );
            wxString GetUsername () const;
            void SetPassword ( const wxString& pass );
            wxString GetPassword () const;
            NiUtils::HASH Hash () const;
#if 0
            bool operator== ( const Server& server );
#endif
#if 0
            bool operator== ( const NiGroup::SubGroup& group ) const;
#endif
            wxIPV4address GetIPV4 () const;
            bool RequireAuthinfo () const;
    };
    typedef std::vector < Server > ServerList;
    typedef std::vector < Server* > ServerPtrList;

    struct OF_MATCH_SERVER
    {
        private:
            wxString m_value;
        public:
            OF_MATCH_SERVER ( const wxString& value )
                : m_value (value)
            {}
            bool operator () ( ServerList::value_type& value )
            {
                return ( m_value == value.GetAddress () || m_value == value.GetLabel() );
            }
    };
    class NiServerDialog : public wxDialog
    {
        public:
            NiServerDialog ( wxWindow* parent, wxWindowID id, Server& server,
                             const wxString& title = _("Add server") );
            ~NiServerDialog ();
        private:
            enum {
                ID_LABEL,
                ID_ADDRESS,
                ID_LABEL_USER,
                ID_USER,
                ID_LABEL_PASS,
                ID_PASS,
                ID_PORT,
                ID_LOGIN
            };
            void OnCheckbox ( wxCommandEvent& event );
            void OnEnter ( wxCommandEvent& event );
            void OnOk ( wxCommandEvent& event );
            void DoUI ();
            void Checkbox ( const bool& check );
            bool CheckInput ();
            Server& m_server;
            DECLARE_EVENT_TABLE ();
    };
}
#endif //


