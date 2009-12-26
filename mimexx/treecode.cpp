
struct Article
{
  std::string m_from, m_subject, m_messageid, m_references;
  int m_refer_count;
  Article ( const std::string &f, const std::string &s,
	    const std::string &m, const std::string &r)
   {
     m_from = f, m_subject = s, m_messageid = m , m_references = r;
     m_refer_count = count_of_references();
  };
  int count_of_references ()
  {
    std::string::iterator iter;
    int count = 0;
    for ( iter = m_references.begin(); iter < m_references.end(); ++iter)
      if ( *iter == '<' ) ++count;
    return count;
  }
  Article ( const std::string &m )
    : m_messageid(m) {};
  Article (){};
  void print ()
  {
    std::cout << "From: " << m_from 
	      << "\n  \tSubject:" << m_subject
	      << "\n  \t\tID:" << m_messageid << endl;
  };
  void clear ()
  {
    m_from.erase();
    m_subject.erase ();
    m_messageid.erase ();
  };
  bool operator == ( const Article& r)
  {
    //if (!r.m_messageid.empty())
    return m_messageid == r.m_messageid;
    //if (!r.m_subject.empty ()) return m_subject == r.m_subject;
      //return false;
  }
  Article& set_messageid ( const std::string &m )
  {
    m_messageid = m;
    return *this;
  };
};
inline bool operator< ( const Article& l, const Article& r )
{
  return l.m_refer_count < r.m_refer_count;
//   if (!r.m_from.empty()) return l.m_from < r.m_from;
//   if (!r.m_subject.empty()) return l.m_subject < r.m_subject;
//   if (!r.m_messageid.empty()) return l.m_messageid < r.m_messageid;
  return false;
};
inline std::ostream& operator<<( std::ostream &o, Article& t )
{
  o << t.m_subject << " " << t.m_refer_count;
  //    << "\n";
  return o;
}

typedef Beyooo::Tree::Tree<std::string, Article> xTree;
typedef Beyooo::Tree::TreeNode<std::string, Article> xNode;

std::string last_reference ( const std::string& refer, const int& p)
{
  if ( refer.empty () ) return "";
  std::string::const_iterator i;
  std::string line;
  int x = 0;
  for ( i = refer.begin (); i < refer.end (); ++i)
    {
      if ( *i == '\t' || *i == '\n' || *i == ' ' ) continue;
      line += *i;
      if ( *i == '>')
	{
	  if ( x == p)
	    {
	      return line;
	    }
	  ++x;
	  line.erase ();
	}
    }
  return "";
}
//   xTree *news_tree;
//   xNode *news_node;
//   news_tree = new xTree ("Root",Article ());
//   SQLite3 sqlite;
//   if (!sqlite.open ("localhost.db")) return 0;
//   if ( !sqlite.hastable ("System"))
//     std::cout << "System not exists." << endl;
//   if ( !sqlite.hastable ("localhost_GL"))
//     {
//       std::string sql = "CREATE TABLE localhost_GL (_GROUP_ varchar(25), _DESC_ varchar(30));";
//       if ( sqlite.exec (sql))
// 	{
// 	  std::cout << "Create table localhost_GL OK."<<endl;
// 	}else
// 	std:: cout << "Create table localhost_GL Fail." << endl;
//     }

// 	  int o_subject, o_from, o_date, o_messageid, o_references, o_bytes,
// 	    o_lines, o_xref;
// 	  //last = first + 200;
// 	  std::vector<Article> articles;
// 	  if ( nntp.list_overview_fmt (resp) )
// 	    {
// 	      o_subject = nntp.overview_fmt ( resp, "Subject" );
// 	      o_from = nntp.overview_fmt ( resp, "From" );
// 	      o_date = nntp.overview_fmt ( resp, "Date");
// 	      o_messageid = nntp.overview_fmt ( resp, "Message-ID");
// 	      o_references = nntp.overview_fmt ( resp, "References");
// 	      o_bytes = nntp.overview_fmt ( resp, "Bytes");
// 	      o_lines = nntp.overview_fmt ( resp, "Lines");
// 	      o_xref = nntp.overview_fmt ( resp, "Xref");
// 	      if ( nntp.xover (first,last, resp))
// 		{
// 		  std::string::iterator i;
// 		  for ( i = resp.begin (); i < resp.end(); ++i)
// 		    if ( *i == '\n')
// 		      {
// 			std::string xsubject, xfrom, xmessageid, xreferences,
// 			  xref;
// 			if (o_subject != 0)
// 			  nntp.xover_extract ( line, xsubject, o_subject);
// 			if (o_from != 0 )
// 			  nntp.xover_extract ( line, xfrom, o_from);
// 			if (o_messageid != 0)
// 			  nntp.xover_extract ( line, xmessageid, o_messageid);
// 			if (o_references != 0)
// 			  nntp.xover_extract ( line, xreferences, o_references);
// 			if ( o_xref != 0 )
// 			  nntp.xover_extract ( line, xref, o_xref );
// 			articles.push_back (Article (xfrom, xsubject,
// 						     xmessageid, xreferences));
// 			line.erase ();
// 		      }else line += *i;
// 		}
	      
// 	      std::sort (articles.begin (), articles.end());
// 	      std::vector<Article>::iterator aiter;
// 	      for (aiter = articles.begin (); aiter < articles.end(); ++aiter)
// 		if ( !aiter->m_references.empty())
// 		  {
// 		    bool cc = false;
// 		    for ( int i = 0; i <= aiter->m_refer_count; ++i)
// 		      {
// 			std::string ref = last_reference (aiter->m_references,
// 							  aiter->m_refer_count-i);
// 			news_node = news_tree->find (ref);
// 			if (news_node != NULL)
// 			  {
// 			    news_tree->insert (aiter->m_messageid, *aiter,
// 					       news_node);
// 			    cc = true;
// 			    break;
// 			  }
//  		      }
// 		    if ( cc == false )
// 		      {
// 			news_node = news_tree->find_subject(aiter->m_subject);
// 			if (news_node != NULL)
// 			  news_tree->insert (aiter->m_messageid,
// 					     *aiter,news_node);
// 			else
// 			  {
// 			    news_node = \
// 			      news_tree->insert (
// 						 last_reference(aiter->m_references,0),*aiter);
// 			    news_tree->add_subject(aiter->m_subject,news_node);
// 			  }
// 		      }
// 		  }else
// 		  {
// 		    news_node = news_tree->insert (aiter->m_messageid, *aiter);
// 		    news_tree->add_subject (aiter->m_subject, news_node);
// 		  }
// 	    }

//   news_tree->print ();
//   delete news_tree;
