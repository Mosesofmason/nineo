///////////////////////////////////////////////////////////////////////////
//////////                  Author: newblue <newblue@gmail.com>
//////////   Copyright by newblue, 2008.
////////////////////////////////////////////////////////////////////////////////
#include "articledb.hpp"
#include <wx/wxsqlite3.h>
#include <algorithm>
#include "decode.hpp"
#include "log.hpp"
#include "field.hpp"

namespace NiDB
{
    MiniOverview::MiniOverview () 
    {}
    MiniOverview::~MiniOverview ()
    {
    }

    MiniOverview& MiniOverview::operator = ( const NiNntp::Overview& ov )
    {
        SetLines        ( ov.GetLines () );
        SetBytes        ( ov.GetBytes () );
        SetFrom         ( ov.GetFrom () );
        SetSubject      ( ov.GetSubject () );
        SetMessageID    ( ov.GetMessageID () );
        SetReferences   ( ov.GetReferences () );
        SetXRef         ( ov.GetXRef () );
        return *this;
    }
    MiniOverview& MiniOverview::operator << ( const NiNntp::Overview& ov )
    {
        //m_from      = ov.m_from;
        //m_subject   = ov.m_subject;
        //m_msgid     = ov.m_msgid;
        //m_refs      = ov.m_refs;
        //m_xref      = ov.m_xref;
        //m_lines     = ov.m_lines;
        //m_bytes     = ov.m_bytes;
        return *this;
    }
    Overview::Overview ( MiniOverview* miniov )
        : m_miniov ( miniov ) 
    {}
    Overview::~Overview ()
    {
    }
    Overview& Overview::operator = ( const NiNntp::Overview& ov )
    {
        SetId   ( ov.GetId () );
        if ( m_miniov != NULL )
        {
           *(const_cast <MiniOverview*> (m_miniov)) = ov;
        }
        return *this;
    }
    Overview& Overview::operator << ( NiNntp::Overview& ov )
    {
        SetId   ( ov.GetId () );
        if ( m_miniov != NULL )
        {
            //! (*m_miniov) << ov;
        }
        return *this;
    }
    
    struct OF_CLEAN_MAP
    {
        void operator () ( MiniOVMap::value_type& o )
        {
            if ( o.second != NULL )
            {
                delete o.second;
                o.second = NULL;
            }
        }
    };
    void DbCache::InitMap ()
    {
        NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger (); 
        loger->Debug(wxT("DbCache::InitMap."));
    }
    void DbCache::CleanMap ()
    {
        NiUtils::Loger* loger = NiUtils::DefaultLoger::GetLoger ();
        loger->Message (wxT("Clean map."));
        if  ( !OvMap.empty() )
        {
            std::for_each ( OvMap.begin(), OvMap.end(), OF_CLEAN_MAP () );
            OvMap.clear ();
        }
    }
    DbCache::DbCache ()
    {
        InitMap ();
    }
    DbCache::~DbCache ()
    {
        CleanMap ();
    }
    size_t DbCache::Size ()
    {
        return OvMap.size ();
    }
    MiniOverview* DbCache::Cache ( const NiUtils::HASH& hash )
    {
        MiniOverview* miniov  = NULL;
        MiniOVMap::iterator itr = OvMap.find ( hash );
        if ( itr != OvMap.end () )
        {
            miniov = itr->second;
        }
        else
        {
            miniov = new MiniOverview;
            if ( miniov != NULL )
            {
                OvMap.insert ( std::make_pair ( hash, miniov ) );
            }
        }
        return miniov;
    }
    MiniOverview* DbCache::Add ( const NiUtils::HASH& hash )
    {
        return Cache (hash);
    }
    MiniOVMap DbCache::OvMap;
    DbCacher::DbCacher ( DbCache& cache )
        : m_cache (cache)
    {}
    DbCacher::~DbCacher ()
    {
        NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
        loger->Debug (wxString::Format (wxT("Cache: %d"), m_cache.Size()));
    }
    MiniOverview* DbCacher::Cache ( const NiUtils::HASH& hash )
    {
        return m_cache.Add ( hash );
    }
    using namespace NiUtils;

    OverviewPlus::OverviewPlus ()
        : m_hmsgid (0), m_father(0)
    {}

    OverviewPlus::OverviewPlus ( const unsigned int& id, const HASH& hi, const HASH& hf )
        : m_hmsgid(hi), m_father(hf)
    {
        SetId (id);
    }

    OverviewPlus::OverviewPlus ( const OverviewPlus& ov )
    {
        OverviewPlus& ovp = const_cast <OverviewPlus&> (ov);
        m_id = ovp.m_id;
        m_hmsgid = ovp.m_hmsgid;
        m_father = ovp.m_father;
        m_lines = ovp.m_lines;
        m_bytes = ovp.m_bytes;
        m_from = ovp.m_from;
        m_subject = ovp.m_subject;
        m_date = ovp.m_date;
        m_msgid = ovp.m_msgid;
        m_refs = ovp.m_refs;
        m_xref = ovp.m_xref;
    }
    OverviewPlus::~OverviewPlus ()
    {}

    HASH OverviewPlus::GetMessageIdHash () const
    {
        return m_hmsgid;
    }
    wxString OverviewPlus::Label () const
    {
        return wxString::Format (wxT("%d"), GetId ());
    }
    HASH OverviewPlus::GetFatherHash () const
    {
        return m_father;
    }
    void OverviewPlus::SetFatherHash ( const NiUtils::HASH& hash )
    {
        m_father = hash;
    }
    wxString OverviewPlus::IgnoreCharset ( const wxString& charset ) const
    {
        wxString ncharset = charset.Upper ();
        if  ( 
                ncharset == wxT("GB18030")  || 
                ncharset == wxT("X-GBK")   || 
                ncharset == wxT("GBK")         
            ) 
        {
            return wxT("GB2312");
        }
        return charset;
    }
    wxString OverviewPlus::DeFrom ( const wxMBConv& default_conv ) const
    {
        wxString from = GetFrom(), buf;
        wxRegEx re (NiRegEx::MatchEncodeField), mre (NiRegEx::MatchEmail);

        if ( re.Matches (from)  &&  re.GetMatchCount() == 4 )
        {
            wxString encode, c, data;
            encode  = re.GetMatch (from, 1);
            c       = re.GetMatch (from, 2);
            data    = re.GetMatch (from, 3);
#if 0
            wxLogDebug (wxT("%d %s %s %s"), re.GetMatchCount(), 
                                            encode.c_str(),
                                            c.c_str(),
                                            data.c_str());
#endif
            if ( c.Upper() == wxT("B") )
                data = NiDecode::Base64Decode (data);
            else if ( c.Upper() == wxT("Q") )
                data = NiDecode::QPDecode (data);
            encode = IgnoreCharset (encode);
            wxCSConv conv (encode);
            if  ( conv.IsOk () )
            {
                buf = conv.cMB2WC (data.To8BitData());
            }
            else
            {
                wxString message = wxString::Format (wxT("wxCSConv encode error %s"),
                                                     encode.c_str());
                NiUtils::Loger *loger = NiUtils::DefaultLoger::GetLoger ();
                loger->Message (message);
                buf = data;
            }
#if 0
             wxLogMessage (wxT("%d %s %s %s"), re.GetMatchCount(), 
                                            encode.c_str(),
                                            c.c_str(),
                                            wxString::From8BitData (
                                                buf.mb_str( wxConvLocal )
                                                                    ).c_str());
#endif
        return buf;
       }
        return wxString ( GetFrom().To8BitData(), default_conv );;
    }
    void OverviewPlus::Mini ()
    {
        NiNntp::wxStringPtr null;
        m_date = null;
        m_msgid = null;
        m_refs = null;
        m_xref = null;
    }
    bool OverviewPlus::IsOk () const
    {
        return ( m_from.get() != NULL && m_subject.get() != NULL );
    }
    OverviewPlus& OverviewPlus::operator = ( const NiNntp::Overview& ov )
    {
        m_father = 0;
        m_hmsgid = NiHash (ov.GetMessageID());
        Overview::operator = (ov);
        return *this;
    }
    OverviewPlus& OverviewPlus::operator = ( const OverviewPlus& ov )
    {
        m_hmsgid = ov.m_hmsgid;
        m_father = ov.m_father;
        Overview::operator = (ov);
        return *this;
    }

    ReferencesParser::ReferencesParser ( const wxString& refs )
        : m_refs ( &refs )
    {
        m_vector.reserve (30);
        Parser (refs);
    }
    ReferencesParser::ReferencesParser ()
        : m_refs (NULL)
    {}
    ReferencesParser::~ReferencesParser ()
    {}
    bool ReferencesParser::operator () ( const wxString& refs )
    {
        Parser (refs);
        m_refs = &refs;
        return ( m_vector.size () != 0 );
    }
    void ReferencesParser::Parser ( const wxString& refs )
    {
        const wxChar* bptr = NULL, *ptr = NULL, *eptr = NULL;
        bptr = ptr = refs.c_str();
        eptr = bptr + refs.Len ();
        int start = -1, end = -1;
        m_vector.clear ();

        for ( ; ptr < eptr; ++ptr )
        {
            if ( *ptr == wxT('<') )
            {
                start = ptr - bptr;
            }
            else if ( *ptr == wxT('>') && start != -1 )
            {
                end = ptr - bptr + 1;
                m_vector.push_back ( std::make_pair ( start, end ) );
                start = end = -1;
            }
        }
    }
    wxString ReferencesParser::GetItem ( const size_t& idx ) const
    {
        wxASSERT ( idx < m_vector.size () && m_refs != NULL );
        size_t start, end;
        start   = m_vector [ idx ].first;
        end     = m_vector [ idx ].second;
        return m_refs->Mid ( start, end - start );
    }
    NiUtils::HASH ReferencesParser::GetItemHash ( const size_t& idx ) const
    {
        return NiUtils::NiHash (GetItem(idx));
    }
    size_t ReferencesParser::Count () const
    {
        return  m_vector.size();
    }

    ArticleDB::ArticleDB ( /*DbCache& cache */)
        : m_db ( new wxSQLite3Database ), m_lastid (0)//, DbCacher (cache)
    {
    }
    ArticleDB::~ArticleDB ()
    {
        ClearOVMap ();
    }
    //!  OV table:
    //!  ID, Subject, From, Date, MessageID, References, Bytes, Lines, XRef.
    //!  Ex: ID_Hash, MessageID_Hash, LastReferncesHash, Status
    void ArticleDB::CreateOVTable ()
    { 
        if ( m_db.get () != NULL ) 
        {
            wxString sql;
            sql << wxT("CREATE TABLE IF NOT EXISTS OVTable ( ")
                << wxT(" _Id INTEGER,")
                << wxT(" _Father INTEGER,")
                << wxT(" _MIDHash INTEGER,")
                << wxT(" _Subject TEXT,")
                << wxT(" _From TEXT,")
                << wxT(" _Date TEXT,")
                << wxT(" _MessageID TEXT,")
                << wxT(" _References TEXT,")
                << wxT(" _Bytes INTEGER,")
                << wxT(" _Lines INTEGER,")
                << wxT(" _XRef TEXT")
                << wxT(" );\n");
            sql << wxT("CREATE INDEX IF NOT EXISTS OVTableMIDHash ON OVTable(_MIDHash);\n");
            m_db->ExecuteUpdate (sql);
        }
    }
    void ArticleDB::CreateArticleTable ()
    {
        if ( m_db.get() != NULL )
        {
            wxString sql;
            sql << wxT("CREATE TABLE IF NOT EXISTS ArticleTable (")
                << wxT("_MIDHash INTEGER, _Article TEXT );\n")
                << wxT("CREATE INDEX IF NOT EXISTS ArticleTableIndex ON ArticleTable (_MIDHash);\n");
            m_db->ExecuteUpdate (sql);
        }
    }
    void ArticleDB::CreateSubjectMapTable ()
    {
        if ( m_db.get () != NULL )
        {
            wxString sql;
            sql << wxT("CREATE TABLE IF NOT EXISTS SubjectTable ")
                << wxT("(_SubjectHash INTEGER, _MessageIdHash INTEGER);")
                << wxT("CREATE INDEX IF NOT EXISTS SubjectIndex ON SubjectTable")
                << wxT("(_SubjectHash);");
            m_db->ExecuteUpdate (sql);
        }
        
    }
    bool ArticleDB::Open ( const wxString& dbname )
    {
        if ( m_db.get() )
        {
            try
            {
                m_db->Open ( dbname );
                CreateOVTable ();
                CreateArticleTable ();
                CreateSubjectMapTable ();
                LoadOV ();
                Thread ();
                return true;
            }
            catch ( wxSQLite3Exception& e )
            {
                wxLogDebug (e.GetMessage());
                return false;
            }
        }
        return false;
    }
    bool ArticleDB::AddOV ( const NiNntp::Overview& ov )
    {
        OverviewPlus* ovp = new OverviewPlus;
        if ( ovp )
        {
            *ovp = ov;
            NiUtils::HASH hash = ovp->GetMessageIdHash ();
            if ( !HasOV ( hash ) )
            {
                SetFather ( *ovp );
                InsertOV ( hash, ovp );
                BufferOV ( *ovp );
                ovp->Mini ();
                return true;
            }
            else
            {
                delete ovp;
                return true;
            }
        }
        return false;
    }
    bool ArticleDB::HasOV ( const NiUtils::HASH& hash )
    {
        OverviewPlusPMap::iterator itr = m_ovmap.find ( hash );
        return itr != m_ovmap.end ();
    }
    bool ArticleDB::Commit ()
    {
        if ( m_buff.get () != NULL )
        {
            if ( m_db.get() && m_buff->Len() != 0 )
            {
                m_db->Begin ();
#ifdef __WXDEBUG__
                try
                {
                    m_db->ExecuteUpdate ( *m_buff );
                }
                catch ( wxSQLite3Exception& e )
                {
                    wxLogDebug (e.GetMessage ());
                }
#else
                m_db->ExecuteUpdate ( *m_buff );
#endif
                m_db->Commit ();
            }
            m_buff = std::auto_ptr <wxString> (NULL);
            return true;
        }
        return false;
    }
    unsigned int ArticleDB::LastId ( const unsigned int& first, const unsigned int& last ) const
    {
        if ( first == 0 || last == 0 )
        {
            return m_lastid;
        }
        return wxMin ( wxMax ( m_lastid+1, first ) , last );
    }
    bool ArticleDB::BufferOV ( const OverviewPlus& ov )
    {
        if ( m_buff.get () == NULL )
        {
            m_buff = std::auto_ptr <wxString> ( new wxString );     
        }
        if ( m_buff.get () != NULL )
        {
            wxString from = ov.GetFrom(), subject = ov.GetSubject ();
            from.Replace (wxT("'"), wxT("''"), true );
            subject.Replace (wxT("'"), wxT("''"), true );
            *m_buff << wxT("INSERT INTO OVTable VALUES ( ")
                    << ov.GetId ()               << wxT(", ")
                    << ov.GetFatherHash ()              << wxT(", ")    
                    << ov.GetMessageIdHash ()           << wxT(", ")
                    << wxT("'") << subject              << wxT("', ")
                    << wxT("'") << from                 << wxT("', ")
                    << wxT("'") << ov.GetDate ()        << wxT("', ")
                    << wxT("'") << ov.GetMessageID ()   << wxT("', ")
                    << wxT("'") << ov.GetReferences ()  << wxT("', ")
                    << ov.GetBytes ()                   << wxT(", ")
                    << ov.GetLines ()                   << wxT(", ")
                    << wxT("'") << ov.GetXRef ()        << wxT("' );\n");
        }
        return false;
    }
    bool ArticleDB::InsertOV ( const NiUtils::HASH& hash, OverviewPlus* ov )
    {
        m_ovmap.insert ( std::make_pair ( hash, ov ) );
        m_lastid = wxMax ( ov->GetId (), m_lastid );
        return true;
    }
    void ArticleDB::SetFather ( OverviewPlus& ov )
    {
        NiUtils::HASH thash;
        ReferencesParser refsparser ( ov.GetReferences () );
        size_t count, pos;
        pos = count = refsparser.Count ();
        bool has = false;
        NiField::Field field;
        NiUtils::HASH hash = 0, subjecthash = 0;
        field (ov.GetSubject());
        subjecthash = field.DataStripReHash ();
        wxString sql;

        if ( pos != 0 )
        {
            while (  pos != 0 )
            {
                thash = refsparser.GetItemHash (--pos);
                if ( HasOV ( thash ) )
                {
                    ov.SetFatherHash ( thash );
                    has = true;
                    break;
                }
            }
            if ( !has && m_db.get() != NULL )
            {
                sql.Clear ();
                sql << wxT("SELECT _MessageIdHash FROM SubjectTable ")
                    << wxT("WHERE _SubjectHash = ")
                    << subjecthash 
                    << wxT(";");
                wxSQLite3ResultSet result = m_db->ExecuteQuery (sql);
                if ( result.NextRow () )
                {
                    hash = result.GetInt (0);
                    ov.SetFatherHash (hash);                    
                    has = true;
                }
            }
        }
        if ( m_db.get() != NULL && ( !has || count == 0 ) )
        {
            sql.Clear ();
            sql << wxT("INSERT INTO SubjectTable VALUES ( ")
                << subjecthash 
                << wxT(", ")
                << ov.GetMessageIdHash ()
                << wxT(");");
            m_db->ExecuteUpdate (sql);
        }
    }
    void ArticleDB::LoadOV ()
    {
        wxString sql (wxT("SELECT _Id, _MIDHash, _Father FROM OVTable;"));
        if ( m_db.get() && m_db->TableExists ( wxT("OVTable") ) )
        {
            NiUtils::TimeReport tr;
            tr.Start ();
            wxSQLite3ResultSet result = m_db->ExecuteQuery (sql);
            tr.End ();
            tr (wxT("Query"));
            size_t count = 0;
            unsigned int id;
            NiUtils::HASH msgid, father;
            OverviewPlus *ov = NULL;
            tr.Start ();
            while ( result.NextRow () )
            {
                id      = result.GetInt (0);
                msgid   = result.GetInt (1); 
                father  = result.GetInt (2);
                ov = new OverviewPlus ( id, msgid, father );
                if ( ov != NULL )
                {
                    InsertOV (msgid, ov);
                }
                ++count;
            }
            tr.End ();
            tr (wxT("Load result."));
            wxLogDebug (wxT("Count: %d %d"), count, 
                        sizeof (OverviewPlus) * count + sizeof (NiUtils::HASH) * count );
        }
    }
    struct OF_ClearOVMap 
    {
        public:
            operator () ( OverviewPlusPMap::value_type& o )
            {
                if ( o.second != NULL )
                {
                    delete o.second;
                    o.second = NULL;
                }
            }
    };
    void ArticleDB::ClearOVMap ()
    {
        if ( !m_ovmap.empty() )
        {
            std::for_each ( m_ovmap.begin(), m_ovmap.end(), OF_ClearOVMap());
        }
    }
    void ArticleDB::Thread ()
    {
        OverviewPlusPMap::iterator bi, ei, pi;
        bi = m_ovmap.begin ();
        ei = m_ovmap.end ();
        NiUtils::HASH father = 0;
        size_t count = 0;
        for ( ; bi != ei; ++bi )
        {
            father = bi->second->GetFatherHash ();
            if ( father == 0 )
            {
                ++count;
                m_tree.NodeLink (bi->second);
            }
            else
            {
                pi = m_ovmap.find ( father );
                if ( pi != m_ovmap.end () )
                {
                    m_tree.NodeLink ( pi->second, bi->second );
                }
            }
        }
        wxLogDebug (wxT("Build tree %d node."), m_tree.NodeCount ());
    }
    void ArticleDB::PreLoad ( OverviewPlus* ov )
    {
        wxASSERT ( ov != NULL );
        wxString sql;
        sql << wxT("SELECT _Subject, _From FROM OVTable WHERE _MIDHash = ")
            << ov->GetMessageIdHash () << wxT(";");
        if ( m_db.get() != NULL )
        {
            wxSQLite3ResultSet result = m_db->ExecuteQuery (sql);
            if ( result.NextRow () )
            {
                ov->SetSubject  (result.GetAsString(0));
                ov->SetFrom     (result.GetAsString(1));
            }
        }
    }
    size_t ArticleDB::Depth ( const size_t& idx )
    {
        NiTree::Node *node = m_tree.At (idx);
        wxASSERT ( node != NULL );
        return node->Depth ();
    }
    wxString ArticleDB::GetField ( const size_t& idx, const FieldType& field ) 
    {
        wxString result;
        wxLogDebug (wxT("GetField %d %d %d"), idx, m_ovmap.size, m_tree.NodeCount());
        wxASSERT (idx >= 0 && idx < m_ovmap.size () && idx < m_tree.NodeCount() );
        OverviewPlus *ov = static_cast <OverviewPlus*> (m_tree.At (idx));
        wxASSERT ( ov != NULL );
        if ( !ov->IsOk () )
            PreLoad (ov);
        if ( field == ID )
        {
            result = wxString::Format (wxT("%d"), ov->GetId());
        }
        else if ( field == SUBJECT )
        {
            result = ov->GetSubject ();
        }else if ( field == FROM )
        {
            result = ov->GetFrom ();
        }
        return result;
    }
    size_t ArticleDB::Count () 
    {
        return m_ovmap.size ();
    }
    void ArticleDB::Test ()
    {
    }
    ////////////////////////////////////////////////////////////////////////////////

    ArticleDBPtrMap*    ArticleDBManager::m_articledb = NULL;
    wxMutex             ArticleDBManager::m_lock;
    bool ArticleDBManager::OpenManager ()
    {
        wxMutexLocker locker ( m_lock );
        if ( m_articledb == NULL )
        {
            m_articledb = new ArticleDBPtrMap;
        }
        return true;
    }
    bool ArticleDBManager::CloseManager ()
    {
        wxMutexLocker locker ( m_lock );
        if ( m_articledb != NULL )
        {
            ArticleDBPtrMap::iterator bi, ei;
            bi = m_articledb->begin();
            ei = m_articledb->end ();
            for ( ; bi != ei; ++bi )
            {
                delete bi->second;
            }
            delete m_articledb;
            m_articledb = NULL;
            return true;
        }
        return false;
    }
    ArticleDB* ArticleDBManager::GetDB ( const NiGroup::SubGroup& group )
    {
        wxMutexLocker locker ( m_lock );
        ArticleDB* db = NULL;
        if ( m_articledb == NULL ) return db;
        wxString groupname;
        groupname << wxString::From8BitData(group.GetServer().ToUTF8())
                  << wxString::From8BitData(group.Name ().ToUTF8());

        NiUtils::HASH hash = NiUtils::NiHash (groupname);
        ArticleDBPtrMap::iterator itr = m_articledb->find ( hash );
        if ( itr == m_articledb->end () )
        {
            db =  new ArticleDB;
            wxASSERT ( db != NULL );
            if ( ! db->Open (NiConfig::GroupArticleDataFile (group)) )
            {
                wxLogDebug (wxT("Open ArticleDB error."));
                delete db;
                db = NULL;
            }
            else
            {
                m_articledb->insert ( std::make_pair (hash, db) );
            }
        }
        else
        {
            db = itr->second;
        }
        return db;
    };
}
