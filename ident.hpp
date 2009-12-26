#ifndef __NEXT_NINEO_T_DEFINE__
#define __NEXT_NINEO_T_DEFINE__
/**
 *    \file  ident.hpp
 *   \brief  用户信息类
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
#include <wx/regex.h>
#include <wx/treebook.h>
#include <wx/listctrl.h>
#include <wx/arrimpl.cpp>
#include <wx/button.h>
#include <wx/sizer.h>
#include <vector>

namespace NiIdent
{
    /** 
     * \class NiIdent 
     * \brief 用户信息类，用来储存处理用户ID的数据类
     */
    class NiIdent
    {
        public:
            NiIdent ( const wxString& label = _("Default label") );
            ~NiIdent ();
            wxString Label () const;
            void Label ( const wxString& label = _("Default label"));
            bool operator () ( const wxString& server, const wxString& group );
            bool operator () ( const wxString& m );
            bool operator% ( const wxString& m );
            bool operator| ( const wxString& m );
            bool operator==  ( const wxString& m );
            NiIdent& operator= ( const NiIdent& id );

            void Match ( const wxString& match );
            void Regex ( const wxString& re );
            wxString Match () const;
            wxString Regex () const;
            bool IsMatch ( const wxString& m );
            bool IsRegex ( const wxString& re );
            void Name ( const wxString& name );
            void Email ( const wxString& email );
            void Signature ( const wxString& signature );

            wxString Name () const;
            wxString Email () const;
            wxString Signature () const;
        private:
            wxString m_name, m_email, m_signature, m_label;
            wxString m_regex, m_match;
            wxRegEx *m_re;
    };
    typedef NiIdent Ident;
    typedef std::vector <NiIdent> NiIdentArray;
    class NiIdentDialog : public wxDialog
    {
        public:
            NiIdentDialog ( wxWindow *parent, wxWindowID id, Ident& ident,
                            const wxString& title = wxT("Setup ident"),
                            long style = wxOK|wxCANCEL );
            ~NiIdentDialog ();
        private:
            enum {
                ID_USERNAME,
                ID_EMAIL,
                ID_SIGNATURE
            };
            void DoUI ( long style );
            void OnOk ( wxCommandEvent& event );
            void OnEntry ( wxCommandEvent& event );
            bool IsEmail ( const wxString& email );
            bool CheckInput ();
            Ident& m_ident;
            DECLARE_EVENT_TABLE ()
    };
    class NiIdentListCtrl : public wxListCtrl
    {
        public:
            NiIdentListCtrl ( wxWindow *parent, wxWindowID id, NiIdentArray& idents,
                            const wxString& name = wxT("NiIdentListCtrl"));
            ~NiIdentListCtrl ();
            virtual wxListItemAttr* OnGetItemAttr ( long item ) const;
            virtual int OnGetItemImage ( long item ) const;
            virtual int OnGetItemColumnImage ( long item, long column ) const;
            virtual wxString OnGetItemText (  long item, long column ) const;
        private:
            NiIdentArray &m_idents;
            DECLARE_EVENT_TABLE ()
    };

    /**
     * \class NiIdentManager 
     * \brief 用户信息管理类
     */
    class NiIdentManager : public wxDialog
    {
        public:
            NiIdentManager ( wxWindow* parent, wxWindowID id, 
                             const wxString& title = _("Ident manager"));
            ~NiIdentManager ();
        private:
            enum {ID_VIEW,ID_NEW,ID_DEL,ID_MODIF};
            NiIdentArray m_idents;
            DECLARE_EVENT_TABLE ()
    };
};
#endif //
