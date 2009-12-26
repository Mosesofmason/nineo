#include "uu.hpp"
namespace Beyooo
{
  namespace Mime
  {
    static const unsigned char UUEncodeTable[64] = {
      '`', '!', '"', '#', '$', '%', '&', '\'',
      '(', ')', '*', '+', ',', '-', '.', '/',
      '0', '1', '2', '3', '4', '5', '6', '7',
      '8', '9', ':', ';', '<', '=', '>', '?',
      '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
      'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
      'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
      'X', 'Y', 'Z', '[', '\\',']', '^', '_'
    };
    inline unsigned int uu_get_line ( std::string &line,
				      const std::string &buffer, long offset = 0)
    {
      std::string::const_iterator start_pos = buffer.begin () + offset;
      std::string::const_iterator i_pos;
      long count = 0;
      line.erase ();
      if ( start_pos >= buffer.end() ) return 0;
      for (i_pos = start_pos; (i_pos - start_pos) < 45 &&
	     i_pos < buffer.end();
	   ++count, ++i_pos )
	line += *i_pos;
      return count;
    }
    
    void uu_encode (std::string &dest_stream, const std::string &src_stream)
    {
      std::string::const_iterator ip_begin, ip_end, ip_pos;
      long pos = 0, count = 0;
      std::string line;
      while (1)
	{
	  count = uu_get_line (line, src_stream, pos);
	  if ( count == 0) break;
	  ip_pos = ip_begin = line.begin ();
	  ip_end = line.end ();
	  dest_stream += UUEncodeTable[count];
	  for ( ; ip_pos <= ip_end-3; ip_pos+=3)
	    {
	      dest_stream += UUEncodeTable [((unsigned char)*ip_pos>> 2)];
	      dest_stream += UUEncodeTable [(((unsigned char)*ip_pos&0x03)<<4)|
					     ((unsigned char)*(ip_pos+1)>>4)];
	      dest_stream += UUEncodeTable [(((unsigned char)*(ip_pos+1)&0x0f)
					      <<2) |
					     ((unsigned char)*(ip_pos+2)>>6)];
	      dest_stream += UUEncodeTable [(unsigned char)*(ip_pos+2)& 0x3f];
	    }

	  if (ip_end - ip_pos == 2) {
	    dest_stream += UUEncodeTable[(unsigned char)*ip_pos>>2];
	    dest_stream += UUEncodeTable[(((unsigned char)*ip_pos&0x03)<<4) | 
					  ((unsigned char) *(ip_pos+1)>>4)];
	    dest_stream += UUEncodeTable[(((unsigned char)*(ip_pos+1)&0x0f)
					   <<2)];
	    dest_stream += UUEncodeTable[0];
	  }else if (ip_end-ip_pos == 1) {
	    dest_stream += UUEncodeTable[(unsigned char)*ip_pos >> 2];
	    dest_stream += UUEncodeTable[((unsigned char)*ip_pos & 0x03) << 4];
	    dest_stream += UUEncodeTable[0];
	    dest_stream += UUEncodeTable[0];
	  }
	  dest_stream += "\n";
	  pos += count;
	  if ( count != 45) break;
	}
    }

    static const char UUDecodeTable [256] = {
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
      16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
      32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
      48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
      16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 62, 31,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };

    static const char UULenTable[256] = {
      1,   5,   5,   5,   9,   9,   9,  13,  13,  13,  17,  17,  17,
      21,  21,  21,  25,  25,  25,  29,  29,  29,  33,  33,  33,  37,
      37,  37,  41,  41,  41,  45,  45,  45,  49,  49,  49,  53,  53,
      53,  57,  57,  57,  61,  61,  61,  65,  65,  65,  69,  69,  69,
      73,  73,  73,  77,  77,  77,  81,  81,  81,  85,  85,  85,  52,
      -10,  -1, -65, 103,  -4,   0,  64,   8,   0,   0,   0,  96,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, -96, 108,   1,
      64,   7,   0,   0,   0,  12, 103,   1,  64,  84,  -9,  -1, -65,
      106, -92,   0,  64, 116, 112,   1,  64, -48, -78,  27,  64,   1,
      0,   0,   0,   0,   0,   0,   0,  32,  -9,  -1, -65,  12, 103,
      1,  64, -96, 108,   1,  64, 120, -61,  25,  64,-119,-126,   4,
      8, -56, 122,   1,  64, -56, 122,   1,  64,-101, -65,  25,  64,
      120, -61,  25,  64,   0,   0,   0,   0,  30,  10,   0,  64,   0,
      3,   0,  64,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,  12, 103,   1,  64,   2,   0,   0,
      0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      -56, 122,   1,  64,  64,   7,   0,  64, -68, 108,   1,  64, -48,
      -78,  27,  64,  30,  10,   0,  64,   3,   0,   0,   0,   0,   3,
      0,  64,   0,   0,   0,  64, -84,-125,   4,   8, -84,-125,   4,
      8,  12, 103,   1,  64,   2,   0,   0,   0
    };
  
    void uu_decode (std::string &dest_stream, const std::string &src_stream)
    {
      int i, j, c, cc, count = 0, z1, z2, z3, z4;
      z1 = z2 = z3 = z4 = 0;
      std::string::const_iterator ipos = src_stream.begin ();

      while ( (ipos+4) < src_stream.end () )
	{
	  while ( *ipos == '\n' || *ipos == '\r') ++ipos;

	  i = UUDecodeTable[(int)(unsigned char)*ipos++];
	  j = UULenTable[i] - 1;
	
	  while ( j > 0 )
	    {
	      c  = UUDecodeTable[(int)(unsigned char)*ipos++] << 2;
	      cc = UUDecodeTable[(int)(unsigned char)*ipos++];
	      c |= (cc >> 4 );
	    
	      if ( i-- > 0 ) dest_stream += (unsigned char)c;

	      cc <<= 4;
	      c    = UUDecodeTable[(int)(unsigned char)*ipos++];
	      cc  |= (c >> 2);
	      if ( i--  > 0) dest_stream += (unsigned char)cc;
	    
	      c <<= 6;
	      c  |= UUDecodeTable[(int)(unsigned char)*ipos++];
	      if ( i-- > 0) dest_stream += (unsigned char)c;
	    
	      j -= 4;
	    }
	}
    }
  }
}
