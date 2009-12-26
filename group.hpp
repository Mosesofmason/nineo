#ifndef __NEXT_NINEO_GROUP_DEFINE__
#define __NEXT_NINEO_GROUP_DEFINE__
/**
 *    \file  group.hpp
 *   \brief  组类型的实现
 *
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
#include <wx/dynarray.h>
#include <wx/list.h>
#include "utils.hpp"
#include "regex.hpp"
#include <vector>
#include "string.hpp"

namespace NiGroup
{
   /** 
     * \class NiGroup 
     * \brief 组数据类，基类
     */
    class NiGroup
    {
        protected:
            NiString::GroupString m_name;
        public:
            NiGroup ();
            ~NiGroup ();
            //! 获取组名
            wxString Name () const;
            //! 设置组名
            void SetName ( const wxString& name );
            //! 组名前缀
            wxString Prefix () const ;
            //! 组名前缀的hash值
            NiUtils::NiHASH PrefixHash () const;
            //! 组名的hash值
            NiUtils::NiHASH NameHash () const;
    };
    typedef std::vector <NiGroup> NiGroupArray;
    typedef NiGroup Group;
    typedef std::vector <Group> GroupList;

    /**
     * \class NiUnsubGroup  
     * \brief 未被订阅组类型
     */
    class NiUnsubGroup : public NiGroup
    {
        private:
            unsigned int m_last, m_first;
            bool m_post;
        public:
            NiUnsubGroup ( const wxString& name, 
                           const unsigned int& first, 
                           const unsigned int& last, 
                           const bool& post );
            NiUnsubGroup ();
            ~NiUnsubGroup ();
            bool IsValid () const;
            unsigned int Last () const;
            unsigned int First () const;
            bool Post () const;
            void Last ( const unsigned int& last );
            void First ( const unsigned int& first );
            void Post ( const bool& post );
            NiUnsubGroup& operator() ( const wxString& line );
    };
    typedef std::vector <NiUnsubGroup> NiUnsubGroupArray;
    typedef NiUnsubGroup UnsubGroup;
    typedef std::vector <UnsubGroup> UnsubGroupList;

    class SubGroup : public NiGroup
    {
        public:
            SubGroup ( const wxString& server, const wxString& name );
            SubGroup ();
            ~SubGroup ();
            wxString GetServer () const;
            void SetServer ( const wxString& server );
            bool operator== ( const SubGroup& t );
        private:
            wxString m_server;
    };
    typedef std::vector <SubGroup> SubGroupList;
    typedef std::vector <SubGroup*> SubGroupPtrList;
};
#endif //
