#include "bstream.hpp"

Bstream::Bstream ()
{
  clear ();
}

Bstream::~Bstream ()
{
  clear ();
}

void Bstream::start ()
{
  clear ();
  m_isstart = true;
}

void Bstream::end ()
{
  m_isstart = false;
}

bool Bstream::push ( const unsigned char& ch)
{
  if ( m_isstart == false ) return false;
  m_chararray.push_back ( ch );
  return true;
}

bool Bstream::push ( const char& ch)
{
  if ( m_isstart == false) return false;
  m_chararray.push_back ( (unsigned char) ch);
  return true;
}

bool Bstream::push ( const unsigned char* ch)
{
  if ( m_isstart == false) return false;
  m_chararray.push_back (*ch);
  return true;
}

bool Bstream::push ( const char* ch)
{
  if ( m_isstart == false) return false;
  m_chararray.push_back ( (unsigned char) *ch);
  return true;
}

bool Bstream::push ( const unsigned char ch)
{
  if ( m_isstart == false ) return false;
  m_chararray.push_back ( ch );
  return true;
}

bool Bstream::push ( const char ch)
{
  if ( m_isstart == false ) return false;
  m_chararray.push_back ( ch );
  return true;
}
 
bool Bstream::get ( size_t pos , unsigned char& ch)
{
  if ( pos >= 0 && pos <= m_chararray.size ())
    {
      ch = m_chararray[pos];
      return true;
    }
  else
    return false;
}

bool Bstream::pop ( unsigned char& ch)
{
  if ( is_empty () ) return false;
  //ch = m_chararray.pop_back ();
  return true;
}

