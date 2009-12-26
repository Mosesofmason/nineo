#ifndef __NEXT_NINEO_SUMMARYVIEW_DEFINE__
#define __NEXT_NINEO_SUMMARYVIEW_DEFINE__
////////////////////////////////////////////////////////////////////////////////
//////////                     Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "config.hpp"
#include <wx/dynarray.h> 
#include <wx/sashwin.h>
#include "vlist.hpp"
#include "group.hpp"
#include "articledb.hpp"

class NiSummaryView : public wxVisualList
{
    public:
        NiSummaryView ( wxWindow* parent, wxWindowID id );
        ~NiSummaryView ();
        bool Load ( const NiGroup::SubGroup& group );
        virtual void OnDrawLine ( wxDC& dc, const size_t& line, const size_t& col,
                                  const wxRect& rect );
    private:
        NiDB::ArticleDB *m_db;
        DECLARE_EVENT_TABLE ()
};

const int DEFAULT_COL_WIDTH = 100;

class NiTreeListHeader;
class NiTreeList;

class NiTreeListColumn : public wxObject
{
private:
  wxString m_label;
  int m_width, x, y, w, h;
  bool m_show;
public:
  NiTreeListColumn ( const wxString& label, const int& width = DEFAULT_COL_WIDTH, const bool& show = true );
  NiTreeListColumn ( const NiTreeListColumn& sc );
  ~NiTreeListColumn ();
  void SetWidth ( const int& width );
  void SetLabel ( const wxString& label );

  wxString GetLabel ();
  int GetWidth ();
  bool IsShow ();
  void SetCoord ( const int& ox, const int& oy, const int& ow, const int& oh );
  void GetCoord ( int& ox, int& oy, int& ow, int& oh );
};

class NiTreeListCtrl: public wxControl
{
    public:
        NiTreeListCtrl ()
            : m_header (NULL), m_list(NULL)
        {}
        NiTreeListCtrl ( wxWindow *parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = NStyle,
                const wxValidator &validator = wxDefaultValidator,
                const wxString& name = wxT("TreeList"))
            : m_header (NULL), m_list (NULL)
        {
            Create (parent, id, pos, size, style, validator, name);
#if 0
            const wxSize csize(480,380);
            SetVirtualSize(csize);
            SetMinSize ( csize );
            SetSize (csize);
            SetClientSize (csize);
#endif
            UI ();
        }
        virtual ~NiTreeListCtrl ()
        {
        };
        virtual void Refresh (bool erase, const wxRect* rect);

    protected:
        friend class NiTreeList;
        friend class NiTreeListHeader;

        NiTreeListHeader* m_header;
        NiTreeList* m_list;

        void DoUpdateUI ();

    private:
        void UI ();
        void OnSize (wxSizeEvent& event);
        DECLARE_EVENT_TABLE()
            DECLARE_DYNAMIC_CLASS(NiTreeListCtrl)
};
#endif //
