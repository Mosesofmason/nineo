#include "yenc.hpp"

namespace Beyooo
{
  namespace Mime
  {
  inline long yenc_get_line (std::string &line,
			     const std::string &src_stream, long pos = 0)
  {
    std::string::const_iterator ipos = src_stream.begin ();
    long count = 0;
    if ((ipos+pos) >= src_stream.end ()) return count;
    line.erase ();
    for ( ipos += pos; ipos < src_stream.end () && count < 128;
	  ++ipos, ++count)  line += *ipos;
    return count;
  }
    void yenc_encode ( std::string &dest_stream,
		       const std::string &src_stream, crc32_type &crc)
    {
      std::string line;
      long count = 0, pos = 0, llen = 0;

      crc = crc32 (0, 0);

      while (1)
	{
	  line.erase ();
	  count = yenc_get_line (line, src_stream, pos);
	
	  if (count == 0) break;
	
	  crc = crc32(crc, &line);
	  for (std::string::const_iterator ipos = line.begin();
	       ipos < line.end(); ipos++)
	    {
	      if ( llen > 127 )
		{
		  dest_stream += '\n';
		  llen = 0;
		}
	      switch ((char) ((int) *ipos+42))
		{
		case '\0':
		case '\t':
		case '\n':
		case '\r':
		case '=':
		case '\033':
		  dest_stream += '=';
		  dest_stream += (char) ((int)*ipos+42+64);
		  llen += 2;
		  break;
		case '.':
		  if ( llen == 0 ) {
		    dest_stream += '=';
		    dest_stream += (char) ((int)*ipos+42+64);
		    llen += 2;
		  }else {
		    dest_stream += (char) ((int)*ipos+42);
		    llen++;
		  }
		  break;
		default:
		  dest_stream += (char) ((int)*ipos+42);
		  llen++;
		  break;
		}
	    }
	  pos += count;
	}
    }
    void yenc_decode ( std::string &dest_stream, const std::string &src_stream)
    {
      for (std::string::const_iterator ipos = src_stream.begin ();
	   ipos < src_stream.end (); )
	{
	  if (*ipos == '=' )
	    {
	      if (++ipos < src_stream.end () )
		{
		  dest_stream += ( char ) ((int)*ipos - 64 - 48);
		  ipos++;
		}
	    }else if ( *ipos == '\n' || *ipos == '\r' )
	    {
	      ipos ++;
	    }else
	    {
	      dest_stream += (char)((int) *ipos++ - 42);
	    }
	}
    }
  }
}
