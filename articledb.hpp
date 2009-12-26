#ifndef __NEXT_NINEO_ARTICLEDB_DEFINE__
#define __NEXT_NINEO_ARTICLEDB_DEFINE__
/**
 *    \file  articledb.hpp
 *   \brief  实现储存帖子的数据库类
 *    
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-12
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++,borland free commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */
#include <wx/wx.h>
#include <map>
#include <memory>

#include "config.hpp"
#include "safe.hpp"
#include "ov.hpp"
#include "utils.hpp"
#include "regex.hpp"
#include "string.hpp"
#include "threadtree.hpp"

//!  定义wxSQLite3Database
class wxSQLite3Database;

namespace NiDB
{
    class ArticleDB;
    class OverviewPlus;
    class DbCache;
    class DbCacher;

    typedef std::auto_ptr <wxSQLite3Database> wxSQLite3DatabasePtr;
    class MiniOverview : 
        public NiNntp::Lines,   public NiNntp::Bytes,   public NiNntp::From,    
        public NiNntp::Subject, public NiNntp::MessageID,
        public NiNntp::References,                      public NiNntp::XRef
    {
        public:
            MiniOverview ();
            ~MiniOverview ();
            MiniOverview& operator = ( const NiNntp::Overview& ov );
        private:
            MiniOverview& operator <<( const NiNntp::Overview& ov );
    };
    typedef std::map <NiUtils::HASH, MiniOverview*> MiniOVMap;

    class Overview : public NiNntp::Id
    {
        public:
            Overview ( MiniOverview* miniov );
            ~Overview ();
            Overview& operator = ( const NiNntp::Overview& ov );
            Overview& operator << ( NiNntp::Overview& ov );
            
        private:
            const MiniOverview* m_miniov;
    };
    typedef std::map < NiUtils::HASH, Overview* > OverviewMap;
    typedef std::auto_ptr < OverviewMap > OverviewMapPtr;
    class DbCache : public Safe
    {
        public:
            DbCache ();
            ~DbCache ();
        protected:
            friend class DbCacher;
            MiniOverview*  Add ( const NiUtils::HASH& hash );
            size_t Size ();
        private:
            static void InitMap ();
            static void CleanMap ();
            static MiniOverview* Cache ( const NiUtils::HASH& hash );
            void Clean ();
            static MiniOVMap OvMap;
    };

    class DbCacher 
    {
        public:
            DbCacher ( DbCache& cache );
            ~DbCacher ();
        protected:
            MiniOverview* Cache ( const NiUtils::HASH& hash );
        private:
            DbCache& m_cache;
    };

    using namespace NiUtils;
    class OverviewPlus : public NiNntp::Overview, public NiTree::Node
    {
        private:
            HASH m_hmsgid, m_father;
        protected:
            friend class ArticleDB;
            void Mini ();
            virtual wxString Label () const;
        public:
            bool IsOk () const;
            OverviewPlus ( const unsigned int& id, const HASH& hi, const HASH& hf );
            OverviewPlus ( const OverviewPlus& ov );
            OverviewPlus ();
            ~OverviewPlus ();
            wxString DeFrom ( const wxMBConv& default_conv = wxConvLocal ) const;
            wxString IgnoreCharset ( const wxString& charset ) const;
            HASH GetMessageIdHash () const;
            HASH GetFatherHash () const;
            void SetFatherHash ( const NiUtils::HASH& hash );
            OverviewPlus& operator= ( const NiNntp::Overview& ov );
            OverviewPlus& operator= ( const OverviewPlus& ov );
    };

    class ReferencesParser
    {
        public:
            ReferencesParser ( const wxString& refs );
            ReferencesParser ();
            ~ReferencesParser ();
            wxString GetItem ( const size_t& idx ) const;
            NiUtils::HASH GetItemHash ( const size_t& idx ) const;
            size_t Count () const;
            bool operator () ( const wxString& refs );
        private:
            void Parser ( const wxString& refs );
            typedef std::pair < int, int > pairs;
            typedef std::vector <pairs> pairs_vector;
            pairs_vector m_vector;
            const wxString* m_refs;
            wxRegEx m_re;
    };
    typedef  std::auto_ptr <OverviewPlus> OverviewPlusPtr;
    typedef  std::map <HASH, OverviewPlus> OverviewPlusMap;
    typedef  std::map <HASH, OverviewPlus*> OverviewPlusPMap;
    //!  \class ArticleDB
    //!  \brief 用来储存新闻组帖子的数据库类
    class ArticleDB : public Safe
    {
        public:
            ArticleDB ();//< 初始化构造，new wxSQLite3Database
            ~ArticleDB ();
            bool Open ( const wxString& dbname );//< 打开数据库 
            bool AddOV ( const NiNntp::Overview& ov );
            bool Commit ();
            unsigned int LastId ( const unsigned int& first = 0, 
                                   const unsigned int& last = 0 ) const;
            void Test ();
            enum FieldType
            {
                ID,
                SUBJECT,
                FROM,
                DATE,
                MESSAGEID,
                LINES,
                BYTES,
                REFERENCES,
                XREFS
            };
            wxString GetField ( const size_t& idx, const FieldType& field );
            size_t Depth ( const size_t& idx );
            size_t Count ();
        private:
            void PreLoad ( OverviewPlus* ov );
            void Thread ();
            bool inline HasOV ( const NiUtils::HASH& hash );
            bool inline InsertOV ( const NiUtils::HASH& hash, OverviewPlus* ov );
            bool BufferOV ( const OverviewPlus& ov ); 
            void SetFather ( OverviewPlus& ov );    
            void ClearOVMap ();
            void CreateOVTable ();
            void CreateArticleTable ();
            void CreateSubjectMapTable ();
            void LoadOV ();
            wxSQLite3DatabasePtr m_db;
            OverviewPlusPMap m_ovmap;
            std::auto_ptr < wxString > m_buff;
            NiTree::Tree m_tree;
            unsigned int m_lastid;
    };
    typedef std::map < NiUtils::HASH,  ArticleDB* >  ArticleDBPtrMap;
    class ArticleDBManager
    {
        private:
            ArticleDBManager ();
            ~ArticleDBManager ();
            static ArticleDBPtrMap* m_articledb;
            static wxMutex m_lock;
        public:
            static bool OpenManager ();
            static ArticleDB* GetDB ( const NiGroup::SubGroup& group );
            static bool CloseManager ();
    };
}
#endif //


