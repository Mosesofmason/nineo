#ifndef __QPENCODING__
#define __QPENCODING__

#include <string>
#include <ctype.h>
namespace Mime
{
  void qp_encode ( std::string &dest_stream, const std::string &src_stream);
  void qp_decode ( std::string &dest_stream, const std::string &src_stream);
}


#endif
