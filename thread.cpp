////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////

#include "thread.hpp"
#include "log.hpp"
#include "articledb.hpp"
namespace NiThread
{
    using namespace NiNntp;
    using namespace NiServer;
    using namespace NiGroup;

    ThreadManager::ThreadManager ()
        : m_status (false)
    {}

    struct OF_CLEAN_TASKLIST
    {
        void operator () ( TaskList::value_type& v )
        {
            delete v;
            v = NULL;
        }
    };
    ThreadManager::~ThreadManager ()
    {
        wxLogDebug (wxT("ThreadManage task count: %3d"), m_tasklist.size());
        wxLogDebug (wxT("ThreadManage thread count: %3d"), m_threadlist.size());
#if 0
        if ( !m_tasklist.empty () )
        {
            std::for_each ( m_tasklist.begin(), m_tasklist.end(), 
                            OF_CLEAN_TASKLIST ()
                           );
            m_tasklist.clear ();
        }
#endif
    }
    bool ThreadManager::TaskDone ( Task* task )
    {
        wxMutexLocker locker ( m_lock );
        TaskList::iterator ritr, titr;
        ritr = std::find ( m_runlist.begin(), m_runlist.end(), task );
        if ( ritr != m_runlist.end () )
        {
            m_runlist.erase (ritr);
        }
        titr = std::find ( m_tasklist.begin (), m_tasklist.end(), task );
        if ( titr != m_tasklist.end () )
        {
            m_tasklist.erase (titr);
        }
        delete task;
        ObUpdate (TASK_DONE);
        return true;
    }
    bool ThreadManager::ThreadDone ( Thread* thread )
    {
        wxMutexLocker locker ( m_lock );
        return RemoveThread (thread);
    }
    bool ThreadManager::RemoveThread ( Thread* thread )
    {
        wxMutexLocker locker ( m_lock );
        ThreadList::iterator itr;
        itr = std::find ( m_threadlist.begin (), m_threadlist.end (), thread );
        if ( itr != m_threadlist.end () )
        {
            m_threadlist.erase (itr);
            wxString message = wxString::Format (wxT("%5d thread done"), thread->GetId());
            NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
            loger->Message (message);
            return true;
        }
        return false;
    }
    ThreadManager& ThreadManager::operator << ( const wxString& msg )
    {
        wxMutexLocker locker ( m_lock );
        NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
        loger->Message (msg);
        return *this;
    }
    void ThreadManager::Offline ()
    {
        m_status = false;
    }
    void ThreadManager::Online ()
    {
        m_status = true;
    }
    bool ThreadManager::Run ()
    {
        bool ret = true;
        if ( m_status && !m_threadlist.empty () )
        {
            for ( ThreadList::iterator itr = m_threadlist.begin ();
                  itr < m_threadlist.end ();
                  ++itr )
            {
                if ( *itr )
                {
                    if ( (*itr)->Run () == wxTHREAD_NO_ERROR )
                    {
                        NULL;
                    }
                }
            }
        }
        return ret;
    }
    bool ThreadManager::AddTask ( Task* task )
    {
        wxMutexLocker locker ( m_lock );
        if ( HasTask ( task ) ) return true;
        m_tasklist.push_back (task);
        m_waitlist.push_back (task);
        ObUpdate (TASK_ADD);
        return true;
    }
    bool ThreadManager::AddTaskUpdateGroup ( SubGroup& group )
    {
        TaskNntpUpdateGroup* task = new TaskNntpUpdateGroup ( group );
        return AddTask (task);
    }
    bool ThreadManager::AddTaskUpdateGrouplist ( Server& server )
    {
        TaskNntpUpdateGrouplist *task = new TaskNntpUpdateGrouplist (server);
        return AddTask ( task );
    }
    size_t ThreadManager::CountTask () const
    {
        return m_tasklist.size ();
    }
   wxString ThreadManager::GetTaskAt ( const size_t& idx ) const
    {
        wxASSERT ( idx < m_tasklist.size () );
        return m_tasklist[idx]->Message();
    }
    struct OF_CMP_TASK_THREAD
    {
        public:
            OF_CMP_TASK_THREAD ( Thread& thread )
                : m_thread (thread)
            {}
            bool operator () ( Task* task )
            {
                return m_thread (task);
            }
        private:
            Thread& m_thread;
    };
    Task* ThreadManager::GetTask ( Thread& thread )
    {
        wxMutexLocker locker ( m_lock );
        Task* task = NULL;
        if ( m_waitlist.empty () ) return task;
        TaskList::iterator itr;
        
        itr = std::find_if ( m_waitlist.begin(), m_waitlist.end(), 
                             OF_CMP_TASK_THREAD (thread) );
        if ( itr != m_waitlist.end () )
        {
            task = *itr;
            m_runlist.push_back (task);
            m_waitlist.erase (itr);
        }
        return task;
    }
    bool ThreadManager::HasTask ( Task* task )
    {
        bool exits = false;

        if ( std::find ( m_tasklist.begin(), m_tasklist.end (), task ) 
                != 
             m_tasklist.end() )
        {
            exits = true;
        }
        return exits;
    }
    inline bool ThreadManager::CreateThread ( Thread* thread ) const
    {
        return ( thread && thread->Create () == wxTHREAD_NO_ERROR );
    }
    bool ThreadManager::AddTaskDebug ( const size_t& count )
    {
        size_t c = count;
        while ( c-- )
        {
            AddTask ( new TaskDebug );
        }
        return false;
    }
    bool ThreadManager::AddThreadDebug ( const size_t& count )
    {
        size_t c = 0;
        bool isok = false;
        while ( c++ < count )
        {
            Thread* thread = new ThreadDebug (*this);
            if ( thread )
            {
                if ( CreateThread (thread) )
                {
                    m_threadlist.push_back (thread);
                    isok = true;
                }
                else
                {
                   isok =  false;
                   break;
                }
            }
        }
        return isok;
    }

    bool ThreadManager::AddThreadNntp ( const NiServer::Server& server )
    {
        bool isok = false;
        Thread *thread  = new ThreadNntp (*this, server);
        if ( thread && CreateThread (thread) )
        {
            isok = true;
            m_threadlist.push_back (thread);
        }
        return isok;
    }
    ////////////////////////////////////////////////////////////////////////////////
    wxMutex DefaultManager::m_lock;
    ThreadManager* DefaultManager::m_manager = NULL;
    ThreadManager* DefaultManager::Manager ()
    {
        wxMutexLocker locker (m_lock);
        if ( m_manager == NULL )
        {
            m_manager = new ThreadManager;
        }
        return m_manager;
    }
    ////////////////////////////////////////////////////////////////////////////////
    Task::Task ( const TaskType& type )
        : m_type ( type )
    {}
    Task::~Task ()
    {}

    Task::TaskType Task::Type () const
    {
        return m_type;
    }
    wxString Task::Message ()
    {
        return wxT("Task");
    }
    TaskNntp::TaskNntp ()
        : Task (NNTP)
    {}
    TaskNntp::~TaskNntp ()
    {}
    bool TaskNntp::Work ( Nntp& nntp )
    {
        return false;
    }
    wxString TaskNntp::GetServer () const
    {
        return wxEmptyString;
    }
   
    TaskNntpUpdateGroup::TaskNntpUpdateGroup ( SubGroup& group )
        : m_group (group)
    {}
    TaskNntpUpdateGroup::~TaskNntpUpdateGroup ()
    {}
    bool TaskNntpUpdateGroup::Work ( Nntp& nntp )
    {
        NiRespDebug debug;
        NiRespGroup group;

        wxCSConv conv (wxT("GB2312"));
        wxString groupname = wxString::From8BitData (m_group.Name().mb_str(conv).data());
        bool ret = false;
        if ( nntp.Group (groupname, group) )
        {
            NiDB::ArticleDB *db = NiDB::ArticleDBManager::GetDB (m_group);
            wxASSERT ( db != NULL );
            NiRespOverview respov (*db);
            size_t start = db->LastId ( group.First(), group.Last() );
            size_t end = group.Last ();
            wxLogDebug (wxT("%s %d %d ( %d %d )"), groupname.c_str(), 
                                                   group.First(), group.Last(),
                                                   start, end );
            if ( start < end && nntp.Xover ( start, end, respov ) )
            {
                ret = true;
            }
        }
        return ret;
    }
    wxString TaskNntpUpdateGroup::GetServer () const
    {
        return m_group.GetServer ();
    }
    wxString TaskNntpUpdateGroup::Message ()
    {
        return wxString::Format (wxT("Update group %s from %s"), 
                                 m_group.Name().c_str(),
                                 m_group.GetServer().c_str());
    }
    TaskNntpUpdateGrouplist::TaskNntpUpdateGrouplist ( const Server& server )
        : m_server (server)
    {}
    TaskNntpUpdateGrouplist::~TaskNntpUpdateGrouplist ()
    {}
    wxString TaskNntpUpdateGrouplist::Message ()
    {
        return wxString::Format (wxT("Update %s grouplist."),
                                 m_server.GetAddress().c_str());
    }
    bool TaskNntpUpdateGrouplist::Work ( Nntp& nntp )
    {
        NiDB::ActiveDB *db = NiDB::ActiveDBManager::GetDB (m_server);
        if ( db != NULL )
        {
            NiRespListGroup resp (*db);
            return nntp.List (resp);
        }
        return true;
    }
    wxString TaskNntpUpdateGrouplist::GetServer () const
    {
        return m_server.GetAddress();
    }
    
    size_t TaskDebug::count = 0;

    TaskDebug::TaskDebug ()
        : Task (DEBUG)
    {
        m_count = count;
        wxLogDebug (wxT("+TaskDebug id:%3d"), m_count);
        ++count;
    }
    wxString TaskDebug::Message ()
    {
        return wxString::Format (wxT("TaskDebug %3d"), m_count);
    }
    TaskDebug::~TaskDebug ()
    {
        wxLogDebug (wxT("-TaskDebug id:%3d"), m_count);
        --count;
    }
    ////////////////////////////////////////////////////////////////////////////////
    
    class TaskView: public wxListCtrl, public NiUtils::ObClient
    {
        public:
            TaskView ( wxWindow* parent, wxWindowID id, long style = NStyle );
            ~TaskView ();
        protected:
            virtual void ObUpdate ( ObServer& server, const int& uid );
            virtual wxString OnGetItemText ( long item, long column ) const;
        private:
            enum
            {
                ID_TIMER
            };
            void OnResize ( wxSizeEvent& event );
            void OnTimer ( wxTimerEvent& event );
            wxTimer m_timer;
            DECLARE_EVENT_TABLE ()
    };
    BEGIN_EVENT_TABLE ( TaskView, wxListCtrl )
        EVT_SIZE ( TaskView::OnResize )
        EVT_TIMER ( ID_TIMER, TaskView::OnTimer )
    END_EVENT_TABLE ()
    TaskView::TaskView ( wxWindow* parent, wxWindowID id, long style )
        : wxListCtrl ( parent, id, wxDefaultPosition, wxDefaultSize,
                       style|wxLC_VIRTUAL|wxLC_REPORT),
         m_timer ( this, ID_TIMER ) 
    {
        ThreadManager *manager = DefaultManager::Manager ();
        wxASSERT ( manager != NULL );
        (*manager) += this;
        InsertColumn ( 1, _("Task:") );
        m_timer.Start (2000);
    }
    void TaskView::ObUpdate ( ObServer& server, const int& uid )
    {
        ThreadManager *manager = DefaultManager::Manager ();
        SetItemCount ( manager->CountTask () );
        Refresh ();
    }
    wxString TaskView::OnGetItemText ( long item, long column ) const
    {
        ThreadManager *manager = DefaultManager::Manager ();
        wxString task;
        if ( item < manager->CountTask () )
        {
            task = manager->GetTaskAt ( item );
        }
        return task;
    }
    void TaskView::OnResize ( wxSizeEvent& event )
    {
        wxSize size = event.GetSize ();
        SetColumnWidth ( 1, size.GetWidth () );
        event.Skip ();
    }
    void TaskView::OnTimer ( wxTimerEvent& event )
    {
    }
    TaskView::~TaskView ()
    {}
    class LogView: public wxListCtrl 
    {
        public:
            LogView ( wxWindow* parent, wxWindowID id, long style = NStyle );
            ~LogView ();
        private:
            enum 
            {
                ID_TIMER
            };
            void OnTimer ( wxTimerEvent& event );
            void OnHScroll ( wxScrollWinEvent& event );
            virtual wxString OnGetItemText ( long item, long column ) const;
            wxString m_last;
            wxTimer m_timer;
            DECLARE_EVENT_TABLE ()
    };
    BEGIN_EVENT_TABLE ( LogView, wxListCtrl )
        EVT_TIMER ( ID_TIMER, LogView::OnTimer )
        EVT_SCROLLWIN ( LogView::OnHScroll ) 
    END_EVENT_TABLE ()
    LogView::LogView ( wxWindow* parent, wxWindowID id, long style )
    : wxListCtrl ( parent, id, wxDefaultPosition, wxDefaultSize, 
                   style|wxLC_REPORT|wxLC_VIRTUAL|wxLC_NO_HEADER|wxVSCROLL ),
     m_timer (this, ID_TIMER )
    {
        InsertColumn ( 1, _("Message") );
        m_timer.Start (3000);
    }
    void LogView::OnHScroll ( wxScrollWinEvent& event )
    {
        wxSize size = GetClientSize ();
        event.Skip ();
    }
    void LogView::OnTimer ( wxTimerEvent& event )
    {
        NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
        wxASSERT (loger != 0 );
        size_t size = loger->Size ();
        wxString log;
        log = loger->GetLog ( size - 1 );
        if ( size == 0 )
        {
            m_timer.Start (500);
        }
        else if ( m_last != log )
        {
            m_timer.Start ( 500 );
            SetItemCount (size);
            ScrollLines (size);
            RefreshItems( size - 50, size);
            Refresh ();
            m_last = log;
        }
        else 
        {
            m_timer.Start (500);
        }
    }
    wxString LogView::OnGetItemText ( long item, long column ) const
    {
        NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
        size_t size = loger->Size ();
        wxString log;
        if ( size > item )
        {
            log = loger->GetLog (item);               
        }
        return log;
    }
    LogView::~LogView ()
    {}

    BEGIN_EVENT_TABLE ( TaskPanel, wxPanel )
        EVT_SIZE ( TaskPanel::OnResize )
    END_EVENT_TABLE ()
    TaskPanel::TaskPanel ( wxWindow* parent, wxWindowID id, long style,
                           const wxString& name )
        : wxPanel ( parent, id, wxDefaultPosition, wxDefaultSize, style, name )
    {
        wxBoxSizer *main =  new wxBoxSizer (wxVERTICAL);
        this->SetSizer (main);
        TaskView *tv = new TaskView ( this, ID_TASK_VIEW );
        LogView *lv = new LogView ( this, ID_LOG_VIEW );

        main->Add ( tv, 0, wxALL|wxEXPAND, 1 );
        main->Add ( lv, 0, wxALL|wxEXPAND, 1 );
        main->SetSizeHints (this);
        SetAutoLayout (true);
    }
    void TaskPanel::OnResize ( wxSizeEvent& event )
    {
        wxSize size = event.GetSize ();
        wxSize child_size ( size.GetWidth(), size.GetHeight()/2-3 );
        TaskView *tv = static_cast <TaskView*>(FindWindow(ID_TASK_VIEW));
        LogView *lv = static_cast <LogView*> (FindWindow (ID_LOG_VIEW));
        wxASSERT ( tv != NULL && lv != NULL );
        tv->SetMinSize ( child_size );
        tv->SetColumnWidth ( 0, child_size.GetWidth() - 5 );
        lv->SetMinSize ( child_size );
        lv->SetColumnWidth ( 0, child_size.GetWidth () - 20 );
        event.Skip ();
    }
    TaskPanel::~TaskPanel ()
    {}
    ////////////////////////////////////////////////////////////////////////////////
    Thread::Thread ( ThreadManager& manager )
        : m_manager (manager)
    {}
    Thread::~Thread ()
    {
        m_manager.ThreadDone (this);
    }
    Thread::ExitCode Thread::Entry ()
    {
        return 0;
    }

    bool Thread::operator () ( Task* task )
    {
        return false;
    }

    ThreadDebug::ThreadDebug ( ThreadManager& manager )
        : Thread (manager)
    {}

    ThreadDebug::~ThreadDebug ()
    {}

    Thread::ExitCode ThreadDebug::Entry ()
    {
        while (1)
        {
            Task* task = m_manager.GetTask (*this);
            if ( task )
            {
                m_manager << wxString::Format ( wxT("%5d %s"), GetId (), 
                        task->Message ().c_str() );
                size_t xx = 100;
                while (xx--)
                {
                    NiUtils::Loger *loger = NiUtils::GetLoger ();
                    loger->Debug (wxT("Debug thread work."));
                    Sleep (50);
                }
                m_manager.TaskDone (task);
            }
            else 
            {
                break;
            }
        }
        return 0;
    }
    bool ThreadDebug::operator () ( Task* task )
    {
        return ( task != NULL && task->Type () == Task::DEBUG );
    }
    ////////////////////////////////////////////////////////////////////////////////

    ThreadNntp::ThreadNntp ( ThreadManager& manager, const NiServer::Server& server )
        : Thread ( manager ), m_server ( server )
    {}
    ThreadNntp::~ThreadNntp ()
    {}
    bool ThreadNntp::ConnectServer ( Nntp& nntp,const Server& server, NiResp& resp ) const
    {
        return ( nntp.Open (server.GetIPV4 (), resp) );
    }
    bool ThreadNntp::Authinfo ( Nntp& nntp, const Server& server, NiResp& resp ) const
    {
        return nntp.Authinfo ( server.GetUsername(), server.GetPassword(), resp );
    }

    Thread::ExitCode ThreadNntp::Entry ()
    {
        NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
        wxString msg;
        msg = wxString::Format ( wxT("%5d Working, connect to server %s [%s %d] "),
                                 GetId (),
                                 m_server.GetLabel ().c_str(),
                                 m_server.GetAddress ().c_str(),
                                 m_server.GetPort () );
        loger->Message (msg);
        Nntp nntp;
        NiRespDebug debug;
        bool isok = true;
        if ( !( isok && nntp.Open ( m_server.GetIPV4(), debug ) ) )
        {
            loger->Warr(wxString::Format (wxT("%5d Connect to server %s is fail."),
                                          GetId (),
                                          m_server.GetAddress ().c_str()
                                          ));
            isok = false;
        }
        if ( isok )
        {
            loger->Message(wxString::Format (wxT("%5d Connect to server %s is ok."),
                                             GetId (),
                                             m_server.GetAddress().c_str()
                                             ));
        }
        if ( isok && m_server.RequireAuthinfo() && !nntp.Authinfo( m_server, debug ) ) 
        {
            loger->Warr (wxString::Format (wxT("%5d Authinfo server %s is fail."),
                                           GetId (),
                                           m_server.GetAddress().c_str()));
            isok = false;
        }

        if ( isok )
        {
            if ( isok )
            {
                loger->Message(wxString::Format (wxT("%5d Process task."), GetId ()));
            }
            short count = 0;
            NiUtils::TimeReport timereport;
            while (1)
            {
                ++count;
                Task* task = m_manager.GetTask (*this);
                TaskNntp* worker = dynamic_cast <TaskNntp*> (task);
                if ( worker != NULL )
                {
                    timereport.Start ();
                    worker->Work (nntp);
                    timereport.End ();
                    timereport (wxString::Format (wxT("Thread %5d "), GetId ()));
                    m_manager.TaskDone (task);
                }
                else
                {
                    loger->Message(wxString::Format (wxT("%5d Nothing to do."), GetId ()));
                    break;
                }
            }
            nntp.Quit ();
        }
        return 0;
    }
    bool ThreadNntp::operator () ( Task* task )
    {
        if ( task != NULL && task->Type() == Task::NNTP )
        {
            TaskNntp* nntp_task = dynamic_cast <TaskNntp*> ( task );
            if ( nntp_task != NULL )
            {
                return ( nntp_task->GetServer () == m_server.GetLabel () 
                         ||
                         nntp_task->GetServer () == m_server.GetAddress () );
            }
        }
        return false;
    }
}
