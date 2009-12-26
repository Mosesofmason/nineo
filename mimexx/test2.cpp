#include "mime.hpp"
#include "nntp.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Sqlite++/sqlitexx.hpp"
#include "date.hpp"
#include "btree.hpp"
#include <unistd.h>
#include <iosfwd>
#include <iomanip>

void print_hex (const std::string& str )
{
  std::string::const_iterator ii;
  int ccc = 0;
  for (ii = str.begin(); ii < str.end (); ++ii)
    {
      ++ccc;
      std::cout.setf (std::ios::showbase);
      std::cout << std::setw(5)
		<< std::hex << (int)(unsigned char)*ii ;
      if ( (ccc % 8) == 0 ) std::cout << endl;
    }
  std::cout.unsetf (std::ios::showbase|std::ios::hex);
  std::cout << endl;
}
void utf16_gbk (const std::string& utf16)
{
  iConv converter;
  converter.init ("UTF-16", "GBK");
  try
    {
      std::string buf;
      converter.convert (utf16, buf);
      std::cout << buf << endl;
    }
  catch (iConvException &e)
    {
      print_hex(utf16);
      std::cout << "UTF16 TO GBK " 
		<< e.error () << " " << e.pos() << endl;
    }
  catch (...)
    {
      std::cout << "unkown error"<< endl;
    }
}
void utf16_gbk2 (const std::string& utf16)
{
  std::string out;
  int ret = iConvConverter ("UTF-16", "GBK", utf16, out);
  std::cout << "U2G2:"<< ret << endl;
}
int main ( const int argc, const char** argv)
{
  
  Mime::Mime mime_message;
  DateTime date_parser;
  unntp nntp;
  bool rc;
  if (nntp.connect ("localhost", 119 ))
    {
      id_type first, last, num;
      std::string gn, resp;
      Mime::OverviewFmt overview;
      if (nntp.list_overview_fmt (resp))
	overview.init (resp);
      else return 1;
      rc = nntp.group ( "cn.fan", num, first, last, gn);
      resp.erase ();
      if ( rc && nntp.xover (first, last, resp))
	{
	  std::string line, subject, from;
	  for ( std::string::iterator i = resp.begin(); i < resp.end(); ++i)
	    {
	      if ( *i == '\n' )
		{
		  overview.parse ( line );
		  from = overview.from (); 
		  subject = overview.subject ();
		  std::cout << overview.id () << " From: "<< from << endl;
		  from = Mime::decode_field (from);
		  utf16_gbk(from);
		  std::cout << "-----------------------------------"<<endl;
		  std::cout << "Subject: " << subject << endl;
		  subject = Mime::decode_field (subject);
		  utf16_gbk(subject);
		  std::cout << endl;
		  line.erase ();
		}
	      else line+=*i;
	    }
	}
//       if ( rc )
//  	{
// 	  //first += 300;
// 	  //last = first + 100;
// 	  for ( id_type ss = first; ss <= last; ++ss )
// 	    {
// 	      resp.erase ();
// 	      if (nntp.head (ss, resp ))
// 		{
// 		  std::string from, subject;
// 		  std::cout << resp << endl;
//  		  mime_message.parser (resp);
// 		  mime_message.close ();
//   		  from = mime_message.get_header ("From", false);
//  		  subject = mime_message.get_header("Subject", false);
//   		  std::cout << ss << " From: " << from << endl
//   			    << "Subjcet: " << subject
//   			    << endl;
// 		  from = Mime::decode_field (from);
//  		  subject = Mime::decode_field(subject);
//  		  utf16_gbk(from);
//   		  utf16_gbk(subject);
// 		  utf16_gbk2(from);
// 		  utf16_gbk2(subject);
//  		  std::cout << endl << endl;
// 		  mime_message.clear ();
// 		  usleep (300000);
// 		}
// 	    }
//     }
    }
  return 1;
}

