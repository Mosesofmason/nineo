#ifndef __GROUP_LIST_H__
#define __GROUP_LIST_H__
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "Sqlite++/sqlitexx.hpp"
#include "newstype.hpp"

namespace News
{
  class News;
  class GroupList
  {
    struct Group
    {
      std::string group, desc;
      long time;
      id_type first, last;
      bool post;
      Group ()
      	: time (0), first(0), last(0), post(false), group(""), desc("")
      {}
      void clear ()
      {
	time = 0; first = 0; last = 0; post = false;
	group.erase(); desc.erase();
      }
      void parse_list ( const std::string& line )
      {
	std::string sgroup, sfirst, slast, spost;
	std::string::const_iterator ii = line.begin ();
	for ( ; ii < line.end (); ++ii )
	  {
	    if ( *ii == '\r' || *ii == '\t' ) continue;
	    if ( *ii == ' ' ) break;
	    sgroup += *ii;
	  }
	if ( ii < line.end () && *ii == ' ' ) ++ii;
	for ( ; ii < line.end (); ++ii )
	  {
	    if ( *ii == ' ' ) break;
	    sfirst += *ii;
	  }
	if ( ii < line.end() && *ii == ' ')++ii;
	for ( ; ii < line.end(); ++ii )
	  {
	    if ( *ii ==  ' ' ) break;
	    slast += *ii;
	  }
	if ( ii < line.end () && *ii == ' ') ++ii;
	for ( ; ii < line.end (); ++ii )
	  {
	    if ( *ii == '\r' || *ii == ' ' ) continue;
	    if ( *ii == '\n' ) break;
	    spost += *ii;
	  }
	std::istringstream sf (sfirst), sl(slast);
	id_type ifirst = 0, ilast = 0;
	sf >> ifirst;
	sl >> ilast;
	group = sgroup;
	first = ifirst;
	last = ilast;
	post = ( spost == "y" ? true : false );
      }
      void parse_desc ( const std::string& line )
      {
	std::string sgroup, sdesc;
	std::string::const_iterator i = line.begin();
	for (; i < line.end();++i)
	  {
	    if ( *i == ' ' || *i == '\t') break;
	    sgroup += *i;
	  }
	if ( i < line.end() && (*i == ' ' || *i == '\t'))++i;
	for (; i < line.end() ; ++i )
	  {
	    if ( *i == '\r' ) continue;
	    if ( *i == '\n' ) break;
	    sdesc += *i;
	  }
	group = sgroup;
	desc = sdesc;
      }
    };// end of group
    typedef map < std::string, Group > GroupHash;
  private:
    friend News;
    GroupHash m_groups;
    std::string m_server;
    bool m_clear;
    void parser_load_line ( const std::string& line, Group& group );
    
  public:
    GroupList();
    GroupList( const std::string& server, const bool cf = false );
    ~GroupList();
    void clear ();
    void set_server ( const std::string& server );
    void set_clear ( const bool cf = false );
    void parse_list ( const std::string& list );
    void parse_desc ( const std::string& list );
    void print ();
    bool save ( const std::string& filename );
    bool load ( const std::string& filename );
    bool sql ( const std::string& server, std::string &sql );
  };
}
#endif
