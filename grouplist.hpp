#ifndef __NEXT_NINEO_GROUPLIST_DEFINE__
#define __NEXT_NINEO_GROUPLIST_DEFINE__ 
#include <wx/wx.h>
#include "config.hpp"
#include "activedb.hpp"

namespace NiGroup
{
    class GrouplistDialog : public wxDialog
    {
        public:
            GrouplistDialog ( wxWindow* parent, wxWindowID id, 
                              const wxString& title = _("newsgroup list") );
            ~GrouplistDialog ();
        private:
            void DoUI ();
            void LoadServer ();
            void LoadClass ( const size_t& idx );
            void LoadGroup ();
            void OnServer ( wxCommandEvent& event );
            void OnClass ( wxCommandEvent& event );
            void OnGroupSel ( wxListEvent& event );
            void OnSubject ( wxCommandEvent& event );
            enum {
                ID_SERVER_CHOICE,
                ID_CLASS_CHOICE,
                ID_GROUP_LIST,
                ID_SUBJECT
            };
            NiDB::ClassList m_classlist;
            int m_index;
            DECLARE_EVENT_TABLE ()
    };
};
#endif //


