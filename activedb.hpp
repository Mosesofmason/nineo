#ifndef __NEXT_NINEO_ACTIVE_DATABASE_DEFINE__
#define __NEXT_NINEO_ACTIVE_DATABASE_DEFINE__
/**
 *    \file  activedb.hpp
 *   \brief  负责处理USENET讨论组网络的讨论组列表，
 *
 *      管理讨论组列表，添加、添加更新处理、查询等功能。
 *
 *  \author  NewBlue (peng hong zhen), newblue@gmail.com
 *
 *  \internal
 *    Created:  2008-2-11
 *   Revision:  $Id: doxygen.templates,v 1.4 2008/02 mehner Exp $
 *   Compiler:  gcc/g++, borland commandline, msvc6
 *    Company:  NewBlue Studio.
 *  Copyright:  Copyright (c) 2008, NewBlue
 *
 * =====================================================================================
 */

#include <wx/wx.h>
#include <wx/hashmap.h>
#include <wx/regex.h>
#include <map>
#include <algorithm>
#include <wx/ptr_scpd.h>
#include <wx/hashmap.h>
#include <wx/wxsqlite3.h>
#include <wx/thread.h>

#include "config.hpp"
#include "group.hpp"
#include "safe.hpp"
#include "utils.hpp"
#include "regex.hpp"


namespace NiDB
{
    /**
     *  \class NiClass
     *  \brief NiClass 讨论组的所属分类的类型，例如：cn.fan所属分类为cn。
     */
    class NiClass 
    {
        public:

            /**
             *  \brief NiClass 构造函数
             *  \param  name 分类名
             *  \param  num 该分类的的组的数量
             */

            NiClass ( const wxString& name, const unsigned int& num );


            /**
             *  \brief NiClass 拷贝构造函数
             *  \param  NiClass 目标组分类
             */
            NiClass ( const NiClass& c );

            /**
             *  \brief NiClass 拷贝构造函数，根据组的信息提取分类信息。
             *  \param  group 目标组
             */
            NiClass ( const NiGroup::NiUnsubGroup& group );

            NiClass ();
            /**
             *  \brief 析构函数
             */
            ~NiClass ();
            /**
             *  \brief 设置该分类的组的数量
             *  \param  n 组的数量
             */
            void Num ( const unsigned int& n );
            /**
             *  \brief 获取该分类的组的数量
             *  \return 该分类组的数量 
             */
            unsigned int Num () const ;
            /**
             *  \brief 设置组的分类名称
             *  \param  name 名称
             */
            void Name ( const wxString& name );
            /**
             *  \brief 组的分类名称
             *  \return 分类名
             */
            wxString Name () const;
            /**
             *  \brief 该分类的hash值
             *  \return hash值
             */
            NiUtils::NiHASH Hash () const;
            /**
             *  \brief 从 group 提取组分类信息
             *  \param  group 组
             */
            NiClass& operator= ( const NiGroup::NiUnsubGroup& group );
        private:
            /** 该分类的组的数量 */
            unsigned int m_num;     
            /** 分类名 */
            wxString m_name;
            /** 组hash值 */
            NiUtils::NiHASH m_hash;
    };
    typedef std::map < NiUtils::NiHASH, NiClass* > ClassIndex;
    typedef std::map < NiUtils::NiHASH, NiGroup::NiUnsubGroup* > GroupIndex;
    typedef std::vector < NiClass > ClassList;


    /**
     *  \class NiActiveDB
     *  \brief 管理新闻组列表的类
     */
    class NiActiveDB : public Safe
    {
        public:
            wxMutex lock; // thread lock, use it in update data.
            NiActiveDB ();
            ~NiActiveDB ();

            /**
             *  \brief Open 打开组列表数据库
             *  \param filename 数据库文件名
             *  \return 无返回，但会有异常产生 
             */
            void Open ( const wxString& filename );

            /**
             *  \brief Close 关闭组列表数据库
             */
            void Close ();

            /**
             *  \brief BeginUpdate 开始更新新闻组列表的操作。
             *  \see EndUpdate
             *  \param  default_count 缓冲大小
             */
            void BeginUpdate ( const size_t default_count = 3000 );

            /**
             *  \brief EndUpdate 结束更新组列表，跟 BeginUpdate 配对操作函数，
             *  调用前需要调用 BeginUpdate 函数。该函数还会更新每个讨论组分类的数量。
             *  @see BeginUpdate
             */
            void EndUpdate ();

            /**
             *  \brief Add 添加讨论组列表项，该操作不会直接写入数据库，会缓冲，直到执行
             *   EndUpdate 操作。
             *   @see EndUpdate
             *   @see BeginUpdate
             *  \param  us_group 讨论组列表项
             *  \return 如果成功返回true，失败返回false
             */
            bool Add ( const NiGroup::NiUnsubGroup& us_group );
            unsigned int GroupCount ();
            bool HasGroup ( const wxString& gn );
            bool HasGroup ( const NiUtils::NiHASH& hash );
            bool HasClass ( const NiUtils::NiHASH& hash );
            bool ListClass ( ClassList& list );
            bool ListGroup ( const NiClass& oclass, NiGroup::UnsubGroupList& list );
            bool ListGroup ( const NiClass& oclass, NiGroup::UnsubGroupList& list,
                             const size_t& count = 1000 );
            size_t CountGroup ( const NiClass& oclass );
        private:
            void DoUpdate ();
            void UpdateClassTable ();
            bool LoadGroupIndex (); 
            bool LoadClassIndex ();
            void ClearClassIndex ();
            bool AddClass ( const NiClass& c );
            void ClearGroupIndex ();
            bool InitClassTable (); 
            bool InitGroupTable (); 

            void FormatGroupToSQL ( wxString& sql, 
                    const NiGroup::NiUnsubGroup& group ) const;
            void FormatClassToSQL ( wxString& sql,
                    const NiClass& C ) const;
            void FormatUpdateClassSQL ( wxString& sql, const NiClass& C ) const;
            wxSQLite3Database m_db; 
            GroupIndex m_groupindex;
            ClassIndex m_classindex;
            NiGroup::NiUnsubGroupArray* m_grouparray; 
    }; 
    typedef NiActiveDB ActiveDB;
    typedef std::map < NiUtils::HASH, ActiveDB* > ActiveDBPtrMap;
    class ActiveDBManager
    {
        private:
            static wxMutex m_lock;
            static ActiveDBPtrMap m_activedb; 
            ActiveDBManager ();
            ~ActiveDBManager ();
        public:
            static bool OpenManager ();
            static ActiveDB* GetDB ( const NiServer::Server& server );
            static bool CloseManager ();
    };
    
};
#endif //
