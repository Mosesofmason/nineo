#ifndef __NEXT_NINEO_RESPONE_DEFINE__
#define __NEXT_NINEO_RESPONE_DEFINE__
/**
 *    \file  respone.hpp
 *   \brief  处理新闻组传输协议的结果
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
#include "config.hpp"
#include "group.hpp"
#include "db.hpp"
#include "ov.hpp"
#include "safe.hpp"

namespace NiNntp
{

    //! 新闻组协议的状态代码
    typedef unsigned short CODE;

    //! 状态码转状态信息
    //! 
    wxString NiNntpCodeToMessage ( const CODE& code );
    
    /**
     * \class NiResp 
     * \brief NNTP协议返回信息处理基类
     */
    class NiResp
    {
        public:
            NiResp ();
            ~NiResp ();
            wxString Status ();
            CODE Code ();

            virtual NiResp& operator() ( const wxString& line );
            virtual NiResp& operator<< ( const wxString& line );

            static inline bool CheckCode ( const CODE& code );
            wxString Message ();
        protected:
            void ClearStatus ();
            bool Status ( const wxString& line );
            inline bool PushStatus ( const wxString& line );
        private:
            wxString m_status;
            CODE m_code;
    };

    class NiRespDataCount : public NiResp 
    {
        public:
            NiRespDataCount ();
            ~NiRespDataCount ();
            virtual NiResp& operator () ( const wxString& line );
            virtual NiResp& operator << ( const wxString& line );
            size_t DataCount () const;
            size_t LineCount () const;
        private:
            size_t m_datacount, m_linecount;
    };
    /**
     *  \class NiRespOverview 
     */
    class NiRespOverview : public NiResp, private NiDB::SafeLiveLocker
    {
        public:
            NiRespOverview ( NiDB::ArticleDB& db );
            ~NiRespOverview ();
            virtual NiResp& operator () ( const wxString& line );
            virtual NiResp& operator << ( const wxString& line );
        private:
            NiDB::ArticleDB& m_db;
            OverviewPtr m_ov;
    };

    /** 
     * \class NiRespListGroup 
     * \brief 处理ListGroup的返回结果
     */
    class NiRespListGroup : public NiResp
    {
        public:
            NiRespListGroup ( NiDB::NiActiveDB& db );
            ~NiRespListGroup ();
           virtual NiResp& operator () ( const wxString& line );
            virtual NiResp& operator << ( const wxString& line );
        private:
            NiGroup::NiUnsubGroup m_group;
            NiDB::NiActiveDB& m_db;
            unsigned int m_count;
    };

    class NiRespML: public NiResp
    {
        public:
            NiRespML ();
            ~NiRespML ();

            virtual NiResp& operator () ( const wxString& line );
            virtual NiResp& operator << ( const wxString& line );
        private:
            wxString m_data;
    };
    /** 
     * \class NiRespDebug 
     * \brief 用于调试的结果处理类，把获得的结果通过wxLogDebug输出。
     */
    class NiRespDebug : public NiResp
    {
        public:
            NiRespDebug ();
            ~NiRespDebug ();
            virtual NiResp& operator () ( const wxString& line );
            virtual NiResp& operator << ( const wxString& line );
    };
    class NiRespGroup : public NiResp
    {
        public:
            NiRespGroup ();
            ~NiRespGroup ();

            unsigned int First ();
            unsigned int Last ();
            unsigned int Count ();
            wxString Group ();
            virtual NiResp& operator () ( const wxString& line );
            virtual NiResp& operator << ( const wxString& line );
        private:
            unsigned int m_first, m_last, m_count;
            wxString m_group;
            wxRegEx m_re;
    };
};


#endif //
