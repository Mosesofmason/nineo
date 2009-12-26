#ifndef __NEWS_H__
#define __NEWS_H__
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include "Sqlite++/sqlitexx.hpp"
#include "newsrc.hpp"
#include "overview.hpp"
#include "date.hpp"
#include "grouplist.hpp"
#include <vector>
#include "newstype.hpp"

namespace News
{
  class GroupList;
  class ListGroup
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
    };
    typedef map< std::string, Group > GroupHash;
  private:
    GroupHash m_groups;
  public:
    ListGroup();
    ~ListGroup();
    void parse_list (const std::string& list );
    void parse_desc (const std::string& list );
    void print ();
  };
  class News
  {
    struct Server {
      std::string server, user, pass;
      unsigned int port;
      Server& operator= ( const Server& os)
      {
	server = os.server; user = os.user; pass = os.pass; port = os.port;
      }
      Server ()
	: port(119)
      {}
      Server ( const std::string& fserver,
	       const std::string& fuser, const std::string& fpass,
	       const unsigned int& fport)
	: server(fserver), user(fuser), pass(fpass), port (fport)
      {}
      Server ( const std::string& fuser, const std::string& fpass)
	: user(fuser), pass(fpass)
      {}
      Server ( const std::string& fserver, const unsigned int& fport)
	: server(fserver), port(fport)
      {}
      Server ( const std::string& fserver )
	: server (fserver)
      {}
      void print ()
      {
	std::cout << "Server: [" << server << "]\tUser: [" << user
		  << "]\tPass: [" << pass << "]\tPort: ["  << port
		  << "]" << endl;
      }
      void clear ()
      {
	port = 119;
	server.erase(); user.erase(); pass.erase();
      }
    };
    typedef vector < Server > ServerList;
    struct SGroup { 
      // Subscirbe group struct.
      std::string group, alias, table, server, topics;
      unsigned int unread, last;
      SGroup& operator= ( const SGroup& sg )
      {
	group = sg.group;
	alias = sg.alias;
	table = sg.table;
	server = sg.server;
	topics = sg.topics;
	unread = sg.unread;
	last = sg.last;
      }
      SGroup ()
	: unread(0), last(0)
      {}
      SGroup ( const std::string& fg, const::string& fa,
	       const std::string& ft, const::string& fs,
	       const std::string& fts,
	       const unsigned int& tu, const unsigned int& tl)
	: group(fg), alias (fa), table (ft), server(fs), topics(fts),
	  unread(tu), last(tl)
      {}
      void clear ()
      {
	group.erase(); alias.erase(); table.erase(); server.erase();
	topics.erase();
	unread = 0; last = 0;
      }
      void print ()
      {
	std::cout << "Group: [" << group << "]\n\tAlias: [" << alias
		  << "]\tTable: [" << table << "]\n\tServer: [" << server
		  << "]\tTopics: [" << topics << "]\t\n\t"
		  << "Unread: [" << unread << "]\tLast: [" << last << "]"
		  << endl;
      }
    };
    typedef vector < SGroup > SGroupList;
    
    struct PushItem {
      std::string server, group, article;
      OverView ov;
      PushItem ()
      {}
      PushItem (const std::string& fserver, const std::string& fgroup,
		const OverView& fov, const std::string& farticle)
	:server(fserver), group(fgroup), ov(fov), article(farticle)
      {}
      void clear ()
      {
	server.erase (); group.erase (); article.erase ();
	ov.clear ();
      }
      PushItem& operator= ( const PushItem& item )
      {
	server = item.server;
	group = item.group;
	article = item.article;
	ov = item.ov;
	return *this;
      }
    };
    typedef vector < PushItem > PushQueue;
  private:
    SQLite3::SQLite3 m_db;
    Newsrc::Newsrc m_newsrc;
    std::string m_newsrcfilename;
    unsigned short m_queue_size;
    PushQueue m_pushqueue;
    void build_pushitem_sql ( const PushItem& item, std::string &sql );
    void init ();
    void check_table ();
    void strip_server_name ( const std::string &in, std::string& out );
    void strip_dot ( std::string& in);
    bool create_list_table ( const std::string& table );
    bool is_subscribe ( const std::string& server, const std::string& group );
    void do_group_name ( const std::string& gn, std::string& ngn);
  public:
    void set_queue_size ( unsigned short queue_size );
    void update_queue ();
    void push_queue (const std::string& server, const std::string& group,
		     const OverView& ov, const std::string& article );
    bool open (const std::string& datafile, const std::string newsrc="newsrc");
    id_type last_id ( const std::string& server, const std::string& group );
    bool add_server (const Server& server );
    bool add_server (const std::string& server, const unsigned int& port, 
		     const std::string& user, const std::string& pass );
    bool add_server (const std::string& server, const std::string& port, 
		     const std::string& user, const std::string& pass );
    bool set_server ( const Server& ss );
    bool del_server (const std::string& server);
    bool has_server (const std::string& server);
    bool has_group  (const SGroup& sg );
    bool has_group (const std::string& server, const std::string& group);
    bool del_group ( const SGroup& sg );
    bool del_group ( const std::string& server, const std::string& group );
    bool add_group ( const SGroup& sg );
    bool set_group_server ( const SGroup& sg, const std::string &server );
    bool set_group_topics ( const SGroup& sg );
    bool set_group_alias ( const SGroup& sg );
    bool subscribe_group (const std::string& server,const std::string& group);
    bool add_article ( const std::string& server, const std::string& group,
		       const OverView& ov, const std::string& article );
    bool add_article ( const SGroup& sg, const OverView& ov,
		       const std::string& article );
    bool list_sgroup_with_server ( SGroupList& sg, const std::string &server );
    bool list_sgroup_with_topics ( SGroupList& sg, const std::string &topics );
    News& operator<< ( GroupList& gl );
    bool list_server ( Server& ss );
    bool list_sgroup ( SGroup& sg );
    bool list_servers ( ServerList& sl );
    bool list_sgroups ( SGroupList& sgl );
    bool list_overview ( OverviewList& ovl, const std::string& group,
			 bool cf = false );
    bool list_overview ( OverviewList& ovl, const SGroup& sg, bool cf = false);
    bool list_overview_with_status ( OverviewList& ovl, const SGroup& sg,
				     const NewsStatus& status,bool cf = false);
    bool list_overview_with_where ( OverviewList& ovl, const SGroup&sg,
				    const std::string& where, bool cf = false);
    bool del_overview ( const SGroup& sg, const OverviewList& ovl );
    bool del_overview ( const SGroup& sg, const OverView& ov );
    bool expire ( const unsigned int& year, const unsigned int& month,
		  const unsigned int& day );
    bool get_table_with_sgroup ( SGroup& sg );
    bool get_alias_with_sgroup ( SGroup& sg );
    bool get_topics_with_sgroup ( SGroup& sg );
    bool get_unread_with_sgroup ( SGroup& sg );
    bool get_last_with_sgroup ( SGroup& sg );
    bool get_article ( const SGroup& sg,
		       const std::string& messageid, std::string& article );
    bool mark_as_bozo ( const SGroup& sg, const id_type& id );
    bool mark_as_bozo ( const SGroup& sg, const OverView& ov );
    bool mark_as_spam ( const SGroup& sg, const id_type& id );
    bool mark_as_spam ( const SGroup& sg, const OverView& ov );
    bool mark_as_hate ( const SGroup& sg, const id_type& id );
    bool mark_as_hate ( const SGroup& sg, const OverView& ov );
    bool mark_as_unread ( const SGroup& sg, const id_type& id );
    bool mark_as_unread ( const SGroup& sg, const OverView& ov );
    bool mark_as_read ( const SGroup& sg, const id_type& id );
    bool mark_as_read ( const SGroup& sg, const OverView& ov );
    bool mark_as_tick ( const SGroup& sg, const id_type& id );
    bool mark_as_tick ( const SGroup& sg, const OverView& ov );
    bool mark_as_dor ( const SGroup& sg, const id_type& id );
    bool mark_as_dor ( const SGroup& sg, const OverView& ov );
    bool mark_as_fav ( const SGroup& sg, const id_type& id );
    bool mark_as_fav ( const SGroup& sg, const OverView& ov );
    bool mark_as_status ( const SGroup& sg, const OverView& ov );
    bool mark_as_status ( const SGroup& sg, const OverviewList& ovl);
    bool mark_as_status ( const SGroup& sg, const OverView& ov,
			  const NewsStatus& status );
    bool mark_as_status ( const SGroup& sg, const id_type& id,
			  const NewsStatus& status );
    bool mark_as_status ( const SGroup& sg, const OverviewList& ovl,
			  const NewsStatus& status );
    unsigned int unread_of_sgroup ( const SGroup& sg );
    bool clear ();
    bool begin ();
    bool rollback ();
    bool commit ();
    bool end();
    News(unsigned short queue_size = 100, bool debug = false );
    ~News();
  };
}


#endif
