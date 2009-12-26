#ifndef __NSTRING__
#define __NSTRING__
#include <vector>
#include <string>
using namespace std;
namespace nstring
{
  typedef std::vector<std::string> nstringarray;
  string string_trim_blank (const string *src, int* start);
  nstringarray tokenizer ( const string& str,const char sep );
  string string_replace (const string& source_string, const string& srep,
			 const string& trep, const int count);
  string cut_string_by_tag ( const string& s_string, const string& s_tag,
			     const string& e_tag );
  string cut_string_by_tag ( const string s_string, const string s_tag,
			     const string e_tag );
  string lower_string ( const string& s_string);
  string upper_string ( const string& s_string );
  string int2str ( const int& num );
}
#endif
