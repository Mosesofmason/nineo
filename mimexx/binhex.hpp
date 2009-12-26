#ifndef __BINHEX__
#define __BINHEX__
#include <iostream>
#include <string>

namespace nmime
{
  void binhex_encode (std::string &dest, const std::string &src);
  void binhex_decode (std::string &dest, const std::string &src);
  
}

#endif // end of __BINHEX__
