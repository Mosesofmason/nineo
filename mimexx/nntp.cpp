#include "nntp.hpp"

static const char* CRLF="\r\n";

unntp::unntp()
{
  m_sock = NULL;
  //std::cout << "UNNTP." << endl;
  try{
    m_sock = new CPPSocket::Socket ();
    m_sock->open (CPPSocket::Socket::TCP);
  }
  catch ( std::exception &e )
    {
      std::cout << "UNNTP: Std::Exception: " << e.what () << endl;
      delete m_sock;
      m_sock = NULL;
    }
  catch (...)
    {
      m_sock = NULL;
      std::cout << "UNNTP: Other exception." << endl;
    }
  //std::cout << "UNNTP." << endl;
}

unntp::~unntp ()
{
  //std::cout << "~Unntp" << endl;
  if ( m_sock != NULL )
    {
      quit ();
      m_sock->close ();
      try{
	delete m_sock;
      }
      catch ( std::exception &e)
	{
	  std::cout << "~UNNTP: Std " << e.what () << endl;
	}
      catch ( CPPSocket::Exception &e )
	{
	  eLog(e);
	}
    }
  //std::cout << "~Unntp" << endl;
}
bool unntp::connect ( const std::string& host, const int &port )
{
  CPPSocket::Address address ;
  try
    {
      address = CPPSocket::Address ( host, port , true );
    }
  catch ( CPPSocket::Exception &e )
    {
#ifdef NDEBUG
      eLog (e);
#endif
      return false;
    }
  if ( m_sock == NULL ) return false;
  try
    {
      m_sock->connect (address);
    }
  catch (CPPSocket::Exception &e)
    {
#ifdef NDEBUG
      eLog (e);
#endif
      return false;
    }
  std::string resp;
  read_line (resp);
  std::cout << resp;
  return true;
}

// bool unntp::isconnect ()
// {
//   return m_sock.isConnected ();
// }

bool unntp::send_cmd ( const std::string &cmd )
{
  std::string _cmd;
  if ( cmd.find ("\r\n") == std::string::npos )
    {
      for ( std::string::const_iterator i = cmd.begin (); i < cmd.end(); ++i)
	if ( *i != '\r' && *i != '\n') _cmd += *i;
      _cmd += "\r\n";
    }else _cmd = cmd;
    
  if ( m_sock == NULL ) return false;
   try
    {
      m_sock->send (_cmd.c_str(),_cmd.size());
    }
  catch ( CPPSocket::Exception &e )
    {
#ifdef NDEBUG
      eLog ( e );
#endif
      return false;
    }
  return true;
}

bool unntp::read_line ( std::string &r )
{
  std::string line, tmp;
  char* buff = new char[1024+1];
  int size_data, want_size;
  std::string::size_type pos;
  if ( m_sock == NULL ) return false;
  try
    {
      while (1)
	{
	  size_data = m_sock->recv (buff, 1024, CPPSocket::Socket::PEEK);
	  if ( size_data > 1024 )
	    buff[1024] = '\0';
	  else buff[size_data] = '\0';
	  tmp = buff;
	  //std::cout << "DEBUG: " << tmp.size () << endl;
	  pos = tmp.find ("\r\n"); 
	  if ( pos != std::string::npos )
	    {
	      want_size = pos+2;
	      //std::cout << "DEBUG: 1 " << want_size << endl;
	      size_data = m_sock->recv (buff, want_size);
	      //std::cout << "DEBUG: 2 " << size_data << endl;
	      buff[size_data] = '\0';
	      line += buff;
	      break;
	    } else {
	    want_size = tmp.size();
	    //std::cout << "KDEBUG: " << want_size << endl;
	    size_data = m_sock->recv (buff, want_size);
	    buff[size_data] ='\0';
	    line += buff;
	  }
	}
    }
  catch ( CPPSocket::Exception &w )
    {
      delete [] buff;
      return false;
    }
  for ( std::string::iterator i = line.begin (); i < line.end(); ++i)
    {
      if ( *i != '\r' ) r += *i;
    }
  delete[] buff;
  return true;
}
bool unntp::read_lines ( std::string &r )
{
  std::string line;
  while (1)
    {
      line.erase ();
      if ( read_line (line) )
	if ( line == ".\n" ) break;
      r += line;
    }
  return true;
}

bool unntp::help ( std::string &r )
{
  if ( !send_cmd ("HELP\r\n" ) ) return false;
  std::string line;
  bool rc;
  if (read_line (line))
    {
      int code = str2code (line);
      if ( (code/100) == 1 )
	return  read_lines(r);
    }
  return false;
}
bool unntp::body ( const News::id_type& id, std::string &b)
{
  b.erase ();
  std::string cmd;
  std::ostringstream oss;
  oss << "BODY " << id << "\r\n";
  cmd = oss.str ();
  if ( send_cmd ( cmd ) )
    {
      std::string line;
      if ( read_line ( line ) )
	{
	  int code = str2code (line);
	  if ( code/100 == 2 )
	    {
	      read_lines ( b );
	      return true;
	    }
	}
    }
  return false;
}

bool unntp::article ( const News::id_type& id, std::string &a )
{
  a.erase ();
  std::string cmd;
  std::ostringstream oss;
  oss << "ARTICLE " << id << "\r\n";
  cmd = oss.str ();
  if ( send_cmd ( cmd ) )
    {
      std::string line;
      if ( read_line ( line ) )
	{
	  int code = str2code ( line );
	  if ( code/100 == 2 )
	    {
	      read_lines (a);
	      return true;
	    }
	}
    }
  return false;
}

bool unntp::head ( const News::id_type& id, std::string &r)
{
  r.erase ();
  std::string cmd;
  std::ostringstream oss;
  oss << "HEAD " << id << "\r\n";
  cmd = oss.str ();
  if ( send_cmd ( cmd ) )
    {
      std::string line;
      if (read_line ( line ))
	{
	  int code = str2code (line);
	  if ( code/100 == 2 )
	    {
	      read_lines ( r );
	      return true;
	    }
	}
    }
  return false;
}

bool unntp::xover_extract ( const std::string& xover,
			    News::id_type& num, std::string& subject,
			    std::string& author, std::string& date,
			    std::string& messageid, std::string& references,
			    std::string& xref, 
			    int& bytes, int& lines )
{
  if ( xover.empty () ) return false;
  std::string::const_iterator iter = xover.begin ();
  std::string line;
  for (line.erase() ; iter < xover.end() && *iter != '\t' ; ++iter)
    line += *iter;
  //std::cout << "num:" << line << endl;
  {
    std::istringstream is(line);
    is >> num;
  }
  
  if ( iter < xover.end() && *iter == '\t') ++iter;
  for (line.erase() ; iter < xover.end() && *iter != '\t'; ++iter )
    line += *iter;
  //std::cout << "subject:" << line <<endl;
  subject = line;
  
  if (iter < xover.end() && *iter == '\t' ) ++iter;
  for ( line.erase (); iter < xover.end() && *iter != '\t'; ++iter )
    line += *iter;
  //std::cout << "author:" << line << endl;
  author = line;
  
  if (iter < xover.end() && *iter == '\t' ) ++iter;
  for ( line.erase (); iter < xover.end() && *iter != '\t'; ++iter )
    line +=  *iter;
  //std::cout << "date:" << line << endl;
  date = line;

  if (iter < xover.end() && *iter == '\t' ) ++iter;
  for ( line.erase(); iter < xover.end() && *iter != '\t'; ++iter )
    line += *iter;
  //std::cout << "messageid:" << line << endl;
  messageid = line;
  
  if ( iter < xover.end() && *iter == '\t' ) ++iter;
  for ( line.erase (); iter < xover.end() && *iter != '\t'; ++iter )
    line += *iter;
  //std::cout << "references:" << line << endl;
  references = line;
  
  if (iter < xover.end () && *iter == '\t' ) ++iter;
  for ( line.erase (); iter < xover.end() && *iter != '\t'; ++iter )
    line += *iter;
  //std::cout << "bytes:" << line << endl;
  {
    std::istringstream is (line);
    is >> bytes;
  }
  
  if (iter < xover.end() && *iter == '\t' ) ++iter;
  for ( line.erase (); iter < xover.end () && *iter != '\t'; ++iter )
    line += *iter;
  //std::cout << "lines:" << line << endl;
  {
    std::istringstream is (line);
    is >> lines;
  }
  if ( iter < xover.end() && *iter == '\t' )++iter;
  for (line.erase (); iter < xover.end() && *iter != '\t'; ++iter)
    line += *iter;
  xref = line;
  return true;
}

bool unntp::xover_extract ( const std::string& xover,
			    std::string& r, const int& pos)
{
  int x = 0;
  std::string::const_iterator i;
  for ( i = xover.begin(); i < xover.end();++i)
    if ( x == pos )
      break;
    else if ( *i == '\t' ) ++x;
  if ( i == xover.end() ) return false;
  for ( r.erase(); i < xover.end() && *i != '\t' && *i != '\n' && *i != '\r';
	++i)r += *i;
  return true;
}
bool unntp::list_overview_fmt ( std::string &r)
{
  if ( send_cmd("LIST OVERVIEW.FMT\r\n"))
    {
      std::string line;
      if ( read_line (line))
	{
	  int code = str2code (line);
	  if ( code/100 == 2 )
	    return read_lines(r);
	}
    }
  return true;
}
int unntp::overview_fmt ( const std::string &r, const std::string &h )
{
  int c = 0;
  std::string t;
  for ( std::string::const_iterator i = r.begin(); i<r.end(); ++i)
    {
      if ( *i == ':') continue;
      if ( *i == '\n')
	{
	  ++c;

	  if ( t == h )
	    {
	      //std::cout << "D:" << c << " " << t << "=" << h << endl;
	      return c;
	    }
	  t.erase ();
	}else t += *i;
    }
  return 0;
}
int unntp::str2code ( std::string &line )
{
  std::string code;
  int icode = 0;
  if ( line.find (" ") != std::string::npos )
    {
      code = line.substr (0, line.find (" "));
      if ( code.size () != 3 ) return 0;
      std::istringstream ss (code);
      ss >> icode;
      //std::cout << "C:" << icode << endl;
      return icode;
    }
  else
    return 0;
}
bool unntp::list_desc ( std::string &r )
{
  r.erase ();
  bool rc = send_cmd ("LIST NEWSGROUPS\r\n");
  if ( !rc ) return false;
  std::string line;
  if (read_line (line))
    {
      int code = str2code (line);
      if ( code == 215)
	return read_lines(r);
    }
  return false;
}
bool unntp::list ( std::string &r )
{
  r.erase ();
  bool rc = send_cmd ("LIST\r\n");
  if ( !rc ) return false;
  std::string line;
  if (read_line (line))
    {
      int code = str2code (line);
      if ( code == 215)
	return read_lines(r);
    }
  return false;
}
bool unntp::xover ( const News::id_type& start, const News::id_type& end,
		    std::string& r)
{
  r.erase ();
  if ( end < start ) return false;
  std::ostringstream os;
  os << "XOVER "
     << ( start != 0 ? start : 1 )
     << "-"
     << ( end != 0 && end >= start ? end : start )
     << "\r\n";
  if (send_cmd (os.str()))
    {
      std::string line;
      if (read_line (line))
	{
	  int code = str2code(line);
	  if ( code/100 == 2)
	    {
	      read_lines (r);
	      return true;
	    }
	}
    }
  return false;
}
bool unntp::group ( const std::string &g, News::id_type &num,
		    News::id_type &first, News::id_type &last,
		    std::string &gn )
{
  std::string cmd = "GROUP " + g + "\r\n";
  send_cmd ( cmd );
  std::string line, sn, sf, sl, groupname;
  std::string::iterator li;
  if (read_line (line))
    {
      int code = str2code ( line );
      if ( code == 211 )
	{
	  li = line.begin ();
	  for (; li < line.end (); li++ )
	    if ( *li == ' ' ) break;
	  
	  if ( *li == ' ') ++li;
	  for (; li < line.end () && *li != ' '; ++li)
	    sn += *li;
	  
	  if ( *li == ' ') ++li;
	  for (; li < line.end () && *li != ' '; ++li)
	    sf += *li;

	  if ( *li == ' ') ++li;
	  for (; li < line.end () && *li != ' '; ++li)
	    sl += *li;
	  
	  if ( *li == ' ') ++li;
	  for ( ; li < line.end () && *li != ' '; ++li)
	    groupname += *li;
	  gn = groupname;
	  //if ( sn.size () == 0 || sf.size () == 0 || sl.size () == 0 )
	  //return false;
	  std::istringstream ssn(sn), ssf(sf), ssl(sl);
	  ssn >> num;
	  ssf >> first;
	  ssl >> last;
	  return true;
	}
    }
  return false;
}

bool unntp::quit ()
{
  if ( m_sock == NULL ) return false;
  return send_cmd ("QUIT\r\n");
}
