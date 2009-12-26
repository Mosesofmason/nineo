#include <string>
#include <iostream>
#include <fstream>
#include "date.hpp"
#include "news.hpp"
#include "nntp.hpp"
#include "grouplist.hpp"
#include "newsrc.hpp"
#include "overview.hpp"
#include "newstype.hpp"
#include <unistd.h>
#include <algorithm>

int main (const int argc, const char* argv[] )
{
  unntp nntp;
  bool rc;
  News::News news(4700, false);
  News::News::Server ss;
#if 1

  if (1 && news.open ("123.db"))
    {
      if (!news.has_server ("localhost"))
	{
	  std::cout << "No found server, add it." << endl;
	  news.add_server ("localhost", 119, "", "");
	}
      if (!news.has_group("localhost", "cn.fan"))
	{
	  std::cout << "No found group, add it."  << endl;
	  news.subscribe_group ("localhost", "cn.fan");
	}
      News::News::SGroup sg;
      sg.server = "localhost";
      sg.group = "cn.comp.software.shareware";
      sg.topics = "software";
      sg.alias = "c.c.s.shareware";
      if (!news.has_group (sg))
	{
	  std::cout << "No found group cn.comp.software.shareware"
		    << endl;
	  news.add_group(sg);
	}
      News::OverviewList ovl, ovl2, bozo_l, spam_l, hate_l, fav_l,tick_l,dor_l;
      sg.clear ();
      sg.server = "localhost";
      sg.group = "cn.fan";
      sg.table = "cn_fan";
      if( 1 && news.list_overview_with_status (ovl,sg, News::NewsUnread))
	{
  	  std::cout << ovl.size() << endl;
 	  News::OverviewList::iterator oi;
	  unsigned int mod;
	  for ( oi = ovl.begin(); 1 && oi < ovl.end(); ++oi )
	    {
	      mod = oi->id % 10;
	      if ( mod == 0 )
		{
		  //std::cout << oi->id << endl;
		  oi->status = News::NewsRead;
		  ovl2.push_back (*oi);
		}
 	    }
	  std::cout << ovl2.size() << endl;
	  news.mark_as_status ( sg, ovl2 );
	  ovl2.clear ();
	  std::cout << "Done." << endl;
	  std::cin >> mod;
 	}
    }
  if ( 0 && nntp.connect("localhost", 119))
    {

      News::id_type first, last, num;
      std::string line, resp, gn, article;
      if ( nntp.list_overview_fmt ( resp ))
	{
	  News::OverviewFMT overviewfmt;
	  News::OverView overview;
	  overviewfmt.init ( resp );
	  if ( nntp.group ("cn.fan", num, first, last, gn ) )
	    {
	      resp.erase ();
	      int count = 0;
 	      //news.begin ();
	      if ( nntp.xover (first, last, resp ))
		{
		  for ( std::string::iterator i = resp.begin(); i < resp.end();
			++i )
		    {
		      //if (count == 50 ) break;
		      if ( *i == '\r' ) continue;
		      if ( *i == '\n' )
			{
			  ++count;
 			  if ( 0 &&  count < 4000 )
 			    {
 			      line.erase ();
 			      continue;
 			    }
			  overview.clear ();
			  overviewfmt.parse (line);
			  overviewfmt >> overview;
 			  article.erase ();
			  std::cout << overview.id << endl;
 			  nntp.article (overview.id, article);
			  overview.status = News::NewsUnread;
			  news.push_queue ("localhost", "cn.fan",overview,article);
 			  line.erase ();
			  usleep (120000);
			}
		      else line += *i;
		    }
		}
	      news.update_queue ();
  	      //news.end ();
	    }
	}
    }
  return 1;
#endif
}
