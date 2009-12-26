#include "news.hpp"
namespace News
{
  ListGroup::ListGroup()
  {
  }
  ListGroup::~ListGroup ()
  {
  }
  void ListGroup::parse_list ( const std::string& list )
  {
    std::string::const_iterator ii = list.begin();
    std::string line;
    while (1)
      {
	std::string group, first, last, post;
	for (; ii < list.end(); ++ii )
	  {
	    if ( *ii == '\r' ) continue;
	    if ( *ii == ' ' ) break;
	    group += *ii;
	  }
	if (ii < list.end ())++ii;
	for (; ii < list.end(); ++ii )
	  {
	    if ( *ii == ' ') break;
	    first += *ii;
	  }
	if (ii < list.end())++ii;
	for (; ii < list.end(); ++ii )
	  {
	    if ( *ii == ' ') break;
	    last += *ii;
	  }
	if (ii < list.end() )++ii;
	for ( ; ii < list.end (); ++ii )
	  {
	    if ( *ii == '\n' ) break;
	    post += *ii;
	  }
	if ( ii < list.end()) ++ii;
	//std::cout << group << "\t" << first << "\t" << last << "\t"
	//<< post << endl;
	istringstream sf (first), sl (last);
	id_type id_first = 0, id_last = 0;
	sf >> id_first;
	sl >> id_last;
	//std::cout << group << " " << id_first << " " << id_last << endl;
	GroupHash::iterator ip;
	ip = m_groups.find(group);
	if ( ip == m_groups.end() )
	  {
	    Group struct_group;
	    struct_group.group = group;
	    struct_group.first = id_first;
	    struct_group.last = id_last;
	    struct_group.post = (post == "y" ? true : false);
	    m_groups[group] = struct_group;
	  }
	else
	  {
	    ip->second.group = group;
	    ip->second.first = id_first;
	    ip->second.last = id_last;
	    ip->second.post = ( post == "y" ? true : false );
	  }
	if ( ii == list.end ())break;
      }
  }
  void ListGroup::parse_desc ( const std::string& list )
  {
    std::string::const_iterator li = list.begin ();
    while (1)
      {
	std::string group, desc;
	for (; li < list.end (); ++li )
	  {
	    if ( *li == ' ' || *li == '\t' )break;
	    group += *li;
	  }
	if ( li < list.end() ) ++li;
	for (; li < list.end (); ++li )
	  {
	    if ( *li == '\n' ) break;
	    desc += *li;
	  }
	if ( li < list.end() ) ++li;
	//std::cout << ":" << group << " || " << desc << endl;
	GroupHash::iterator gi;
	gi = m_groups.find (group);
	if ( gi == m_groups.end() )
	  {
	    Group struct_group;
	    struct_group.group = group;
	    struct_group.desc = desc;
	    m_groups [group] = struct_group;
	  }
	else
	  {
	    gi->second.desc = desc;
	  }
	if ( li == list.end ()) break;
      }
  }
  void ListGroup::print ()
  {
    GroupHash::iterator ii = m_groups.begin();
    for ( ; ii != m_groups.end(); ++ii )
      std::cout << ii->first << " " << ii->second.first
		<< " " << ii->second.last
		<< " => " << ii->second.desc << " |"
		<<endl;
  }
  //alt.adoption.agency.losninos
  News::News ( unsigned short queue_size, bool debug)
    : m_db(debug), m_queue_size(queue_size)
  {
    m_pushqueue.reserve (m_queue_size+1);
    //std::cout << "News"<<endl;
  }
  News::~News ()
  {
    m_newsrc.save (m_newsrcfilename);
    m_db.close ();
  }
  void News::check_table ()
  {
    if ( !m_db.hastable ("SERVER"))
      {
	std::ostringstream sql;
	sql << "CREATE TABLE SERVER (_SERVER varchar(20),"
	    << " _PORT INTERGER, _USER varchar(20), _PASS varchar(20))";
	m_db.exec (sql.str());
      }
    if ( !m_db.hastable ("SUBSCRIBE_GROUP"))
      {
	std::ostringstream sql;
	sql << "CREATE TABLE SUBSCRIBE_GROUP "
	    << "(_GROUP varchar(30), _ALIASES varchar(30), "
	    << "_UNREAD Integer, _LAST Integer, _TABLE varchar(30),"
	    << " _SERVER varchar(30), _TOPICS varchar(20))";
	m_db.exec (sql.str());
      }
  }
  bool News::list_sgroup_with_server (SGroupList& sg,const std::string& server)
  {
    if ( server.empty() ) return false;
    std::string cserver;
    std::ostringstream sql;
    SGroup sgroup;
    strip_server_name ( server, cserver );
    sql << "SELECT _GROUP, _ALIASES, _UNREAD, _LAST, _TABLE, _TOPICS FROM"
	<< " SUBSCRIBE_GROUP WHERE _SERVER = '"
	<< SQLite3::qoute_string(cserver) << "';";
    if ( m_db.execute (sql.str()))
      {
	while (m_db.nextrow())
	  {
	    if (m_db.count_of_column () == 6 )
	      {
		sgroup.clear ();
		sgroup.server = cserver;
		if (m_db.type_of_column(0) == SQLITE_TEXT)
		  sgroup.group = m_db.field_text(0);
		if (m_db.type_of_column(1) == SQLITE_TEXT)
		  sgroup.alias = m_db.field_text(1);
		if (m_db.type_of_column(2) == SQLITE_INTEGER)
		  sgroup.unread = m_db.field_int(2);
		if (m_db.type_of_column(3) == SQLITE_INTEGER)
		  sgroup.last = m_db.field_int(3);
		if (m_db.type_of_column(4) == SQLITE_TEXT)
		  sgroup.table = m_db.field_text(4);
		if (m_db.type_of_column(5) == SQLITE_TEXT)
		  sgroup.topics = m_db.field_text(5);
		//sgroup.print ();
		sg.push_back (sgroup);
	      }
	  }
	m_db.finish ();
	return true;
      }
    return false;
  }
  bool News::has_group  (const SGroup& sg )
  {
    return has_group(sg.server, sg.group);
  }
  bool News::list_overview_with_where ( OverviewList& ovl, const SGroup&sg,
					const std::string& where, bool cf)
  {
    if ( sg.table.empty() ) return false;
    if (cf) ovl.clear();
    std::string table;
    table = sg.table + "_OV";
    if ( !m_db.hastable(table) ) return false;
    std::ostringstream os;
    os << "SELECT _ID, _SUBJECT, _FROM, _DATE, _MESSAGEID, _REFERENCES, _REFC,"
       << "_BYTES, _LINES, _XREF, _STATUS FROM "
       << SQLite3::qoute_string(table) << "WHERE " << where
       << " ORDER BY _REFC,_TIMESTAMP;";
    if ( m_db.execute(os.str()))
      {
	OverView ov;
	while (m_db.nextrow ())
	  {
	    if ( m_db.count_of_column () == 11 )
	      {
		ov.clear ();
		if ( m_db.type_of_column (0) == SQLITE_INTEGER )
		  ov.id = (unsigned int)m_db.field_int(0);
		if ( m_db.type_of_column (1) == SQLITE_TEXT )
		  ov.subject = m_db.field_text(1);
		if ( m_db.type_of_column (2) == SQLITE_TEXT )
		  ov.from = m_db.field_text(2);
		if ( m_db.type_of_column (3) == SQLITE_TEXT )
		  ov.date = m_db.field_text(3);
		if ( m_db.type_of_column (4) == SQLITE_TEXT )
		  ov.messageid = m_db.field_text(4);
		if ( m_db.type_of_column (5) == SQLITE_TEXT )
		  ov.references = m_db.field_text(5);
		if ( m_db.type_of_column (6) == SQLITE_INTEGER )
		  ov.ref_count = m_db.field_int(6);
		if ( m_db.type_of_column (7) == SQLITE_INTEGER )
		  ov.bytes = m_db.field_int(7);
		if ( m_db.type_of_column (8) == SQLITE_INTEGER )
		  ov.lines = m_db.field_int(8);
		if ( m_db.type_of_column (9) == SQLITE_TEXT )
		  ov.xref = m_db.field_text(9);
		if ( m_db.type_of_column (10) == SQLITE_INTEGER )
		  ov.status = (NewsStatus)m_db.field_int(10);
		ovl.push_back (ov);
	      }
	  }
	m_db.finish();
	return true;
      }
    return false;
  }
  bool News::del_overview ( const SGroup& sg, const OverviewList& ovl )
  {
    if ( sg.table.empty() || sg.table.find(".") != std::string::npos)
      return false;
    OverviewList::const_iterator ovi;
    std::string sql, ov_table, pool_table;
    ov_table = sg.table + "_OV"; pool_table = sg.table + "_POOL";
    if ( !m_db.hastable (ov_table) || !m_db.hastable(pool_table))
      return false;
    sql += "BEGIN;";
    for ( ovi = ovl.begin (); ovi < ovl.end(); ++ovi )
      {
	std::ostringstream osql;
	osql << "DELETE FROM " << ov_table << " WHERE _MESSAGEID = '"
	     << SQLite3::qoute_string((*ovi).messageid)
	     << "' AND _ID = " << (*ovi).id << ";"
	     << "DELETE FROM " << pool_table << " WHERE _MESSAGEID = '"
	     << SQLite3::qoute_string((*ovi).messageid) << "';";
	sql += osql.str();
      }
    std::ostringstream usql;
    usql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = (SELECT _UNREAD FROM "
	 << "SUBSCRIBE_GROUP WHERE _GROUP = '"
	 << SQLite3::qoute_string (sg.group)
	 <<  "' AND _SERVER = '"
	 << SQLite3::qoute_string (sg.server)
	 << "') - " << ovl.size()
	 << " WHERE _GROUP = '"
	 << SQLite3::qoute_string (sg.group) << "' AND _SERVER = '"
	 << SQLite3::qoute_string (sg.server) << "';";
    sql += usql.str();
    sql += "COMMIT;";
    return m_db.exec (sql);
  }
  bool News::del_overview ( const SGroup& sg, const OverView& ov )
  {
    if ( sg.table.empty () || sg.table.find(".") != std::string::npos )
      return false;
    std::string ov_table = sg.table, pool_table = sg.table;
    ov_table += "_OV";  pool_table += "_POOL";
    if ( !m_db.hastable (ov_table) || !m_db.hastable (pool_table))
      return false;
    std::ostringstream osql;
    osql << "BEGIN;"
	 << "DELETE FROM " << ov_table << " WHERE _MESSAGEID = '"
	 << SQLite3::qoute_string(ov.messageid) << "' AND _ID =" << ov.id
	 << ";" << "DELETE FROM " << pool_table << " WHERE _MESSAGEID = '"
	 << SQLite3::qoute_string(ov.messageid) << "';"
	 << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = "
	 << " (SELECT _UNREAD FROM SUBSCRIBE_GROUP WHERE _GROUP = '"
	 << SQLite3::qoute_string(sg.group) << "' AND _SERVER = '"
	 << SQLite3::qoute_string(sg.server) << "') - 1 WHERE "
	 << " _SERVER = '" << SQLite3::qoute_string(sg.server) << "' AND "
	 << " _GROUP = '" << SQLite3::qoute_string(sg.group) << "';"
	 << "COMMIT;";
    return m_db.exec (osql.str());
  }
  bool News::expire ( const unsigned int& year, const unsigned int& month,
		      const unsigned int& day )
  {
    Date::DateTime expire_date (year, month, day);
    SGroupList sgl;
    SGroupList::iterator sgiter;
    OverviewList ovl;
    list_sgroups ( sgl );
    for ( sgiter = sgl.begin(); sgiter < sgl.end(); ++sgiter )
      {
	sgiter->print();
// 	list_overview_with_status ( ovl, *sgiter, NewsUnread, true );
// 	std::cout << "Size of " << sgiter->group << " " << ovl.size () << endl;
	list_overview_with_status ( ovl, *sgiter, NewsBozo, true );
	std::cout << "Size of " << sgiter->group << " " << ovl.size () << endl;
	std::cout << "Clean bozo mark." << endl;
	del_overview ( *sgiter, ovl);
	//list_overview_with_sgroup ( ovl, *sgiter, NewsSpam );
	//list_overview_with_sgroup ( ovl, *sgiter, NewsHate );

      }
  }
  bool News::list_overview (OverviewList& ovl, const SGroup& sg, bool cf)
  {
    if ( sg.table.empty() ) return false;
    if (cf) ovl.clear();
    std::string table;
    table = sg.table + "_OV";
    if ( !m_db.hastable(table) ) return false;
    std::ostringstream os;
    os << "SELECT _ID, _SUBJECT, _FROM, _DATE, _MESSAGEID, _REFERENCES, _REFC,"
       << "_BYTES, _LINES, _XREF, _STATUS FROM "
       << SQLite3::qoute_string(table)
       << " ORDER BY _REFC,_TIMESTAMP;";
    if ( m_db.execute(os.str()))
      {
	OverView ov;
	while (m_db.nextrow ())
	  {
	    if ( m_db.count_of_column () == 11 )
	      {
		ov.clear ();
		if ( m_db.type_of_column (0) == SQLITE_INTEGER )
		  ov.id = (unsigned int)m_db.field_int(0);
		if ( m_db.type_of_column (1) == SQLITE_TEXT )
		  ov.subject = m_db.field_text(1);
		if ( m_db.type_of_column (2) == SQLITE_TEXT )
		  ov.from = m_db.field_text(2);
		if ( m_db.type_of_column (3) == SQLITE_TEXT )
		  ov.date = m_db.field_text(3);
		if ( m_db.type_of_column (4) == SQLITE_TEXT )
		  ov.messageid = m_db.field_text(4);
		if ( m_db.type_of_column (5) == SQLITE_TEXT )
		  ov.references = m_db.field_text(5);
		if ( m_db.type_of_column (6) == SQLITE_INTEGER )
		  ov.ref_count = m_db.field_int(6);
		if ( m_db.type_of_column (7) == SQLITE_INTEGER )
		  ov.bytes = m_db.field_int(7);
		if ( m_db.type_of_column (8) == SQLITE_INTEGER )
		  ov.lines = m_db.field_int(8);
		if ( m_db.type_of_column (9) == SQLITE_TEXT )
		  ov.xref = m_db.field_text(9);
		if ( m_db.type_of_column (10) == SQLITE_INTEGER )
		  ov.status = (NewsStatus)m_db.field_int(10);
		if ( !m_newsrc.ismark (sg.group, ov.id))
		  ovl.push_back (ov);
	      }
	  }
	m_db.finish();
	return true;
      }
    return false;
  }
  bool News::mark_as_bozo ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status ( sg, id, NewsBozo );
  }
  bool News::mark_as_bozo ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status ( sg, ov, NewsBozo );
  }
  bool News::mark_as_spam ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status ( sg, id, NewsSpam );
  }
  bool News::mark_as_spam ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status ( sg, ov, NewsSpam );
  }
  bool News::mark_as_hate ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status ( sg, id, NewsHate );
  }
  bool News::mark_as_hate ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status ( sg, ov, NewsHate );
  }
  bool News::mark_as_unread ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status ( sg, id, NewsUnread );
  }
  bool News::mark_as_unread ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status ( sg, ov, NewsUnread );
  }
  bool News::mark_as_read ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status ( sg, id, NewsRead);
  }
  bool News::mark_as_read ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status ( sg, ov, NewsRead);
  }
  bool News::mark_as_tick ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status (sg, id, NewsTick );
  }
  bool News::mark_as_tick ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status ( sg, ov, NewsTick );
  }
  bool News::mark_as_dor ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status ( sg, id, NewsDor);
  }
  bool News::mark_as_dor ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status (sg, ov, NewsDor);
  }
  bool News::mark_as_fav ( const SGroup& sg, const id_type& id )
  {
    return mark_as_status ( sg, id, NewsFav);
  }
  bool News::mark_as_fav ( const SGroup& sg, const OverView& ov )
  {
    return mark_as_status (sg, ov, NewsFav);
  }
  bool News::mark_as_status ( const SGroup& sg, const OverView& ov,
			      const NewsStatus& status )
  {
    if (sg.group.empty () || sg.server.empty() || sg.table.empty())
      return false;
    if ( status == NewsUnread )
      {
	m_newsrc.unmark (sg.group, ov.id);
	return true;
      }
    else if ( status == NewsRead )
      {
	m_newsrc.mark (sg.group, ov.id);
	return true;
      }
    std::string table = sg.table;
    table += "_OV";
    if ( !m_db.hastable(table)) return false;
    std::ostringstream sql;
    sql << "BEGIN;"
	<< "UPDATE " << table << " SET _STATUS = " << status
	<< " WHERE _MESSAGEID = '" << SQLite3::qoute_string(ov.messageid)
	<< "' AND _ID = " << ov.id << ";"
	<< "UPDATE SUBSCRIBE_GROUP SET _UNREAD = (SELECT COUNT(_ID) FROM "
	<< table
	<< " WHERE _STATUS = " << NewsUnread << ") WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group) << "' AND _SERVER = '"
	<< SQLite3::qoute_string(sg.server) << "';";
    sql  << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = ( SELECT _UNREAD FROM "
	 << " SUBSCRIBE_GROUP WHERE _GROUP = '"
	 << SQLite3::qoute_string(sg.group)
	 << "' AND _SERVER = '"
	 << SQLite3::qoute_string(sg.server)
	 << "') " << ( status == NewsUnread ? "+" : "-") << " 1 "
	 << "WHERE _GROUP = '"
	 << SQLite3::qoute_string(sg.group)
	 << "' AND _SERVER = '"
	 << SQLite3::qoute_string(sg.server)
	 << "';";
    sql << "COMMIT;";
    return m_db.exec (sql.str());
  }
  bool News::mark_as_status ( const SGroup& sg, const id_type& id,
				  const NewsStatus& status )
  {
    if (sg.group.empty () || sg.server.empty() || sg.table.empty())
      return false;
    if ( status == NewsUnread )
      {
	m_newsrc.unmark (sg.group, id);
	return true;
      }
    else if ( status == NewsRead )
      {
	m_newsrc.mark (sg.group, id);
	return true;
      }
    std::string table = sg.table;
    table += "_OV";
    if ( !m_db.hastable(table)) return false;
    std::ostringstream sql;
    sql << "BEGIN;"
	<< "UPDATE " << table << " SET _STATUS = " << status
	<< " WHERE _ID = " << id << ";";
    sql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = ( SELECT _UNREAD FROM "
	<< " SUBSCRIBE_GROUP WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group)
	<< "' AND _SERVER = '"
	<< SQLite3::qoute_string(sg.server)
	<< "') " << (status == NewsUnread ? "+" : "-") << " 1 "
	<< " WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group)
	<< "' AND _SERVER = '"
	<< SQLite3::qoute_string(sg.server)
	<< "';";
    
// 	<< "UPDATE SUBSCRIBE_GROUP SET _UNREAD = (SELECT COUNT(_ID) FROM "
// 	<< table
// 	<< " WHERE _STATUS = " << NewsUnread << ") WHERE _GROUP = '"
// 	<< SQLite3::qoute_string(sg.group) << "' AND _SERVER = '"
// 	<< SQLite3::qoute_string(sg.server) << "';"
    sql << "COMMIT;";
  
    return m_db.exec (sql.str());
  }
  bool News::mark_as_status ( const SGroup& sg, const OverView& ov )
  {
    if (sg.group.empty () || sg.server.empty() || sg.table.empty())
      return false;
    if ( ov.status == NewsUnread )
      {
	m_newsrc.unmark ( sg.group, ov.id );
	return true;
      }
    else if ( ov.status == NewsRead )
      {
	m_newsrc.mark ( sg.group, ov.id );
	return true;
      }
    std::string table = sg.table;
    table += "_OV";
    if ( !m_db.hastable(table)) return false;
    std::ostringstream sql;
    sql << "BEGIN;"
	<< "UPDATE " << table << " SET _STATUS = " << ov.status
	<< " WHERE _MESSAGEID = '" << SQLite3::qoute_string(ov.messageid)
	<< "' AND _ID = " << ov.id << ";";
//     	<< "UPDATE SUBSCRIBE_GROUP SET _UNREAD = (SELECT COUNT(_ID) FROM "
// 	<< table
// 	<< " WHERE _STATUS = " << NewsUnread << ") WHERE _GROUP = '"
// 	<< SQLite3::qoute_string(sg.group) << "' AND _SERVER = '"
// 	<< SQLite3::qoute_string(sg.server) << "';"
    sql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = ( SELECT _UNREAD FROM "
	<< " SUBSCRIBE_GROUP WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group)
	<< "' AND _SERVER = '"
	<< SQLite3::qoute_string(sg.server)
	<< "') " << (ov.status == NewsUnread ? "+" : "-") << " 1 "
	<< "WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group)
	<< "' AND _SERVER = '"
	<< SQLite3::qoute_string(sg.server)
	<< "';";
    sql << "COMMIT;";
    
// 	<< "UPDATE SUBSCRIBE_GROUP _UNREAD = (SELECT COUNT(_ID) FROM "
// 	<< table
// 	<< " WHERE _STATUS = " << NewsUnread << "); COMMIT;";
    return m_db.exec (sql.str());
  }
  bool News::mark_as_status ( const SGroup& sg, const OverviewList& ovl)
  {
    if ( sg.group.empty() || sg.server.empty() || sg.table.empty())
      return false;
    if ( ovl.size() == 0 ) return true;
    std::string table = sg.table, sql;
    table += "_OV";
    if ( !m_db.hastable (table)) return false;
    sql += "BEGIN;";
    OverviewList::const_iterator oi;
    unsigned int unread_count = 0;
    for ( oi = ovl.begin(); oi < ovl.end(); ++oi )
      {
	if ( (*oi).status == NewsUnread )
	  {
	    //std::cout << "NewsUnread" << endl;
	    m_newsrc.unmark (sg.group, (*oi).id);
	    continue;
	  }
	else if ( (*oi).status == NewsRead )
	  {
	    //std::cout << "NewsRead" << endl;
	    m_newsrc.mark (sg.group, (*oi).id);
	    continue;
	  }
	if ( (*oi).status == NewsUnread ) ++unread_count;
	std::ostringstream osql;
	osql << "UPDATE " << table << " SET _STATUS = " << (*oi).status
	     << " WHERE _MESSAGEID = '"
	     << SQLite3::qoute_string((*oi).messageid)
	     << "' AND _ID = " << (*oi).id << ";";
	sql += osql.str();
      }
    if (unread_count)
      {
	std::ostringstream usql;
	usql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = ( SELECT _UNREAD FROM "
	     << " SUBSCRIBE_GROUP WHERE _GROUP = '"
	     << SQLite3::qoute_string(sg.group)
	     << "' AND _SERVER = '"
	     << SQLite3::qoute_string(sg.server)
	     << "') " << (ovl.size() == unread_count ? " + " : " - ")
	     << (ovl.size()==unread_count?unread_count:ovl.size()-unread_count)
	     << " WHERE _GROUP = '"
	     << SQLite3::qoute_string(sg.group)
	     << "' AND _SERVER = '"
	     << SQLite3::qoute_string(sg.server)
	     << "';";
	sql += usql.str();
      }
//     usql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = (SELECT COUNT(_ID) FROM "
// 	 << table
// 	 << " WHERE _STATUS = " << NewsUnread << ") WHERE _GROUP = '"
// 	 << SQLite3::qoute_string(sg.group) << "' AND _SERVER = '"
// 	 << SQLite3::qoute_string(sg.server) << "';"
// 	 << "COMMIT;";
    sql += "COMMIT;";
    return m_db.exec (sql);
  }
  unsigned int News::unread_of_sgroup ( const SGroup& sg )
  {
    if (sg.group.empty() || sg.server.empty()) return 0;
    std::ostringstream sql;
#if 0
    sql << "SELECT COUNT(_ID) FROM " << sg.table 
	<< "_OV WHERE _STATUS = 0;";
#else
    sql << "SELECT _UNREAD FROM SUBSCRIBE_GROUP WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group)
	<< "' AND _SERVER = '" << SQLite3::qoute_string(sg.server)
	<< "';";
#endif
    if ( m_db.execute (sql.str()))
      {
	if (m_db.nextrow () && m_db.count_of_column () == 1 &&
	    m_db.type_of_column (0) == SQLITE_INTEGER )
	  {
	    unsigned int c = 0;
	    c = m_db.field_int(0);
	    m_db.finish ();
	    return c;
	  }
      }
    return 0;
  }
  bool News::mark_as_status ( const SGroup& sg, const OverviewList& ovl,
			      const NewsStatus& status )
  {
    if ( sg.group.empty() || sg.server.empty() || sg.table.empty())
      return false;
    if ( ovl.size() == 0 ) return true;
    std::string table = sg.table, sql;
    table += "_OV";
    if ( !m_db.hastable(table)) return false;
    sql += "BEGIN;";
    OverviewList::const_iterator oi;
    for ( oi = ovl.begin(); oi < ovl.end(); ++oi )
      {
	if (status == NewsUnread )
	  {
	    m_newsrc.unmark (sg.group, (*oi).id);
	    continue;
	  }
	else if ( status == NewsRead )
	  {
	    m_newsrc.mark ( sg.group, (*oi).id);
	    continue;
	  }
	std::ostringstream osql;
	osql << "UPDATE " << table << " SET _STATUS = " << status
	     << " WHERE _MESSAGEID = '"
	     << SQLite3::qoute_string((*oi).messageid)
	     << "' AND _ID = " << (*oi).id << ";";
	sql += osql.str();
      }
    std::ostringstream usql;
    usql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = (SELECT COUNT(_ID) FROM "
	 << table
	 << " WHERE _STATUS = " << NewsUnread << ") WHERE _GROUP = '"
	 << SQLite3::qoute_string(sg.group) << "' AND _SERVER = '"
	 << SQLite3::qoute_string(sg.server) << "';"
	 << "COMMIT;";
    sql += usql.str();
    sql += "COMMIT;";
    return m_db.exec (sql);
  }
  bool News::list_overview ( OverviewList& ovl, const std::string& group,
			     bool cf)
  {
    if ( group.empty() ) return false;
    if (cf) ovl.clear ();
    OverView ov;
    std::ostringstream os;
    std::string table;
    table = group;
    strip_dot(table);
    os << "SELECT _ID, _SUBJECT, _FROM, _DATE, _MESSAGEID, _REFERENCES, _REFC,"
       << "_BYTES, _LINES, _XREF, _STATUS FROM "
       << SQLite3::qoute_string(table)
       << "_OV ORDER BY _REFC,_TIMESTAMP;";

    if ( m_db.execute(os.str()))
      {
	while (m_db.nextrow ())
	  {
	    if ( m_db.count_of_column () == 11 )
	      {
		ov.clear ();
		if ( m_db.type_of_column (0) == SQLITE_INTEGER )
		  ov.id = (unsigned int)m_db.field_int(0);
		if ( m_db.type_of_column (1) == SQLITE_TEXT )
		  ov.subject = m_db.field_text(1);
		if ( m_db.type_of_column (2) == SQLITE_TEXT )
		  ov.from = m_db.field_text(2);
		if ( m_db.type_of_column (3) == SQLITE_TEXT )
		  ov.date = m_db.field_text(3);
		if ( m_db.type_of_column (4) == SQLITE_TEXT )
		  ov.messageid = m_db.field_text(4);
		if ( m_db.type_of_column (5) == SQLITE_TEXT )
		  ov.references = m_db.field_text(5);
		if ( m_db.type_of_column (6) == SQLITE_INTEGER )
		  ov.ref_count = m_db.field_int(6);
		if ( m_db.type_of_column (7) == SQLITE_INTEGER )
		  ov.bytes = m_db.field_int(7);
		if ( m_db.type_of_column (8) == SQLITE_INTEGER )
		  ov.lines = m_db.field_int(8);
		if ( m_db.type_of_column (9) == SQLITE_TEXT )
		  ov.xref = m_db.field_text(9);
		if ( m_db.type_of_column (10) == SQLITE_INTEGER )
		  ov.status = (NewsStatus)m_db.field_int(10);
		if (!m_newsrc.ismark (group, ov.id))
		  ovl.push_back (ov);
	      }
	  }
	m_db.finish();
	return true;
      }
    return false;
  }

  bool News::list_overview_with_status ( OverviewList& ovl, const SGroup& sg,
					 const NewsStatus& status, bool cf )
  {
    if ( sg.table.empty() ) return false;
    if (cf) ovl.clear ();
    OverView ov;
    std::ostringstream os;
    std::string table;
    table = sg.table; table += "_OV";
    os << "SELECT _ID, _SUBJECT, _FROM, _DATE, _MESSAGEID, _REFERENCES, _REFC,"
       << "_BYTES, _LINES, _XREF FROM "
       << SQLite3::qoute_string(table) 
      //<< " WHERE _STATUS = " << status
       << " ORDER BY _REFC,_TIMESTAMP;";
    if ( m_db.execute(os.str()))
      {
	while (m_db.nextrow ())
	  {
	    if ( m_db.count_of_column () == 10 )
	      {
		ov.clear ();
		ov.status = status;
		if ( m_db.type_of_column (0) == SQLITE_INTEGER )
		  ov.id = (unsigned int)m_db.field_int(0);
		if ( m_db.type_of_column (1) == SQLITE_TEXT )
		  ov.subject = m_db.field_text(1);
		if ( m_db.type_of_column (2) == SQLITE_TEXT )
		  ov.from = m_db.field_text(2);
		if ( m_db.type_of_column (3) == SQLITE_TEXT )
		  ov.date = m_db.field_text(3);
		if ( m_db.type_of_column (4) == SQLITE_TEXT )
		  ov.messageid = m_db.field_text(4);
		if ( m_db.type_of_column (5) == SQLITE_TEXT )
		  ov.references = m_db.field_text(5);
		if ( m_db.type_of_column (6) == SQLITE_INTEGER )
		  ov.ref_count = m_db.field_int(6);
		if ( m_db.type_of_column (7) == SQLITE_INTEGER )
		  ov.bytes = m_db.field_int(7);
		if ( m_db.type_of_column (8) == SQLITE_INTEGER )
		  ov.lines = m_db.field_int(8);
		if ( m_db.type_of_column (9) == SQLITE_TEXT )
		  ov.xref = m_db.field_text(9);
// 		if ( m_db.type_of_column (10) == SQLITE_INTEGER )
// 		  ov.status = (NewsStatus)m_db.field_int(10);
		if ( status == NewsUnread &&
		     !m_newsrc.ismark(sg.group, ov.id))
		  {
		    ovl.push_back (ov);
		  }
		else if ( status == NewsRead &&
			  m_newsrc.ismark (sg.group,ov.id))
		  {
		    ovl.push_back (ov);
		  }
		else
		  {
		    if ( status == ov.status )
		      ovl.push_back (ov);
		  }
	      }
	  }
	m_db.finish();
	return true;
      }
    return false;
  }
  bool News::list_sgroup_with_topics (SGroupList& sg,const std::string& topics)
  {
    std::ostringstream sql;
    SGroup sgroup;
    sql << "SELECT _GROUP, _ALIASES, _UNREAD, _LAST, _TABLE, _SERVER FROM"
	<< " SUBSCRIBE_GROUP WHERE _TOPICS = '"
	<< SQLite3::qoute_string(topics) << "';";
    if ( m_db.execute (sql.str()))
      {
	while (m_db.nextrow())
	  {
	    if (m_db.count_of_column () == 6 )
	      {
		sgroup.clear ();
		sgroup.topics = topics;
		if (m_db.type_of_column(0) == SQLITE_TEXT)
		  sgroup.group = m_db.field_text(0);
		if (m_db.type_of_column(1) == SQLITE_TEXT)
		  sgroup.alias = m_db.field_text(1);
		if (m_db.type_of_column(2) == SQLITE_INTEGER)
		  sgroup.unread = m_db.field_int(2);
		if (m_db.type_of_column(3) == SQLITE_INTEGER)
		  sgroup.last = m_db.field_int(3);
		if (m_db.type_of_column(4) == SQLITE_TEXT)
		  sgroup.table = m_db.field_text(4);
		if (m_db.type_of_column(5) == SQLITE_TEXT)
		  sgroup.server = m_db.field_text(5);
		//sgroup.print ();
		sg.push_back (sgroup);
	      }
	  }
	m_db.finish ();
	return true;
      }
    return false;
  }
  bool News::get_article ( const SGroup& sg,
			   const std::string& messageid, std::string& article)
  {
    if (sg.table.empty() || messageid.empty()) return false;
    std::string table;
    table = sg.table; table += "_POOL";
    if ( !m_db.hastable (table)) return false;
    std::ostringstream osql;
    osql << "SELECT _ARTICLE FROM " << table
	 << " WHERE _MESSAGEID ='"<< SQLite3::qoute_string (messageid) <<"';";
    if ( m_db.execute (osql.str()))
      {
	if ( m_db.nextrow () && m_db.count_of_column () == 1 &&
	     m_db.type_of_column (0) == SQLITE_TEXT )
	  {
	    article = m_db.field_text(0);
	    m_db.finish();
	    return true;
	  }
      }
    m_db.finish ();
    return false;
  }
  bool News::get_table_with_sgroup ( SGroup& sg )
  {
    if ( sg.server.empty() || sg.group.empty() ) return false;
    std::ostringstream sql;
    sql << "SELECT _TABLE FROM SUBSCRIBE WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group) << "', _SERVER ='"
	<< SQLite3::qoute_string(sg.server) << "';";
    if ( m_db.execute(sql.str()))
      {
	if (m_db.nextrow () && m_db.count_of_column () == 1 &&
	    m_db.type_of_column(0) == SQLITE_TEXT)
	  {
	    sg.table = m_db.field_text(0);
	    m_db.finish ();
	    return true;
	  }
      }
    m_db.finish ();
    return false;
  }
  bool News::get_alias_with_sgroup ( SGroup& sg )
  {
    if ( sg.server.empty() || sg.group.empty()) return false;
    std::ostringstream sql;
    sql << "SELECT _ALIAS FROM SUBSCRIBE WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group) << "', _SERVER ='"
	<< SQLite3::qoute_string(sg.server) << "';";
    if ( m_db.execute(sql.str()))
      {
	if (m_db.nextrow () && m_db.count_of_column () == 1 &&
	    m_db.type_of_column(0) == SQLITE_TEXT)
	  {
	    sg.alias = m_db.field_text(0);
	    m_db.finish ();
	    return true;
	  }
      }
    m_db.finish ();
    return false;    
  }
  bool News::get_topics_with_sgroup ( SGroup& sg )
  {
    if ( sg.server.empty() || sg.group.empty()) return false;
    std::ostringstream sql;
    sql << "SELECT _TOPICS FROM SUBSCRIBE WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group) << "', _SERVER ='"
	<< SQLite3::qoute_string(sg.server) << "';";
    if ( m_db.execute(sql.str()))
      {
	if (m_db.nextrow () && m_db.count_of_column () == 1 &&
	    m_db.type_of_column(0) == SQLITE_TEXT)
	  {
	    sg.topics = m_db.field_text(0);
	    m_db.finish ();
	    return true;
	  }
      }
    m_db.finish ();
    return false;    
  }
  bool News::get_unread_with_sgroup ( SGroup& sg )
  {
    if ( sg.server.empty() || sg.group.empty()) return false;
    std::ostringstream sql;
    sql << "SELECT _UNREAD FROM SUBSCRIBE WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group) << "', _SERVER ='"
	<< SQLite3::qoute_string(sg.server) << "';";
    if ( m_db.execute(sql.str()))
      {
	if (m_db.nextrow () && m_db.count_of_column () == 1 &&
	    m_db.type_of_column(0) == SQLITE_INTEGER )
	  {
	    sg.table = m_db.field_int(0);
	    m_db.finish ();
	    return true;
	  }
      }
    m_db.finish ();
    return false;
  }
  bool News::get_last_with_sgroup ( SGroup& sg )
  {
    if ( sg.server.empty() || sg.group.empty()) return false;
    std::ostringstream sql;
    sql << "SELECT _LAST FROM SUBSCRIBE WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group) << "', _SERVER ='"
	<< SQLite3::qoute_string(sg.server) << "';";
    if ( m_db.execute(sql.str()))
      {
	if (m_db.nextrow () && m_db.count_of_column () == 1 &&
	    m_db.type_of_column(0) == SQLITE_INTEGER)
	  {
	    sg.last = m_db.field_int(0);
	    m_db.finish ();
	    return true;
	  }
      }
    m_db.finish ();
    return false;
  }
  bool News::set_server ( const Server& ss )
  {
    std::string server;
    std::ostringstream sql;
    strip_server_name (ss.server, server);
    sql << "UPDATE SERVER SET _PORT = " << ss.port 
	<< ", _USER ='" << SQLite3::qoute_string (ss.user)
	<< "', _PASS  ='" << SQLite3::qoute_string (ss.pass)
	<< "' WHERE _SERVER = '" << SQLite3::qoute_string(server)
	<< "';";
    return m_db.exec (sql.str());
  }
  News& News::operator<< ( GroupList& gl )
  {
    if ( gl.m_server.empty() ) return *this;
    GroupList::GroupHash::iterator ii = gl.m_groups.begin();
    //std::cout << "News << GL " << endl;
    std::string sql, table;
    strip_server_name(gl.m_server, table);
    if ( !m_db.hastable(table+"_GL"))
      {
	std::ostringstream cs;
	cs << "CREATE TABLE " << SQLite3::qoute_string (table+"_GL")
	   << " ( _GROUP varchar(20), _FIRST Integer, _LAST Integer,"
	   << " _POST varchar(1), _DESC varchar(15));";
	if (!m_db.exec (cs.str())) return *this;
      }
    sql += "BEGIN;";
    if ( gl.m_clear )
      sql += "DELETE FROM " + SQLite3::qoute_string(table) + "_GL;";

    for ( ; ii != gl.m_groups.end(); ++ii )
      {
	std::ostringstream ss;
 	ss << "REPLACE INTO " << table
	   << "_GL (_GROUP, _FIRST, _LAST, _POST, _DESC) VALUES ('"
	   << SQLite3::qoute_string(ii->second.group) << "', "
	   << ii->second.first << ", " << ii->second.last << ", '"
	   << SQLite3::qoute_string((ii->second.post ? "y" : "n")) << "', '"
	   << SQLite3::qoute_string(ii->second.desc) << "');";
	sql += ss.str();
      }
    sql += "COMMIT;";
    m_db.exec (sql);
    return *this;
  }
  bool News::is_subscribe ( const std::string& server, const std::string& group )
  {
    if ( m_db.hastable ("SUBSCRIBE_GROUP"))
      {
	std::string sql;
	sql = "SELECT COUNT(_GROUP) FROM SUBSCRIBE_GROUP WHERE ";
	sql += " _GROUP = '"+ SQLite3::qoute_string(group)+"' AND";
	sql += " _SERVER = '" + SQLite3::qoute_string(server)+ "' ";
	if (m_db.execute (sql))
	  {
	    if ( m_db.nextrow () &&
		 m_db.count_of_column () == 1 &&
		 m_db.type_of_column(0) == SQLITE_INTEGER &&
		 m_db.field_int(0) != 0)
	      {
		m_db.finish ();
		return true;
	      }
	  }
      }
    m_db.finish ();
    return false;
  }
  bool News::clear ()
  {
    return m_db.exec ("VACUUM");
  }
  bool News::list_server ( Server& ss )
  {
    if ( ss.server.empty() ) return false;
    std::ostringstream sql;
    std::string server;
    strip_server_name(ss.server, server);
    sql << "SELECT _SERVER, _PORT, _USER, _PASS FROM SERVER WHERE _SERVER ='"
	<< SQLite3::qoute_string(server) << "';";
    if ( m_db.execute (sql.str()))
      {
	if (m_db.nextrow() && m_db.count_of_column() == 4)
	  {
	    if (m_db.type_of_column (0) == SQLITE_TEXT )
	      ss.server = m_db.field_text(0);
	    if (m_db.type_of_column (1) == SQLITE_INTEGER)
	      ss.port = m_db.field_int(1);
	    if (m_db.type_of_column (2) == SQLITE_TEXT )
	      ss.user = m_db.field_text(2);
	    if (m_db.type_of_column (3) == SQLITE_TEXT )
	      ss.pass = m_db.field_text(3);
	    m_db.finish ();
	    return true;
	  }
      }
    return false;
  }
  bool News::list_sgroup ( SGroup& sg )
  {
    if ( sg.group.empty() || sg.server.empty()) return false;
    std::ostringstream sql;
    std::string server;
    strip_server_name (sg.server, server);
    sql << "SELECT _ALIASES, _UNREAD, _LAST, _TABLE, _TOPICS FROM "
	<< "SUBSCRIBE_GROUP WHERE _GROUP ='" << sg.group << "' AND "
	<< "_SERVER ='" << server << "';";
    if (m_db.execute (sql.str()))
      {
	if ( m_db.nextrow() && m_db.count_of_column() == 5)
	  {
	    if (m_db.type_of_column(0) == SQLITE_TEXT )
	      sg.alias = m_db.field_text(0);
	    if (m_db.type_of_column(1) == SQLITE_INTEGER )
	      sg.unread = m_db.field_int(1);
	    if (m_db.type_of_column(2) == SQLITE_INTEGER )
	      sg.last = m_db.field_int(2);
	    if (m_db.type_of_column(3) == SQLITE_TEXT )
	      sg.table = m_db.field_text(3);
	    if (m_db.type_of_column(4) == SQLITE_TEXT )
	      sg.topics = m_db.field_text(4);
	    m_db.finish ();
	    return true;
	  }
      }
    return false;
  }
  bool News::list_servers ( ServerList& sl )
  {
    std::string sql = "SELECT _SERVER, _PORT, _USER, _PASS FROM SERVER";
    Server ss;
    if ( m_db.execute(sql))
      {
	while (m_db.nextrow())
	  {
	    ss.clear ();
	    if ( m_db.count_of_column () == 4 )
	      {
		if (m_db.type_of_column (0) == SQLITE_TEXT)
		  ss.server = m_db.field_text (0);
		if (m_db.type_of_column(1) == SQLITE_INTEGER)
		  ss.port = m_db.field_int(1);
		if (m_db.type_of_column(2) == SQLITE_TEXT)
		  ss.user = m_db.field_text(2);
		if (m_db.type_of_column(3) == SQLITE_TEXT)
		  ss.pass = m_db.field_text(3);
		if (!ss.server.empty())
		  sl.push_back (ss);
	      }
	  }
	m_db.finish ();
	return true;
      }
    return false;
  }
  bool News::list_sgroups ( SGroupList& sgl )
  {
    std::string sql = "SELECT _GROUP, _ALIASES, _UNREAD, _LAST, _TABLE, _SERVER, _TOPICS FROM SUBSCRIBE_GROUP;";
    if ( m_db.execute (sql))
      {
	SGroup sg;
	while (m_db.nextrow())
	  {
	    if ( m_db.count_of_column () == 7 )
	      {
		sg.clear ();
		if (m_db.type_of_column (0) == SQLITE_TEXT)
		  sg.group = m_db.field_text(0);
		if (m_db.type_of_column(1) == SQLITE_TEXT)
		  sg.alias = m_db.field_text(1);
		if (m_db.type_of_column(2) == SQLITE_INTEGER)
		  sg.unread = m_db.field_int(2);
		if (m_db.type_of_column(3) == SQLITE_INTEGER)
		  sg.last = m_db.field_int(3);
		if (m_db.type_of_column(4) == SQLITE_TEXT)
		  sg.table = m_db.field_text(4);
		if (m_db.type_of_column(5) == SQLITE_TEXT)
		  sg.server = m_db.field_text(5);
		if (m_db.type_of_column(6) == SQLITE_TEXT)
		  sg.topics = m_db.field_text(6);
		if (!sg.server.empty() && !sg.group.empty())
		  sgl.push_back(sg);

	      }
	  }
	m_db.finish();
	return true;
      }
    return false;
  }
  bool News::set_group_server ( const SGroup& sg, const std::string& server )
  {
    std::string cserver, nserver;
    std::ostringstream sql;
    strip_server_name (sg.server, cserver);
    strip_server_name (server, nserver);
    sql << "UPDATE SUBSCRIBE_GROUP SET _SERVER = '"
	<< SQLite3::qoute_string (nserver) << "' WHERE _SERVER = '"
	<< SQLite3::qoute_string (cserver) << "' AND _GROUP = '"
	<< SQLite3::qoute_string (sg.group) << "'; ";
    return m_db.exec (sql.str());
  }
  bool News::set_group_topics ( const SGroup &sg )
  {
    std::string server;
    std::ostringstream sql;
    strip_server_name (sg.server, server);
    sql << "UPDATE SUBSCRIBE_GROUP SET _TOPICS = '"
	<< SQLite3::qoute_string(sg.topics) << "' WHERE _GROUP = '"
	<< SQLite3::qoute_string(sg.group) << "' AND _SERVER = '"
	<< SQLite3::qoute_string(server) << "';";
    return m_db.exec (sql.str());
  }
  bool News::set_group_alias ( const SGroup &sg )
  {
    std::string server;
    std::ostringstream sql;
    strip_server_name (sg.server, server);
    sql << "UPDATE SUBSCRIBE_GROUP SET _ALIAS = '"
	<< SQLite3::qoute_string (sg.alias)
	<< "' WHERE _GROUP ='" << SQLite3::qoute_string(sg.group)
	<< "' AND _SERVER = '" << SQLite3::qoute_string(server)
	<< "';";
    return m_db.exec (sql.str());
  }

  bool News::add_group ( const SGroup& sg )
  {
    if ( sg.group.empty() || sg.server.empty() ) return false;
    std::string server, table, sql;
    strip_server_name (sg.server, server);
    if ( !sg.table.empty() )
      {
	table = sg.table;
	strip_dot (table);
      }
    else
      {
	do_group_name (sg.group, table);
      }
    bool has_sub = is_subscribe(server, sg.group),
      has_ov  = m_db.hastable (table+"_OV"),
      has_idx = m_db.hasindex (table+"_OV"),
      has_pool = m_db.hasindex (table+"_POOL");
    if ( !has_sub || !has_ov || !has_idx || !has_pool )
      {
	sql += "BEGIN TRANSACTION;";
	if ( !has_sub )
	  {
	    std::ostringstream s_sql;
	    s_sql << "INSERT INTO SUBSCRIBE_GROUP "
		  << "(_GROUP, _ALIASES, _UNREAD, _LAST, _TABLE,"
		  << " _SERVER, _TOPICS)"
		  << " VALUES ('" << SQLite3::qoute_string(sg.group) << "','"
		  << SQLite3::qoute_string(sg.alias) << "', 0, 1,'"
		  << table << "' , '"
		  << SQLite3::qoute_string(server) << "','"
		  << SQLite3::qoute_string(sg.topics) << "');";
	    sql += s_sql.str();
	  }
	if ( !has_ov )
	  {
	    std::ostringstream ov_sql;
	    ov_sql << "CREATE TABLE " << table << "_OV ( "
		   << "_ID Integer PRIMARY KEY UNIQUE, _SUBJECT varchar(20),"
		   << " _FROM varchar(30),"
		   << " _DATE varchar(30),_TIMESTAMP varchar(30),"
		   << " _MESSAGEID varchar(20),"
		   << "_REFERENCES varchar(10), _REFC Integer, _BYTES Integer,"
		   << "_LINES Integer,"
		   << "_XREF varchar(20), _STATUS Integer"
		   << ");";
	    sql += ov_sql.str();
	  }
	if ( !has_idx )
	  {
	    std::ostringstream sql_idx;
	    sql_idx << "CREATE INDEX " << table << "_OV_idx ON "
		    << table << "_OV ("
		    << "_ID, _MESSAGEID, _REFERENCES, _TIMESTAMP, _STATUS"
		    << ");";
	    sql += sql_idx.str();
	  }
	if ( !has_pool )
	  {
	    std::ostringstream sql_pool;
	    sql_pool << "CREATE TABLE " << table
		     << "_POOL (_MESSAGEID varchar(20) PRIMARY KEY UNIQUE, "
		     << " _ARTICLE varchar(200));";
	    sql += sql_pool.str();
	  }
	sql += "COMMIT";
	return m_db.exec (sql);
      }
    return false;
  }
  bool News::subscribe_group ( const std::string& server, const std::string& group )
  {
    std::string c_server, table;
    do_group_name (group, table);
    strip_server_name (server, c_server);
    bool
      has_sub = is_subscribe(server, group ),
      has_ov  = m_db.hastable (table+"_OV"),
      has_idx = m_db.hasindex (table+"_OV"),
      has_pool = m_db.hasindex (table+"_POOL");
    if ( !has_sub || !has_ov || !has_idx || !has_pool )
      m_db.exec ("BEGIN TRANSACTION");
    if ( !has_sub )
      {
	std::ostringstream s_sql;
	s_sql << "INSERT INTO SUBSCRIBE_GROUP "
	      << "(_GROUP, _UNREAD, _LAST, _TABLE, _SERVER)"
	      << " VALUES ('" << SQLite3::qoute_string(group) << "' , 0, 1,'"
	      << table << "' , '" << SQLite3::qoute_string(c_server) << "' )";
	m_db.exec ( s_sql.str() );
      }
    if ( !has_ov )
      {
	std::ostringstream ov_sql;
	ov_sql << "CREATE TABLE " << table << "_OV ( "
	       << "_ID Integer PRIMARY KEY UNIQUE, _SUBJECT varchar(20),"
	       << " _FROM varchar(30),"
	       << " _DATE varchar(30),_TIMESTAMP varchar(30),"
	       << " _MESSAGEID varchar(20),"
	       << "_REFERENCES varchar(10), _REFC Integer, _BYTES Integer, "
	       << "_LINES Integer,"
	       << "_XREF varchar(20), _STATUS Integer"
	       << ")";
 	m_db.exec ( ov_sql.str() );
      }
    if ( !has_idx )
      {
	std::ostringstream sql_idx;
	sql_idx << "CREATE INDEX " << table << "_OV_idx ON "
		<< table << "_OV ("
		<< "_ID, _REFERENCES, _DATE, _STATUS" << ")";
	m_db.exec ( sql_idx.str() );
      }
    if ( !has_pool )
      {
	std::ostringstream sql_pool;
	sql_pool << "CREATE TABLE " << table
		 << "_POOL (_MESSAGEID varchar(20) PRIMARY KEY UNIQUE, "
		 << " _ARTICLE varchar(200))";
	m_db.exec ( sql_pool.str());
      }
    if ( !has_sub || !has_ov || !has_idx || !has_pool )
      {
	bool rc = m_db.exec ("COMMIT");
	return rc;
      }
    else return true;
  }
  id_type News::last_id ( const std::string& server,const std::string& group)
  {
    std::string sql;
    sql = "SELECT _LAST FROM SUBSCRIBE_GROUP WHERE _GROUP = '" \
      + SQLite3::qoute_string(group) + "' AND _SERVER='" \
      + SQLite3::qoute_string(server) + "'";
    id_type last_id = 0;
    if ( m_db.execute (sql) )
      {
	if (m_db.nextrow() && m_db.count_of_column() >= 1 &&
	    m_db.type_of_column(0) == SQLITE_INTEGER )
	  {
	    last_id = m_db.field_i64(0);
	    m_db.finish ();
	    return last_id;
	  }
      }
    m_db.finish();
    return 0;
  }
  bool News::begin ()
  {
    return m_db.exec ("BEGIN TRANSACTION");
  }
  bool News::rollback ()
  {
    return m_db.exec ("ROLLBACK TRANSACTION");
  }
  bool News::commit ()
  {
    return m_db.exec ("COMMIT");
  }
  bool News::end()
  {
    return m_db.exec ("END TRANSACTION");
  }

  void News::set_queue_size ( unsigned short queue_size )
  {
    m_queue_size = queue_size;
    m_pushqueue.reserve ( m_queue_size + 1 );
  }
  void News::update_queue ()
  {
    std::cout << m_pushqueue.size () << " Update queue." << endl;
    if ( !m_pushqueue.empty () )
      {
	PushQueue::iterator i = m_pushqueue.begin ();
	std::string sql, ts;
	sql = "BEGIN;";
	for (; i < m_pushqueue.end(); ++i )
	  {
	    build_pushitem_sql ( *i, ts);
	    sql += ts;
	  }
	SGroupList sgl; 
	list_sgroups ( sgl );
	for ( SGroupList::iterator i = sgl.begin(); i < sgl.end(); ++i)
	  {
	    if ( (*i).table.empty() ||
		 (*i).table.find(".") != std::string::npos ) continue;
	    std::ostringstream usql;
	    usql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = (SELECT COUNT(_ID) "
		 << " FROM " << (*i).table << "_OV "
		 << " WHERE _STATUS = " << NewsUnread << ")"
		 << " WHERE _GROUP = '"
		 << SQLite3::qoute_string ((*i).group)
		 << "' AND _SERVER = '"
		 << SQLite3::qoute_string ((*i).server) << "';";
	    sql += usql.str();
	  }
	sql += "COMMIT";
	if (m_db.exec(sql))
	  m_pushqueue.clear();
      }
  }

  void News::push_queue (const std::string& server, const std::string& group,
			 const OverView& ov, const std::string& article )
  {
    std::string sql;
    m_pushqueue.push_back(PushItem(server, group, ov, article));
    if ( m_pushqueue.size() >= m_queue_size )
      update_queue ();
  }
  void News::build_pushitem_sql ( const PushItem& item, std::string &sql )
  {
    Date::DateTime datetime = item.ov.date;
    std::ostringstream ov_sql, group_sql;
    std::string pool_sql;
    std::string c_server, c_table;
    do_group_name ( item.group, c_table );
    strip_server_name (item.server, c_server);
    id_type last = last_id(item.server, item.group);
    last = ( item.ov.id > last ? item.ov.id : last );
    sql.erase ();

//     group_sql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = "
// 	      << "(SELECT _UNREAD FROM SUBSCRIBE_GROUP WHERE _GROUP = '"
// 	      << SQLite3::qoute_string (item.group) << "' AND _SERVER = '"
// 	      << SQLite3::qoute_string (item.server) << "') + 1, _LAST = "
// 	      << last << " WHERE _GROUP = '"
// 	      << SQLite3::qoute_string(item.group)
// 	      << "'" << " AND _SERVER = '"
// 	      << SQLite3::qoute_string(item.server) << "';";
//     sql += group_sql.str();
    ov_sql << "REPLACE INTO " << c_table << "_OV "
	   << "( _ID, _SUBJECT, _FROM, _DATE, _TIMESTAMP, _MESSAGEID, "
	   << "_REFERENCES, _REFC, _BYTES, _LINES, _XREF, _STATUS)"
	   << " VALUES (" << item.ov.id << ", '"
	   << SQLite3::qoute_string(item.ov.subject) << "', '"
	   << SQLite3::qoute_string(item.ov.from) + "', '"
	   << SQLite3::qoute_string(item.ov.date) + "', '"
	   << SQLite3::qoute_string(datetime.timestamp()) + "', '"
	   << SQLite3::qoute_string(item.ov.messageid) + "', '"
	   << SQLite3::qoute_string(item.ov.references) + "', "
	   << item.ov.ref_count << ", " << item.ov.bytes << ", "
	   << item.ov.lines << ", '" << SQLite3::qoute_string(item.ov.xref)
	   << "'," << item.ov.status << ");";
    sql += ov_sql.str();
    sql += "REPLACE INTO " + c_table + "_POOL (_MESSAGEID, _ARTICLE)" \
      + " VALUES ('" + SQLite3::qoute_string(item.ov.messageid) + "', '" \
      + SQLite3::qoute_string(item.article) + "');";
  }
  bool News::add_article ( const SGroup& sg, const OverView& ov,
			   const std::string& article )
  {
    if (sg.server.empty() || sg.group.empty() ) return false;
    return add_article (sg.server, sg.group, ov, article);
  }
  bool News::add_article (const std::string& server, const std::string& group,
			  const OverView& ov, const std::string& article )
  {
    std::string c_server, c_table;
    do_group_name ( group, c_table );
    strip_server_name (server, c_server);
    if (!m_db.hastable(c_table+"_OV")) return false;
    if (!m_db.hastable(c_table+"_POOL")) return false;
    if (!m_db.hasindex(c_table+"_OV")) return false;
    Date::DateTime datetime = ov.date;
    std::ostringstream ov_sql, group_sql;
    std::string pool_sql,sql;
    id_type last = last_id(server, group);
    last = ( ov.id > last ? ov.id : last );

    group_sql << "UPDATE SUBSCRIBE_GROUP SET _UNREAD = "
	      << " (SELECT _UNREAD FROM SUBSCRIBE_GROUP WHERE _GROUP = '"
	      << SQLite3::qoute_string (group) << "' AND _SERVER = '"
	      << SQLite3::qoute_string (server) << "') + 1, _LAST = "
	      << last 
	      << " WHERE _GROUP = '" << SQLite3::qoute_string(group) << "'"
	      << " AND _SERVER = '" << SQLite3::qoute_string(server) << "';";

    ov_sql << "REPLACE INTO " << c_table << "_OV "
	   << "( _ID, _SUBJECT, _FROM, _DATE, _TIMESTAMP, _MESSAGEID, "
	   << "_REFERENCES, _REFC, _BYTES, _LINES, _XREF, _STATUS)"
	   << " VALUES ("
	   << ov.id << ", '"
	   << SQLite3::qoute_string(ov.subject) << "', '"
	   << SQLite3::qoute_string(ov.from) << "', '"
	   << SQLite3::qoute_string(ov.date) << "', '"
	   << SQLite3::qoute_string(datetime.timestamp()) << "', '"
	   << SQLite3::qoute_string(ov.messageid) << "', '"
	   << SQLite3::qoute_string(ov.references) << "', "
	   << ov.ref_count << ", " << ov.bytes << ", " << ov.lines << ", '"
	   << SQLite3::qoute_string(ov.xref) << "'," << ov.status << ");";

    pool_sql = "REPLACE INTO " + c_table + "_POOL (_MESSAGEID, _ARTICLE)" \
      + " VALUES ('" + SQLite3::qoute_string(ov.messageid) + "', '" \
      + SQLite3::qoute_string(article) + "');";
    sql = "BEGIN;";
    sql += group_sql.str() + ov_sql.str() + pool_sql;
    sql += "COMMIT;";
    return m_db.exec (sql);
  }
  bool News::add_server (const Server& server )
  {
    return add_server (server.server, server.port, server.user, server.pass);
  }
  bool News::add_server(const std::string& server, const std::string& port, const std::string& user, const std::string& pass )
  {
    istringstream ss(port);
    unsigned int l_port = 0;
    ss >> l_port;
    if ( l_port == 0 ) l_port = 199;
    return add_server(server, l_port, user, pass);
  }
  bool News::add_server(const std::string& server, const unsigned int& port, const std::string& user, const std::string& pass)
  {
    bool rc = has_server (server);
    if (!rc)
      {
	m_db.bind_start ("INSERT INTO SERVER ('_SERVER', '_PORT', '_USER', '_PASS') VALUES (?,?,?,?)");
	m_db.bind (1, server);
	m_db.bind (2, (int)port);
	m_db.bind (3, user);
	m_db.bind (4, pass);
	rc = m_db.bind_end ();
	return rc;
      }
    return rc;
  }
  void News::strip_server_name (const std::string &in, std::string& out )
  {
    std::string::const_iterator ii;
    std::string::size_type pos;
    pos = in.find("://");
    if ( pos != std::string::npos )
      out = in.substr(pos+3);
    else out = in;
    pos = out.find ("/");
    if ( pos != std::string::npos )
      out = out.substr(0,pos);
  }
  void News::strip_dot (std::string& in )
  {
    for (std::string::iterator ii = in.begin(); ii < in.end(); ++ii)
      if ( *ii == '.' ) *ii = '_';
  }
  void News::do_group_name ( const std::string& gn, std::string& ngn )
  {
    std::string::const_iterator ci;
    std::ostringstream ss;
    const char hex_table[] = "0123456789ABCDEF_____";
    unsigned int hh;
    for ( ci = gn.begin(); ci < gn.end(); ++ci )
      {
	if ( *ci == '\r' || *ci == '\n' || *ci == '\'' ) continue;
	if ( (unsigned char)*ci >= 0x80 )
	  {
	    hh = (unsigned int)((unsigned char)*ci/16);
	    ss << (hh > 16 ? '_' : hex_table[hh]);
	  }else ss << *ci;
      }
    ngn = ss.str();
    strip_dot(ngn);
  }
  bool News::has_server (const std::string& server)
  {
    std::ostringstream sql;
    sql << "SELECT count(_SERVER) FROM SERVER WHERE _SERVER == '"
	<< SQLite3::qoute_string(server) << "'";
    bool rc = m_db.execute (sql.str());
    if (!rc) return false;
    if (m_db.nextrow () && m_db.type_of_column(0) == 1)
      {
	int r = m_db.field_int(0);
	m_db.finish ();
	return r != 0;
      }
    m_db.finish ();
    return rc;
  }
  bool News::has_group ( const std::string& server, const std::string& group)
  {
    return is_subscribe ( server, group );
  }
  bool News::del_group ( const SGroup& sg )
  {
    return del_group ( sg.server, sg.group );
  }
  bool News::del_group ( const std::string& server, const std::string& group)
  {
    if ( has_server (server) && has_group (server, group))
      {
	std::string table;
	std::ostringstream sql;
	do_group_name (group, table);
	sql << "DELETE FROM SUBSCRIBE_GROUP WHERE _GROUP='"
	    << group << "' AND _SERVER = '"
	    << SQLite3::qoute_string ( server ) << "';";
	if ( m_db.hastable (table+"_OV"))
	  sql << "DROP TABLE " << table << "_OV;";
	if ( m_db.hastable (table+"_POOL"))
	  sql << "DROP TABLE " << table << "_POOL;";
	sql << "COMMIT;";
	return m_db.exec (sql.str());
      }
      return false;
  }
  bool News::del_server ( const std::string& server )
  {
    bool rc = has_server(server);
    if ( rc )
      {
	std::ostringstream sql, grouplist_sql;
	grouplist_sql << "SELECT _GROUP,_TABLE FROM SUBSCRIBE_GROUP WHERE "
		      << " _SERVER = '" << SQLite3::qoute_string(server)
		      << "'";
	sql << "BEGIN;";
	sql << "DELETE FROM SERVER WHERE _SERVER = '"
	    << SQLite3::qoute_string(server) << "';";
	if (m_db.execute (grouplist_sql.str()))
	  {
	    while (m_db.nextrow ())
	      {
		if (m_db.count_of_column () == 2)
		  {
		    std::string table, group;
		    if (m_db.type_of_column(0) == SQLITE_TEXT)
		      group = m_db.field_text(0);
		    if (m_db.type_of_column(1) == SQLITE_TEXT)
		      table = m_db.field_text(1);
		    std::cout << "Group: "<< group << " " << table << endl;
		    sql << "DELETE FROM SUBSCRIBE_GROUP WHERE _GROUP='"
			<< group << "' AND _SERVER = '"
			<< SQLite3::qoute_string ( server ) << "';";
		    if ( m_db.hastable (table+"_OV"))
		      sql << "DROP TABLE " << table << "_OV;";
		    if ( m_db.hastable (table+"_POOL"))
		      sql << "DROP TABLE " << table << "_POOL;";
// 		    if ( m_db.hasindex (table+"_OV"))
// 		      sql << "DROP INDEX " << table << "_OV_idx;";
		  }
	      }
	  }
	m_db.finish ();
	sql << "COMMIT;";
	if (m_db.exec (sql.str()))
	  return true;
      }
    return false;
  }

  void News::init ()
  {

  }
  bool News::open (const std::string& datafile, const std::string newsrc )
  {
    bool rc =  m_db.open (datafile);
    m_newsrcfilename = newsrc;
    m_newsrc.load (newsrc);
    if ( rc )
      {
	check_table();
	SGroupList sgl;
	list_sgroups (sgl);
	for ( SGroupList::iterator i = sgl.begin(); i < sgl.end(); ++i)
	  {
	    m_newsrc.subscribe ((*i).group);
	  }
      }
    return rc;
  }
}
  
