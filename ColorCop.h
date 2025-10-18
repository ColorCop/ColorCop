// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCop.h : main header file for the Color Cop application
//

#if !defined(AFX_HTMLCOP_H__EC2A34E4_4FAA_11D3_81A0_A79013DBA62A__INCLUDED_)
#define AFX_HTMLCOP_H__EC2A34E4_4FAA_11D3_81A0_A79013DBA62A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"        // main symbols
#include "ColorCopDlg.h"    // Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp:
// See ColorCop.cpp for the implementation of this class
//

class CColorCopApp : public CWinApp
{
public:
    CColorCopDlg dlg;
    CColorCopApp();
    ~CColorCopApp();


protected:
    void CColorCopApp::CloseApplication();
    void CColorCopApp::LoadDefaultSettings();
    CString CColorCopApp::GetTempFolder();
    BOOL CColorCopApp::GetShellFolderPath(char * pShellFolder, char * pShellPath);
    void CColorCopApp::ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags);

    // multi mon
    void CColorCopApp::ClipOrCenterRectToMonitor(LPRECT prc, UINT flags);

    bool InstanceRunning();
    HANDLE m_hMutex;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorCopApp)
    public:
    virtual BOOL InitInstance();
    virtual BOOL InitApplication();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CColorCopApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLCOP_H__EC2A34E4_4FAA_11D3_81A0_A79013DBA62A__INCLUDED_)
