#ifndef __MIME_DATE__
#define __MIME_DATE__
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <time.h>

namespace Date
{
  int week_of_day (const unsigned int &oy, const unsigned int &om,
		   const unsigned int &od );
  
  typedef struct
  {
    std::string name;
    int timezone;
  }TimeZone;

  const static TimeZone TZ_Table [] =
    {
      {"EST", -500},               /* US Eastern Standard Time*/
      {"CST", -600},               /* US Central */
      {"MST", -700},               /* US Mountain */
      {"PST", -800},               /* US Pacific  */
      {"EDT", -400},               /* US Eastern Daylight Time */
      {"CDT", -500},               /* US Central Daylight Time */
      {"MDT", -600},               /* US Mountain Daylight Time */
      {"PDT", -700},               /* US Pacific Daylight Time */
      {"CET", 100},                       /* Central European */
      {"MET", 100},                       /* Middle European */
      {"MEZ", 100},                       /* Middle European */
      {"MSK", 300},                       /* Moscow */
      {"HKT", 800},
      {"JST", 900},
      {"KST", 900},                       /* Korean Standard */
      {"CAST", 930},                      /* Central Autsralian */
      {"EAST", 1000},                     /* Eastern Autsralian */
      {"NZST", 1200},                     /* New Zealand Autsralian */
      {"EET", 200},                       /* Eastern European */
      {"", 0}
    };

  class DateTime
  {
  public:
    DateTime ()
      : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0),m_second(0),
      m_sign(0), m_tz(0)
    {};
  
    DateTime ( const unsigned int &y, const unsigned int &m,
	       const unsigned int &d, const unsigned int &h,
	       const unsigned int &mi, const unsigned int &s)
      : m_year(y), m_month(m), m_day(d), m_hour(h), m_minute(mi), m_second(s),
	m_tz(0), m_sign(0)
    {}
    DateTime ( const unsigned int &y, const unsigned int &m,
	       const unsigned int &d )
      : m_year(y), m_month(m), m_day(d), m_sign(0), m_tz(0), m_hour(0),
	m_minute(0), m_second(0)
    {}
    bool operator< ( DateTime& dt )
    {
      return to_seconds () < dt.to_seconds();
    }
    bool operator> ( DateTime& dt )
    {
      return to_seconds () > dt.to_seconds ();
    }
    bool operator== ( DateTime& dt )
    {
      return to_seconds () == dt.to_seconds ();
    }
    bool operator<= ( DateTime& dt )
    {
      return to_seconds() <= dt.to_seconds();
    }
    bool operator>= ( DateTime& dt )
    {
      return to_seconds() >= dt.to_seconds();
    }
    DateTime& operator++ ()
    {
      ++m_second;
      m_minute += ( m_second/60 );
      m_second %= 60;
      return  *this;
    }
    DateTime& operator-- ()
    {
      --m_second;
      if ( m_second < 0 )
	{
	  --m_minute;
	  m_second += 60;
	}
      return *this;
    }
    std::string operator>> ( std::string& ss )
    {
      ss = string();
      return ss;
    }
    DateTime& operator<< ( std::string& st )
    {
      parser (st);
      return *this;
    }
    void now ();
    std::string timestamp ();
    DateTime ( const std::string &datestring );
    void clear ();
    void print ();
    std::string string (const std::string format = "%a, %d %b %H:%M:%S %Y %z");
    const unsigned int& year ();
    const unsigned int& month ();
    const unsigned int& day ();
    const unsigned int& week ();
    const unsigned int& hour ();
    const unsigned int& minute ();
    const unsigned int& min ();
    const unsigned int& sec ();
    const unsigned int& second ();
    void operator=(const std::string &date);
    void parse ( const std::string& date )
    {
      parser (date);
    }
  private:
    unsigned long to_seconds ();
    int parser_timezone ( const std::string &tz);
    void parser ( const std::string &date);
    void set_time ( const unsigned int& h, const unsigned int& m,
		    const unsigned int& s);
    int m_sign;
    unsigned int m_tz, m_year, m_month, m_day, m_week;
    unsigned int null, m_hour, m_minute, m_second;
    std::string m_tzname;
    int is_week_day ( const std::string& n);
    int is_month ( const std::string &n );
    int is_day ( const std::string &n );
    int is_year ( const std::string &n );
    bool is_time ( const std::string &n );
  };
}
#endif
