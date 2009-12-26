#ifndef __NEXT_NINEO_VISUAL_LIST_DEFINE__
#define __NEXT_NINEO_VISUAL_LIST_DEFINE__
#include <wx/wx.h>
#include "config.hpp"

class wxVisualList : public wxPanel
{
    public:
        wxVisualList ( wxWindow* parent, wxWindowID id, long style = NStyle,
                       const wxString& name = wxT("VisualList"));
        ~wxVisualList ();
        void SetCount ( const size_t& count );
        void AddColumn ( const wxString& label, const size_t& width );
    protected:
        friend class wxVList;
        friend class Header;
        void OnMouse ( wxMouseEvent& event );
        virtual void OnDrawLine ( wxDC& dc, const size_t& line, const size_t& col, 
                                  const wxRect& rect );
    private:
        void DoUI ();
        void CalcScroll ();
        void OnResize ( wxSizeEvent& event );
        void OnPaint ( wxPaintEvent& event );
        void OnScroll ( wxScrollEvent& event );
        size_t GetLineHeight () const;
        inline wxRect CalcLineRect ( const wxSize& client, const size_t& line,
                                     const size_t& lineheight ) const;
        void DrawSelectBox ( wxDC& dc, const wxRect& rect );
        int m_select;
        size_t m_count;
        int m_position;
        enum
        {
            ID_VSCROLL,
            ID_HEADER,
            ID_LIST
        };
        wxPoint m_mouse;
        DECLARE_EVENT_TABLE ()
};
#endif //


