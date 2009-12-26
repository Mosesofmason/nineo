#ifndef __BEYOOO_MIME__
#define __BEYOOO_MIME__
#include <ctype.h>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>
#include "base64.hpp"
#include "qoupri.hpp"
#include "mimepart.hpp"
//#include "iconv++.hpp"
#include "nstring.hpp"

namespace Mime
{
  typedef std::pair< string, string > header_type;
  typedef std::vector < header_type > header_list;
  typedef std::vector < MimePart > PartArray;
  typedef std::vector < std::string > AttchArray;
  std::string get_field_encoding (const std::string& field);
  std::string decode_field (const std::string& field);
  class Mime
  {
  public:
    Mime (bool only_ph=false);
    ~Mime ();
    bool parser ( const std::string &raw_message );
    bool close ();
    void clear ();
    bool add_header ( const std::string &key, const std::string &value);
    std::string get_header ( const std::string &name, const std::string &no = "", const bool &ignore_case = false  );
//     std::string get_header ( const std::string &name, bool ignore_case = false );
    std::string get_de_header ( const std::string &name,
				bool ign_case = false);
    string decode_field ( const std::string &field );
    const std::string& get_contenttype () const;
    const std::string& get_contentsubtype () const;
    const std::string& get_charset () const;
    const std::string& get_contentencoding () const;
    std::string get_body ( const bool &decode = false );
    const std::string& get_boundary ();
    void get_body ( std::string& body );
    void get_contenttype ( std::string &type );
    void get_contentsubtype ( std::string &subtype );
    void get_charset (std::string &charset );
    void get_contentencoding ( std::string &encoding );
    int part_size ();
    int attch_size();
    string get_attch ( const int pos);

  private:
    inline std::string lower ( const std::string& str );
    bool do_parser ();
    void do_parser_mime ( std::string &body );
    void do_parser_mime2( const std::string &body );
    inline bool is_header ( const std::string &line );
    bool parser_header ( const std::string &line );
    void do_findattch ( std::string &body );
      
    bool only_parser_header;
    std::string m_raw_message, m_content_type, m_content_sub_type,
      m_charset, m_content_transfer_encoding, m_subject, m_body, m_boundary;
    header_list m_headers;
    PartArray m_parts;
    AttchArray m_attch;
  };
}

#endif // END OF __BEYOOO_MIME__
