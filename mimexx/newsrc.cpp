#include "newsrc.hpp"

namespace News
{
  namespace Newsrc
  {
    void Range::print()
    {
      if ( min )
	std::cout << "[" << min << "-" << max << "]" << endl;
      else
	std::cout << max << endl;
    }

    std::string Range::str ()
    {
      std::ostringstream os;
      if ( max && min )
	{
	  os << min << "-" << max;
	  return os.str();
	}
      else if ( max )
	{
	  os << max;
	  return os.str();
	}
      else return "";
    }
    void Range::set ( const id_type& fmin, const id_type& fmax )
    {
      max = 0; min = 0;
      if ( fmin && fmin < fmax )
	{
	  max = fmax;
	  min = fmin;
	}
      else if ( !fmin )
	max = fmax;
    }
    void Range::set (const id_type& id )
    {
      min = 0;
      max = id;
    }
    void Range::parse_num ( const std::string& num )
    {
      min = 0;
      std::istringstream is(num);
      is >> max;
    }
    void Range::parse_range ( const std::string& range )
    {
      std::istringstream is (range);
      char ch;
      is >> min;
      is >> ch;
      is >> max;
    }
    bool Range::operator< ( const Range& range ) const
    {
      if ( min && range.min )
	{
	  return min < range.min;
	}
      else if ( !min && range.min )
	{
	  return max < range.min ;
	}
      else if ( min && !range.min )
	{
	  return min < range.max;
	}
      else return max < range.max;
    }
    bool Range::operator== ( const Range& range )
    {
      if ( min != 0 )
	return ( max == range.max && min == range.min );
      else
	return ( max <= range.max && max >= range.min );
    }
    bool Range::operator== ( const id_type& id )
    {
      return ( min ? false : id == max );
    }
    bool Range::in ( const id_type& id )
    {
      if ( min )
	return ( min > id && id < max );
      return ( id == max );
    }
    bool Range::in ( const Range& range )
    {
      if ( min && range.min )
	return ((range.min >= min && range.min <= max && range.max <= max )
		//||
		//(range.min <= min && range.min <= max && range.max <= max )
		);
      else if ( !min && range.min )
	return ( max >= range.min && max <= range.max );
      else if ( min && !range.min )
	return ( range.max >= min && range.max <= max );
      return false;
    }
    bool Range::operator/= ( const id_type& id )
    {
      return in ( id );
    }
    bool Range::operator/= ( const Range& range)
    {
      return in ( range );
    }
    bool Range::operator+= ( const Range& range )
    {
      return inter ( range );
    }
    bool Range::inter ( const Range& range )
    {
      id_type tmin, tmax;
      if (min && range.min)
	{
	  tmin = ( min < range.min ? range.min : min );
	  tmax = ( max < range.max ? max : range.max );
	  if (tmin < tmax)
	    {
	      return true;
	    }
	  else if ((int)(tmin-tmax) == 1 || (int)(tmin-tmax) == -1)
	    {
	      return true;
	    }
	  else if ( range.min - max == 1 || min - range.max == 1)
	    {
	      return true;
	    }
	  else if ( max == range.min )
	    {
	      return true;
	    }
	  else return false;
	}
      else if ( !min && range.min )
	{
	  return ((range.min <= max && range.max >= max) ||
		  ((range.min-max) == 1 || (max-range.max) == 1));
	}
      else if ( min && !range.min )
	{
	  return ((range.max >= min && range.max <= max)
		  ||
		  ((min-range.max) == 1 || (range.max-max) == 1));
	}
      else if ( !min && !range.min)
	{
	  return (max - range.max == 1 || range.max - max == 1);
	}
      return false;
    }
    bool Range::inter ( const id_type& id )
    {
      return in ( id );
    }
    bool Range::operator+ ( const id_type& id )
    {
      return merge (id);
    }
    bool Range::operator+ ( const Range& range )
    {
      return merge (range);
    }

    bool Range::operator/ ( const id_type& id)
    {
      if ( id >= min && id <= max ) return true;
      if ( id + 1 == min ) min = id;
      if ( id - 1 == max ) max = max;
      return true;
    }
    bool Range::operator/ ( const Range& range )
    {
      if ( !(range.max > max && range.min < min ) &&
	   !(range.max < max && range.min > min )) return false;
      if ( range.min < min ) min = range.min;
      if ( range.max > max ) max = range.max;
      return true;
    }
    bool Range::merge ( const Range& range )
    {
      if (!inter(range)) return false;
      if ( min && range.min )
	{
	  if ( range.min + 1 == min )
	    min = range.min;
	  if ( range.max - 1 == max )
	    max = range.max;
	  if ( min >= range.min)
	    min = range.min;
	  if ( max <= range.max )
	    max = range.max;
	  if ( max == range.min )
	    max = range.max;
	  return true;
	}
      else if ( !min && range.min )
	{
	  if ( max+1 == range.min )
	    {
	      min = max;
	      max = range.max;
	      return true;
	    }
	  if ( max-1 == range.max )
	    {
	      min = range.min;
	      return true;
	    }
	  if ( range.min <= max && range.max >= max )
	    {
	      min = range.min;
	      max = range.max;
	      return true;
	    }
	}
      else if ( min && !range.min )
	{
	  if ( range.max + 1 == min )
	    {
	      min = range.max;
	      return true;
	    }
	  if ( range.max - 1 == max )
	    {
	      max = range.max;
	      return true;
	    }
	  if ( range.max >= min && range.max <= max )
	    {
	      return true;
	    }
	}
      else if ((!min && !range.min))
	{
	  if (max - range.max == 1)
	    {
	      min = range.max;
	      return true;
	    }
	  else if (range.max - max == 1)
	    {
	      min = max;
	      max = range.max;
	    }
	}
      return in (range);
      return false;
    }
    bool Range::merge ( const id_type& id )
    {
      if ( min )
	{
	  if ( id >= min && id <= max )
	    return true;
	  if ( id + 1 == min )
	    {
	      min = id;
	      return true;
	    }
	  else if ( id - 1 == max )
	    {
	      max = id;
	      return true;
	    }
	}
      else
	{
	  if ( id + 1 == max )
	    {
	      min = id;
	      return true;
	    }
	  else if ( id - 1 == max )
	    {
	      id_type ti;
	      ti = max;
	      max = id;
	      min = ti;
	      return true;
	    }
	}
      return false;
    }
    void Range::clear ()
    {
      max = 0;
      min = 0;
    }
    Range& Range::operator= ( const Range& range )
    {
      if ( range.min )
	min = range.min;
      else min = 0;
      max = range.max;
      return *this;
    }
    Range& Range::operator= ( const std::string& range )
    {
      clear ();
      std::string l_r;
      std::string::const_iterator i;
      for ( i = range.begin(); i < range.end() && *i == ' '; ++i);
      while ( i < range.end () && (*i != ' ' && *i != ','))
	l_r += *i++;
	
      if (l_r.find ("-") != std::string::npos )
	{
	  parse_range (l_r);
	}
      else
	{
	  parse_num (l_r);
	}
      return *this;
    }
    bool Range::add ( const Range& range )
    {
      if ( min && range.min )
	{
	  if ( in (range) ) return true;
	}
      else 
	{
	  if ( max + 1 == range.max )
	    {
	      min = max; max = range.max;
	      return true;
	    }
	  else if ( max - 1 == range.max )
	    {
	      min  = range.min;
	      return true;
	    }
	}
      return false;
    }
    bool Range::operator<< ( const Range& range )
    {
      return add ( range );
    }
    bool Range::add ( const id_type& id )
    {
      if ( id + 1 == min )
	{
	  min = id;
	  return true;
	}
      else if ( id - 1 == max )
	{
	  max = id;
	  return true;
	}
      else return false;
    }
    bool Range::operator<< ( const id_type& id )
    {
      return add ( id );
    }
    bool Range::del ( const id_type& id, Range& r )
    {
      if ( id >= min && id <= max )
	{
	  r.max = max;
	  r.min = id + 1;
	  max = id - 1;
	  return true;
	}
      else return false;
    }
    bool Newsrc2::load ()
    {
    }
    bool Newsrc2::save ()
    {
    }
    void Newsrc2::print ()
    {
      GroupHash::iterator i;
      std::cout << "size of groups: " << m_groups.size() << endl;
      for ( i = m_groups.begin(); i != m_groups.end(); ++i )
	{
	  (i->second).print();
	}
    }
    bool Newsrc2::merge ()
    {
      for (GroupHash::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
	{
	  i->second.merge ();
	}
    }
    bool Newsrc2::mark ( const std::string& group, const id_type& id )
    {
      //std::cout << "Newsrc2 mark: " << group << " => " << id << endl;
      GroupHash::iterator i;
      i = m_groups.find (group);
      if ( i != m_groups.end () && i->second.group == group )
	{
	  Ranges::iterator ri;
	  Range range;
	  range.set (id);
	  
	  if (i->second.ranges.size () >= 1 )
	    {
	      for (ri=i->second.ranges.begin();ri<i->second.ranges.end();++ri)
		{
		  if ( *ri /= range )
		    {
		      *ri / range;
		      break;
		    }
		  else if ( *ri += range )
		    {
		      (*ri) + range;
		      i->second.merge ();
		      break;
		    }
		}
	      if ( ri == i->second.ranges.end() )
		{
		  i->second.ranges.push_back (range);
		}
	    }
	  else i->second.ranges.push_back (range);
	  i->second.merge ();
	}
      else
	{
	  Group new_group;
	  new_group.group = group;
	  new_group.subscribe =  true;
	  new_group.ranges.push_back (id);
	  m_groups[group] = new_group;
	}
    }
    bool Newsrc2::unmark ( const std::string& group, const id_type& id )
    {
      
    }
    bool Newsrc2::ismark ( const std::string& group, const id_type& id )
    {
      
    }
    void Newsrc2::subscribe ( const std::string& group )
    {
      GroupHash::iterator pos;
      pos = m_groups.find (group);
      if ( pos == m_groups.end() )
	{
	  Group new_group;
	  new_group.group = group;
	  new_group.subscribe = true;
	  m_groups[group] = new_group;
	}
      else
	{
	  if ( pos->second.group == group )
	    pos->second.subscribe = true;
	}
    }
    Newsrc::Newsrc ()
    {}
    Newsrc::~Newsrc()
    {}
    bool Newsrc::load ( const std::string& filename )
    {
      std::ifstream ifile ( filename.c_str());
      if ( !ifile ) return false;
      std::string line;
      char ch;
      bool r;
      Group group;
      GroupHash::iterator ipos;
      while (1)
	{
	  r = ifile.get ( ch );
	  if ( ch == '\n' || !r )
	    {
	      group.clear ();
	      group.parser (line);
	      if ( !group.group.empty() )
		{
		  ipos = m_groups.find( group.group );
		  if ( ipos == m_groups.end() )
		    {
		      m_groups[group.group] = group;
		    } else ipos->second << group;
		}
	      line.erase ();
	      if (!r) break;
	    }
	  else
	    line += ch;
	}
      ifile.close ();
    }
    void Newsrc::print ()
    {
      GroupHash::iterator ip;
      for ( ip = m_groups.begin (); ip != m_groups.end(); ++ip )
	{
	  ip->second.print();
	}
    }
    bool Newsrc::save ( const std::string& filename )
    {
      std::ofstream ofile ( filename.c_str() );
      if (!ofile) return false;
      GroupHash::iterator ipos;
      std::string line;
      for ( ipos = m_groups.begin(); ipos != m_groups.end(); ++ipos )
	{
	  if ( ipos->second.ids.size() )
	    {
	      ipos->second.str(line);
	      ofile << line << "\n";
	    }
	}
      ofile.close ();
      return true;
    }
    void Newsrc::mark ( const std::string& group, const id_type& id )
    {
      GroupHash::iterator i;
      i = m_groups.find ( group );
      if ( i != m_groups.end () )
	{
	  if ( std::find ((i->second).ids.begin(), (i->second).ids.end(),id) 
	       == i->second.ids.end())
	    i->second.ids.push_back (id);
	}
      else
	{
	  Group g;
	  g.group = group;
	  g.ids.push_back (id);
	  m_groups[group] = g;
	}
    }
    void Newsrc::unmark ( const std::string& group, const id_type& id )
    {
      GroupHash::iterator i;
      i = m_groups.find (group);
      if ( i != m_groups.end() )
	{
	  i->second.ids.erase(std::remove (i->second.ids.begin(),
					   i->second.ids.end(), id),
			      i->second.ids.end());
	}
    }
    void Newsrc::subscribe ( const std::string& group )
    {
      GroupHash::iterator i;
      i = m_groups.find (group);
      if ( i != m_groups.end() )
	{
	  i->second.subscribe = true;
	}
    }
    bool Newsrc::ismark ( const std::string& group, const id_type& id )
    {
      GroupHash::iterator i;
      i = m_groups.find (group);
      if ( i != m_groups.end() )
	{
	  if ( std::find (i->second.ids.begin(),
			  i->second.ids.end(), id ) != i->second.ids.end())
	    return true;
	}
      return false;
    }
  }
}
