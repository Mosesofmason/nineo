#ifndef __NEWS_RC_H__
#define __NEWS_RC_H__
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "newstype.hpp"

namespace News
{
  namespace Newsrc
  {
    class Range
    {
    private:
      id_type max, min;
    public:
      Range ()
	: max(0), min(0)
      {}
      Range ( const id_type &id )
	: max(id), min(0)
      {}
      Range ( const id_type &fmin, const id_type& fmax )
	: max(fmax), min(fmin)
      {
	if ( max < min )
	  {
	    max = 0;
	    min = 0;
	  }
      }
      void print();
      std::string str ();
      void set ( const id_type& fmin, const id_type& fmax );
      void set (const id_type& id );
      void parse_num ( const std::string& num );
      void parse_range ( const std::string& range );
      bool operator< ( const Range& range ) const;
      bool operator== ( const Range& range );
      bool operator== ( const id_type& id );
      bool in ( const id_type& id );
      bool in ( const Range& range );
      bool operator/= ( const id_type& id );
      bool operator/= ( const Range& range);
      bool operator+= ( const Range& range );
      bool inter ( const Range& range );
      bool inter ( const id_type& id );
      bool operator/ ( const id_type& id);
      bool operator/ ( const Range& range );
      bool operator+ ( const id_type& id );
      bool operator+ ( const Range& range );
      bool merge ( const Range& range );
      bool merge ( const id_type& id );
      void clear ();
      Range& operator= ( const Range& range );
      Range& operator= ( const std::string& range );
      bool add ( const Range& range );
      bool operator<< ( const Range& range );
      bool add ( const id_type& id );
      bool operator<< ( const id_type& id );
      bool del ( const id_type& id, Range& r );
    };
    typedef vector< Range > Ranges;
    class Newsrc2
    {
      struct Group
      {
	std::string group;
	bool subscribe;
	Ranges ranges;
	Group()
	  : subscribe(false)
	{}
	void print ()
	{
	  std::cout << group << ": " << endl;
	  for ( Ranges::iterator i = ranges.begin(); i < ranges.end(); ++i )
	    {
	      std::cout << "\t[" << i->str() << "]"<< endl;
	    }
	}
	void clear ()
	{
	  subscribe = false;
	  group.erase(); ranges.clear ();
	}
	std::string str ()
	{
	  std::ostringstream os;
	  os << group << ( subscribe ? ":" : "!");
	  if ( ranges.size() )
	    for (Ranges::iterator i = ranges.begin(); i < ranges.end(); ++i)
	      {
		os << ( i == ranges.begin () ? " " : ",")
		   << (*i).str();
	      }
	  else os << 1;
	  return os.str();
	}
	void merge ()
	{
	  Range range1, range2;
	  Ranges result;
	  Ranges::const_iterator it, st, tt;
#if 0
	  std::cout << "List: " << endl;
	  for ( it = ranges.begin (); it < ranges.end (); ++it )
	    {
	      range1 = *it;
	      std::cout << range1.str() << " ";
	    }
	  std::cout << endl;
#endif
	  for ( it = ranges.begin (); it < ranges.end (); ++it)
	    {
	      st = it + 1;
	      if ( st == ranges.end() )
		{
		  result.push_back (*it);
		  break;
		}
	      range1 = *it;
	      range2 = *st;
	      if (range1 /= range2)
		{
		  for ( tt = st; tt < ranges.end(); ++tt )
		    {
		      range2 = *tt;
		      if ((range1 /= range2))
			  st = tt; 
		      else break;
		    }
		  result.push_back (range1);
		  it = st;
		}
	      else if (range1 += range2)
		{
		  for (tt = st; tt < ranges.end(); ++tt )
		    {
		      range2 = *tt;
		      if (range1 += range2)
			{
			  st = tt; range1 + range2;
			}
		      else break;
		    }
		  result.push_back (range1); 
		  it = st;
		}
	      else
		{
		  result.push_back (*it);
		}
	      while (result.size() >= 2)
		{
		  range1 = result.back(); result.pop_back();
		  range2 = result.back(); result.pop_back();
		  std::cout << "<<" << range2.str() << " "
			    << range1.str() << endl;
		  if ( (range1 += range2) || (range2 += range1) )
		    {
		      range1 + range2;
		      result.push_back (range1);
		    }
		  else
		    {
		      result.push_back(range2);
		      result.push_back(range1);
		      break;
		    }
		}
	    }
	}
	void parser ( const std::string& line )
	{
	  std::string item;
	  std::string::const_iterator i = line.begin ();
	  Range range;
	  for ( ; i < line.end (); ++i )
	    {
	      if ( *i == ':' || *i == '!')
		{
		  subscribe = ( *i == ':' ? true : false );
		  group  = item;
		  item.erase();
		  ++i;
		  break;
		}
	      item += *i;
	    }
	  for ( ; i <= line.end(); ++i)
	    {
	      if ( *i == ',' || *i == '\n' || i == line.end())
		{
		  //parse_id (item);
		  range = item;
		  ranges.push_back ( range );
		  item.erase ();
		  if  ( i == line.end() ) break;
		}
	      else if ( *i == ' ' )
		{
		  continue;
		}
	      else item += *i;
	    }
	}
#if 0
	  std::cout << "Result: "<< endl;
	  for ( it = result.begin (); it < result.end (); ++it )
	    {
	      range1 = *it;
	      std::cout << range1.str() << " ";
	    }
	  std::cout << endl;
#endif
      }; // end of Group
      typedef map < std::string, Group > GroupHash;
    private:
      GroupHash m_groups;
      
    public:
      Newsrc2 ()
      {}
      ~Newsrc2 ()
      {}
      void print ();
      bool load ();
      bool save ();
      bool merge ();
      bool mark ( const std::string& group, const id_type& id );
      bool unmark ( const std::string& group,  const id_type& id );
      bool ismark ( const std::string& group, const id_type& id );
      void subscribe ( const std::string& group );
    };// end of Newsrc2
    
    class Newsrc
    {
      typedef vector < id_type > id_list;
      struct Group
      {
	std::string group;
	id_list ids;
	bool subscribe;
	void str ( std::string& line )
	{
	  line.erase();
	  std::ostringstream os;
	  os << group << ( subscribe ? ":" : "!");
	  std::sort (ids.begin (), ids.end());
	  ids.erase(std::unique (ids.begin(), ids.end()), ids.end());
	  id_list::iterator i, it, ip;
	  if ( ids.size () != 0 )
	    for ( i = ids.begin() ; i < ids.end(); ++i )
	      {
		if ( i+1 < ids.end() && (*i)+1 == *(i+1))
		  {
		    for ( it = i+1; it+1 < ids.end() ; ++it )
		      if ( (*it)+1 != *(it+1) ) break;
		    os << ( i != ids.begin() ? "," : " ")
		       << *i << "-" << *it;
		    i = it;
		  }
		else
		  os << ( i != ids.begin() ? "," : " ") << *i;
	      }
	  line = os.str();
	}
	void print ()
	{
	  std::sort (ids.begin (), ids.end());
	  ids.erase (std::unique (ids.begin(), ids.end()),
		     ids.end());
	  id_list::iterator i, it, ip;
	  std::cout << "Size: " << ids.size () << endl;
	  // 	for ( i = ids.begin (); i < ids.end(); ++i )
	  // 	  std::cout << *i << " ";
	  // 	std::cout << endl;
	  std::cout << group << ( subscribe ? ":" : "!");
	  if ( ids.size () != 0 )
	    for ( i = ids.begin() ; i < ids.end(); ++i )
	      {
		if ( i+1 < ids.end() && (*i)+1 == *(i+1))
		  {
		    for ( it = i+1; it+1 < ids.end() ; ++it )
		      if ( (*it)+1 != *(it+1) ) break;
		    std::cout << ( i != ids.begin() ? "," : " ")
			      << *i << "-" << *it;
		    i = it;
		  }
		else
		  std::cout << ( i != ids.begin() ? "," : " ") << *i;
	      }
	  std::cout << endl;
	}
	void clear ()
	{
	  group.erase ();   ids.clear ();
	}
	void parse_id ( const std::string& id_line)
	{
	  if (id_line.find ("-") != std::string::npos)
	    {
	      std::istringstream si (id_line);
	      id_type start = 0, end = 0;
	      char ch;
	      si >> start;
	      si >> ch;
	      si >> end;
	      if ( start != 0 && end != 0 )
		for ( id_type s = start; s <= end ; ++s )
		  ids.push_back (s);
	    }
	  else
	    {
	      std::istringstream si ( id_line );
	      id_type id = 0;
	      si >> id;
	      if ( id != 0 ) ids.push_back(id);
	    }
	}
	void parser ( const std::string& line )
	{
	  std::string sgroup, id;
	  std::string::const_iterator i = line.begin ();
	  for ( ; i < line.end (); ++i )
	    {
	      if ( *i == ':' || *i == '!')
		{
		  subscribe = ( *i == ':' ? true : false );
		  ++i;
		  break;
		}
	      sgroup += *i;
	    }
	  for ( ; i <= line.end(); ++i)
	    {
	      if ( *i == ',' || *i == '\n' || i == line.end())
		{
		  parse_id (id);
		  id.erase ();
		  if  ( i == line.end() ) break;
		}
	      else if ( *i == ' ' )
		{
		  continue;
		}
	      else
		{
		  id += *i;
		}
	    }

	  std::sort (ids.begin(), ids.end());
	  std::unique (ids.begin(), ids.end());
	  group = sgroup;
	}
	Group& operator+ (Group& g2)
	{
	  if ( g2.group == group )
	    {
	      std::copy ( g2.ids.begin(), g2.ids.end(), std::back_inserter(ids));
	      std::sort ( ids.begin(), ids.end() );
	      std::unique ( ids.begin(), ids.end());
	    }
	  return *this;
	}
	Group& operator<< (Group& g2 )
	{
	  if ( g2.group == group )
	    {
	      std::copy ( g2.ids.begin(), g2.ids.end(), std::back_inserter(ids));
	      std::sort ( ids.begin(), ids.end() );
	      std::unique ( ids.begin(), ids.end() );
	    }
	  return *this;
	}
	Group& operator= (Group& g2 )
	{
	  group = g2.group;
	  ids = g2.ids;
	  return *this;
	}
      };
      // end of Group
      typedef map < std::string , Group > GroupHash;
    private:
      GroupHash m_groups;
    public:
      Newsrc ();
      ~Newsrc ();
      bool load ( const std::string& filename );
      bool save ( const std::string& filename );
      bool ismark ( const std::string& group, const id_type& id );
      void mark ( const std::string& group, const id_type& id );
      void unmark ( const std::string& group, const id_type& id );
      void subscribe ( const std::string& group );
      void print ();
    };
  }
}
#endif
