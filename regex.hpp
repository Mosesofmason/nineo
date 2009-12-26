#ifndef __NEXT_NINEO_REGEX_DEFINE__
#define __NEXT_NINEO_REGEX_DEFINE__
/**
 *    \file  regex.hpp
 *   \brief  正则匹配定义
 *
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-12
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++,borland free commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */


#include <wx/wx.h>
#include <wx/regex.h>
#include <vector>
#include <utility>
#include <pcre.h>

namespace NiRegEx
{
    class NiPcre 
    {
        public:
            typedef std::pair < int, int > pair;
            typedef std::vector < pair > pairs_vector;
            static size_t MAX_VECTOR_SIZE;
            NiPcre ( const wxString& pattern, const size_t& vc_size = MAX_VECTOR_SIZE );
            NiPcre ( const size_t& vc_size = MAX_VECTOR_SIZE );
            bool Compile ( const wxString& pattern );
            bool IsValid ();
            bool Matches ( const wxString& data );
            bool FindAll ( const wxString& data );
            size_t GetMatchCount () const;
            void GetMatch ( size_t& start, size_t& end, const size_t& match = 0 ) const;
            inline wxString GetMatch ( const wxString& data, 
                                       const size_t& match = 0 ) const
            {
                return GetSubString ( data, match );
            }
            wxString GetSubString ( const wxString& data, const size_t& match = 0 ) const;
            ~NiPcre ();
            wxString Version ();
        private:
            bool UCompile ( const wxString& pattern );
            bool UMatches ( const wxString& data );
            size_t inline GetMatchStart ( const size_t& match = 0 ) const;
            size_t inline GetMatchEnd   ( const size_t& match = 0 ) const;
            size_t inline GetMatchLen   ( const size_t& match = 0 ) const;
            bool __Compile__ ( const char* cpattern, const size_t& len );
            bool __Matches__ ( const char* buf, const size_t& len  );
            bool __FindAll__ ( const char* buf, const size_t& len );
            void InitVector ( const size_t& vc_size = MAX_VECTOR_SIZE );
            pcre *m_re;
            wxString m_error;
            int m_errorcode, m_options;
            std::vector <int> m_vector;
            pairs_vector m_vecpairs;
    };
    class RegEx : private wxRegEx
    {
        public:
            RegEx ();
            RegEx(const wxString& expr, int flags = wxRE_DEFAULT);
            ~RegEx ();
            bool Compile(const wxString& pattern, int flags = wxRE_DEFAULT);
            bool IsValid ();

            bool GetMatch(size_t& start, size_t& len, size_t index = 0) const;
            wxString GetMatch(const wxString& text, size_t index = 0) const;
            size_t GetMatchCount() const;
            bool Matches(const wxChar* text, int flags = 0) const;
            bool Matches(const wxChar* text, int flags, size_t len) const;
            bool Matches(const wxString& text, int flags = 0) const;
            bool MatchesAll ( const wxString& text, int flags = 0 ) const;
            int Replace(wxString* text, const wxString& replacement, size_t maxMatches = 0) const;
            int ReplaceAll(wxString* text, const wxString& replacement) const;
            int ReplaceFirst(wxString* text, const wxString& replacement) const;
            typedef std::pair < int, int > offset_pairs;
            typedef std::vector < offset_pairs > offset_vector;
        private:
            offset_vector m_vector;
    };


    //! 匹配组名的前缀
    const wxString CatchGroupnamePrefix (wxT("^([^\\.]*).*"));
    const wxRegEx ReCatchGroupnamePrefix ( CatchGroupnamePrefix );

    //! 匹配组名列表格式
    const wxString MatchListGroup (wxT("^([^ ]*) ([[:digit:]]*) ([[:digit:]]*) (.)$"));
    const wxRegEx ReMatchListGroup ( MatchListGroup );

    //! 匹配空行
    const wxString MatchSpaceLine (wxT("^[[[:blank:]][[:cntrl:]]]*$"));
    const wxRegEx RematchSpaceLine ( MatchSpaceLine );

      //!  匹配Overview
      // -"0" or article number (see below)
      // -Subject header content
      // -From header content
      // -Date header content
      // -Message-ID header content
      // -References header content
      // -:bytes metadata item
      // -:lines metadata item

    const wxString MatchOverview (wxT("^([[:digit:]]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\t([[:digit:]]*)\t([[:digit:]]*)\t(.*)$"));
    const wxRegEx ReMatchoverview ( MatchOverview );
    const wxString MatchEncodeField (wxT("=\\?([^\\?]*)\\?(.)\\?([^\\?]*)\\?="));
    const wxString MatchMessageID (wxT("(<[^@]*@[^@]*>)*"));
    const wxString MatchEmail (wxT("([^@]+)@([[:alnum:].-_].)+([[:alnum:]]+)"));
};

#endif //
