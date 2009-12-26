#ifndef __BASE64__
#define __BASE64__
#include <string>

namespace Mime
{
  void base64_encode ( std::string &dest, const std::string &src );
  void base64_decode ( std::string &dest, const std::string &src );
}

#endif
