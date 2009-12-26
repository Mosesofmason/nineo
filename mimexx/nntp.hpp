#ifndef __NNTP____
#define __NNTP____
#include <string>
#include <iostream>
#include <sstream>
#include "newstype.hpp"
#include <cppsocket/tcpclient.h>
#include <cppsocket/address.h>
#include <cppsocket/stringbuffer.h>
#include <cppsocket/exception.h>
#include <cppsocket/socket.h>

#define NDEBUG 1


inline void eLog (CPPSocket::Exception &ee )
{
  std::cout << "E:" << ee.getMethod() 
	    <<": " << ee.getMessage ()
	    <<" |" << ee.getCode () << "\n";
}
typedef long long int int64;

class unntp
{
public:
  unntp ();
  ~unntp ();
  bool connect ( const std::string& host, const int &port);
  bool help ( std::string &r );
  bool quit ();
  bool list ( std::string &r );
  bool list_desc ( std::string &r );
  bool group ( const std::string &g, News::id_type &num, News::id_type &first,
	       News::id_type &last, std::string &gn );
  bool head ( const News::id_type& id, std::string &r );
  bool body ( const News::id_type& id, std::string &b );
  bool article ( const News::id_type& id, std::string &a );
  bool xover ( const News::id_type& start, const News::id_type& end,
	       std::string &r);
  bool xover_extract ( const std::string& xover,
		       News::id_type& num, std::string& subject,
		       std::string& author, std::string& date,
		       std::string& messageid, std::string& references,
		       std::string& xref, 
		       int& bytes, int& lines);
  bool xover_extract ( const std::string& xover,std::string& r,const int &pos);
  bool list_overview_fmt ( std::string &r );
  int overview_fmt ( const std::string &r, const std::string &h);
  
private:
  CPPSocket::Socket *m_sock;
  bool send_cmd ( const std::string &cmd );
  bool read_lines ( std::string &r );
  bool read_line ( std::string &line );
  int str2code ( std::string &line );
};

  
#endif
