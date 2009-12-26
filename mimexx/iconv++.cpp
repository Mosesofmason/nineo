#include "iconv++.hpp"
bool CheckChineseToUTF16IsOK ( const std::string& encode,
			     const std::string& utf16_string)
{
  iConv converter;
  std::cout << "Check convert." << endl;
  return false;
}
bool ChineseToUTF16 (const std::string& encode, const std::string& from,std::string& to )
{
  iConv converter;
  std::cout << encode << " To UTF16. " << endl;
  if(converter.init (encode, "UTF-16"))
    {
      try
	{
	  converter.convert ( from, to );
	  return true;
	}
      catch ( iConvException &e)
	{
	  std::cout  << e.error () << " " << e.pos() << endl;
	  
	  if ( e.pos () == 0 && encode == "UTF-8" &&
	       converter.init("GBK", "UTF-16"))
	    {
	      std::cout << "let me try next." << endl;
	      try
		{
		  converter.convert ( from, to );
		  return true;
		}
	      catch ( iConvException &e )
		{
		  std::cout << "try second error: " << e.error ()
			    << " " << e.pos () << endl;
		  return false;
		}
	    }
	  return false;
	}
    }
  return true;
}
		     
void FixGB2312 ( const std::string& from, std::string& to )
{
  to.erase ();
  std::string::const_iterator fi = from.begin ();
  unsigned int strip_161_64 = 0;
  for ( ; fi < from.end(); ++fi)
    {
      if (ToUC(*fi) == 161 && fi+1 < from.end() && *(fi+1) == 64)
	++strip_161_64;
    }
  if ( strip_161_64 > 3 )
    {
      for (fi = from.begin (); fi != from.end (); ++fi )
	{
	  if (ToUC(*fi) == 161 && fi+1 < from.end() && *(fi+1) == 64)
	    ++fi;
	  else to += *fi;
	}
    }else
    to = from;
}

bool iConv::init (const std::string& from, const std::string& to)
{
  if ( m_cd != (iconv_t)(-1)) iconv_close (m_cd);
  m_cd = iconv_open (to.c_str(), from.c_str());
  if ( m_cd == (iconv_t)(-1) ) 
    return false;
  else
    return true;
}
void iConv::gen_fill (std::string& fill )
{
  if (!is_ok ()) { fill.erase(); return; }
  size_t from_size = m_fillcharter.size();
  size_t to_size = m_fillcharter.size() * 4;
  char *outbuf = new char[to_size];
  const char* inptr = m_fillcharter.c_str();
  char* outptr = outbuf;
  int ret;
  ret = iconv ( m_cd, (char**)&inptr, &from_size, &outptr, &to_size);
  if (outptr != outbuf)
    {
      fill.erase ();
      for (char* ii = outbuf; ii <= outptr; ++ii)
	fill += *ii;
    }
  delete[] outbuf;
}
int iConvConverter ( const std::string& from_encode,
		      const std::string& to_encode,
		      const std::string& from,
		      std::string& to )
{
  iconv_t cv;
  cv = iconv_open ( from_encode.c_str(), to_encode.c_str());
  if ( cv == (iconv_t)(-1) ) return -1;
  size_t from_size = from.size();
  size_t to_size = from_size*4;
  char *outbuf = new char[to_size];
  const char* inptr = from.c_str();
  char *outptr = outbuf;
  std::cout << "Content: "<< from << endl;
  int ret = iconv(cv, (char**)&inptr, &from_size, &outptr, &to_size);
  if ( ret == (size_t)(-1))
    {
      std::cout << "Ret:" << ret << " " << errno << endl;
      delete[] outbuf;
      iconv_close(cv);
      if (from_size > 0)
	{
	  std::cout << "FSize:" << from_size << " "
		    << (from.size() - from_size) << endl;
	  return (from.size()-from_size)+1;
	}
      else
	return -1;
    }
  if ( outptr != outbuf )
    for ( const char* ii = outbuf; ii < outptr; ++ii )
      to += *ii;
  iconv_close(cv);
  return 0;
}
bool iConv::convert ( const std::string& from, std::string& to )
{
  if ( !is_ok ()) return false;
  size_t from_size = from.size();
  size_t to_size = from_size*4;
  char *outbuf = new char[to_size];
  const char* inptr = from.c_str();
  char* outptr = outbuf;
  int ret;
  ret = iconv ( m_cd, (char**)&inptr, &from_size, &outptr, &to_size);
  if ( from_size > 0 && ( errno = EILSEQ || ret == (size_t)(-1)))
    {
      delete[] outbuf;
      throw iConvException ("Illegal byte sequence", (from.size()-from_size));
    }

  if ( ret == (size_t)(-1))
    {
      delete[] outbuf;
      return false;
    }
  if (outptr != outbuf )
    for (char* ii = outbuf; ii < outptr; ++ii)
      to += *ii;
  delete[] outbuf;
  return true;
}

