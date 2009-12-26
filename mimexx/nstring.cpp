#include "nstring.hpp"
namespace nstring
{
  string string_trim_blank (const string *src, int* start)
  {
    string::const_iterator begin = src->begin (), end = src->end()-1;
    string dsp;
    while ( (begin < end)  &&
	     ( *begin == ' ' || *begin == '\t' ||
	       *begin == '\r' || *begin == '\n')) begin++;
    if ( start ) *start = (begin-(src->begin()));
	    
    while ( begin <= end && (*end == ' ' ||
	    *end == '\t' || *end == '\r' || *end == '\n'))end--;
    for (; begin <= end; ++begin ) dsp += *begin;
    return dsp;
  }

  nstringarray tokenizer ( const string& str,const char sep )
  {
    nstringarray tmp;
    string::const_iterator ipos;
    string tmp_str;
    for ( ipos = str.begin (); ipos <= str.end (); ++ipos)
      {
	if ( ipos == str.end())
	  {
	    tmp.push_back( tmp_str );
	    break;
	  }
	if ( *(ipos) == sep )
	  {
	    tmp.push_back ( tmp_str );
	    tmp_str.erase ();
	    continue;
	  }
	tmp_str += *(ipos);
      }
    return tmp;
  }
  string string_replace (const string& source_string, const string& srep,
			 const string& trep, const int count)
  {
    string m_string = source_string;
    string::size_type str_pos, s_pos = 0;
    int m_count = count;
    while (1)
      {
	if ( m_count == 0 ) break;
	str_pos = m_string.find ( srep, s_pos);
	if ( str_pos == string::npos ) break;
	m_string.replace ( str_pos, srep.size (), trep);
	s_pos = str_pos;
	--m_count;
      }	
    return m_string;
  }
  
  string cut_string_by_tag ( const string& s_string, const string& s_tag,
			     const string& e_tag )
{
  string my_str = s_string;
  string::size_type s_pos, e_pos;
  my_str.erase ();
  s_pos = s_string.find (s_tag);
  e_pos = s_string.find (e_tag, s_pos+s_tag.size ());
  if (s_pos != string::npos && e_pos != string::npos) 
    {
      s_pos = s_pos + s_tag.size ();
      my_str = s_string.substr (s_pos, (e_pos-s_pos));
    }
  return my_str;
}
string cut_string_by_tag ( const string s_string, const string s_tag,
			   const string e_tag )
{
  string my_str = s_string;
  string::size_type s_pos, e_pos;
  my_str.erase ();
  s_pos = s_string.find (s_tag);
  e_pos = s_string.find (e_tag, s_pos+s_tag.size ());
  if (s_pos != string::npos && e_pos != string::npos) 
    {
      s_pos = s_pos + s_tag.size ();
      my_str = s_string.substr (s_pos, (e_pos-s_pos));
    }
  return my_str;
}

  string lower_string ( const string& s_string)
  {
    string my_str;
    my_str.erase ();
	
    if (s_string.size() == 0) return my_str;
    for ( string::const_iterator ipos = s_string.begin () ; ipos < s_string.end (); ++ipos)
      {
	my_str += tolower ( (int) *ipos);
      }
    return my_str;
  }

  string upper_string ( const string& s_string )
  {
    string my_str;
    my_str.erase ();
	
    if (s_string.size () == 0) return my_str;
    for ( string::const_iterator ipos = s_string.begin (); ipos < s_string.end (); ++ipos)
      {
	my_str += toupper ( (int) *ipos);
      }
    return my_str;
  }
//   string int2str ( const int& num )
//   {
//     string str;
//     char buff[50];
//     sprintf (buff, "%d", num);
//     str = buff;
//     return str;
//   }

//   string stripnull ( const char* buff, size_type len)
//   {
//     string stringbuff;
//     for ( size_type pos=0; pos < len; ++pos)
//       if ( *(buff+pos) != '\0' ) stringbuff += *(buff+pos);
//     return stringbuff;
//   }  
}
		     
