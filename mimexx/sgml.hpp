#ifndef __SGML__
#define __SGML__

#include <iostream>
#include <string>
#include <algorithm>

class SGML
{
private:
  std::string m_rawdata;
  
public:
  SGML () {};
  ~SGML() {};
  void reset ();
  void feed (const std::string& data );
  virtual int handle_data (std::string::const_iterator &begin,
			   std::string::const_iterator &end);
  
  virtual void handle_starttag ();
  virtual void handle_endtag ();
  virtual void handle_charref ();
  virtual void handle_entityref ();
  virtual void handle_comment ();
  virtual void handle_decl ();
  virtual void handle_pi ();
};

#endif
