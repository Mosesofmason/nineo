#include "qoupri.hpp"

namespace Mime
{
  static const unsigned char HexEncodeTable[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
  };
  
  void qp_encode ( std::string &dest_stream, const std::string &src_stream)
  {
    std::string::const_iterator ipos = src_stream.begin ();
    std::string::const_iterator epos = src_stream.end ();
    long llen = 0;
    for ( ; ipos < epos; ipos++) {
      if (llen == 0 && *ipos == '.') {
	/*
	 * Special rule: encode '.' at the beginning of a line, so
	 * that some mailers aren't confused.
	 */
	dest_stream += '=';
	dest_stream += HexEncodeTable[((unsigned char)*ipos) >> 4];
	dest_stream += HexEncodeTable[((unsigned char)*ipos) & 0x0f];
	llen += 3;
      }
      else if ( (*ipos >= 33 && *ipos <= 60) ||
		(*ipos >= 62 && *ipos <= 126) ||
		(*ipos == 9  || *ipos == 32))
	{
	  dest_stream += *ipos;
	  llen++;
	}
      else if (*ipos == '\n') {
	/*
	 * If the last character before EOL was a space or tab,
	 * we must encode it. If llen > 74, there's no space to do
	 * that, so generate a soft line break instead.
	 */
	if ((epos-ipos) > 0 && (*(ipos-1) == 9 || *(ipos-1) == 32))
	  {
	    *(dest_stream.end()-1) = '=';
	    if (llen <= 74) {
	      dest_stream += HexEncodeTable[((unsigned char)*(ipos-1)) >> 4];
	      dest_stream += HexEncodeTable[((unsigned char)*(ipos-1))& 0x0f];
	      llen += 2;
	    }
	    dest_stream += '\n';
	  }
	/*
	 * Fix the soft line break condition from above
	 */

	if ((epos-ipos)>0 && ( *(ipos-1) == 9 || *(ipos-1) == 32) &&
	    *(dest_stream.end()-1) == '=') {
	  dest_stream += '=';
	  dest_stream += HexEncodeTable[((unsigned char)*(ipos-1)) >> 4];
	  dest_stream += HexEncodeTable[((unsigned char)*(ipos-1)) & 0x0f];
	  dest_stream += '\n';
	}
	llen = 0;
      }
      else {
	dest_stream += '=';
	dest_stream += HexEncodeTable[((unsigned char)*ipos) >> 4];
	dest_stream += HexEncodeTable[((unsigned char)*ipos) & 0x0f];
	llen += 3;
      }

      /*
       * Lines must be shorter than 76 characters (not counting CRLF).
       * If the line grows longer than that, we must include a soft
       * line break.
       */

      if ((ipos+1) < epos && *(ipos+1) != '\n' &&
	  (llen >= 75 ||
	   (!(( *(ipos+1) >= 33 && *(ipos+1) <= 60) ||
	      ( *(ipos+1) >= 62 && *(ipos+1) <= 126)) &&llen >= 73)
	   ))
	{
	  dest_stream += '=';
	  dest_stream += '\n';
	  llen = 0;
	}
    }
  }
  
  void qp_decode ( std::string &dest_stream, const std::string &src_stream)
  {
    std::string::const_iterator ipos = src_stream.begin ();
    std::string::const_iterator epos = src_stream.end ();
    unsigned char val;
    //       cout << "Quoted-Printable B"
    // 	   << src_stream
    // 	   << "Quoted-Printable E" << endl;
	
    //       for ( ; ipos < epos ; ++ipos)
    // 	{
    // 	  if ( *(ipos) == '=' ) break;
    // 	  dest_stream += *ipos;
    // 	}
    for (; ipos < epos; )
      {
	if ( *ipos == '=' && (ipos+2) < epos && isxdigit (*(ipos+1)) && isxdigit(*(ipos+2)))
	  {
	    val  = (isdigit(*(ipos+1)) ?
		    (*(ipos+1)-'0') : (tolower(*(ipos+1))-'a'+10)) << 4;
	    val |= ((isdigit(*(ipos+2))) ?
		    (*(ipos+2)-'0') : (tolower(*(ipos+2))-'a'+10));
	    dest_stream += val;
	    ipos += 3;
	  }else if (*ipos == '=' && (ipos+1) < epos && (*(ipos+1) == '\012' || *(ipos+1) == '\n' ) )
	  {
	    ipos += 2;
	  }else if ( *ipos == '=' && ((ipos+2) < epos && ( *(ipos+2) == '\015') || (*(ipos+1) == '\r'&&*(ipos+2) == '\n')))
	  {
	    ipos += 3;
	  }else 
	  {
	    dest_stream += *ipos;
	    ++ipos;
	  }
      }
  }
}


