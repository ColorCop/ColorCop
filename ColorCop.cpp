// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCop.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ColorCop.h"
#include "ColorCopDlg.h"
#include "Defaults.h"

#define INI_FILE _T("\\Color_Cop.dat")
#define INI_FILE_DIR _T("\\ColorCop")

#define MONITOR_CENTER   0x0001        // center rect to monitor
#define MONITOR_CLIP     0x0000        // clip rect to monitor
#define MONITOR_WORKAREA 0x0002        // use monitor work area
#define MONITOR_AREA     0x0000        // use monitor entire area

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp

BEGIN_MESSAGE_MAP(CColorCopApp, CWinApp)
    //{{AFX_MSG_MAP(CColorCopApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp construction


CColorCopApp::CColorCopApp()
{
    m_hMutex = nullptr;
}

CColorCopApp::~CColorCopApp()
{
    if (m_hMutex != nullptr)
    {
        CloseHandle(m_hMutex);
    }
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CColorCopApp object

CColorCopApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp initialization

BOOL CColorCopApp::InitInstance()
{
    // multiple instances are not allowed?
    if (!(dlg.m_Appflags & MultipleInstances))
    {
        // multiple instances are not allowed. check if we have one running
            if(InstanceRunning())
            {
                // TODO(j4y): find the current instance and bring forward instead of a msg.  fixes issue #4
                AfxMessageBox(IDS_APP_RUNNING);

                // error instead
                return false;
            }
    }

        // set the main window
        m_pMainWnd = &dlg;

        int nResponse = dlg.DoModal();        // Launch the color cop dialog

        if ((nResponse == IDOK)||(nResponse == IDCANCEL)) {

            CloseApplication();        // write the data to a file
        }

    return FALSE;
}

// uses a Mutex to figure out if there is an instance of color cop running
bool CColorCopApp::InstanceRunning()
{
    m_hMutex = CreateMutex(NULL, true, _T("ColorCop_Mutex"));

    if (m_hMutex)
    {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            CloseHandle(m_hMutex);  // Close the duplicate handle
            return true;
        }
        return false;
    }

    return false;
}

BOOL CColorCopApp::GetShellFolderPath(char* pShellFolder, char* pShellPath)
{
    // pShellFolder can be one of the following
    // AppData, Cache, Cookies, Desktop, Favorites, Fonts, History, NetHood,
    // Personal, Printhood, Programs, Recent, SendTo, Start Menu, Startup,
    // Templates, ShellNew
    DWORD rc;
    DWORD length = MAX_PATH;
    DWORD type = REG_SZ;
    HKEY hkey;

    rc = RegOpenKeyEx(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
        0, KEY_READ, &hkey);

    if (rc == ERROR_SUCCESS){
        rc = RegQueryValueEx(hkey, pShellFolder, NULL, &type,
                (BYTE *) pShellPath, &length);
        RegCloseKey(hkey);
    }

    if (rc == ERROR_SUCCESS) {
        return TRUE;
    } else {
        return FALSE;
    }
}

CString CColorCopApp::GetTempFolder()
{
    CString strTmpPath;

    GetShellFolderPath("AppData", strTmpPath.GetBuffer(MAX_PATH));
    strTmpPath.ReleaseBuffer();

    return strTmpPath;
}

void CColorCopApp::ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags)
{
    RECT rc;
    GetWindowRect(hwnd, &rc);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    dlg.WinLocX= rc.left;
    dlg.WinLocY= rc.top;
}

BOOL CColorCopApp::InitApplication()
{
    CString strInitFile = GetTempFolder();

    strInitFile += INI_FILE_DIR;

    strInitFile += INI_FILE;

    CFile file;
    if(file.Open(strInitFile, CFile::modeRead)) {
        CArchive ar(&file, CArchive::load);
        Serialize(ar);
    } else {
        // First time we are running color Cop
        CString strAppDirectory = GetTempFolder();
        strAppDirectory += INI_FILE_DIR;
        CreateDirectory(strAppDirectory.GetBuffer(MAX_PATH), NULL);
        strAppDirectory.ReleaseBuffer();

        LoadDefaultSettings();

        ClipOrCenterWindowToMonitor(::GetForegroundWindow(), MONITOR_CENTER);;
        // set the window to be in the middle
    }

    return CWinApp::InitApplication();
}

void CColorCopApp::LoadDefaultSettings() {

    // This function is called when we can't find a .DAT file
    // with the persistent variables, or it was an old dat file.
    // Set the default settings and custom colors.

    dlg.m_Reddec   = GetRValue(kDefaultSeedColor);
    dlg.m_Greendec = GetGValue(kDefaultSeedColor);
    dlg.m_Bluedec  = GetBValue(kDefaultSeedColor);

    dlg.m_Appflags = 0;
    dlg.m_Appflags |= AlwaysOnTop;
    dlg.m_Appflags |= AutoCopytoClip;
    dlg.m_Appflags |= ModeHTML;
    dlg.m_Appflags |= EasyMove;
    dlg.m_Appflags |= Sampling1;
    dlg.m_Appflags |= ExpandedDialog;
    dlg.m_Appflags |= MultipleInstances;
    dlg.m_Appflags |= MAGWHILEEYEDROP;
    dlg.m_Appflags |= SpaceRGB;

    dlg.WinLocX = kDefaultWinLocX;
    dlg.WinLocY = kDefaultWinLocY;

    dlg.m_iSamplingOffset = kDefaultSamplingOffset;

    // Set all custom color blocks to the default custom color
    for (int i = 0; i < kCustomColorCount; ++i) {
        dlg.CustColorBank[i] = kDefaultCustomColor;
    }

    for (int i = 0; i < kHistoryCount; ++i) {
        dlg.ColorHistory[i] = kDefaultColorHistory[i];
    }
}

void CColorCopApp::CloseApplication() {
    ////////////////////////////////////////////////////////////
    // This function writes the settings to a file.  It is the
    // last thing the application will do. It will only write to
    // a file when the dialog has been closed IDOK or IDCANCEL


    CString strInitFile = GetTempFolder();

    strInitFile += INI_FILE_DIR;

    strInitFile += INI_FILE;

    CFile file;
    if(file.Open(strInitFile,CFile::modeWrite|CFile::modeCreate))
    {
        CArchive ar(&file, CArchive::store);
        Serialize(ar);
    }

    ReleaseMutex(m_hMutex);

    return;
}

void CColorCopApp::Serialize(CArchive& ar)
{
    if (ar.IsStoring())    {
        // storing code
        try {
            ar << dlg.m_Reddec;
            ar << dlg.m_Greendec;
            ar << dlg.m_Bluedec;
            ar << dlg.m_Appflags;
            ar << dlg.WinLocX;
            ar << dlg.WinLocY;
            for (int j = 0; j < kCustomColorCount; j++)        //load custom color values to array
                ar << dlg.CustColorBank[j];
            for(int w = 0; w < kHistoryCount; w++)        // save color history values
                ar << dlg.ColorHistory[w];

            ar <<  dlg.m_iSamplingOffset;

        } catch (CArchiveException*) {
            AfxMessageBox(IDS_ERROR_SAVING);
        }
    } else {
        // loading code
        try {
            ar >> dlg.m_Reddec;
            ar >> dlg.m_Greendec;
            ar >> dlg.m_Bluedec;
            ar >> dlg.m_Appflags;
            ar >> dlg.WinLocX;
            ar >> dlg.WinLocY;
            for (int j = 0; j < kCustomColorCount; j++)        //load custom color values to array
                ar >> dlg.CustColorBank[j];
            for(int w = 0; w < kHistoryCount; w++)        // save color history values
                ar >> dlg.ColorHistory[w];

            ar >>  dlg.m_iSamplingOffset;

        } catch (CArchiveException*) {
            AfxMessageBox(IDS_ERROR_LOADING);
        }
    }
}
