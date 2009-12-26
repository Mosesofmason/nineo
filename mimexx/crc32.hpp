#ifndef __CRC32__
#define __CRC32__
#include <string>

namespace Beyooo
{
  namespace Mime
  {
    typedef unsigned long crc32_type;
    const char Z_NULL = 0;
    crc32_type crc32 ( crc32_type crc, const std::string *buff);
  }
}

#endif
