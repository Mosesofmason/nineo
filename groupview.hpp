#ifndef __NEXT_NINEO_GROUPVIEW_DEFINE__
#define __NEXT_NINEO_GROUPVIEW_DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "config.hpp"
#include <wx/treectrl.h>
#include "server.hpp"

class NiGroupView: public wxTreeCtrl
{
    public:
        NiGroupView ( wxWindow* parent, wxWindowID id, 
                const long& style = NStyle|
                wxTR_HAS_BUTTONS|
                wxTR_FULL_ROW_HIGHLIGHT|
                wxTR_ROW_LINES, 
                const wxString& name = wxT("GroupView") );
        bool LoadConfig ();
        bool AddNewServer ();
        bool HasServer ( const NiServer::Server& server );
        bool UpdateGrouplist ( const int& pos = -1 );
        bool AddGroup ( const NiGroup::SubGroup& group );
        bool HasGroup ( const NiGroup::SubGroup& group );
        bool GetGroup ( const wxTreeItemId& id, NiGroup::SubGroup& group );
        void ItemMenu ();
        void SyncAll ();
        ~NiGroupView ();
    private:
        bool HasGroup ( const NiGroup::SubGroup* group ) const;
        bool AddServer ( const NiServer::Server& server );
        bool AddNewServer ( const NiServer::Server& server );
        NiServer::ServerPtrList m_serverlist;       
        NiGroup::SubGroupPtrList m_grouplist;
        DECLARE_EVENT_TABLE ()
};
#endif //
