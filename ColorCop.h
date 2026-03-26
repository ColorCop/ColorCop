// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCop.h : main header file for the Color Cop application
//

#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"        // main symbols
#include "ColorCopDlg.h"    // Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp:
// See ColorCop.cpp for the implementation of this class
//
class CColorCopApp : public CWinApp {
 public:
    CColorCopDlg dlg;
    CColorCopApp();
    ~CColorCopApp();

 protected:
    void CloseApplication();
    void LoadDefaultSettings();
    CString GetTempFolder();
    BOOL GetShellFolderPath(char * pShellFolder, char * pShellPath);
    void ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags);

    // multi mon
    void ClipOrCenterRectToMonitor(LPRECT prc, UINT flags);

    bool InstanceRunning();
    HANDLE m_hMutex;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorCopApp) // NOLINT(whitespace/comments)
 public:
    virtual BOOL InitInstance();
    virtual BOOL InitApplication();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL // NOLINT(whitespace/comments)

// Implementation

    //{{AFX_MSG(CColorCopApp) // NOLINT(whitespace/comments)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG // NOLINT(whitespace/comments)
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}} // NOLINT(whitespace/comments)
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
