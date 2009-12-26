// #include "uuencoding.hpp"
// #include "base64.hpp"
// #include "xxencoding.hpp"
// #include "qpencoding.hpp"
// #include "yenc.hpp"
#include "mime.hpp"
#include "iconv++.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iosfwd>
#include <iomanip>
#include <cunistd>

int main ( const int argc, const char** argv)
{
  std::string in_file;
  std::string in_buffer, out_buffer, de_buffer;
  Beyooo::Mime::Mime mime_message;
  char ch;
  //nmime::crc32_type crc;
  
  if ( argc < 2 ) exit (0);
  if ( argc >= 2 )
    in_file = *(argv+1);
  
  cout << "IN FILENAME:" << in_file << endl;
  ifstream ifile ( in_file.c_str(), ios::in|ios::binary);
  if ( ifile )
    {
      in_buffer.erase ();
      out_buffer.erase ();
      
      while ( ifile.get(ch) )
	{
	  in_buffer += ch;
	}

      ifile.close ();
      iConv conv("GBK", "UTF-8");
      iConv conv2("UTF-8", "GBK");
      //std::cout << conv.is_ok () << endl;
      std::string fix;
      FixGB2312 (in_buffer, fix);
      in_buffer = fix;
      while (1)
	{
	  try
	    {
	      conv.convert(in_buffer,out_buffer);
	      break;
	    }
	  catch ( iConvException& e )
	    {
	      std::string errs;
	      std::string::iterator is = in_buffer.begin()+e.pos(), es;
	      std::cout << e.error () << " " << e.pos()
			<<endl;
	      std::cout << "[[" << endl;
	      for ( es = is; es < in_buffer.end(); ++es )
		{
		  std::cout << std::setw(6)
			    << std::setfill (' ')
		    //<< std::ios::hex
			    << (unsigned int) ToUC(*es);
		  if ( ((unsigned char)*es) < 0x80) break;
		}
	      std::cout << endl << "]]" << endl;
	      if (!in_buffer.empty())*is = ' ';
	    }
	}
      in_buffer.erase ();
      try
	{
	  conv2.convert(out_buffer, in_buffer);
	  std::cout << in_buffer;
	}
      catch (...)
	{
	  std::cout << "convert error." << endl;
	}
//       mime_message.parser (in_buffer);
//       mime_message.close ();
//       //cout << mime_message.part_size () << endl;
//       cout <<"Charset:" << mime_message.get_charset ()
// 	   <<" Content:" << mime_message.get_contentencoding ()
// 	   <<" Type:" << mime_message.get_contenttype ()
// 	   <<" SubType:" << mime_message.get_contentsubtype ()
// 	   <<" Parts:" << mime_message.part_size()
// 	   <<" Attchs:" << mime_message.attch_size ()
// 	   << "\n";
//       if ( mime_message.attch_size() > 0 )
// 	cout << "================================="<<endl
// 	     << mime_message.get_attch(1) << endl
// 	     << "================================="<<endl;
	
      
// //       cout << "B----------------------------------B" << endl
// // 	   << mime_message.get_body ()
// // 	   << "B----------------------------------B" << endl;

//       cout << "====================================" << endl;
// //       nmime::base64_encode (out_buffer, in_buffer);
// //       //cout << out_buffer << endl;
// //       //cout << hex << crc << endl;
// //       nmime::base64_decode (de_buffer, out_buffer);
// //       cout << out_buffer << endl;
// //       cout << ( de_buffer == in_buffer ) << endl;

    }
  return 1;
}
