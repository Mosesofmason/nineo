////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include "nntp.hpp"
#include "db.hpp"
#include "fontencode.hpp"
#include "regex.hpp"
#include "threadtree.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include "thread.hpp"

int main ( const int& argc, const char* argv[] )
{
    wxInitializer initializer;
    if ( !initializer )
    {
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
    wxSocketBase::Initialize();

    NiDB::ActiveDBManager::OpenManager ();
    NiDB::ArticleDBManager::OpenManager ();
     
#if 0
    size_t ov_count = 0;
    NiTree::Tree tree;

    NiTree::Node *node, *pnode;
    size_t count = 500, icx = 0;
    TimeReport tr;
    tr.Start ();
    while ( count-- )
    {
        node = tree.NodeLink ( new NiTree::Node );
        icx = 300;
        while ( icx-- )
        {
            pnode = new NiTree::Node;
            tree.NodeLink ( node, pnode );
        }
    }
    tr.End ();
    tr (wxT("Create tree"));
    size_t nodecount = 0; 
    tr.Start ();
    nodecount = tree.NodeCount ();
    tr.End ();
    wxLogDebug ( wxT("Total: %d"),nodecount );
    tr (wxT("get count"));
    tree.Clean ();
#endif

#if 0 
    NiThread::ThreadManager* tm = NiThread::DefaultManager::Manager ();
#if 0
    NiServer::Server yaako      ( wxT("yaako"), wxT("news.yaako.com"), 119 ),
                     newsfan    ( wxT("newsfan"), wxT("news.newsfan.net"), 119 ),
                     cn99       ( wxT("cn99"), wxT("news.cn99.com"), 119 );
#endif
#if 0
    tm->AddTaskUpdateGrouplist (yaako);
    tm->AddTaskUpdateGrouplist (newsfan);
    tm->AddTaskUpdateGrouplist (cn99);
    tm->AddThreadNntp ( yaako );
    tm->AddThreadNntp ( newsfan );
    tm->AddThreadNntp ( cn99 );
#endif
    NiServer::Server local ( wxT("localhost"), wxT("localhost"), 
                             119, wxT("admin"), wxT("123"));
    tm->AddTaskUpdateGrouplist (local);
    tm->AddThreadNntp (local);
    tm->Online ();
    tm->Run ();
#endif
#if 0

    NiDB::ActiveDBManager::OpenManager ();
    NiDB::ActiveDB *db = NiDB::ActiveDBManager::GetDB ( 
                                  NiServer::Server ( wxT("localhost"), wxT("localhost"), 119));

    if ( db != NULL ) wxLogDebug (wxT("OK"));
    NiDB::ActiveDBManager::CloseManager ();
#endif
    NiGroup::SubGroup group (wxT("localhost"), wxT("cn.fan"));
    wxLogDebug (wxT("%s %s"), group.GetServer().c_str(), group.Name().c_str() );
    NiDB::ArticleDB *cnfandb = NiDB::ArticleDBManager::GetDB ( group );
    wxASSERT ( cnfandb != NULL );
    cnfandb->Test ();
    char c;
    std::cout << "Exit.";
    std::cin >> c;
    NiDB::ActiveDBManager::CloseManager ();
    NiDB::ArticleDBManager::CloseManager ();
    return 0;
}
