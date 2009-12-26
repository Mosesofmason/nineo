#include "mimepart.hpp"

namespace Mime
{
  MimePart::MimePart ( const std::string& part )
  {
    parser ( part );
  }
  MimePart::MimePart ()
  {}
  MimePart::~MimePart ()
  {}
  void MimePart::clear ()
  {
    m_type.erase ();
    m_tencoding.erase ();
    m_id.erase ();
    m_desc.erase ();
  }
  void MimePart::parser ( const std::string& part )
  {
    clear ();
    do_parser ( part );
  }
  void MimePart::get_content_type ( std::string& type )
  {
    type = m_type;
  }
  void MimePart::get_content_tranfter_encoding ( std::string& encoding )
  {
    encoding = m_tencoding;
  }
  void MimePart::get_content_id ( std::string& id )
  {
    id = m_id;
  }
  void MimePart::get_content_desc ( std::string& desc)
  {
    desc = m_desc;
  }
  void MimePart::get_content ( std::string& content)
  {
    content = m_content;
  }
  const std::string& MimePart::get_content_type ()
  {
    return m_type;
  }
  const std::string& MimePart::get_content_tranfter_encoding ()
  {
    return m_tencoding;
  }
  const std::string& MimePart::get_content_id ()
  {
    return m_id;
  }
  const std::string& MimePart::get_content_desc ()
  {
    return m_desc;
  }
  const std::string& MimePart::get_content ()
  {
    return m_content;
  }
  void MimePart::do_subpart ( std::string& part, const std::string& boundary)
  {
    if ( boundary.empty () ) return;
    std::string::size_type begin_pos = 0, end_pos = 0;
    std::string l_boundary = "--"+boundary;
    unsigned int lb_len = l_boundary.size ();
    while (1)
      {
	begin_pos = part.find (l_boundary, 0);
	if ( begin_pos == std::string::npos ) break;
	end_pos = part.find (l_boundary, begin_pos+l_boundary.size());
	if ( end_pos == std::string::npos ) break;
	m_subparts.push_back (
			      MimeSubPart(part.substr(
						      begin_pos+lb_len+1,
						      end_pos-
						      (begin_pos+lb_len+1)
						      )
					  )
			      );
	part.erase (begin_pos, (end_pos-begin_pos));
      }
    while (1)
      {
	begin_pos = part.find (l_boundary+"--");
	if ( begin_pos == std::string::npos) break;
	part.erase (begin_pos, l_boundary.size()+5);
      }
       
  }
  inline int MimePart::subpart_size ()
  {
    return m_subparts.size ();
  }
  MimeSubPart MimePart::get_subpart ( const int pos )
  {
    if ( m_subparts.size () <= 0 ) return MimeSubPart();
    return m_subparts[pos];
  }
  void MimePart::do_parser ( const std::string& part )
  {
    std::string line;
    std::string::const_iterator i;
    if ( part.empty () ) return;

    for ( i = part.begin (); i <= part.end (); ++i )
      {
	if ( *i == '\n' && i+1 < part.end () &&
	     (*(i+1) == '\t' || *(i+1) == ' ')) continue;
	    
	if ( *i == '\n' || i == part.end () )
	  {
	    if ( line.find ("Content-Type: ") != std::string::npos )
	      {
		m_type = line.substr (14);
	      }
	    else if (line.find("Content-Transfer-Encoding:") != std::string::npos)
	      {
		m_tencoding = line.substr (27);
	      }
	    else if (line.find ("Content-ID:") != std::string::npos)
	      {
		m_id = line.substr (11);
	      }
	    else if (line.find("Content-Description:") != std::string::npos)
	      {
		m_desc = line.substr (20);
	      }
	    line.erase ();
	    if ( i+1 < part.end () && *(i+1) == '\n')
	      {
		++i;
		break;
	      }
	    if ( i+2 < part.end () && *(i+1) == '\r' && *(i+2) == '\n')
	      {
		i += 2;
		break;
	      }
	    if ( i == part.end () ) break;
	  }else if ( *i == '\r' )
	  {
	    continue;
	  }else {
	  line += *i;
	}
      }
      
    while ( i < part.end () &&
	    (*i == '\r' || *i == '\n' || *i == '\t' || *i == ' ') ) ++i;
    std::string type, subtype, boundary, key;
    std::string::iterator x_ip = m_type.begin ();
    while ( *x_ip == ' ' || *x_ip == '\t') ++x_ip;
    for ( ; x_ip < m_type.end () && *x_ip != '/'; ++x_ip)
      {
	type += *x_ip;
      }
    if (*x_ip == '/' ) ++x_ip;
    for (; x_ip < m_type.end () && *x_ip != ';'; ++x_ip )
      {
	subtype += *x_ip;
      }
    while (*x_ip == ' ' || *x_ip == '\t' || *x_ip == ';')++x_ip;
    for ( ; x_ip < m_type.end () && *x_ip != '='; ++x_ip )
      {
	key += *x_ip;
      }
    if ( type == "multipart" &&
	 subtype == "alternative" &&
	 key == "boundary" )
      {
	for ( ++x_ip; x_ip < m_type.end (); ++x_ip)
	  if ( *x_ip != '"' ) boundary += *x_ip;
	std::string my_part;
	while ( i < part.end () ) { my_part += *i; ++i; }
	{ my_part += *i; }
	do_subpart ( my_part, boundary );
      }else
      {
	while ( i < part.end () )
	  {
	    m_content += *i;
	    ++i;
	  }
      }
  }
}

