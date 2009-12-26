#ifndef __NEXT_NINEO_STRING_DEFINE__
#define __NEXT_NINEO_STRING_DEFINE__
/**
 *    \file  string.hpp
 *   \brief  增强wxString的功能
 *
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-15
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++,borland free commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */

#include "utils.hpp"
#include <wx/wx.h>

namespace NiString
{
    wxString inline MBConvLocalString ( const wxString& o )
    {
        return wxString::From8BitData ( o.mb_str ( wxConvLocal ) );
    }
    //!  \class HashString 
    //!  \brief 提供wxString的Hash函数
    class HashString : public wxString
    {
        public:
            HashString ( const wxString& o );
            HashString ();
            ~HashString ();

            //!  取得字符串的hash值
            //!  \return hash值
            NiUtils::NiHASH Hash () const;
    };

    //! \class GroupString
    //!  \brief 提供组名前缀以及组名前缀的hash值。
    class GroupString : public HashString
    {
        public:
            GroupString ( const wxString& o );
            GroupString ();
            ~GroupString ();

            //! 获取组名前缀的hash函数 
            //! \return 组名前缀的hash值
            NiUtils::NiHASH PrefixHash () const;

            //! 获取组名的前缀，例如cn.fan的前缀是cn
            //! \return 组名前缀
            wxString Prefix () const;
    };
};
#endif //


