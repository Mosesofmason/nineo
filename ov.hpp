#ifndef __NEXT_NINEO_OVERVIEW_DEFINE__
#define __NEXT_NINEO_OVERVIEW_DEFINE__
/**
 *    \file  ov.hpp
 *   \brief  Overview 类，用来实现NNTP返回结果Overview处理
 *
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-13
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++,borland free commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */
#include <memory>
#include "utils.hpp"
#include <wx/wx.h>

class wxString;

namespace NiNntp
{
    typedef std::auto_ptr <wxString> wxStringPtr;

    class Id 
    {
        public:
            Id ();
            ~Id ();
            unsigned int GetId () const;
            void SetId ( const unsigned int& id );
        protected:
            unsigned int m_id;
    };

    class Lines
    {
        protected:
            size_t m_lines;
        public:
            Lines ();
            ~Lines ();
            size_t GetLines () const;
            void SetLines ( const size_t& lines );
    };

    class Bytes
    {
        protected:
            size_t m_bytes;
        public:
            Bytes ();
            ~Bytes ();
            size_t GetBytes () const;
            void SetBytes ( const size_t& bytes );
    };

    class From
    {
        protected:
            wxStringPtr m_from;
        public:
            From ();
            ~From ();
            wxString GetFrom () const;
            void SetFrom ( const wxString& from );
    };

    class Subject
    {
        protected:
            wxStringPtr m_subject;
        public:
            Subject ();
            ~Subject ();
            wxString GetSubject () const;
            void SetSubject ( const wxString& subject );
    };

    class Date
    {
        protected:
            wxStringPtr m_date;
        public:
            Date ();
            ~Date ();
            wxString GetDate () const;
            void SetDate ( const wxString& date );
    };

    class MessageID
    {
        protected:
            wxStringPtr m_msgid;
        public:
            MessageID ();
            ~MessageID ();
            wxString GetMessageID () const;
            void SetMessageID ( const wxString& msgid );
    };

    class References
    {
        protected:
            wxStringPtr m_refs;
        public:
            References ();
            ~References ();
            wxString GetReferences () const;
            void SetReferences ( const wxString& refs );            
    };

    class XRef
    {
        protected:
            wxStringPtr m_xref;
        public:
            XRef ();
            ~XRef ();
            wxString GetXRef () const;
            void SetXRef ( const wxString& xref );
    };
    //!  \class Overview
    //!  \brief Overview 结果的处理
    //!  \todo 这个类还需要再分裂，把ParserOV部分分裂出来，成为单独的一部分。
    class Overview 
        : public Id,        public Lines,       public Bytes,
          public Subject,   public From,        public Date,
          public MessageID, public References,  public XRef
    {
        private:
            bool ParserOverview ( const wxString& ov );
        public:
            Overview ();
            ~Overview ();
            Overview& operator () ( const wxString& ov );
            Overview& operator =  ( const Overview& ov );
            Overview& operator << ( Overview& ov );
    };

#if 0
    class Overview
    {
        private:
            unsigned int m_id;
            size_t m_bytes, m_lines;
            wxStringPtr m_subject, m_from;
            inline bool ParserOV ( const wxString& ov );
        protected:
            wxStringPtr  m_date, m_msgid, m_refs, m_xref;

        public:
            Overview ();
            ~Overview ();
            void Id ( const unsigned int& id );
            unsigned int Id () const;
            void Bytes ( const size_t& bytes );
            size_t Bytes () const;
            void Lines ( const size_t& lines );
            size_t Lines () const;
            void Subject ( const wxString& subject );
            wxString Subject () const;
            void From ( const wxString& from );
            wxString From () const;
            void Date ( const wxString& date );
            wxString Date () const;
            void MessageID ( const wxString& msgid );
            wxString MessageID () const;
            void References ( const wxString& refs );
            wxString References () const;
            void XRef ( const wxString& xref );
            wxString XRef () const;
            Overview& operator ()  ( const wxString& ov );
            Overview& operator = ( const Overview& ov );
    };
#endif
    typedef std::auto_ptr <Overview> OverviewPtr;
}
#endif //


