#include "overview.hpp"
namespace News
{
  void OverviewFMT::clear ()
  {
    m_subject.erase ();
    m_from.erase ();
    m_date.erase ();
    m_messageid.erase ();
    m_references.erase ();
    m_bytes.erase ();
    m_lines.erase ();
    m_xref.erase ();
  }
  unsigned short OverviewFMT::ref_count ()
  {
    unsigned short count = 0;
    std::string::iterator i = m_references.begin ();
    for ( ; i < m_references.end(); ++i )
      if ( *i == '@' ) ++count;
    return count;
  }
  void OverviewFMT::parse ( const std::string& overview )
  {
    std::string::const_iterator i;
    std::string line;
    int pos = 0;
    clear ();
    for ( i = overview.begin(); i <= overview.end(); ++i)
      {
	if ( *i == '\t' || i == overview.end())
	  {
	    //std::cout << pos << " " << line << endl;
	    if (pos == 0)
	      m_id = line;
	    else if (pos == mp_subject)
	      m_subject = line;
	    else if (pos == mp_from)
	      m_from = line;
	    else if (pos == mp_date)
	       m_date = line;
	    else if (pos == mp_messageid)
	      m_messageid = line;
	    else if ( pos == mp_references)
	      m_references = line;
	    else if ( pos == mp_bytes )
	      m_bytes = line;
	    else if ( pos == mp_lines )
	      m_lines = line;
	    else if ( pos == mp_xref )
	      m_xref = line;
	    line.erase ();
	    if ( i == overview.end () )break;
	    ++pos;
	  }
	else line += *i;
      }
  }
  const std::string& OverviewFMT::subject ()
  {
    return m_subject;
  }
  const std::string& OverviewFMT::from ()
  {
    return m_from;
  }
  const std::string& OverviewFMT::date ()
  {
    return m_date;
  }
  const std::string& OverviewFMT::messageid ()
  {
    return m_messageid;
  }
  const std::string& OverviewFMT::references ()
  {
    return m_references;
  }
  const std::string& OverviewFMT::xref ()
  {
    return m_xref;
  }
  const std::string& OverviewFMT::id()
  {
    return m_id;
  }
  int OverviewFMT::bytes ()
  {
    std::istringstream ss(m_bytes);
    int bytes = 0;
    ss >> bytes;
    return bytes;
  }
  int OverviewFMT::lines ()
  {
    std::istringstream ss (m_lines);
    int lines = 0;
    ss >> lines;
    return lines;
  }
  void OverviewFMT::init ( const std::string& overviewfmt )
  {
    std::string::const_iterator i;
    std::string line;
    int pos = 0;
    for (i = overviewfmt.begin(); i < overviewfmt.end(); ++i)
      {
	if ( *i == '\n' )
	  {
	    ++pos;
	    if ( line.find ("Subject") != std::string::npos)
	      {
		//std::cout << "Subject: "<< pos << endl;
		mp_subject = pos;
	      }
	    else if ( line.find ("From") != std::string::npos)
	      {
		//std::cout << "From: "<< pos << endl;
		mp_from = pos;
	      }
	    else if ( line.find ("Date") != std::string::npos)
	      {
		//std::cout << "Date: "<< pos << endl;
		mp_date = pos;
	      }
	    else if ( line.find ("Message-ID") != std::string::npos)
	      {
		//std::cout << "MessageID: "<< pos << endl;
		mp_messageid = pos;
	      }
	    else if ( line.find ("References") != std::string::npos)
	      {
		//std::cout << "References: "<< pos << endl;
		mp_references = pos;
	      }
	    else if ( line.find ("Bytes") != std::string::npos )
	      {
		//std::cout << "Bytes: "<< pos << endl;
		mp_bytes = pos;
	      }
	    else if ( line.find ("Lines") != std::string::npos )
	      {
		//std::cout << "Lines"<< pos << endl;
		mp_lines = pos;
	      }
	    else if ( line.find ("Xref") != std::string::npos )
	      {
		//std::cout << "Xref: "<< pos << endl;
		mp_xref = pos;
	      }
	    line.erase();
	  }
	else line += *i;
      }
  }
  
}// namespace
