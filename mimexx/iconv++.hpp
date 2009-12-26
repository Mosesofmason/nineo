#ifndef __ICONV_XX__
#define __ICONV_XX__
#include <string>
#include <iostream>
#include <iconv.h>
#include <errno.h>
#define ToUC(x) (unsigned char)(x)

void FixGB2312 ( const std::string& from, std::string& to );
bool ChineseToUTF16 (const std::string& encode, const std::string& from,
		     std::string& to );
class iConvException
{
private:
  std::string m_msg;
  size_t m_pos;
public:
  iConvException (std::string msg = "Nothing error", size_t pos = 0 )
    : m_msg (msg), m_pos (pos)
  {}
  ~iConvException ()
  {}
  std::string error () { return m_msg; };
  size_t pos () { return m_pos; };
};
class iConv
{
private:
  iconv_t m_cd;
  std::string m_fillcharter;
  void gen_fill (std::string& fill );
public:
  iConv (const std::string& from, const std::string& to, std::string fill=" ")
    : m_fillcharter(fill), m_cd ((iconv_t)(-1))
  {
    m_cd = ((iconv_t)(-1));
    init (from, to);
  }
  iConv ()
    : m_cd((iconv_t)(-1))
  {}
  ~iConv()
  {
    if (m_cd != (iconv_t)(-1))
      iconv_close(m_cd);
  }
  bool init (const std::string& from, const std::string& to);
  bool is_ok () { return (m_cd != (iconv_t)(-1));}
  bool convert (const std::string& from, std::string& to) ;
};
int iConvConverter ( const std::string& from_encode,
		     const std::string& to_encode,
		     const std::string& from,
		     std::string& to );
#endif
