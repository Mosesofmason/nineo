#include "exstring.hpp"
#include <string>
#include <iostream>
#include <fstream>

int main  ()
{
  std::ifstream ifile ("localhost.lst", std::ios::binary|std::ios::in);
  char ch;
  std::string line;
  if ( ifile )
    {
      while ( ifile.get (ch) )
	{
	  if ( ch == '\n' )
	    {
	      line = line.substr (0, line.find ("\0"));
	      ExString::match_type r=ExString::match(line, "*software");
	      if ( r )
		std::cout << line << " "  << r << endl;
	      line.erase ();
	    }
	  else line+=ch;
	}
    }
  return 1;
}
