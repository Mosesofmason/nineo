#include "mimesubpart.hpp"
namespace Mime
{
  MimeSubPart::MimeSubPart ( const std::string& part )
  {
    clear ();
    do_parser ( part );
  }
  MimeSubPart::MimeSubPart ()
  {}
  MimeSubPart::~MimeSubPart ()
  {}
    
  void MimeSubPart::clear ()
  {
    m_content.erase ();
    m_type.erase ();
    m_id.erase ();
    m_desc.erase ();
    m_tencoding.erase ();
    m_dispos.erase ();
    m_content_location.erase ();
    m_content_base.erase ();
  }
    
  void MimeSubPart::parser ( const std::string& part )
  {
    clear ();
    do_parser ( part );
  }
    
  void MimeSubPart::do_parser ( const std::string& part)
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
	    else if( line.find ("Content-Disposition: ") != std::string::npos)
	      {
		m_dispos = line.substr (21);
	      }
	    else if (line.find ("Content-Disposition:") != std::string::npos)
	      {
		m_dispos = line.substr(20);
	      }
	    else if (line.find ("Content-Location: ") != std::string::npos)
	      {
		m_content_location = line.substr(18);
	      }
	    else if (line.find ("Content-Location:") != std::string::npos)
	      {
		m_content_location = line.substr(17);
	      }
	    else if (line.find ("Content-Base: ") != std::string::npos)
	      {
		m_content_base = line.substr (14);
	      }
	    else if (line.find ("Content-Base:") != std::string::npos)
	      {
		m_content_base = line.substr(13);
	      }
	    line.erase ();
	    if ( i+1 < part.end () && *(i+1) == '\n')
	      {
		i +=2;
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
    while ( i < part.end () ) {
      m_content += *i;
      ++i;
    }
  }
}

