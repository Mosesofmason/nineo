#ifndef __EX_STRING__
#define __EX_STRING__
#include <string>
#include <algorithm>

namespace ExString
{
  bool is_digit ( const std::string& str );
  void to_upper ( std::string& str );
  void to_lower ( std::string& str );
  void trim ( std::string& str );
  void rtrim ( std::string& str );
  enum match_type {ABORT = -1 , FALSE = 0 , TRUE = 1};

  match_type mmatch ( const std::string& text,
		      const std::string& pattern,
		      std::string::size_type pos = 0);
  match_type match ( const std::string& text,
		     const std::string& pattern,
		     std::string::size_type pos = 0 );

  inline match_type wildmat(const std::string& text,
			    const std::string& pattern);
  
}
#endif
