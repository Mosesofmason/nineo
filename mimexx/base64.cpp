#include "base64.hpp"
namespace Mime
{
  static const unsigned char B64EncodeTable[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
  };

  static const int B64DecodeTable[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
  };

  inline unsigned int base64_get_line ( std::string &line,
					const std::string &src,
					long offset = 0 )
  {
    std::string::const_iterator i_pos = src.begin () + offset;;
    long count = 0;
    line.erase ();
    
    if ( i_pos >= src.end() ) return 0;
    for ( ; line.size() < 57 && i_pos < src.end(); ++count, ++i_pos)
      line += *i_pos;
    return count;
  }
  
  void base64_encode ( std::string &dest, const std::string &src )
  {
    std::string::const_iterator ip_end, ip_pos;
    unsigned char c1, c2, c3;
    long pos = 0, count = 0;
    int last;
    std::string line;
    
    while (1)
      {
	count = base64_get_line (line, src, pos);
	if ( count == 0) break;
	ip_pos = line.begin ();
	ip_end = line.end ();

	for ( ; ip_pos <= ip_end-3; ip_pos+=3)
	  {
	    c1 = c2 = c3 = 0;
	    c1 = (unsigned char) *ip_pos;
	    c2 = (unsigned char) *(ip_pos+1);
	    c3 = (unsigned char) *(ip_pos+2);
	    dest += B64EncodeTable [( c1>> 2 )];
	    dest += B64EncodeTable [( (c1&0x03) <<4 )|( c2 >>4 )];
	    dest += B64EncodeTable [(( c2 & 0x0f) <<2 ) | ( c3 >>6)];
	    dest += B64EncodeTable [ (c3 & 0x3f) ];
	  }
	last = ip_end-ip_pos;
	if ( last != 0)
	  if (last == 2) {
	    c1 = (unsigned char) *ip_pos;
	    c2 = (unsigned char) *(ip_pos+1);
	    dest += B64EncodeTable [ c1 >> 2 ];
	    dest += B64EncodeTable[ (( c1 & 0x03 ) << 4)|(( c2 & 0xf0) >> 4)];
	    dest += B64EncodeTable[(( c2 & 0x0f) << 2 )];
	    dest += '=';
	  }else if (last == 1) {
	    c1 = (unsigned char) *ip_pos;
	    dest += B64EncodeTable[ c1 >> 2];
	    dest += B64EncodeTable[ ( c1 & 0x03 ) << 4 ];
	    dest += '=';
	    dest += '=';
	  }
	dest += "\n";
	pos += count;
	if ( count != 57) break;
      }
  }
  void base64_decode ( std::string &dest, const std::string &src)
  {
    int z1, z2, z3, z4;
    std::string::const_iterator ipos = src.begin ();
    //cout << "B64: src size " << src.size () << endl;
    z1 = z2 = z3 = z4 = 0;
    while (ipos+2 < src.end () && src.size() >= 4)
      {
	if ( *ipos == '\n' || *ipos == '\r' || *ipos == '\t' || *ipos == ' ')
	  {
	    ++ipos;
	    continue;
	  }
	while ((z1 = B64DecodeTable[(int)*ipos]) != -1 && ipos < src.end())
	  {
	    if ((z2 = B64DecodeTable[(int)(unsigned char)*(ipos+1)]) == -1)
	      break;
	    if ((z3 = B64DecodeTable[(int)(unsigned char)*(ipos+2)]) == -1)
	      break;
	    if ((z4 = B64DecodeTable[(int)(unsigned char)*(ipos+3)]) == -1)
	      break;
	    dest += (z1 << 2) | (z2 >> 4);
	    dest += (z2 << 4) | (z3 >> 2);
	    dest += (z3 << 6) | (z4);
	    ipos += 4;
	    while ( *ipos == '\n' || *ipos == '\r' ) ++ipos;
	  }
	if ( z1 == -1 || ipos >= src.end ()) break;
	if (z1 != -1 && z2 != -1 && (ipos+2)<src.end () && *(ipos+2) == '='){
	  dest += (z1 << 2) | (z2 >> 4);
	  ipos+=2;
	}else if (z1 != -1 && z2 != -1 && z3 != -1 &&
		  (ipos+3) < src.end() && *(ipos+3) == '=') {
	  dest += (z1 << 2) | (z2 >> 4);
	  dest += (z2 << 4) | (z3 >> 2);
	  ipos+=3;
	}else ++ipos;
      }
  }
}

