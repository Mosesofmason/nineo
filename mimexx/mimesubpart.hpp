#ifndef __MIME_SUB_PART__
#define __MIME_SUB_PART__
#include <string>

namespace Mime
{
  class MimeSubPart
  {
  public:
    MimeSubPart ( const std::string& part );
    MimeSubPart ();
    ~MimeSubPart();

    void parser ( const std::string& part );
    void clear ();

  private:
    void do_parser ( const std::string& part );
    std::string m_content, m_type, m_id, m_desc, m_tencoding, m_dispos, m_content_location, m_content_base;
      
  };
  //     MimeSubPart::operator= ( const std::string& r)
  //     {
  //       m_raw_message = r.m_raw_message;
  //       m_content_type = r.m_content_type;
  //       m_content_sub_type = r.m_content_sub_type;
  //       m_charset = r.m_charset;
  //       m_content_transfter_encoding = r.m_content_transfter_encoding;
  //       m_subject = r.m_subject;
  //       m_body = r.m_body;
  //       m_boundary = r.m_boundary;
  //       m_headers = r.m_headers;
  //       m_parts = r.parts;
  //     }
}

#endif //END_OF_MIME_SUB_PART
