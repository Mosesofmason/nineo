#ifndef __MIMEPART__
#define __MIMEPART__
#include <string>
#include <vector>
#include "mimesubpart.hpp"

namespace Mime
{
  typedef std::vector<MimeSubPart> SubPartArray;
  class MimePart
  {
  private:
    std::string m_type, m_tencoding, m_id, m_desc, m_content;
    SubPartArray m_subparts;
    void init ();
    void do_parser ( const std::string& part );
    void do_subpart(std::string& part, const std::string& boundary);
  public:
    MimePart ( const std::string& part );
    MimePart ();
    ~MimePart ();

    inline int subpart_size ();
    MimeSubPart get_subpart ( const int pos);
    void parser ( const std::string& part );
    void clear ();
    void content_type ( const std::string& type );
    void content_tranfter_encoding ( const std::string& encoding );
    void content_id ( const std::string& id );
    void content_desc ( const std::string& desc);
    void content ( const std::string& content);

    const std::string& get_content_type ();
    const std::string& get_content_tranfter_encoding ();
    const std::string& get_content_id ();
    const std::string& get_content_desc ();
    const std::string& get_content ();
      
    void get_content_type ( std::string& type);
    void get_content_tranfter_encoding ( std::string& encoding );
    void get_content_id ( std::string& id);
    void get_content_desc ( std::string& desc);
    void get_content ( std::string& content);
  };
}

#endif
