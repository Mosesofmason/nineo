#ifndef __NEXT_NINEO_THREAD_DEFINE__
#define __NEXT_NINEO_TRHEAD_DEFINE__ 
/**
 *    \file  thread.hpp
 *   \brief  多线程任务处理 
 *
 *
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-29
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++,borland free commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */


#include <wx/wx.h> 
#include "config.hpp" 
#include <wx/thread.h>
#include <vector>
#include <algorithm>
#include "group.hpp"
#include "server.hpp"
#include "nntp.hpp"
#include "db.hpp"
#include "observer.hpp"

namespace NiThread
{
    using namespace NiNntp;
    using namespace NiServer;
    using namespace NiGroup;

    class Task;
    class ThreadManager;
    class Thread;
    class TaskNntp;

    typedef std::vector <wxThread*> ThreadList;
    typedef std::vector <Task*> TaskList;

    class ThreadManager : public wxMutex, public NiUtils::ObServer
    {
        public:
            void Offline ();
            void Online ();
            bool AddTaskDebug ( const size_t& count = 20 );
            bool AddTaskUpdateGrouplist ( Server& server );
            bool AddTaskUpdateGroup ( SubGroup& group );
            bool AddThreadDebug ( const size_t& count = 10 );
            bool AddThreadNntp ( const NiServer::Server& server );
            ThreadManager& operator<< ( const wxString& msg );
            bool Run ();
            enum
            {
                TASK_DONE,
                TASK_DEL,
                TASK_ADD = -1,
            };
            wxString GetTaskAt ( const size_t& idx ) const;
            size_t CountTask () const;
        protected:
            ThreadManager ();
            ~ThreadManager ();
            friend class DefaultManager;
            friend class Thread;
            friend class Task;
            friend class ThreadNntp;
            friend class ThreadDebug;
            bool ThreadDone ( Thread* thread );
            bool TaskDone ( Task* task );
            Task* GetTask ( Thread& thread ); 
        private:
            bool AddTask ( Task* task );
            bool RemoveThread ( Thread* thread );
            inline bool CreateThread ( Thread* thread ) const;
            bool HasTask ( Task* task );
            ThreadList m_threadlist;        
            TaskList m_tasklist, m_waitlist, m_runlist;
            bool m_status;
            wxMutex m_lock;
    };
    class DefaultManager
    {
        public:
            static ThreadManager* Manager ();
        private:
            DefaultManager () {};
            ~DefaultManager () {};
            static ThreadManager* m_manager;
            static wxMutex m_lock;
    };

    class Task 
    {
        public:
            enum TaskType
            {
                EMPTY, 
                NNTP,
                DEBUG
            };
            Task ( const TaskType& type = EMPTY );
            ~Task ();
            TaskType Type () const;
            virtual wxString Message ();
        private:
            TaskType m_type;
    };

    class TaskDebug : public Task 
    {
        public:
            TaskDebug ();
            ~TaskDebug ();
            static size_t count;
            virtual wxString Message ();
        private:
            size_t m_count;
    };

    class TaskNntp : public Task
    {
        public:
            TaskNntp ();
            ~TaskNntp ();
            virtual bool Work ( Nntp& nntp );
            virtual wxString GetServer () const;
    };
    class TaskNntpUpdateGroup : public TaskNntp 
    {
        public:
            TaskNntpUpdateGroup ( SubGroup& group );
            ~TaskNntpUpdateGroup ();
            virtual bool Work ( Nntp& nntp );
            virtual wxString GetServer () const;
            virtual wxString Message ();
        private:
            SubGroup m_group;
    };
    class TaskNntpUpdateGrouplist : public TaskNntp
    {
        public:
            TaskNntpUpdateGrouplist ( const Server& server );
            ~TaskNntpUpdateGrouplist ();
            virtual bool Work ( Nntp& nntp );
            virtual wxString GetServer () const;
            virtual wxString Message ();
        private:
            Server m_server;
    };
        
    class TaskPanel : public wxPanel
    {
        public:
            TaskPanel ( wxWindow* parent, wxWindowID id, long style = 0, 
                        const wxString& name = wxT("TaskPanel") );
            ~TaskPanel ();
        private:
            enum
            {
                ID_TASK_VIEW,
                ID_LOG_VIEW
            };
            void OnResize ( wxSizeEvent& event );
            DECLARE_EVENT_TABLE ()
    };
    class Thread : public wxThread
    {
        public:
            Thread ( ThreadManager& manager );
            ~Thread ();
            virtual ExitCode Entry ();
            virtual bool operator () ( Task* task );
        protected:
            ThreadManager& m_manager;
    };

    class ThreadDebug : public Thread 
    {
        public:
            ThreadDebug ( ThreadManager& manager );
            ~ThreadDebug ();
            virtual ExitCode Entry ();
            virtual bool operator () ( Task* task );
    };
    
    class ThreadNntp : public Thread
    {
        public:
            ThreadNntp ( ThreadManager& manager, const NiServer::Server& server );
            ~ThreadNntp ();
            virtual ExitCode Entry ();
            virtual bool operator () ( Task* task );
        private:
            bool ConnectServer ( Nntp& nntp, const Server& server,NiResp& resp ) const ;
            bool Authinfo ( Nntp& nntp, const Server& server, NiResp& resp ) const;
            Server m_server;
    };
}
#endif //


