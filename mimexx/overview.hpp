#ifndef __OVERVIEW_H__
#define __OVERVIEW_H__
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "newstype.hpp"
namespace News
{
  enum NewsStatus {
    NewsBozo = -3,
    NewsSpam = -2,
    NewsHate = -1, 
    NewsUnread = 0,
    NewsRead = 1,
    NewsTick = 2,
    NewsDor = 6, // dormant
    NewsFav = 8 // favor
  };
  struct OverView
  {
    id_type id;
    std::string subject, from, date, messageid, references, xref;
    unsigned int bytes, lines;
    unsigned short ref_count;
    NewsStatus status;
    OverView ()
      :id(0), bytes(0), lines(0), ref_count(0),status(NewsUnread)
    {}
    OverView ( const OverView& ov )
      : id(0), bytes(0), lines(0), ref_count(0), status(NewsUnread)
    {
      id = ov.id;
      subject = ov.subject;
      from = ov.from;
      date = ov.date;
      messageid = ov.messageid;
      references = ov.references;
      xref = ov.xref;
      bytes = ov.bytes;
      lines = ov.lines;
      ref_count = ov.ref_count;
      status = ov.status;
    }
    void print ()
    {
      std::cout << "ID: [" << id << "] Subject: [" << subject << "]\n\t"
		<< "From: [" << from << "]\n\tDate: [" << date << "]\n\t"
		<< "MessageID: [" << messageid << "]\n\tXref:" 
		<< xref <<"]\n\t"
		<< "Bytes: " << bytes << " Lines:" << lines << " ref:"
		<<  ref_count << " Status: " << status << "\n\n";
    }
    OverView& operator= ( OverView& ov )
    {
      id = ov.id;
      subject = ov.subject;
      from = ov.from;
      date = ov.date;
      messageid = ov.messageid;
      references = ov.references;
      xref = ov.xref;
      bytes = ov.bytes;
      lines = ov.lines;
      ref_count = ov.ref_count;
      status = ov.status;
      return *this;
    }
    OverView& operator= ( const OverView& ov )
    {
      id = ov.id;
      subject = ov.subject;
      from = ov.from;
      date = ov.date;
      messageid = ov.messageid;
      references = ov.references;
      xref = ov.xref;
      bytes = ov.bytes;
      lines = ov.lines;
      ref_count = ov.ref_count;
      return *this;
    }
    void clear ()
    {
      subject.erase(); from.erase(); date.erase(); messageid.erase();
      references.erase(); xref.erase ();
      id = 0;
      bytes = 0;
      lines = 0;
      ref_count = 0;
      status = NewsUnread;
    }
  };
  typedef vector< OverView > OverviewList;
  class OverviewFMT
  {
  private:
    int mp_subject, mp_from, mp_date, mp_messageid, mp_references, mp_bytes,
      mp_lines, mp_xref;
    std::string m_id, m_subject, m_from, m_date, m_messageid, m_references,
      m_bytes, m_lines, m_xref;
    unsigned short ref_count ();
  public:
    OverviewFMT ()
      : mp_subject(0), mp_from(0), mp_date(0), mp_messageid(0), mp_references(0), mp_bytes(0), mp_lines(0), mp_xref(0)
    {};
    OverviewFMT(const std::string& overviewfmt )
      : mp_subject(0), mp_from(0), mp_date(0), mp_messageid(0), mp_references(0), mp_bytes(0), mp_lines(0), mp_xref(0)
    {
      init ( overviewfmt );
    }
    void operator>> ( OverView& ov )
    {
      ov.subject = subject();
      ov.from = from ();
      ov.date = date ();
      ov.messageid = messageid ();
      ov.references = references ();
      ov.xref = xref ();
      ov.bytes = bytes ();
      ov.lines = lines ();
      ov.ref_count = ref_count ();
      std::istringstream sid (id());
      sid >> ov.id;
      clear ();
    }
    void parse ( const std::string& overview );
    void init ( const std::string& overviewfmt );
    void clear ();
    const std::string& subject ();
    const std::string& from ();
    const std::string& date ();
    const std::string& messageid ();
    const std::string& references ();
    const std::string& xref ();
    const std::string& id();
    int bytes ();
    int lines ();
  };
}
#endif
