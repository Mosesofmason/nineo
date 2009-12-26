#include "bencoder.hpp"

namespace encoder
{

  static unsigned char B64EncodeTable[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
  };

  static unsigned char XXEncodeTable[64] = {
    '+', '-', '0', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
    'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
  };

  static unsigned char BHEncodeTable[64] = {
    '!', '"', '#', '$', '%', '&', '\'', '(',
    ')', '*', '+', ',', '-', '0', '1', '2',
    '3', '4', '5', '6', '8', '9', '@', 'A', 
    'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
    'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 
    'S', 'T', 'U', 'V', 'X', 'Y', 'Z', '[', 
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'h', 
    'i', 'j', 'k', 'l', 'm', 'p', 'q', 'r'
  };

  static unsigned char HexEncodeTable[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
  };

      // encode and decode uuencoding.
  void Encoder::uu_encode ( const unsigned char* stream, const sizt_t len)
  {
    size_t llen;
    const unsigned char* ps = stream;
    
    for ( ; (llen+3) <= len; ++pstream)
      {
	llen = ( ps - stream);
	if ( ( llen % 45) == 0 )
	  {
	    if ( llen != 0 ) encode_str += "\n" ;
	    if (( len - llen ) >= 45)
	      {	encode_str += UUEncodeTable[45]; }
	    else
	      {	encode_str += UUEncodeTable[ len -  llen ]; }
	  }
	encode_str += UUEncodeTable[(*ps >> 2)];
	encode_str += UUEncodeTable [ (((*ps & 0x03) << 4) | *(ps+1) >> 4) ];
      	encode_str += UUEncodeTable [ ((*(ps+1) & 0xf) << 2) | *(ps+2) >> 6];
      	encode_str += UUEncodeTable [ *(ps+2) & 0x3f ];
      }	
    if ( ( len - llen ) == 2 )
      {
	encode_str += UUEncodeTable [ *ps >> 2];
	encode_str += UUEncodeTable [(( *ps & 0x03 ) << 4) | 
				     ( *(ps+1) >> 4)];
	encode_str += UUEncodeTable [((*(ps+1) & 0x0f) << 2)];
	encode_str += UUEncodeTable [0];
      }
    else if (( len - llen ) == 1) {
      encode_str += UUEncodeTable[ *ps >> 2];
      encode_str += UUEncodeTable[ (*ps & 0x03) << 4];
      encode_str += UUEncodeTable[0];
      encode_str += UUEncodeTable[0];
    }
  }
  
  void Encoder::uu_decode ( const char* src_str, size_t len);

    // encode and decode xxencoding.
  void Encoder::xx_encode ( const char* stream, size_t len);
  void Encoder::xx_encode ( const char* src_str, size_t len);

    // encode and decode base64.
  void Encoder::b64_encode ( const char* stream, size_t len);
  void Encoder::b64_decode ( const char* src_str, size_t len);

    // encode and decode qouted-printable.
  void Encoder::qp_encode ( const char* stream, size_t len);
  void Encoder::qp_decode ( const char* src_str, size_t len);

    // encode and decode yenc.
  void Encoder::yc_encode ( const char* stream, size_t len);
  void Encoder::yc_decode ( const char* stream, size_t len);

	
  //string decode_uu (const string& src_str);

  // encode and decode for BASE64
  string encode_base64 (const string& src_str)
  {

    string result;
    string::const_iterator ip_ch;
    int bits = 0;
    int char_count = 0;
    int out_cnt = 0;
    int c;

    if (src_str.empty ()) return string ("");

    for ( ip_ch = src_str.begin (); ip_ch < src_str.end (); ++ip_ch)
      {
	c = (unsigned char) *ip_ch;
	bits += c;
	char_count++;
	if (char_count == 3) {
	  result += B64EncodeTable [bits >> 18];
	  result += B64EncodeTable [(bits >> 12) & 0x3f];
	  result += B64EncodeTable [(bits >> 6) & 0x3f];
	  result += B64EncodeTable [bits & 0x3f];
	  bits = 0;
	  char_count = 0;
	} else {
	  bits <<= 8;
	}
      }
    if (char_count != 0) {
      bits <<= 16 - (8 * char_count);
      result += B64EncodeTable [bits >> 18];
      result += B64EncodeTable [(bits >> 12) & 0x3f];
      if (char_count == 1) {
	result += "==";
      } else {
	result +=B64EncodeTable [(bits >> 6) & 0x3f];
	result += '=';
      }
    }
    return result;
 
  inline int unbase64char(char ch)
  {
    int i;
    for(i=0;i<=63;i++) if(B64EncodeTable[i]==ch) break;
    return i;
  }

  string decode_base64 (const string& src_str)
  {
    int base64_value[256];
    int c, i;
    long val;
    unsigned int k = 0;
    
    string::const_iterator ip_ch;
    string result;
    
    for (i = 0; i < 256; i++)
      base64_value[i] = -1;

    for (i = 0; i < 64; i++)
      base64_value [ (int) B64EncodeTable[i] ] = i;
    base64_value['='] = 0;


    if ( src_str.empty () ) return string ("");

    val = c = 0;
    
    for (ip_ch = src_str.begin ();  ip_ch < src_str.end () ; ++ip_ch) {
      k = ((unsigned char) *ip_ch) % 256;
      if (base64_value[k] < 0)
	continue;
      
      val <<= 6;
      val += base64_value[k];
      if (++c < 4)
	continue;
      /* One quantum of four encoding characters/24 bit */
      result +=  val >> 16; /* High 8 bits */
      result += (val >> 8) & 0xff;/* Mid 8 bits */
      result += val & 0xff;	/* Low 8 bits */
      val = c = 0;
    }
    return result;
  }

  // encode and decode for XXencoding
  string encode_xx (const string& src_str)
  {
    string encode_str;
    string::const_iterator str_ip;
	
    for ( str_ip = src_str.begin () ; (str_ip+3) <= src_str.end (); str_ip += 3)
      {
	if ( ((str_ip - src_str.begin ()) % 45) == 0 )
	  {
	    if (str_ip != src_str.begin ()) encode_str += "\n" ;
	    if ((src_str.end () - str_ip) >= 45)
	      {	encode_str += XXEncodeTable[45];	}
	    else
	      {	encode_str += XXEncodeTable[src_str.end () - str_ip]; }
			
	  }
	encode_str += XXEncodeTable[(*str_ip >> 2)];
	encode_str += XXEncodeTable[ (((*str_ip & 0x03) << 4) | (*(str_ip+1) >> 4))];
      	encode_str += XXEncodeTable [ ((*(str_ip+1) & 0xf) << 2) | ( *(str_ip+2) >> 6) ];
      	encode_str += XXEncodeTable [ (*(str_ip+2) & 0x3f ) ];
      }	
    if ( ( src_str.end() - str_ip ) == 2 )
      {
	encode_str += XXEncodeTable [ *str_ip >> 2];
	encode_str += XXEncodeTable [(( *str_ip & 0x03 ) << 4) | 
				     ( *(str_ip+1) >> 4)];
	encode_str += XXEncodeTable [((*(str_ip+1) & 0x0f) << 2)];
	encode_str += XXEncodeTable [0];
      }
    else if ( (src_str.end () - str_ip) == 1) {
      encode_str += UUEncodeTable[ *str_ip >> 2];
      encode_str += UUEncodeTable[(*str_ip & 0x03) << 4];
      encode_str += UUEncodeTable[0];
      encode_str += UUEncodeTable[0];
    }
    return encode_str;
  }
  //string decode_xx (const string& src_str);

  // encode and decode for BinHex
  //string encode_binhex (const string& src_str);
  //string decode_binhex (const string& src_str);

  // encode and decode for quoted-printable
  string encode_qp (const string& src_str)
  {
    string encode_str;
    string::const_iterator s_ip;
    unsigned char tp, first, second;
	
    for ( s_ip = src_str.begin (); s_ip != src_str.end (); ++ s_ip)
      {
	tp = *s_ip;
	if ( (int)tp > 127)
	  {
	    encode_str += "=";
	    first = (unsigned char) (tp >> 4);
	    if ( first > 9 ) 
	      {
		first += 55;
	      }else 
	      { 
		first = (unsigned char) (first + 48);
	      }

	    second = (unsigned char) ( tp & 15 );
	    if ( second > 9 )
	      {
		second += 55;
	      }else
	      {
		second += 48;
	      }
	    encode_str += first;
	    encode_str += second;
	  }else
	  {
	    encode_str += *s_ip;
	  }
      }
    return encode_str;
  }
  string decode_qp (const string& src_str)
  {
    string decode_str;
    unsigned char tp, tc, first, second;
	
    string::const_iterator s_ip = src_str.begin (), e_ip = src_str.end ();
	
    for ( ; s_ip != e_ip ; ++ s_ip )
      {
	if ( *s_ip == '=' && (s_ip + 1) != e_ip && (s_ip + 2) != e_ip)
	  {
	    tc = NULL;
	    first = *(s_ip + 1);
	    second = *(s_ip + 2);
	    if (first == '\r' && second == '\n')
	      {
		s_ip += 2;
		continue;
	      }
	    if ( first == '\n')
	      {
		s_ip += 1;
		continue;
	      }
	    
	    //cout << first << second <<endl;
			
	    if ( first >= 65 ) 
	      {	first -= 55;	}
	    else
	      {	first -= 48;	}
			
	    second = *(s_ip + 2);
	    if ( second >= 65 )
	      {	second -= 55;	}
	    else
	      {	second -=  48; }
	    tc = (unsigned char) ( first << 4 );
	    tc |= second;
	    decode_str += (unsigned char) tc;
	    s_ip += 2;
	  }else
	  {
	    decode_str += *s_ip;
	  }
      }
    return decode_str;
  }
  
  string convert_rot13 (const string& src_str)
  {
    string result;
    string::const_iterator i_pos;
    result.erase ();
    char ch;

    for ( i_pos = src_str.begin (); i_pos < src_str.end (); ++i_pos)
      {
	ch = (char)(*i_pos);
	if ( isalpha(ch) )
	  {
	    if ( tolower (ch) < 'n' )
	      {
		result += (char)(ch + 13);
	      }
	    else {
	      result += (char)(ch - 13);
	    }
	  }
	else {
	  result += ch;
	}
      }
    return result;
  }
  string encode_rot13 (const string& src_str)
  {
    return convert_rot13 (src_str);
  }
  string decode_rot13 (const string& src_str)
  {
    return convert_rot13 (src_str);
  }

  string decode_hz (const string& src_str)
  {
    string::const_iterator ch_pos, ch2_pos;
    string result;
    char ch;

    for ( ch_pos = src_str.begin (); ch_pos < src_str.end (); ++ch_pos)
      {
	if ( *ch_pos == '~' && (ch_pos+1) < src_str.end () && *(ch_pos+1) == '{')
	  {
	    for ( ch2_pos = ch_pos+2; ch2_pos < src_str.end (); ++ ch2_pos )
	    {
	      if ( *ch2_pos == '~' && (ch2_pos+1) < src_str.end () && *(ch2_pos+1) == '}')
		{
		  break;
		}
	      ch = (char) *ch2_pos;
	      if (ch < 33)
		{
		  result +=  ch;
		}else if (ch < 127)
		{
		  ch += 128;
		  result += ch;
		}
	    }
	    ch_pos = ch2_pos+1;
	  }else
	  {
	    result += *ch_pos ;
	  }
      }
    return result;
  }
  
  string encode_hz (const string& src_str)
  {
    string::const_iterator i_ch, i_ch2;
    string result;
    unsigned char ch;

    for ( i_ch = src_str.begin (); i_ch < src_str.end (); ++i_ch)
      {
	ch = *i_ch;
	if ( ch > 127)
	  {
	    result += "~{";
	    for ( i_ch2 = i_ch; i_ch2 < src_str.end (); ++i_ch2)
	      {
		ch = *i_ch2;
		if ( ch < 127)
		  {
		    break;
		  }else
		  {
		    ch -= 128;
		    result += ch;
		  }
	      }
	    result += "~}";
	    result += ch;
	    i_ch = i_ch2;
	  }
	else
	  {
	    result += *(i_ch);
	  }
      }
    return result;
  }


  const wxCharBuffer ConvertToUTF8(wxString anyString)
  {
    return wxConvUTF8.cWC2MB( anyString.wc_str(*wxConvCurrent) ) ;
  }

  // Use the raw UTF-8 data passed to build a wxString
  wxString rawutf8_to_wxstring (const char* rawUTF8)
  {
    return wxString(wxConvUTF8.cMB2WC(rawUTF8), *wxConvCurrent);
  }
}
