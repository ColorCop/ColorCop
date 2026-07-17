// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCop.h : main header file for the Color Cop application
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'pch.h' before including this file for PCH
#endif

#include <filesystem>

#include "resource.h"     // main symbols
#include "ColorCopDlg.h"  // Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp:
// See ColorCop.cpp for the implementation of this class
//
class CColorCopApp : public CWinApp {
     public:
    bool m_PortableMode = false;  // set via command-line switches

    CColorCopDlg dlg;
    CColorCopApp();
    ~CColorCopApp();
    /* Returns the filesystem path used to store the application's persistent
     * state, including ColorCop.dat and other configuration files. In portable
     * mode, this is the directory containing the executable. In installed mode,
     * it is the user's LocalAppData\ColorCop folder. The returned value is a
     * std::filesystem::path, allowing safe and Unicode‑aware path handling.
     */
    std::filesystem::path GetSettingsFolder();

     protected:
    void CloseApplication();
    void LoadDefaultSettings();
    BOOL GetShellFolderPath(LPCTSTR pShellFolder, LPTSTR pShellPath);
    void ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags);
    void ClipOrCenterRectToMonitor(LPRECT prc, UINT flags);
    bool InstanceRunning();
    HANDLE m_hMutex;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorCopApp) // NOLINT(whitespace/comments)
     public:
    virtual BOOL InitInstance();
    bool BringExistingInstanceToFront();
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
