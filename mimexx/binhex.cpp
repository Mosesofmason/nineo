#include "binhex.hpp"

namespace nmime {
//   cosnt char DONE = 0x7F;
//   const char SKIP = 0x7E;
//   const char FAIL = 0x7D;
//   typedef unsigned int crc_type;

//   const static char BinHexTable[] = {
//     // 0x00
//     SKIP, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, SKIP, SKIP, FAIL, FAIL, SKIP, FAIL, FAIL,
//     // 0x10
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0x20
//     SKIP, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//     0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, FAIL, FAIL,
//     // 0x30
//     0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, FAIL,
//     0x14, 0x15, DONE, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0x40
//     0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
//     0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, FAIL,
//     // 0x50
//     0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, FAIL,
//     0x2C, 0x2D, 0x2E, 0x2F, FAIL, FAIL, FAIL, FAIL,
//     // 0x60
//     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, FAIL,
//     0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, FAIL, FAIL,
//     // 0x70
//     0x3D, 0x3E, 0x3F, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0x80
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0x90
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0xA0
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0xB0
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0xC0
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0xD0
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0xE0
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     // 0xF0
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//     FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
//   };
  
//   static const crc_type CRCTable[] = {
//     0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
//     0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
//     0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
//     0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
//     0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
//     0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
//     0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
//     0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
//     0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
//     0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
//     0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
//     0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
//     0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
//     0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
//     0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
//     0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
//     0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
//     0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
//     0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
//     0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
//     0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
//     0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
//     0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
//     0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
//     0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
//     0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
//     0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
//     0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
//     0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
//     0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
//     0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
//     0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
// };
//   inline void update_crc ( crc_type &crc, int ch)
//   {
//     int idx = ((crc >> 8 ) ^ ch ) ^0xff;
//     crc = ((crc << 8 ) ^ CRCTable [idx]);
//   }
  
//   void binhex_encode ( std::string& dest, const std::string &src)
//   {
//     int last_char = -1, run_len = 1, scratch_pos = 0, line_length = 0;
//     char scratch[8];
//   }

  static const char BHDecodeTable[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1,  1, -1, -1,
    2,  3,  4,  5,  6,  7,  8,  9, 10, 11, -1, -1, -1, -1, -1, -1,
    -1, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, -1, -1, -1, -1, -1,
    -1, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
  };
  
  void binhex_decode ( std::string &dest, const std::string &src)
  {
      std::string::const_iterator ip = src.begin ();
      int z1, z2 ,z3, z4;
      z1 = z2 = z3 = z4 = 0;

      if ( src.empty () ) return;
      if ( *ip == ':' ) ip++;
      while ( ip < src.end () )
      {
          while ((z1 = BHDecodeTable [(int)(unsigned char) *ip]) != -1)
          {
              if ((z2 = BHDecodeTable[(int)(unsigned char) *(ip+1)]) == -1)
                  break;
              if ((z3 = BHDecodeTable[(int)(unsigned char) *(ip+2)]) == -1)
                  break;
              if ((z4 = BHDecodeTable[(int)(unsigned char) *(ip+3)]) == -1)
                  break;
              dest += ( z1 << 2 ) | ( z2 >> 4 );
              dest += ( z2 << 4 ) | ( z3 >> 2 );
              dest += ( z3 << 6 ) | ( z4 );
              ip += 4;
          }
          if ( z1 != -1 && z2 != -1 && z3 != -1 &&
                  (ip+3) < src.end() && *(ip+3) == ':' )
          {
              dest += ( z1 << 2 ) | ( z2 >> 4 );
              dest += ( z2 << 4 ) | ( z3 >> 2 );
              ip += 3;
              break;
          }else break;
      }
  }
  int binhex_write ( std::string &dest, std::string &src )
  {
    std::string tmp_string;
    int rpc = 0, count, tc = 0;
    size_t opc;
    if ( src.empty () ) {  rpc = 0;  return 0; }
  }

  int binhex_decomp ( const std::string &src, std::string &tmpstring,
          char &last, int &rpc, size_t max, size_t &opc )
  {
      size_t count, used = 0;
      char marker = '\220';
      std::string::const_iterator ip = src.begin ();
      std::string::size_type inc = src.size ();
      opc = 0;

      if ( rpc == -256 )
      {
          if ( inc == 0 ) return 0;
          rpc = (int)(unsigned char) *ip++; used++;

          if ( rpc == 0 )
          {
              last = marker;
              tmpstring += marker;
          }else rpc--;

          if ( rpc )
          {
              count = ( max > (size_t) rpc ) ? (size_t) rpc : max;
              tmpstring =  std::string(count, last);
              max -= count;
              opc += count;
              rpc -= count;
          }
      }
}
