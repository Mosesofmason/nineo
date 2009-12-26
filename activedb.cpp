//////////////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////
#include "activedb.hpp"
#include "log.hpp"

namespace NiDB
{
    NiClass::NiClass ( const wxString& name, const unsigned int& num  )
        : m_num(num)
    {
        m_name = name;
        m_hash = NiUtils::NiHash ( name );
    }
    NiClass::NiClass ( const NiClass& c )
    {
        m_num = c.m_num;
        m_name = c.m_name;
        m_hash = c.m_hash;
    }
    NiClass::NiClass ()
        : m_num(0), m_hash (0)
    {}
    NiClass::NiClass ( const NiGroup::NiUnsubGroup& group )
    {
        m_name = group.Prefix();
        m_hash = group.PrefixHash();
        m_num = 0;
    }
    NiClass::~NiClass ()  
    {}
    void NiClass::Num ( const unsigned int& n )
    {
        m_num = n;
    }

    unsigned int NiClass::Num () const
    {
        return m_num;
    }
    void NiClass::Name ( const wxString& name )
    {
        m_name = name;
        m_hash = NiUtils::NiHash (name);
    }
    wxString NiClass::Name () const
    {
        return m_name;
    }
    NiUtils::NiHASH NiClass::Hash () const
    {
        return m_hash;
    }
    NiClass& NiClass::operator= ( const NiGroup::NiUnsubGroup& group )
    {
        m_name = group.Prefix ();
        m_hash = group.PrefixHash ();
        m_num = 0;
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////////// 
    NiActiveDB::NiActiveDB ()
        : m_grouparray (NULL)
    {
    }
    struct FunClearIndex 
    {
        template <typename T> void operator () ( T& val )
        {
            if ( val.second != NULL )
            {
                delete val.second;
                val.second = NULL;
            }
        }
    };

    NiActiveDB::~NiActiveDB()
    {
        if ( m_grouparray)
        {
            delete m_grouparray;
        }
        ClearClassIndex ();
        ClearGroupIndex ();
    }
    void NiActiveDB::ClearGroupIndex ()
    {
        if ( !m_groupindex.empty() )
            std::for_each ( m_groupindex.begin(), m_groupindex.end(),
                    FunClearIndex() );
    }
    void NiActiveDB::ClearClassIndex ()
    {
        if ( !m_classindex.empty() )
            std::for_each ( m_classindex.begin(), m_classindex.end(),
                    FunClearIndex() );

    }
    bool NiActiveDB::LoadClassIndex ()
    {
        ClearClassIndex ();
        wxSQLite3ResultSet result = m_db.ExecuteQuery (
                wxT("SELECT * FROM ClassTable;"));
        while ( result.NextRow () )
        {
            NiClass *c =  new NiClass;
            if ( c )
            {
                c->Name ( result.GetAsString(0) );
                c->Num ( result.GetInt(1) );
                m_classindex.insert ( std::make_pair ( c->Hash (), c ) );
            }
        }
        return true;
    }
    void NiActiveDB::UpdateClassTable ()
    {
        ClassIndex::iterator itr;
        wxString updatesql; 
        size_t count = 0;

        for ( itr = m_classindex.begin (); 
              itr != m_classindex.end(); 
              ++itr 
            )
        {
            if ( itr->second == NULL ) continue;
            wxString sql;
            sql << wxT("SELECT COUNT(namehash) FROM GroupTable where prefix = ")
                << itr->second->Hash () << wxT(";\n");
            count = m_db.ExecuteQuery (sql).GetInt(0);
            if ( count < 0 ) count = 0;
            itr->second->Num ( count );
            FormatUpdateClassSQL ( updatesql, *(itr->second) );
        }
        m_db.ExecuteUpdate (updatesql);
    }
    void NiActiveDB::FormatUpdateClassSQL ( wxString& sql, const NiClass& C ) const
    {
        sql << wxT("UPDATE ClassTable SET num = ") << C.Num () 
            << wxT(" WHERE hash = ") << C.Hash () << wxT(";\n");
    }
    bool NiActiveDB::AddClass ( const NiClass& item )
    {
#if 0
        wxLogDebug (wxT("NiActiveDB::AddClass: %s"), item.Name().c_str());
#endif
        ClassIndex::iterator itr = m_classindex.find ( item.Hash () );
        if ( itr != m_classindex.end () )
            return false;
        NiClass *c = new NiClass (item);
        if ( c )
        {
            wxString sql;
            FormatClassToSQL ( sql, item );
            m_db.ExecuteUpdate ( sql );
            m_classindex.insert ( std::make_pair ( item.Hash (), c ) );
        }
        return true;
    }   
    void NiActiveDB::FormatClassToSQL ( wxString& sql,
                                        const NiClass& C ) const
    {
        sql << wxT("INSERT INTO ClassTable VALUES ( ")
            << wxT("'") << C.Name ()
            << wxT("', '") << C.Num () 
            << wxT("', '") << C.Hash () << wxT("');\n");
    }
    bool NiActiveDB::LoadGroupIndex ()
    {
        wxString sql;
        sql << wxT("SELECT namehash FROM GroupTable;\n");
        wxSQLite3ResultSet result = m_db.ExecuteQuery (sql);
        NiUtils::NiHASH hash = 0;
     
        while ( result.NextRow () )
        {
            hash =  result.GetInt (0);
            m_groupindex.insert ( std::make_pair (hash, (NiGroup::NiUnsubGroup*)NULL) );
        }
        return true;
    }

    void NiActiveDB::Open ( const wxString& filename )
    {
        try 
        {
            m_db.Open (filename);
            InitClassTable ();
            InitGroupTable ();
        }
        catch ( wxSQLite3Exception& e )
        {
            wxString message;
            message = wxString::Format (wxT("NiActiveDB %s [%d %s]"),
                                        filename.c_str(),
                                        e.GetErrorCode(),
                                        e.GetMessage().c_str());
            NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger ();
            loger->Message (message);
        }
    }
    bool NiActiveDB::InitGroupTable ()
    {
        if ( !m_db.TableExists (wxT("GroupTable")) )
        {
            wxString sql;
            sql << wxT("CREATE TABLE GroupTable (")
                << wxT("name TEXT NOT NULL,namehash INTEGER NOT NULL, ")
                << wxT("prefix INTEGER NOT NULL,first INTEGER NOT NULL, ")
                << wxT("last INTEGER NOT NULL, POST INTEGER NOT NULL);\n")
                << wxT("CREATE INDEX GroupTablePrefixIndex ON GroupTable(prefix);")
                << wxT("CREATE INDEX GroupTableNamehashIndex ON GroupTable(namehash);");
            m_db.ExecuteUpdate (sql);
            return true;
        }
        return true;
    }
    void NiActiveDB::FormatGroupToSQL ( wxString& sql, 
                                        const NiGroup::NiUnsubGroup& group ) const
    {
        sql << wxT("INSERT INTO GroupTable VALUES (")
            << wxT("'") << group.Name () << wxT("',")
            << wxT("'") << group.NameHash () << wxT("',")
            << wxT("'") << group.PrefixHash () << wxT("',")
            << wxT("'") << group.First () << wxT("',")
            << wxT("'") << group.Last () << wxT("',")
            << wxT("'") << group.Post () << wxT("');\n");
    }
    bool NiActiveDB::InitClassTable ()
    {  
        if ( !m_db.TableExists (wxT("ClassTable")) )
        {
            wxString sql;
            sql << wxT("CREATE TABLE ClassTable ") 
                << wxT("(name TEXT NOT NULL, num INTEGER, hash INTEGER NOT NULL);\n")
                << wxT("CREATE INDEX ClassTableIndex ON ClassTable(name);");
            m_db.ExecuteUpdate (sql);
            return true;
        }
        return true;
    }
    unsigned int NiActiveDB::GroupCount ()
    {
        return m_db.ExecuteScalar (wxT("SELECT COUNT(namehash) FROM GroupTable;"));
        return 0;
    }
    void NiActiveDB::Close ()
    {
        m_db.Close ();
    }

    bool NiActiveDB::HasGroup ( const wxString& gn )
    {
        return HasGroup (NiUtils::NiHash (gn));
    }
    bool NiActiveDB::HasGroup ( const NiUtils::NiHASH& hash )
    {
        GroupIndex::iterator itr = m_groupindex.find (hash);
        if ( itr != m_groupindex.end () ) 
        {
            return true;
        }
        wxString sql;
        sql << wxT("SELECT COUNT(namehash) FROM GroupTable WHERE namehash = ")
            << hash << wxT(";");
        return m_db.ExecuteQuery (sql).GetInt(0) != 0;
    }
    bool NiActiveDB::HasClass ( const NiUtils::NiHASH& hash )
    {
        ClassIndex::iterator itr = m_classindex.find ( hash );
        if ( itr != m_classindex.end() )
        {
            return true;
        }
        wxString sql;
        sql << wxT("SELECT COUNT(hash) FROM ClassTable WHERE hash = ") 
            << hash  << wxT(";\n");
        return m_db.ExecuteQuery (sql).GetInt(0) != 0;
    }
    bool NiActiveDB::Add ( const NiGroup::NiUnsubGroup& us_group )
    {
        if ( !HasClass ( us_group.PrefixHash () ) )
        {
            NiClass c (us_group);
            AddClass (c);
        }
        if ( !HasGroup ( us_group.NameHash() ) && m_grouparray )
        {
            m_grouparray->push_back ( us_group );
            if ( m_grouparray->size () == m_grouparray->capacity() )
            {
                DoUpdate ();
            }
            return true;
        }
        return false;
    }

    void NiActiveDB::BeginUpdate ( const size_t default_count )
    {
        LoadClassIndex ();
        LoadGroupIndex ();
        if ( m_grouparray == NULL )
        {
            m_grouparray =  new NiGroup::NiUnsubGroupArray;
            wxASSERT ( m_grouparray != NULL );
        }
        if ( m_grouparray )
        {
            m_grouparray->reserve ( default_count );
        }
    }
    void NiActiveDB::DoUpdate ()
    {
        if ( m_grouparray )
        {
            wxString sql;
            size_t count = m_grouparray->size ();
            for (size_t pos = 0; pos < count; ++pos )
            {
                FormatGroupToSQL (sql, m_grouparray->at(pos));
            }
            if ( count )
            { 
                wxMutexLocker locker (lock);
                m_db.Begin ();
                m_db.ExecuteUpdate (sql);
                m_db.Commit ();
                m_grouparray->clear ();
            }
        }
    }
    void NiActiveDB::EndUpdate ()
    {
        DoUpdate ();
        UpdateClassTable ();
        delete m_grouparray;
        m_grouparray = NULL;
    }
    bool NiActiveDB::ListClass ( ClassList& list )
    {
        wxMutexLocker locker (lock);
        static const wxString sql = wxT("SELECT name,num FROM ClassTable WHERE num != 0;");

        wxSQLite3ResultSet result = m_db.ExecuteQuery (sql);
        while ( result.NextRow () )
        {
            list.push_back ( NiClass (result.GetAsString (0), result.GetInt (1)) );
        }   
        return true;
    }
    bool NiActiveDB::ListGroup ( const NiClass& oclass, NiGroup::UnsubGroupList& list )
    {
        wxString sql;
        sql << wxT("SELECT name,first,last,post FROM GroupTable ")
            << wxT("where prefix = ") << oclass.Hash () << wxT(";");
        wxSQLite3ResultSet result = m_db.ExecuteQuery (sql);
        while ( result.NextRow () )
        {
            list.push_back (NiGroup::UnsubGroup (
                                 result.GetAsString(0),
                                 result.GetInt (1),
                                 result.GetInt (2),
                                 result.GetInt (3) != 0) );
        }
        return true;
    }
    bool NiActiveDB::ListGroup ( const NiClass& oclass, NiGroup::UnsubGroupList& list,
                                 const size_t& count )
    {
        size_t list_size = list.size ();
        wxString sql;
        sql << wxT("SELECT name,first,last,post FROM GroupTable ")
            << wxT("WHERE prefix = ") << oclass.Hash () 
            << wxT(" LIMIT ") << count 
            << wxT(" OFFSET ") << list_size
            << wxT(";");
        wxSQLite3ResultSet result = m_db.ExecuteQuery (sql);
        while ( result.NextRow () )
        {
            list.push_back (NiGroup::UnsubGroup (
                             result.GetAsString (0),
                             result.GetInt (1),
                             result.GetInt (2),
                             result.GetInt (3) != 0 ));
        }
        return true;
    }
    size_t NiActiveDB::CountGroup ( const NiClass& oclass )
    {
        size_t size;
        wxString sql;
        sql << wxT("SELECT COUNT(*) FROM GroupTable WHERE prefix = ")
            << oclass.Hash () << wxT(" ;");
        size = m_db.ExecuteScalar (sql);
        return size;
    }
    wxMutex         ActiveDBManager::m_lock;
    ActiveDBPtrMap ActiveDBManager::m_activedb;
    bool ActiveDBManager::OpenManager ()
    {
#if 0
        wxMutexLocker locker ( m_lock );
        if ( m_activedb == NULL )
        {
            m_activedb = new ActiveDBPtrMap;
        }
#endif
        return true;
    }
    ActiveDB* ActiveDBManager::GetDB ( const NiServer::Server& server )
    {
        wxMutexLocker locker ( m_lock );
        ActiveDBPtrMap::iterator itr;
        NiUtils::HASH hash = server.Hash ();
        ActiveDB *db = NULL;

        itr = m_activedb.find ( hash ); 
        if ( itr != m_activedb.end () )
        {
            db = itr->second;
        }
        else
        {
            db = new ActiveDB;
            wxASSERT ( db != NULL );
            db->Open (NiConfig::ServerActiveDataFile ( server ));
            m_activedb.insert ( std::make_pair ( hash, db ) );
        }
        return db;
    }
    bool ActiveDBManager::CloseManager ()
    {
#if 1
        wxMutexLocker locker ( m_lock );
        ActiveDBPtrMap::iterator itr, eitr;
        itr = m_activedb.begin ();
        eitr = m_activedb.end ();
        for ( ; itr != eitr; ++itr )
        {
            delete itr->second;
            itr->second = NULL;
        }
        m_activedb.clear ();
#endif
        return true;
    }
};
