#include "date.hpp"
namespace Date
{
  int week_of_day (const unsigned int &oy, const unsigned int &om,
		   const unsigned int &od )
  {
    int year, dow;
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= om < 3;
    dow = (year + year/4 - year/100 + year/400 + t[om-1] + od) % 7;
    // we use 1(Mon)..7 not 0(Sun)..6 as returned by the previous algorithm
    // so convert
    dow = ( dow == 0 ? 7 : dow);
    return dow;
  }

  void DateTime::operator=(const std::string &date)
  {
    clear ();
    parser (date);
  }
  void DateTime::print ()
  {
    std::cout << "Year: " << m_year << " Month: " << m_month << " Day: "
	      << m_day << " Hour:" << m_hour << " Min: " << m_minute
	      << " Second: " << m_second << " Sign: " << m_sign
	      << " TZ: " << m_tz << endl;
  }
  void DateTime::now ()
  {
    time_t tt= time(0);
    tm *vtm = gmtime (&tt);
    //std::string ct = ctime(&tt);
    //parser (ct);
//     std::cout << vtm->tm_year+1900 << " "
// 	      << vtm->tm_mon << " "
// 	      << vtm->tm_mday << " " 
// 	      << vtm->tm_hour << " "
// 	      << vtm->tm_min << " "
// 	      << vtm->tm_sec << endl;
    m_second = vtm->tm_sec;
    m_minute = vtm->tm_min;
    m_hour = vtm->tm_hour;
    m_day = vtm->tm_mday;
    m_month = vtm->tm_mon+1;
    m_year = vtm->tm_year + 1900;
    m_week = ( vtm->tm_wday == 0 ? 7 : vtm->tm_wday);
    m_sign = 0; m_tz = 0;
  }
  std::string DateTime::timestamp ()
  {
    std::string timestamp;
    //if ( m_seconds == 0 ) return timestamp;
    char* buf = new char[50];
    time_t vtime = (time_t)to_seconds();
    size_t size = strftime ( buf, 49, "%Y-%m-%d %H:%M:%S", gmtime(&vtime));
    for ( size_t cs = 0; cs < size; ++cs )
      timestamp += buf[cs];
    delete[] buf;
    return timestamp;
  }
  std::string DateTime::string ( const std::string format )
  {
    std::string str;
    char* buf = new char[50];
    time_t vtime;
    vtime = (time_t)to_seconds();
    size_t size = strftime ( buf, 49, format.c_str(), gmtime(&vtime));
    for ( size_t cs = 0; cs < size; ++cs )
      str += buf[cs];
    delete[] buf;
    return str;
  }
  void DateTime::parser ( const std::string &date )
  {
    clear ();
    std::string::const_iterator i = date.begin ();
    std::string item;
    while ( i < date.end () && *i == ' ') ++i;
    for ( item.erase() ; i <= date.end() ; ++i)
      {
	if ( *i == ',' || *i == '(' || *i ==  ')' || *i == '\n' || *i == '\r' )
	  continue;
	if (*i == ' ' || i == date.end ())
	  {
	    int tw, ty, tm, td, ttz;
	    bool t = 1;
	    tw =  is_week_day (item);
	    tm = is_month (item);
	    td = is_day (item);
	    ty = is_year (item);
	    t = is_time (item);
// 	    std::cout << item << "  tw " << tw << " tm " << tm
// 		      << " td " << td << " ty " << ty << endl;
	    if ( item == "UTC" || item == "GMT" || parser_timezone (item) != 0)
		m_tzname = item;
	  
	    if ( item.find ("+") != std::string::npos ||
		 item.find ("-") != std::string::npos )
	      {
		if ( item.find('+') != std::string::npos ) 
		  m_sign = 1;
		else if ( item.find('-') != std::string::npos ) 
		  m_sign = -1;
		std::string::size_type pos = item.find ('+');
		if ( pos == std::string::npos )
		  pos = item.find ('-');
		item = item.substr(pos+1);
		std::istringstream stz (item);
		stz >> m_tz;
	      }
	    else if ( tw != 0 )
	      m_week = tw;
	    else if ( tm != 0 )
	      m_month = tm;
	    else if ( td != 0 )
	      m_day = td;
	    else if ( ty != 0 )
	      if ( ty < 100 )
		{
		  if ( ty > 50 )
		    m_year += 1900;
		  else
		    m_year += 2000;
		}else m_year = ty;
	    else if (t)
	      {
		int ihour, iminute, isecond;
		std::string h, m, s;
		std::string::iterator ix = item.begin ();
	      
		for (; ix < item.end() && *ix != ':';++ix)
		  h += *ix;

		if ( ix < item.end () && *ix == ':') ++ix;
		for (; ix < item.end () && *ix != ':'; ++ix)
		  m += *ix;

		if ( ix < item.end () && *ix == ':') ++ix;
		for (; ix < item.end (); ++ix)
		  s += *ix;
		std::istringstream sh(h), sm (m), ss(s);
		sh >> ihour;
		sm >> iminute;
		ss >> isecond;
		set_time (ihour, iminute, isecond);
	      }
	    item.erase();
	  }else item += *i;
      }
  }
  unsigned long DateTime::to_seconds ()
  {
    int tz = m_sign * m_tz;
    unsigned long t_day = 0, t_year = 0, t_hours = 0, t_minutes = 0,
      seconds =0, t_month = m_month-1;
    if ( !m_tzname.empty() && (m_tzname != "GMT" || m_tzname != "UTC"))
      tz = parser_timezone (m_tzname);
    t_day = 31 * t_month+ m_day;
    if (t_month > 1)
      {
 	t_day -= ( t_month * 4 + 27 )/10;
 	if ( m_year%4 == 0 ) t_day++;
      }
    --t_day;
    t_year = m_year - 1970;
    t_day += t_year * 365 + t_year / 4;
    if ((t_year % 4) == 3) t_day++;
    t_hours = m_hour - (tz / 100);
    t_minutes = m_minute - (tz % 100);
    seconds = 60L * (t_minutes + 60L * (t_hours +  24L * t_day))+m_second;
    return seconds;
  }
  DateTime::DateTime ( const std::string &datestring )
  {
    clear ();
    parser (datestring);
  }

  int DateTime::parser_timezone ( const std::string &tz )
  {
    const TimeZone *tp = TZ_Table;
    while ( tp->timezone != 0 )
      {
	if ( tz == tp->name ) return tp->timezone;
	++tp;
      }
    return 0;
  }

  void DateTime::clear ()
  {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_week = 0;
    m_hour = 0;
    m_minute = 0;
    m_second = 0;
    m_sign = 0;
    m_tz = 0;
    m_tzname.erase ();
  }
  
  void DateTime::set_time ( const unsigned int& h, const unsigned int& m,
			    const unsigned int& s)
  {
    m_hour = h;
    m_minute = m;
    m_second = s;
  }
  const unsigned int& DateTime::year ()
  {
    return m_year;
  }
  const unsigned int& DateTime::month ()
  {
    return m_month;
  }
  const unsigned int& DateTime::day ()
  {
    return m_day;
  }
  const unsigned int& DateTime::week ()
  {
    return m_week;
  }
  const unsigned int& DateTime::hour ()
  {
    return m_hour;
  }
  const unsigned int& DateTime::minute ()
  {
    return m_minute;
  }
  const unsigned int& DateTime::min ()
  {
    return m_minute;
  }
  const unsigned int& DateTime::sec ()
  {
    return m_second;
  }
  const unsigned int& DateTime::second ()
  {
    return m_second;
  }
  int DateTime::is_week_day ( const std::string& n )
  {
    if ( n == "Mon" || n == "mon" || n =="Monday" || n == "monday" )
      return 1;
    if ( n == "Tue" || n == "tue" || n == "Tuesday" || n == "tuesday" )
      return 2;
    if ( n == "Wed" || n == "wed" || n == "Wednesday" || n == "Wednesday" )
      return 3;
    if ( n == "Thu" || n == "thu" || n == "Thursday" || n == "thursday" )
      return 4;
    if ( n == "Fri" || n == "fri" || n == "Friday" || n == "friday" )
      return 5;
    if ( n == "Sat" || n == "sat" || n == "Saturday" || n == "saturday" )
      return 6;
    if ( n == "Sun" || n == "sun" || n == "Sunday" || n == "sunday" )
      return 7;
    return 0;
  }

  int DateTime::is_month ( const std::string &n )
  {
    if ( n == "Jan" || n == "January" )
      return 1;
    if ( n == "Feb" || n == "February" )
      return 2;
    if ( n == "Mar" || n == "March" )
      return 3;
    if ( n == "Apr" || n == "April" )
      return 4;
    if ( n == "May" || n == "may" )
      return 5;
    if ( n == "Jun" || n == "June" )
      return 6;
    if ( n == "Jul" || n == "July" )
      return 7;
    if ( n == "Aug" || n == "August" )
      return 8;
    if ( n == "Sep" || n == "September" )
      return 9;
    if ( n == "Oct" || n == "October" )
      return 10;
    if ( n == "Nov" || n == "November" )
      return 11;
    if ( n == "Dec" || n == "December" )
      return 12;
    return 0;
  }
  int DateTime::is_day ( const std::string &n)
  {
    if ( n.size () >= 1 && n.size () <= 2 )
      {
	for ( std::string::const_iterator i = n.begin (); i < n.end (); ++i)
	  if ( !(*i >= '0' && *i <= '9') ) return 0;
	std::istringstream ss (n);
	int num = 0;
	ss >> num;
	return num;
      }
    else return 0;
  }

  int DateTime::is_year ( const std::string &n )
  {
    if ( n.size () != 4 ) return 0;
    bool f = true;
    for ( std::string::const_iterator i = n.begin (); i < n.end(); ++i)
      if (!( *i >= '0' && *i <= '9')) return 0;
    std::istringstream ss (n);
    int num;
    ss >> num;
    return num;
  }

  bool DateTime::is_time ( const std::string &n )
  {
    int c=0;
    std::string::const_iterator i = n.begin ();
    for (; i<n.end (); ++i) if (*i == ':' )++c;
    return c == 2 ? true : false;
  }
}
