#include "grouplist.hpp"

namespace News
{
  GroupList::GroupList ()
    : m_clear(false)
  {}
  GroupList::~GroupList()
  {}
  GroupList::GroupList (const std::string& server, const bool cf)
    : m_server(server), m_clear(cf)
  {}
  bool GroupList::sql ( const std::string& server, std::string& sql )
  {
    GroupHash::iterator i;
    for ( i = m_groups.begin (); i != m_groups.end(); ++i )
      {
	std::ostringstream ss;
	ss << "REPLACE INTO " << server
	   << "_GL (_GROUP, _FIRST, _LAST, _POST, _DESC) VALUES ('"
	   << SQLite3::qoute_string(i->second.group) << "', "
	   << i->second.first << ", " << i->second.last << ", '"
	   << SQLite3::qoute_string((i->second.post ? "y" : "n")) << "', '"
	   << SQLite3::qoute_string(i->second.desc) << "';";
	sql += ss.str();
      }
    return true;
  }
  
  bool GroupList::save ( const std::string& filename )
  {
    ofstream ofile (filename.c_str(), std::ios::out|std::ios::binary);
    const char sep = 0;
    if ( ofile )
      {
	for ( GroupHash::iterator hi = m_groups.begin (); hi != m_groups.end();
	      ++hi )
	  {
	    ofile << hi->second.group << sep
		  << hi->second.first << sep
		  << hi->second.last << sep
		  << (hi->second.post ? "y" : "n" ) << sep
		  << hi->second.desc << "\n";
	  }
	ofile.close ();
	return true;
      }
    else return false;
  }
  void GroupList::clear ()
  {
    m_groups.clear ();
  }
  bool GroupList::load ( const std::string& filename )
  {
    clear ();
    std::string line;
    std::ifstream ifile(filename.c_str(), std::ios::in|std::ios::binary);
    unsigned char ch;
    bool r;
    Group group;
    if (!ifile) return false;
    while (1)
      {
	r = ifile.get(ch);
	if (!r)break;
	if ( ch == '\n' && !line.empty() && line.size ())
	  {
	    group.clear ();
	    parser_load_line (line, group);
	    m_groups[group.group] = group;
	    line.erase ();
	  }
	else line+= ch;
      }
  }
  void GroupList::set_server ( const std::string& server )
  {
    m_server = server;
  }
  void GroupList::set_clear ( const bool cf )
  {
    m_clear = cf;
  }
  void GroupList::parser_load_line ( const std::string& line, Group& group )
  {
    std::string::const_iterator i = line.begin ();
    std::string sgroup, sfirst, slast, spost, sdesc;
    for ( ; i < line.end () ; ++i )
      {
	if ( *i == 0 ) break;
	sgroup += *i;
      }
    if ( i < line.end() && *i == 0) ++i;
    for ( ; i < line.end (); ++i )
      {
	if ( *i == 0 ) break;
	sfirst += *i;
      }
    if ( i < line.end () && *i == 0) ++i;
    for ( ; i < line.end(); ++i )
      {
	if ( *i == 0 ) break;
	slast += *i;
      }
    if ( i < line.end () && *i == 0 ) ++i;
    for ( ; i < line.end (); ++i )
      {
	if ( *i == 0) break;
	spost += *i;
      }
    if ( i < line.end () && *i == 0 ) ++i;
    for ( ; i < line.end (); ++i )
      {
	if ( *i == 0 || *i == '\n') break;
	sdesc += *i;
      }
    std::cout << sgroup << " " << sfirst << " " << slast << " "
	      << spost << " " << sdesc << endl;
    std::istringstream sf(sfirst), sl(slast);
    group.group = sgroup;
    sf >> group.first;
    sl >> group.last;
    group.post = ( spost == "y" ? true : false );
    group.desc = sdesc;
  }
  void GroupList::parse_list ( const std::string& list )
  {
    std::string::const_iterator i = list.begin ();
    GroupHash::iterator ip;
    std::string line;
    Group group;
    for (; i < list.end (); ++i )
      {
	if ( *i == '\r' ) continue;
	if ( *i == '\n' )
	  {
	    group.parse_list (line);
// 	    std::cout << group.group << " " << group.first << " "
// 		      << group.last << " " << group.post << endl;
	    if (!group.group.empty() && group.first != 0 && group.last != 0 )
	      {
		ip = m_groups.find (group.group);
		if ( ip == m_groups.end() )
		  m_groups[group.group] = group;
		else
		  {
		    ip->second.group = group.group;
		    ip->second.first = group.first;
		    ip->second.last = group.last;
		    ip->second.post = group.post;
		  }
	      }
	    line.erase ();
	  }
	else line+= *i;
      }
  }
  void GroupList::parse_desc ( const std::string& list )
  {
    std::string::const_iterator i;
    std::string line;
    GroupHash::iterator ip;
    Group group;
    for ( i = list.begin (); i < list.end(); ++i )
      {
	if ( *i == '\r' ) continue;
	if ( *i == '\n' )
	  {
	    group.parse_desc (line);
	    //std::cout << "DESC:" << group.group << " " << group.desc << endl;
	    if (!group.group.empty())
	      {
		ip = m_groups.find (group.group);
		if ( ip == m_groups.end() )
		  m_groups[group.group] = group;
		else
		  {
		    ip->second.desc = group.desc;
		  }
	      }
	    line.erase();
	  }
	else line+= *i;
      }
  }
  void GroupList::print ()
  {
    std::cout << m_groups.size() << endl;
  }
}
