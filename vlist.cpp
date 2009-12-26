////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "vlist.hpp"

class Column
{
    public:
        Column ();
        Column ( const wxString& label, const size_t& width = 200 );
        ~Column ();        
        size_t GetWidth () const;
        void SetWidth ( const size_t& width );
        virtual void Draw ( wxDC& dc, const wxRect& rect );
        Column& operator+= ( const size_t& width );
        Column& operator-= ( const size_t& width );
        Column& operator++ ();
        Column& operator-- ();
    private:
        size_t m_width;
        wxString m_label;
};
Column::Column ()
{}
Column::Column ( const wxString& label, const size_t& width )
    : m_label (label), m_width (width)
{}
Column& Column::operator+= ( const size_t& width )
{
    m_width += width;
    return *this;
}
Column& Column::operator-= ( const size_t& width )
{
    m_width = ( m_width - width ) > 50 ? m_width - width : 50 ;
    return *this;
}
Column& Column::operator++ ()
{
    ++m_width;
    return *this;
}
Column& Column::operator-- ()
{
    if ( m_width >=50 )
        --m_width;
    return *this;
}
size_t Column::GetWidth () const
{
    return m_width;
}
void Column::SetWidth ( const size_t& width )
{
    m_width = width;
}
void Column::Draw ( wxDC& dc, const wxRect& rect )
{
    dc.DrawText ( m_label, rect.x, rect.y );
}
Column::~Column ()
{}

class Header : public wxWindow
{
    public:
        Header ( wxWindow* parent, wxWindowID id );
        ~Header ();
        void AddColumn ( Column* col );
        size_t Count () const;
        size_t GetWidth ( const size_t& idx ) const;
    private:
        int HitBorder ( const wxPoint& point );
        int InsideCol ( const wxPoint& point );
        int WidthFromBegin ( const int& col );
        void DrawCurrent ();
        void OnMouse ( wxMouseEvent& event );
        void OnPaint ( wxPaintEvent& event );
        void DrawRectangle ( wxDC& dc, const wxRect& rect );
        typedef std::vector < Column* > Columns;
        Columns m_cols;
        bool m_dragging;
        int m_dragcurrent;
        int m_dragpoint;
        DECLARE_EVENT_TABLE ()
};
BEGIN_EVENT_TABLE ( Header, wxWindow )
    EVT_MOUSE_EVENTS    ( Header::OnMouse )
    EVT_PAINT           ( Header::OnPaint )
END_EVENT_TABLE ()    
Header::Header ( wxWindow* parent, wxWindowID id ) 
    : wxWindow ( parent, id, wxDefaultPosition, wxDefaultSize, 0, wxT("Header") ),
      m_dragging (false), m_dragcurrent (-1)
{
    wxFont font = GetFont ();
    SetBackgroundColour ( wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE) );
    if ( font.IsOk () )
    {
        SetClientSize (wxSize (-1, font.GetPointSize() + 10 ));
    }
}
size_t Header::Count () const
{
    return m_cols.size ();
}
size_t Header::GetWidth ( const size_t& idx ) const
{
    wxASSERT ( idx >= 0 && idx < m_cols.size () );
    return m_cols[idx]->GetWidth ();
}
int Header::WidthFromBegin ( const int& col )
{
    int pos = 0, count = m_cols.size (), width = 0;
    for ( ; pos < count && pos != col; ++pos )
    {
        width += (*m_cols[pos]).GetWidth();
    }
    return width;
}
int Header::HitBorder ( const wxPoint& point )
{
    int pos = 0, count = m_cols.size () - 1, ret = -1, width = 0;
    for ( ; pos < count; ++pos )
    {
        width += (*m_cols[pos]).GetWidth ();
        if ( point.x == width - 1 || point.x == width )
        {
            ret = pos;
            break;
        }
    }
    return ret;
}
void Header::OnMouse ( wxMouseEvent& event )
{
    int cwidth = 0, bwidth = 0;
    GetClientSize   ( &cwidth, NULL );
    wxPoint point = event.GetPosition ();

    if ( m_dragging )
    {
        DrawCurrent (); //!  这一行必须加上，不然鼠标拖动的时候，屏幕会花掉。

        if ( event.ButtonUp () )
        {
            m_dragging = false;
            m_dragcurrent = -1;
            wxWindow *parent = GetParent();
            wxASSERT ( parent != NULL );
            parent->Refresh ();
         }
        else if ( point.x <= cwidth  )
        {
            Column *col = m_cols[m_dragcurrent];
            size_t x = 0, total_width = 0;
            total_width = WidthFromBegin ( m_dragcurrent ) + 50;
            if ( point.x > total_width )
            {
                x = m_dragpoint - point.x;
                if ( x > 0 )
                {
                    (*col) -= x;
                    m_dragpoint = point.x;
                }
                else if ( x < 0 )
                {
                    (*col) += (-x);
                    m_dragpoint = point.x;
                }
            }
        }
        Refresh ();
        DrawCurrent ();
    }
    else
    {
        int fp = HitBorder (point);
        if ( fp != -1 && event.LeftDown () )
        {
            m_dragcurrent = fp;
            m_dragging = true;
            m_dragpoint = point.x;
            DrawCurrent ();
        }
        else 
        {
            SetCursor (wxCursor ( fp == -1 ? wxCURSOR_ARROW : wxCURSOR_SIZEWE));
        }
    }
}
void Header::DrawCurrent ()
{
    wxVisualList* parent = static_cast <wxVisualList*> (GetParent ());
    wxASSERT ( parent != NULL );

    int sx = 0, sy = 0, ex = 0, ey = 0;
    ex = sx = m_dragpoint;
    
    GetClientSize           ( NULL, &sy );
    ClientToScreen          ( NULL, &sy );
    parent->GetClientSize   ( NULL, &ey );
    parent->ClientToScreen  ( &ex,  &ey );
    sx = ex;

    wxScreenDC dc;
    dc.SetLogicalFunction (wxINVERT);
    dc.SetPen (wxPen (*wxBLACK, 1, wxDOT));
    dc.SetBrush (*wxTRANSPARENT_BRUSH);
    dc.DrawLine (sx, sy, ex, ey);
    dc.SetLogicalFunction (wxCOPY);
    dc.SetPen (wxNullPen);
    dc.SetBrush (wxNullBrush);
}
void Header::OnPaint ( wxPaintEvent& event )
{
    wxPaintDC   dc (this);
    wxSize      size = GetClientSize ();
    dc.SetFont (GetFont ());

    if ( m_cols.empty () )
    {
        DrawRectangle (dc, wxRect ( 0, 0, size.GetWidth(), size.GetHeight()));
    }
    else
    {
        wxRect  rect ( 0, 0, 0, size.GetHeight () ), textrect;
        Columns::iterator itr;
        for ( itr = m_cols.begin(); itr < m_cols.end(); ++itr )
        {
            if ( (*itr)->GetWidth () <= 0 ) continue;
            rect.width = ( itr+1 != m_cols.end() ? (*itr)->GetWidth() :
                           size.GetWidth() - rect.width );
            textrect.x = rect.x + 5;
            textrect.y = 2;
            textrect.width = (*itr)->GetWidth ();
            textrect.height = rect.height;
            DrawRectangle ( dc, rect );
            (*itr)->Draw (dc, textrect );
            rect.x += (*itr)->GetWidth();
            if ( rect.x >= size.GetWidth() ) 
            {
                break;
            }
        }
    }
}
void Header::DrawRectangle ( wxDC& dc, const wxRect& rect  )
{
    wxPen   op, np, hl, shadow;
    wxBrush ob, nb;
    op      = dc.GetPen ();
    ob      = dc.GetBrush ();
    np      = wxPen     (*wxBLACK);
    nb      = wxBrush   (wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    hl      = wxPen     (wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT));
    shadow  = wxPen     (wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW)); 
    
    dc.SetPen   (hl);
    dc.DrawLine ( rect.x, rect.y , rect.x + rect.width, rect.y );

    dc.SetBrush (nb);
    dc.SetPen (np);

    dc.DrawLine ( rect.x, rect.height - 1, rect.x + rect.width, rect.height - 1);
    dc.DrawLine ( rect.x, rect.y, rect.x, rect.height );

    dc.SetPen   (op);
    dc.SetBrush (ob);
}
void Header::AddColumn ( Column* col ) 
{
    m_cols.push_back ( col );
}
Header::~Header ()
{
    Columns::iterator itr;
    for ( itr = m_cols.begin(); itr < m_cols.end (); ++itr )
    {
        delete (*itr);
        *itr = NULL;
    }
    m_cols.clear ();
}
class wxVList : public wxWindow
{
    public:
        wxVList ( wxWindow *parent, wxWindowID id );
        ~wxVList ();
    private:
        wxPoint m_mouse;
        void OnMouse ( wxMouseEvent& event );
        void OnPaint ( wxPaintEvent& event );
        DECLARE_EVENT_TABLE ()
};
BEGIN_EVENT_TABLE ( wxVList, wxWindow )
    EVT_MOUSE_EVENTS ( wxVList::OnMouse ) 
    EVT_PAINT ( wxVList::OnPaint )
END_EVENT_TABLE ()

wxVList::wxVList ( wxWindow *parent, wxWindowID id )
    : wxWindow ( parent, id, wxDefaultPosition, wxDefaultSize, 0, wxT("wxVListWindow"))
{}
wxVList::~wxVList ()
{}
void wxVList::OnMouse ( wxMouseEvent& event )
{
    wxVisualList *parent = static_cast <wxVisualList*> ( GetParent () );
    wxASSERT ( parent != NULL );
    parent->OnMouse (event);
    event.Skip ();
}
void wxVList::OnPaint ( wxPaintEvent& event )
{
}
BEGIN_EVENT_TABLE ( wxVisualList, wxPanel )
    EVT_SIZE        ( wxVisualList::OnResize )    
    EVT_PAINT       ( wxVisualList::OnPaint )
    EVT_SCROLL      ( wxVisualList::OnScroll )
    EVT_MOUSE_EVENTS( wxVisualList::OnMouse )
END_EVENT_TABLE ()
wxVisualList::wxVisualList ( wxWindow* parent, wxWindowID id, long style, const wxString& name  )
    : wxPanel ( parent, id, wxDefaultPosition, wxDefaultSize, style, name ),
      m_count (0), m_select (-1), m_position (0)
{
    DoUI ();
}
void wxVisualList::DoUI ()
{
    wxBoxSizer *main, *body;
    main = new wxBoxSizer (wxVERTICAL);
    body = new wxBoxSizer (wxHORIZONTAL);
    this->SetSizer (main);
    wxVList* list = new wxVList( this, ID_LIST );
    wxScrollBar *bar = new wxScrollBar ( this, ID_VSCROLL, 
                                         wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL);
    Header *header = new Header ( this, ID_HEADER );
    wxASSERT ( list != NULL && bar != NULL && header != NULL );
    list->SetBackgroundColour ( wxSystemSettings::GetColour (wxSYS_COLOUR_WINDOW));
    bar->Enable (false);
    
    wxFont flist, fheader;
    flist   =   *wxNORMAL_FONT;
        //wxSystemSettings::GetFont (wxSYS_SYSTEM_FONT);
    fheader =   wxSystemSettings::GetFont (wxSYS_DEFAULT_GUI_FONT);

    list->SetFont       (flist);
    header->SetFont     (fheader);

    body->Add ( list, 0, wxEXPAND, 0 );
    body->Add ( bar, 0, wxALL, 0 );
    main->Add ( header, 0, wxALL, 0 );
    main->Add ( body, 0, wxALL, 0 );
    main->SetSizeHints (this);
    SetAutoLayout (true);
}
void wxVisualList::SetCount ( const size_t& count )
{
    m_count = count;
    m_select = -1;
    m_position = 0;
    CalcScroll ();
    Refresh ();
}
void wxVisualList::AddColumn ( const wxString& label, const size_t& width )
{
    Header* header = static_cast <Header*> ( FindWindow (ID_HEADER) );
    wxASSERT ( header != NULL );
    header->AddColumn ( new Column (label, width) );
}
void wxVisualList::CalcScroll ()
{
    wxWindow *list = FindWindow ( ID_LIST );
    wxScrollBar *vscroll = static_cast <wxScrollBar*> (FindWindow(ID_VSCROLL));
    wxASSERT ( list != NULL && vscroll != NULL );
    wxSize lsize = list->GetClientSize ();
    size_t linecount = lsize.GetHeight () / GetLineHeight (), count = 0;
    count = ( (lsize.GetHeight() % GetLineHeight()) < GetLineHeight() ?
                m_count + 1 : m_count );
    vscroll->Enable ( count > 0 ? true : false );
    vscroll->SetScrollbar ( m_position, linecount, count, linecount, true );
}
void wxVisualList::OnMouse ( wxMouseEvent& event )
{
    m_mouse.x = event.GetX ();
    m_mouse.y = event.GetY ();
    if ( event.ButtonDown () )
    {
        size_t lineheight = GetLineHeight (), linecount = m_mouse.y / lineheight, select;
        select = linecount + m_position;
        if ( select < m_count )
        {
            m_select = linecount + m_position;
            Refresh (false);
        }
    }
}
void wxVisualList::OnPaint ( wxPaintEvent& event )
{
    wxWindow *list = FindWindow (ID_LIST);
    Header  *header = static_cast <Header*> (FindWindow (ID_HEADER));
    wxASSERT ( list != NULL && header != NULL );
    wxSize lsize = list->GetClientSize ();
    size_t line_count = 0, lineheight = GetLineHeight ();
    size_t col_count = 0, col_pos = 0, line_pos = 0, total_width = 0;
    line_count = lsize.GetHeight () / lineheight;
    line_count = (line_count + m_position) > m_count ? m_count - m_position : line_count;
    col_count   = header->Count ();
    col_pos     = 0;

    wxPaintDC dc (list);
    dc.SetFont ( list->GetFont () );
    wxRect rect;
    if ( m_position <= m_select && ( m_position + line_count ) >= m_select )
    {
        rect = CalcLineRect ( lsize, m_select - m_position, lineheight );
        DrawSelectBox ( dc, rect );
    }
    for ( total_width = 0, col_pos = 0; col_pos < col_count && m_count != 0; ++col_pos )
    {
        size_t width = header->GetWidth (col_pos);
        if ( col_pos + 1 == col_count )
        {
            int w;
            GetClientSize ( &w, NULL );
            width += w - total_width - width;
        }
        for ( line_pos = 0; line_pos < line_count; ++line_pos )
        {
            rect = CalcLineRect ( lsize, line_pos, lineheight );
            rect.x += total_width;
            rect.width = width;
            OnDrawLine ( dc, line_pos + m_position, col_pos, rect );
        }
        total_width += width;
    }
    event.Skip ();
}
void wxVisualList::OnDrawLine ( wxDC& dc, const size_t& line, const size_t& col, 
                                const wxRect& rect )
{
    wxDCClipper (dc, rect.x, rect.y, rect.width - 1, rect.height);
    dc.DrawText (wxString::Format (wxT("Line %d %d"), line, col), rect.x, rect.y);
}
void wxVisualList::DrawSelectBox ( wxDC& dc, const wxRect& rect )
{
    wxPen pen = dc.GetPen (), newpen = (*wxBLACK_PEN);
    newpen.SetStyle (wxLONG_DASH|wxTRANSPARENT);
    dc.SetPen ( newpen );
    dc.DrawRectangle ( rect.x, rect.y + 1, rect.width - 1, rect.height + 1 );
    dc.SetPen (pen);
}
wxRect wxVisualList::CalcLineRect ( const wxSize& size, const size_t& line, 
                                    const size_t& lineheight ) const
{
    return wxRect ( 1, line * lineheight, size.GetWidth(), lineheight ); 
}
void wxVisualList::OnScroll ( wxScrollEvent& event )
{
    wxScrollBar *bar = static_cast <wxScrollBar*> (FindWindow (ID_VSCROLL));
    wxWindow *list = FindWindow (ID_LIST);
    wxASSERT ( bar != NULL && list != NULL );
    m_position = bar->GetThumbPosition ();
    list->SetFocus ();
    Refresh (false);
}
void wxVisualList::OnResize ( wxSizeEvent& event )
{
    wxSize size = event.GetSize ();
    wxWindow    *list   = FindWindow (ID_LIST);
    wxScrollBar *bar    = static_cast <wxScrollBar*> (FindWindow (ID_VSCROLL));
    wxWindow    *header = FindWindow (ID_HEADER);
    wxASSERT ( list != NULL && bar != NULL );
    wxSize listsize, barsize, headersize;
    listsize    = list->GetSize ();
    barsize     = bar->GetSize ();
    headersize  = header->GetSize ();
    headersize.Set ( size.GetWidth (), headersize.GetHeight() );

    listsize.Set ( size.GetWidth () - barsize.GetWidth(), 
                   size.GetHeight () - headersize.GetHeight () );
    list->SetMinSize ( listsize );
    list->Refresh (false);

    barsize.SetHeight ( size.GetHeight () - headersize.GetHeight() );
    bar->SetMinSize ( barsize );
    bar->Refresh ();

    header->SetMinSize ( headersize );
    header->Refresh (false);

    if ( m_count != 0 ) CalcScroll ();
    event.Skip ();
}
size_t wxVisualList::GetLineHeight () const
{
    return ( GetFont().IsOk () ? GetFont ().GetPointSize () + 4: 12 );
}
wxVisualList::~wxVisualList ()
{}

