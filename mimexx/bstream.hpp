#ifndef _B_STREAM_H__
#define _B_STREAM_H__
#include <vector>

using namespace std;
typedef vector<unsigned char> uchar_array;

class Bstream
{
public:
  Bstream ();
  ~Bstream ();
  void clear () { m_chararray.clear () ;}
  void start ();
  void end ();
  size_t size () { return m_chararray.size (); }
  
  bool push ( const unsigned char& ch);
  bool push ( const char& ch);
  bool push ( const unsigned char* ch);
  bool push ( const char* ch);
  bool push ( const unsigned char ch);
  bool push ( const char ch);
  bool pop ( unsigned char& ch);
  bool get ( size_t pos, unsigned char& ch);
  bool is_empty () { return ( m_chararray.size () == 0 ? true : false) ;}
private:
  bool m_isstart;
  uchar_array m_chararray;
};

#endif _B_STREAM_H__
