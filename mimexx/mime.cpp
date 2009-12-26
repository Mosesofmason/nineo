#include "mime.hpp"

namespace Mime
{
  Mime::Mime (bool only_ph)
  {
    only_parser_header = only_ph;
    clear ();
  }
    
  Mime::~Mime () {}

  bool Mime::parser ( const std::string &raw_message )
  {
    m_raw_message += raw_message;
    return true;
  }
    
  void Mime::clear ()
  {
    m_raw_message.erase ();
    m_content_type.erase ();
    m_content_sub_type.erase ();
    m_charset.erase ();
    m_content_transfer_encoding.erase ();
    m_subject.erase ();
    m_body.erase ();
    m_boundary.erase ();
    m_headers.clear ();
    m_parts.clear();
    m_attch.clear();
  }
  std::string Mime::get_header ( const std::string &name, const std::string &no, const bool &ignore_case)
  {
    if ( m_headers.empty () ) return no;
    header_list::iterator ptr = m_headers.begin ();
    for ( ; ptr < m_headers.end (); ++ptr )
      {
	if ( !ignore_case && ptr->first == name )
	  return ptr->second;
      }
    return "";
  }
  
//   std::string Mime::get_header ( const std::string &name, bool ignore_case)
//   {
//     return get_header ( name, "", ignore_case );
//   }

  std::string Mime::get_de_header ( const std::string &name, bool ign_case)
  {
    std::string header = get_header ( name, "", ign_case );
    if ( header.empty () ) return "";
    return decode_field ( header );
  }
#if 0
  std::string get_field_encoding ( const std::string& field)
  {
    std::string::const_iterator ip = field.begin();
    std::string encode, charset;
    while ( ip < field.end () )
      {
	if ((ip+1) < field.end () && *ip == '=' && *(ip+1) == '?')
	  {
	    ip += 2;
	    charset.erase ();
	    while ( (ip+1) < field.end () && *ip != '?' && *(ip+1) != '=')
	      {
		charset += *ip;
		++ip;
	      }
	    if ( *ip == '?' ) ++ip;
	    ++ip;
	    if (*ip == '?') ++ip;
	    while (ip < field.end())
	      {
		if ((ip+4)<field.end() && *ip == '?' && *(ip+1) == '=' &&
		    *(ip+2) == ' ' && *(ip+3) == '=')
		  {
		    ip += 3;
		    break;
		  }
		else if ((ip+1)<field.end() && *ip == '?' && *(ip+1) == '=')
		  {
		    ip += 2;
		    break;
		  }
		++ip;
	      }
	    charset = nstring::upper_string(charset);
	    if (encode.empty())
	      encode += charset;
	    else if ( encode.find(charset) == std::string::npos )
	      encode += ("|" + charset);
	  }else ++ip;
      }
    return encode;
  }
  int decode_field_count ( const std::string& field )
  {
    int count = 0;
    std::string::size_type pos = 0;
    while (1)
      {
	pos = field.find ("?=", pos);
	if (pos == std::string::npos ) break;
	++count;
	pos += 2;
      }
    return count;
  }
  bool decode_field_1 ( const std::string &field, std::string& result)
  {
    std::cout << "Do 1." << endl;
    std::string::const_iterator ip = field.begin ();
    std::string charset, encode, sresult, tstring, all_encode;
    char encode_type;
    while ( ip < field.end () )
      {
	if ((ip+1) < field.end () && *ip == '=' && *(ip+1) == '?')
	  {
	    ip += 2;
	    charset.erase ();
	    while ( (ip+1) < field.end () && *ip != '?' && *(ip+1) != '=')
	      {
		charset += *ip;
		++ip;
	      }
	    if ( *ip == '?' ) ++ip;
	    if ( *ip != '?' ) encode_type = *ip;
	    ++ip;
	    if (*ip == '?') ++ip;
	    tstring.erase ();
	    while (ip < field.end())
	      {
		if ((ip+4)<field.end() && *ip == '?' && *(ip+1) == '=' &&
		    *(ip+2) == ' ' && *(ip+3) == '=')
		  {
		    ip += 3;
		    break;
		  }
		else if ((ip+1)<field.end() && *ip == '?' && *(ip+1) == '=')
		  {
		    ip += 2;
		    break;
		  }
		tstring += *ip;
		++ip;
	      }
// 	    while (ip < field.end() &&
// 		   (*ip == ' '||*ip == '\r'||*ip == '\t'||*ip == '\n'))++ip;
	    charset = nstring::upper_string(charset);
	    if (charset == "GB2312") charset = "GBK";
	    if (all_encode.empty())
	      all_encode += charset;
	    else if ( all_encode.find(charset) == std::string::npos)
	      all_encode += ("|" + charset);
	    std::string decode;
	    if ( encode_type == 'b' || encode_type == 'B')
	      {
		base64_decode (decode, tstring);
	      }
	    else if ( encode_type == 'q' || encode_type == 'Q')
	      {
		qp_decode (decode, tstring);
		for (std::string::iterator iii = decode.begin();
		     iii < decode.end(); ++iii)
		  if ( *iii == '_' ) *iii = ' ';
	      }
	    sresult += decode;
	  }else
	  {
	    sresult += *ip ;
	    ++ip;
	  }
      }
    if (!all_encode.empty() && all_encode.find("|") == std::string::npos )
      {
	// if (ChineseToUTF16(all_encode, sresult ,result))
	  return true;
      }
    else
      {
	std::cout << "Error: " << sresult << endl
		  << "code:" << all_encode << endl;
      }
    return false;
  }
  bool decode_field_2 ( const std::string &field, std::string& result)
  {
    std::cout << "Do 2." << endl;
    std::string::const_iterator ip = field.begin ();
    std::string sresult, tstring, all_encode;
    bool is_mcode;
    default_charset = get_field_encoding (field);
    if ( default_charset.find("|") != std::string::npos )
      {
	default_charset = default_charset.substr(0, default_charset.find("|"));
	is_mcode;
      }
    while ( ip <= field.end () )
      {
	if ( ip == field.end() )
	  {
	    std::string buf;
// 	    if (ChineseToUTF16("GBK", tstring, buf))
// 	      {
// 		sresult += buf;
// 		tstring.erase();
// 	      }
// 	    else
// 	      {
// 		std::cout << "Do2:" << tstring << endl;
// 		return false;
// 	      }
	  }
	if ((ip+1) < field.end () && *ip == '=' && *(ip+1) == '?')
	  {
	    std::string charset, encode_string, tresult;
	    char encode_type;
	    if ( ChineseToUTF16 ("GBK", tstring, tresult))
	      {
		sresult += tresult;
		tstring.erase();
	      }
	    else
	      {
		std::cout << "Do2:" << tstring << endl;
		return false;
	      }
	    ip += 2;
	    while ( (ip+1) < field.end () && *ip != '?' && *(ip+1) != '=')
	      {
		charset += *ip;
		++ip;
	      }
	    if ( *ip == '?' ) ++ip;
	    if ( *ip != '?' ) encode_type = *ip;
	    ++ip;
	    if (*ip == '?') ++ip;
	    while (ip < field.end())
	      {
		if ((ip+4)<field.end() && *ip == '?' && *(ip+1) == '=' &&
		    *(ip+2) == ' ' && *(ip+3) == '=')
		  {
		    ip += 3;
		    break;
		  }
		else if ((ip+1)<field.end() && *ip == '?' && *(ip+1) == '=')
		  {
		    ip += 2;
		    break;
		  }
		encode_string += *ip;
		++ip;
	      }
// 	    while (ip < field.end() &&
// 		   (*ip == ' '||*ip == '\r'||*ip == '\t'||*ip == '\n'))++ip;
	    charset = nstring::upper_string(charset);
	    if ( charset == "GB2312") charset = "GBK";
	    if (all_encode.empty())
	      all_encode += charset;
	    else 
	      all_encode += ("|" + charset);
	    std::string decode_string;
	    if ( encode_type == 'b' || encode_type == 'B')
	      {
		base64_decode (decode_string, encode_string);
	      }
	    else if ( encode_type == 'q' || encode_type == 'Q')
	      {
		qp_decode (decode_string, encode_string);
		for (std::string::iterator iii = decode_string.begin();
		     iii < decode_string.end(); ++iii)
		  if ( *iii == '_' ) *iii = ' ';
	      }
	    tresult.erase();
	    if (ChineseToUTF16 (charset, decode_string, tresult))
	      sresult += tresult;
	    else
	      {
		std::cout << "Do2:" << decode_string << endl;
		return false;
	      }
	  }else
	  {
	    tstring += *ip ;
	    ++ip;
	  }
      }
    result = sresult;
    return true;
  }
  std::string decode_field ( const std::string &field )
  {
    int count = decode_field_count (field);
    std::string result, encodies;
    encodies = get_field_encoding (field);
    if (count == 0 && ChineseToUTF16 ("GBK", field, result))
      return result;
    else if (encodies.find("|") == std::string::npos)
      {
	if (decode_field_1(field,result))
	  return result;
	else if (decode_field_2(field, result))
	  return result;
      }
    return field;
  }
#endif
  std::string Mime::decode_field ( const std::string &field )
  {
    std::string::const_iterator ip = field.begin (), li;
    std::string charset, de_string, sstring, result, all_encode;
    char encode_type;
    // iConv converter;
//     while ( ip <= field.end () )
//       {
// 	if (ip == field.end ())
// 	  {
// 	    std::string buf;
// 	    if (ChineseToUTF16("GBK", sstring, buf))
// 		result += buf;
// 	    break;
// 	  }
// 	if ((ip+1) < field.end () && *ip == '=' && *(ip+1) == '?')
// 	  {
// 	    std::string buf;
// 	    if (ChineseToUTF16("GBK", sstring, buf))
// 	      result += buf;
// 	    buf.erase();
// 	    sstring.erase ();
// 	    li = ip;
// 	    ip += 2;
// 	    charset.erase ();
// 	    while ( (ip+1) < field.end () && *ip != '?' && *(ip+1) != '=')
// 	      {
// 		charset += *ip;
// 		++ip;
// 	      }
// 	    if ( *ip == '?' ) ++ip;
// 	    if ( *ip != '?' ) encode_type = *ip;
// 	    ++ip;
// 	    if (*ip == '?') ++ip;
// 	    de_string.erase ();
// 	    while (ip < field.end())
// 	      {
// 		if ((ip+4)<field.end() && *ip == '?' && *(ip+1) == '=' &&
// 		    *(ip+2) == ' ' && *(ip+3) == '=')
// 		  {
// 		    ip += 3;
// 		    break;
// 		  }
// 		else if ((ip+1)<field.end() && *ip == '?' && *(ip+1) == '=')
// 		  {
// 		    ip += 2;
// 		    break;
// 		  }
// 		de_string += *ip;
// 		++ip;
// 	      }
// 	    charset = nstring::upper_string(charset);
// 	    if ( charset == "GB2312") charset = "GBK";
// 	    if (all_encode.empty())
// 	      all_encode += charset;
// 	    else all_encode += ("|" + charset);
// 	    std::string decode;
// 	    if ( encode_type == 'b' || encode_type == 'B')
// 	      {
// 		base64_decode (decode, de_string);
// 	      }
// 	    else if ( encode_type == 'q' || encode_type == 'Q')
// 	      {
// 		qp_decode (decode, de_string);
// 	      }
// 	    if (ChineseToUTF16(charset, decode, buf))
// 	      result += buf;
// 	  }else
// 	  {
// 	    sstring += *ip ;
// 	    ++ip;
// 	  }
//       }
    return result;
  }

  bool Mime::add_header ( const std::string &key, const std::string &value)
  {
    //cout << "DEBUG:\n" << key << ":>" << value << endl;
    if ( key == "Content-Type" )
      {
	std::string::const_iterator ip = value.begin (), tp;
	const std::string tag_charset ("charset");
	std::string ct, charset;
	std::string::size_type t_sep;
	  
	while (ip < value.end() && (*ip==' '||*ip=='\r'||*ip=='\t'))++ip;
	for ( ; ip < value.end () && *ip != ' ' && *ip != ';'; ++ip )
	  ct += *ip;
	  
	t_sep = ct.find ("/"); 
	if ( t_sep != string::npos )
	  {
	    m_content_type = ct.substr (0, t_sep );
	    m_content_sub_type = ct.substr (t_sep+1);
	  }else
	  {
	    for (std::string::iterator cip = ct.begin (); cip < ct.end();  ++cip )
	      if ( *cip != '\n' || *cip != '\r' || *cip != '\t')
		m_content_type += *cip;
	  }
	while (ip < value.end() && ( *ip==' '||*ip == '\n'||*ip=='\r'||*ip=='\t'||*ip==';' )) ++ip;

// 	if ( tp != value.end() )
// 	  {
// 	    tp += tag_charset.size();
// 	    while ( tp < value.end() && *tp != ';' )
// 	      if ( *tp != ' ' && *tp != '\t' && *tp != '"' && *tp != '\'' )
// 		charset += *tp;
// 	  }
	tp = std::search ( ip, value.end(), tag_charset.begin(), tag_charset.end() );
	if ( tp != value.end() ) ip = tp;
	while ( ip < value.end () )
	  {
	    if ( *ip == '=' )
	      {
		++ip;
		break;
	      }
	    ++ip;
	  }
	for (; ip < value.end () && *ip != '\n' && *ip != ';' && *ip != ' '; ++ip )
	  if ( *ip != '"' ) charset += *ip;
	if ( m_content_type == "multipart")
	  {
	    m_boundary = charset;
	    m_charset.erase();
	  }
	else
	  m_charset = charset;

      }else if ( key == "Content-Transfer-Encoding")
      {
	for (std::string::const_iterator cip = value.begin ();  cip < value.end();  ++cip )
	  if ( *cip != '\n' || *cip != '\r' || *cip != '\t')
	    m_content_transfer_encoding += *cip;
	//cout << "CTE: " << m_content_transfer_encoding << endl;
      }else if ( key == "Subject" )
      {
	m_subject = value;
      }
    m_headers.push_back ( header_type(key,value) );
    return true;
  }

  bool Mime::close ()
  {
    return do_parser ();
  }

  inline bool Mime::is_header ( const std::string &line )
  {
    return ( line.find (": ") != string::npos ||
	     line.find (":" ) != string::npos );
  }
    
  bool Mime::parser_header ( const std::string &line )
  {
    std::string::size_type sep_pos;
    std::string key, value;
    if ( is_header (line) )
      {
	sep_pos = line.find (": ");
	if ( sep_pos != string::npos )
	  {
	    key = line.substr( 0, sep_pos );
	    value = line.substr (sep_pos+2 );
	    add_header ( key, value );
	    return true;
	  }else
	  {
	    sep_pos = line.find (": ");
	    if ( sep_pos != string::npos )
	      {
		key = line.substr( 0, sep_pos );
		value = line.substr (sep_pos+2 );
		add_header ( key, value );
		return true;
	      }
	  }
      }
    return false;
  }
    
  int Mime::part_size ()
  {
    return m_parts.size ();
  }
      
  bool Mime::do_parser ()
  {
    std::string::const_iterator ip = m_raw_message.begin ();
    std::string line;
    
    for (line.erase () ; ip <= m_raw_message.end () ; ++ip )
      {
	if ( ip == m_raw_message.end () )
	  {
	    parser_header (line);
	    break;
	  }else if ( *ip == '\r' )
	  {
	    continue;
	  }else if ( *ip == '\n' && (ip + 1) < m_raw_message.end ())
	  {
	    if (*(ip+1) == '\t' || *(ip+1) == ' ')
	      {
		ip += 1;
		continue;
	      }
	    else if ((ip+2)<m_raw_message.end () && (*(ip+1) == '\r' && *(ip+2) == '\n'))
	      {
		parser_header ( line );
		ip += 3;
		break;
	      }
	    else if ((*(ip+1) =='\n')) 
	      {
		parser_header ( line );
		ip += 2;
		break;
	      }else
	      {
		parser_header ( line );
		line.erase ();
		continue;
	      }
	  }else 
	  {
	    //if ( ip > m_raw_message.begin () && *(ip-1) == '\n' && *ip == '\t' ) continue;
	    line += *ip;
	  }
      }

    if ( !only_parser_header )
      {
	std::string body = "";
	for ( ; ip < m_raw_message.end (); ++ip)
	  body += *ip;
	if ( lower(get_contenttype ()) == "multipart" )
	  {
	    do_parser_mime ( body );
	  }else do_findattch ( body );
	m_body = body;
      }
    return true;
  }
  inline std::string Mime::lower ( const std::string& str )
  {
    std::string newstr;
    for ( std::string::const_iterator ip = str.begin(); ip < str.end();++ip)
      {
	newstr += tolower ( *ip );
      }
    return newstr;
  }
  void Mime::do_parser_mime ( std::string &body )
  {
    if ( m_boundary.empty () ) return;
    std::string::size_type begin_pos = 0, end_pos = 0;
    std::string boundary = "--"+m_boundary, tmp;
      
    while (1)
      {
	begin_pos = body.find (boundary, 0);
	if ( begin_pos == string::npos ) break;
	end_pos = body.find (boundary, begin_pos+boundary.size());
	if ( end_pos == string::npos ) break;
	m_parts.push_back ( MimePart(
				     body.substr
				     (begin_pos + boundary.size()-1,
				      (end_pos-begin_pos-boundary.size()+1)
				      )
				     )
			    );

	body.erase ( begin_pos, (end_pos-begin_pos));
      }
    while (1)
      {
	begin_pos = body.find (boundary+"--");
	if ( begin_pos == string::npos) break;
	body.erase (begin_pos, m_boundary.size()+5);
      }
  }
  int Mime::attch_size()
  {
    return m_attch.size ();
  }
  string Mime::get_attch ( const int pos)
  {
    int lpos = pos;
    if ( lpos == m_attch.size() ) --lpos;
    if ( lpos <= 0 && lpos >= m_attch.size() ) return string();
    return m_attch[lpos];
  }
    
  void Mime::do_findattch ( std::string &body )
  {
    std::string::size_type begin_pos = 0, end_pos = 0;
    if ( body.empty () ) return;
    while (1)
      {
	begin_pos = body.find ("\nbegin");
	if ( begin_pos == string::npos ) break;
	end_pos = body.find ("\nend", begin_pos + 5);
	if ( end_pos == string::npos ) break;
	cout << "Found Attch from: " << begin_pos
	     << " to " << end_pos << endl;
	// 	  cout << body.substr (begin_pos,
	// 			       (end_pos-begin_pos)+
	// 			       (end_pos+5<body.size() ? 5:4))
	// 	       << endl;
	  
	m_attch.push_back ( body.substr(begin_pos,
					(end_pos-begin_pos)+
					(end_pos+5<body.size() ? 5:4)));
	body.erase (begin_pos,
		    (end_pos-begin_pos)+( end_pos+5<body.size() ? 5:4));
      }
  }
  std::string Mime::get_body ( const bool &decode )
  {
    string body;
    if ( !decode ) return m_body;
    if ( get_contentencoding () == "base64" )
      {
	base64_decode (body, m_body);
      }else if ( get_contentencoding () == "quoted-printable")
      {
	qp_decode (body, m_body);
      }else body = m_body;
    return body;
  }
  const std::string& Mime::get_contenttype () const
  {
    return m_content_type;
  }
  const std::string& Mime::get_contentsubtype () const
  {
    return m_content_sub_type;
  }
  const std::string& Mime::get_charset () const
  {
    return m_charset;
  }
  const std::string& Mime::get_contentencoding() const
  {
    return m_content_transfer_encoding;
  }
}

    
