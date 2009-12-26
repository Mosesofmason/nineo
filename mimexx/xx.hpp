#ifndef __XXENCODING__
#define __XXENCODING__
#include <string>

namespace Beyooo
{
  namespace Mime
  {
    void xx_encode (std::string &dest_stream, std::string &src_stream);
    void xx_decode (std::string &dest_stream, std::string &src_stream);
  }
}
#endif // end of __XXENCODING__
