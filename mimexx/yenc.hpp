#ifndef __YENC__
#define __YENC__
#include <string>
#include "crc32.hpp"

namespace Beyooo
{
  namespace Mime
  {
    void yenc_encode ( std::string &dest_stream,
		       const std::string &src_stream, crc32_type &crc);
    void yenc_decode ( std::string &dest_stream,
		       const std::string &src_stream);
  }
}
#endif 
