#ifndef __UUENCODING__
#define __UUENCODING__
#include <string>
namespace Beyooo
{
  namespace Mime
  {
    // encode and decode for UUENCODING
    void uu_encode (std::string &dest_stream, const std::string &src_stream);
    void uu_decode (std::string &dest_stream, const std::string &src_stream);
  }
}

#endif
