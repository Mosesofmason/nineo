#include "exstring.hpp"
namespace ExString
{
  bool is_digit ( const std::string& str )
  {
    std::string::const_iterator i ;
    for ( i = str.begin(); i < str.end(); ++i )
      if ( (*i < '0' && *i > '9') || *i != '.' ) return false;
    return false;
  }
  void to_upper ( std::string& str )
  {
    std::string::iterator i;
    for ( i = str.begin (); i < str.end(); ++i )
      if ( *i >= 'a' && *i <= 'z' ) *i = *i - ('a'-'A');
  }
  void to_lower ( std::string& str )
  {
    std::string::iterator i;
    for ( i = str.begin(); i < str.end(); ++i )
      if ( *i >= 'A' && *i <= 'Z' ) *i = *i + ('a'-'A');
  }
  void trim ( std::string& str )
  {
    std::string::iterator i;
    for ( i = str.begin (); i < str.end(); ++i )
      if ( *i != ' ' || *i != '\t' ) break;
    if ( i != str.begin () ) str.erase(str.begin(), i);
  }
  void rtrim ( std::string& str )
  {
    std::string::reverse_iterator i;
    for ( i = str.rbegin() ; i < str.rend(); ++i )
      if ( *i != ' ' || *i != '\t' ) break;
    //if ( i != str.rbegin () ) str.erase (str.rbegin(), i );
  }
  inline match_type wildmat(const std::string& text,const std::string& pattern)
  {
    return mmatch ( text, pattern );
  }
  match_type mmatch ( const std::string& text, const std::string& pattern,
		      std::string::size_type pos )
  {
    std::string::const_iterator i = pattern.begin ();
    std::string line;
    match_type matched;
    for ( ; i <= pattern.end (); ++i )
      {
	if ( *i == ' ' || *i == '\t' ) continue;
	if ( *i == ',' || i == pattern.end () )
	  {
	    if ( !line.empty () )
	      {
		matched = match ( text, line, pos );
		if ( matched != FALSE ) return matched;
	      }
	    line.erase ();
	  }
	else line+= *i;
      }
    return FALSE;
  }
  match_type match ( const std::string& text, const std::string& pattern,
		     std::string::size_type pos )
  {
    std::string::const_iterator it = text.begin() , ip = pattern.begin();
    std::string::const_iterator ti, tt, tc;
    bool matched, reverse;
    
    if ( *ip == '*' )
      while ( ip < pattern.end() && *ip == '*' ) ++ip;
    if ( ip == pattern.end()) return TRUE;
    
    ip = pattern.begin ();
    if ( it + pos  >= text.end () ) return FALSE;
    for ( ; ip < pattern.end () && it <= text.end (); it++, ip++ )
      {
	if ( it == text.end() && *ip != '*' ) return ABORT;
	switch ( *ip )
	  {
	  case '\\':
	    ip++;
	  default:
	    if ( *it != *ip ) return FALSE;
	    continue;
	  case '?':
	    continue;
	  case '*':
	    while ( ip < pattern.end() && *ip == '*' ) ++ip;
	    if ( ip == pattern.end() ) return TRUE;
	    if ( *ip == '?' ) return ABORT;
	    for ( ti = ip ; ti < pattern.end (); ++ti )
	      if ( *ti == '?' || *ti == '*' ) break;
	    std::string::size_type tp = text.find (std::string(ip, ti),
						   it-text.begin ());
	    if ( tp != std::string::npos )
	      {
		it = text.begin()+tp+(ti-ip)-1;
		ip = ti - 1;
	      }
	    continue;
// 	  case '[':
// 	    reverse = false;
//  	    if ( ip + 1 < pattern.end() && *(ip+1) == '^' )
//  	      reverse = true;
//  	    if ( reverse ) ip++;
//  	    matched = false;
//  	    if ( ip+1 < pattern.end() && (*(ip+1) == ']' || *(ip+1) == '-'))
//  	      {
//  		if ( *(ip+1) == *it )
//  		  matched = true;
//  		++ip;
//  	      }
// 	    for (char last=*ip;ip+1<pattern.end()&&*(ip+1)!=']';++ip,last=*ip)
// 	      {
// 		if ( *ip == '-' && ip+1 <pattern.end() && *(ip+1) != ']' )
// 		  {
// 		    if ( *it <= *++ip && *it >= last )
// 		      matched = true;
// 		  }
// 		else if ( *it == *ip ) matched == true;
// 	      }
//  	    if ( matched == reverse )
//  	      return FALSE;
//  	    continue;
	  }
      }
    return ( it == text.end () ? TRUE : FALSE );
  }
}
