////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////
#include "summaryview.hpp"
#include <wx/dynarray.h>
#include <wx/arrimpl.cpp>
#include <wx/settings.h>
#include <wx/vlbox.h>
#include <wx/font.h>
#include "log.hpp"

BEGIN_EVENT_TABLE (NiSummaryView, wxVisualList)
END_EVENT_TABLE ()
NiSummaryView::NiSummaryView ( wxWindow* parent, wxWindowID id )
    : wxVisualList ( parent, id ), m_db (NULL)
{
   AddColumn (_("ID"), 20);
   AddColumn (_("Subject"), 100 );
   AddColumn (_("From"), 80 );
}

NiSummaryView::~NiSummaryView ()
{}
void NiSummaryView::OnDrawLine ( wxDC& dc,  const size_t& line, const size_t& col, 
                                 const wxRect& rect ) 
{
    wxDCClipper clip (dc, rect.x, rect.y, rect.width - 1, rect.height);
    wxString text, label;
    if ( m_db != NULL )
    {
        NiDB::ArticleDB::FieldType type;
        switch ( col )
        {
            case 0 : type = NiDB::ArticleDB::ID; break;
            case 1 : type = NiDB::ArticleDB::SUBJECT; break;
            case 2 : type = NiDB::ArticleDB::FROM; break;
            default: type = NiDB::ArticleDB::ID; break;
        };
        text = m_db->GetField (line, type);
        if ( type == NiDB::ArticleDB::SUBJECT )
        {
            size_t depth = m_db->Depth ( line );
            wxString space ( wxT(' '), depth+1);
            label += space;
        }
    }
    label += wxString ( text.To8BitData().data(), wxCSConv (wxT("GB2312")) );
    dc.DrawText (label, rect.x, rect.y);
}
bool NiSummaryView::Load ( const NiGroup::SubGroup& group )
{
    NiUtils::Loger *loger = NiUtils::GetLoger ();
    m_db = NiDB::ArticleDBManager::GetDB (group);
    if ( m_db != NULL )
    {
        SetCount ( m_db->Count () );
        loger->Message (wxString::Format (wxT("Load %s, node %d"),
                                          group.Name().c_str(),
                                          m_db->Count ()));
    }
    return true;
}
NiTreeListColumn::NiTreeListColumn ( const wxString& label, const int& width, const bool& show )
  : m_label (label), m_width(width), m_show(show), x(0), y(0), w(0), h(0)
{}

NiTreeListColumn::NiTreeListColumn ( const NiTreeListColumn& sc )
{
  m_label = sc.m_label;
  m_width = sc.m_width;
  m_show = sc.m_width;
  y = sc.y;
  x = sc.x;
  w = sc.w;
  h = sc.h;
}

NiTreeListColumn::~NiTreeListColumn ()
{}

wxString NiTreeListColumn::GetLabel ()
{
  return m_label;
}

int NiTreeListColumn::GetWidth ()
{
  return m_width;
}

bool NiTreeListColumn::IsShow ()
{
  return m_show;
}

void NiTreeListColumn::SetCoord ( const int& ox, const int& oy, const int& ow, const int& oh )
{
  x = ox;
  y = oy;
  w = ow;
  h = oh;
}

void NiTreeListColumn::GetCoord ( int& ox, int& oy, int& ow, int& oh )
{
  ox = x;
  oy = y;
  ow = w;
  oh = h;
}

void NiTreeListColumn::SetWidth ( const int& width )
{
  m_width = width;
}

void NiTreeListColumn::SetLabel ( const wxString& label )
{
  m_label = label;
}
//WX_DEFINE_ARRAY ( NiTreeListColumn, NiTreeListColumnArray );

WX_DECLARE_OBJARRAY(NiTreeListColumn, NiTreeListColumnArray);
WX_DEFINE_OBJARRAY(NiTreeListColumnArray );

////////////////////////////////////////////////////////////////////////////////
class NiTreeListHeader : public wxWindow
{
public:
  NiTreeListHeader ();
  NiTreeListHeader ( wxWindow* parent, wxWindowID id, const long& style = wxBORDER_NONE, const wxString& name = wxT("NiTreeListHeader"));
  ~NiTreeListHeader ()
  {
    if ( m_resize_cursor ) delete m_resize_cursor;
  }
  void SetOwner ( NiTreeListCtrl* owner );
  bool AddColumn ( NiTreeListColumn& col )
  {
    m_columns.Add ( col );
    m_total_width += col.GetWidth ();
    return true;
  }
  bool DelColumn ( const size_t& idx )
  {
    if ( idx < 0 || idx >= m_columns.GetCount () ) return false;
    m_total_width -= m_columns[idx].GetWidth ();
    m_columns.RemoveAt (idx);
    return true;
  }

  int GetColumnCount ()
  {
    return m_columns.GetCount ();
  }
  NiTreeListColumn& GetColumn ( const size_t& idx )
  {
    wxASSERT ( idx >= 0 && idx < m_columns.GetCount () );
    return m_columns.Item (idx);
  }
  int GetsTotalWidth ()
  {
    return m_total_width;
  }
  
private:
  void DrawCurrent ();
  void DrawRect ( wxDC& dc, const int& x, const& y, const int& w, const int& h );
  
  void OnSize ( wxSizeEvent& event );
  void OnMouse ( wxMouseEvent& event );
  void OnPaint ( wxPaintEvent& event );
  //  void OnSetFocus ( wxFocusEvent& event );
  DECLARE_DYNAMIC_CLASS ( NiTreeListHeader )
  DECLARE_EVENT_TABLE ()
private:
  NiTreeListColumnArray m_columns;
  NiTreeListCtrl* m_owner;
  int m_total_width, m_min_x, m_current_x, m_column, m_headerheight;
  bool m_dragging;
  const wxCursor *m_resize_cursor, *m_current_cursor;
};

NiTreeListHeader::NiTreeListHeader ()
: m_owner (NULL), m_headerheight (0)
{}

NiTreeListHeader::NiTreeListHeader ( wxWindow *parent, wxWindowID id, const long& style, const wxString& name)
  :wxWindow ( parent, id, wxDefaultPosition, wxDefaultSize, style, name ),
   m_owner (NULL), m_headerheight (0), m_total_width(0), m_min_x (0), m_dragging(false), m_current_cursor (NULL),
   m_column (0)
{
  wxFont font = wxSystemSettings::GetFont ( wxSYS_SYSTEM_FONT );
  font.SetWeight (wxFONTWEIGHT_BOLD);
  SetFont (m_font);
  m_resize_cursor = new wxCursor ( wxCURSOR_SIZEWE );
}

void NiTreeListHeader::SetOwner ( NiTreeListCtrl* owner )
{
  m_owner = owner;
}

void NiTreeListHeader::DrawCurrent ()
{
  int x1 = m_current_x;
  int y1 = 0;
  ClientToScreen (&x1, &y1);

  int x2 = m_current_x-1;
#ifdef __WXMSW__
  ++x2; // but why ????
#endif
  int y2 = 0;
  if ( m_owner )
    {
      m_owner->GetClientSize( NULL, &y2 );
      m_owner->ClientToScreen( &x2, &y2 );
    }

  wxScreenDC dc;
  dc.SetLogicalFunction (wxINVERT);
  dc.SetPen (wxPen (*wxBLACK, 1, wxUSER_DASH));
  dc.SetBrush (*wxTRANSPARENT_BRUSH);
  
  dc.DrawLine (x1, y1, x2, y2);
  dc.SetLogicalFunction (wxCOPY);
  dc.SetPen (wxNullPen);
  dc.SetBrush (wxNullBrush);
}
// void KvTreeListHeader::SetupSize ( const wxSize& psize )
// {
//   int w, h, d;
//   this->GetTextExtent (wxT("Hg"), &w, &h, &d);
//   h += d+2;
//   m_headerwidth = w;
//   if ( h != m_headerheight ) m_headerheight = h;
//   int cal_width = w / m_columns.size ();
//   //m_minwidth = cal_width < 100 ? 100 : cal_width;
//   this->SetSize (wxSize(psize.GetWidth (), m_headerheight));
//   this->Refresh ();
// }
void NiTreeListHeader::DrawRect ( wxDC& dc, const int& x, const int& y, const int& w, const int& h )
{
#if !wxCHECK_VERSION(2, 5, 0)
  wxPen pen (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_BTNSHADOW ), 1, wxSOLID);
#else
  wxPen pen (wxSystemSettings::GetColour (wxSYS_COLOUR_BTNSHADOW ), 1, wxSOLID);
#endif
  const int m_corner = 1;
  dc.SetBrush( *wxTRANSPARENT_BRUSH );
#if defined( __WXMAC__  )
  dc.SetPen (pen);
#else // !GTK, !Mac
  wxPen black_pen = *wxBLACK_PEN;
  black_pen.SetWidth (1);
  dc.SetPen( black_pen );
#endif
  //dc.DrawLine( x+w-m_corner+1, y, x+w, y+h );  // right (outer)
  dc.DrawLine( x+w, y, x+w, y+h );
  //dc.DrawRectangle( x, y+h, w, 1 );          // bottom (outer)

#if defined( __WXMAC__  )
  wxPen pen( wxColour( 0x88 , 0x88 , 0x88 ), 1, wxSOLID );
#endif
  dc.SetPen( pen );
  dc.DrawLine( x+w-1 , y, x+w-1 , y+h ); 
  dc.DrawRectangle( x, y+h, w , 1 );      // bottom (inner)

  dc.SetPen( *wxWHITE_PEN );
  dc.DrawRectangle( x , y, w, 1 );   // top (outer)
  dc.DrawLine ( x, y, x, y+h );
}
void NiTreeListHeader::OnPaint ( wxPaintEvent& WXUNUSED(event) )
{
#ifdef __WXGTK__
  wxClientDC dc( this );
#else
  wxPaintDC dc( this );
#endif

  PrepareDC( dc );
  dc.SetFont( GetFont() );

  // width and height of the entire header window
  int w, h;
  this->GetClientSize( &w, &h );
    
  int column = 4;
  dc.SetBackgroundMode(wxTRANSPARENT);
    
  // do *not* use the listctrl colour for headers - one day we will have a
  // function to set it separately
  dc.SetTextForeground( *wxBLACK );
    
  bool is_more = m_total_width >= w ? true : false;
  int pos = 0, col_width = 0, col_count = 0;
  wxString StrCoord;
  col_count = m_columns.GetCount ();
  if ( col_count )
  {
      for ( int col_pos  = 0; col_pos < col_count; ++col_pos )
      {
          if ( !m_columns.Item (col_pos).IsShow ()) continue;
          col_width = ( col_pos == ( col_count - 1 ) ? w - pos : m_columns.Item(col_pos).GetWidth());
          if ( col_width + pos >= w ) --col_width;
          DrawRect (dc, pos, 0, col_width, h - 1);
          int text_wdith = 2, text_x = pos;
          wxDCClipper clipper ( dc, text_x, 0, col_width, h );
          dc.DrawText ( m_columns[col_pos].GetLabel (), text_x + 5, 1 );
          m_columns[col_pos].SetCoord (pos, 0, col_width, h);
          m_columns[col_pos].SetWidth ( col_width );
          pos += col_width;
          if ( pos >= w ) break;
      }
  }
// //       KvTreeListColumnTable::iterator col_itr, last_itr;
// //       last_itr = ( m_columns.size () >= 2 ? m_columns.end() -1 : m_columns.end () );
// //       for ( col_itr = m_columns.begin(); col_itr < m_columns.end(); ++col_itr )
// // 	{
// // 	  if ( !col_itr->IsShow () ) continue;
// // 	  col_width = ( col_itr == last_itr ? w - pos : col_itr->GetWidth () );
// // 	  if ( col_width + pos >= w ) --col_width;
// // 	  this->DrawRect (dc, pos, 0, col_width, h - 1);
// // 	  int text_width = 2, text_x = pos;
// // 	  wxDCClipper clipper (dc, text_x, 0, col_width, h);
// // 	  dc.DrawText (col_itr->GetText (), text_x + 5, 1);
// // 	  col_itr->SetCoord ( pos, 0, col_width, h );
// // 	  col_itr->SetWidth ( col_width );
// // 	  pos += col_width;
// // 	  if ( pos >= w ) break;
// 	}
  else
    {
      DrawRect ( dc, 0, 0, w - 1, h - 1 );
    }
  dc.SetPen ( *wxBLACK_PEN );
  dc.DrawLine (0, h, w, h );
}
void NiTreeListHeader::OnMouse ( wxMouseEvent& event )
{
    long x, y;
    int w, h;
    event.GetPosition (&x, &y);

    GetClientSize (&w, &h);

    m_total_width = w;
    if ( x ==  0 ) event.Skip ();
    if ( m_dragging )
    {

        if ( m_current_x < w )
            this->DrawCurrent ();

        if ( event.ButtonUp () )
        {
            m_dragging = false;
            if ( HasCapture() ) ReleaseMouse ();
            m_columns[m_column].SetWidth (m_current_x - m_min_x);
            this->Refresh ();
            if ( m_owner )
                m_owner->DoUpdateUI ();
            //SendListEvent (wxEVT_COMMAND_LIST_COL_END_DRAG, event.GetPosition());
        }
        else
        {
            m_current_x = wxMax ( m_min_x + 7, x );
            if ( m_current_x < w ) this->DrawCurrent ();
        }
    }
    else
    {
        bool hit_border = false;
        m_min_x = 0;
        int xpos, col_pos = 0, col_count = m_columns.GetCount ();
        int cx, cy, cw, ch;

        for ( col_pos = 0; col_pos < col_count; ++col_pos )
        {
            if ( !m_columns[col_pos].IsShow () ) continue;
            m_columns[col_pos].GetCoord (cx, cy, cw, ch);
            xpos  =  cx + cw;
            m_column = col_pos;
            if ( abs(x-xpos) < 3 && abs(x-w) > 3)
            {
                hit_border = true;
                break;
            }
            if ( x < xpos ) break;
            m_min_x = xpos;
        }
        if ( event.LeftDown () || event.RightUp () )
        {
            if ( hit_border && event.LeftDown () )
            {
                m_dragging = true;
                CaptureMouse ();
                m_current_x = x;
                DrawCurrent();
            }
        }
        else if ( event.LeftDClick () && hit_border )
        {
            // Setup best column size!!!
        }
        else if ( event.Moving () )
        {
            bool set_cursor;
            if ( hit_border )
            {
                set_cursor = m_current_cursor == wxSTANDARD_CURSOR;
                m_current_cursor = m_resize_cursor;
            }
            else
            {
                // click on column.
                set_cursor = m_current_cursor != wxSTANDARD_CURSOR;
                m_current_cursor = wxSTANDARD_CURSOR;
            }
            if ( set_cursor ) this->SetCursor (*m_current_cursor );
        }
    }
}
void NiTreeListHeader::OnSize ( wxSizeEvent& event )
{}

////////////////////////////////////////////////////////////////////////////////
//////////////////// NiTreeList
class NiTreeList : public wxVListBox
{
public:
  NiTreeList ( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& name = wxT("NiTreeList"));
  ~NiTreeList ();
  virtual void OnDrawItem ( wxDC& dc, const wxRect& rect, size_t n ) const;
  virtual wxCoord OnMeasureItem ( size_t n ) const;
  DECLARE_EVENT_TABLE()
  //  DECLARE_DYNAMIC_CLASS ( NiTreeList );
};

NiTreeList::NiTreeList ( wxWindow* parent, wxWindowID id, const wxString& name )
{
  Create ( parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE , name );
}
void NiTreeList::OnDrawItem ( wxDC& dc, const wxRect& rect, size_t n ) const
{}
wxCoord NiTreeList::OnMeasureItem ( size_t n ) const
{
  wxCoord cd;
  return cd;
}
NiTreeList::~NiTreeList ()
{}

////////////////////////////////////////////////////////////////////////////////

void NiTreeListCtrl::UI ()
{
  m_header = new NiTreeListHeader ( this, wxID_ANY, NStyle, wxT("NiTreeListHeader"));
  if ( m_header ) 
    {
      m_header->SetOwner (this);
      m_header->AddColumn ( NiTreeListColumn ( wxT("Subject"), 150 ) );
      m_header->AddColumn ( NiTreeListColumn ( wxT("Author"), 150 ) );
    }
  m_list = new NiTreeList ( this, wxID_ANY, wxT("List") );
}


void NiTreeListCtrl::Refresh ( bool erase, const wxRect* rect )
{
  if ( m_header ) m_header->Refresh (erase, rect);
  if ( m_list ) m_list->Refresh (erase, rect);
}
void NiTreeListCtrl::DoUpdateUI ()
{
  //Refresh();
}

void NiTreeListCtrl::OnSize ( wxSizeEvent& event )
{
  wxSize csize = this->GetClientSize ();
  int w, h, d;
  if ( m_header && m_list )
    {
      m_header->GetTextExtent ( wxT("Hg"), &w, &h, &d );
      h += d + 2;
      wxSize hsize (csize.GetWidth(), h), lsize (csize.GetWidth(), csize.GetHeight() - h );
      m_header->SetSize ( 0, 0, hsize.GetWidth(), hsize.GetHeight() );
      m_list->SetSize ( 0 , h , lsize.GetWidth(), lsize.GetHeight() );
      m_header->Refresh ();
      m_list->Refresh ();			 
    }
}

IMPLEMENT_DYNAMIC_CLASS(NiTreeListHeader, wxWindow)
BEGIN_EVENT_TABLE(NiTreeListHeader,wxWindow)
EVT_PAINT (NiTreeListHeader::OnPaint)
EVT_MOUSE_EVENTS (NiTreeListHeader::OnMouse)
//EVT_SET_FOCUS (NiTreeListHeader::OnSetFocus)
EVT_SIZE (NiTreeListHeader::OnSize)
END_EVENT_TABLE()

//IMPLEMENT_DYNAMIC_CLASS(NiTreeList, wxVListBox)
BEGIN_EVENT_TABLE(NiTreeList, wxVListBox)
END_EVENT_TABLE()


IMPLEMENT_DYNAMIC_CLASS(NiTreeListCtrl, wxControl);
BEGIN_EVENT_TABLE ( NiTreeListCtrl, wxWindow )
EVT_SIZE ( NiTreeListCtrl::OnSize )
END_EVENT_TABLE ()
