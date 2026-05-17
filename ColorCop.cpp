// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCop.cpp : Application entry point and core initialization logic.
// Handles single‑instance enforcement, portable‑mode detection, settings
// loading/saving, and creation of the main ColorCop dialog.

#include "pch.h"
#include "ColorCop.h"
#include "ColorCopDlg.h"
#include "Defaults.h"

#define INI_FILE _T("\\Color_Cop.dat")
#define INI_FILE_DIR _T("\\ColorCop")

#define MONITOR_CENTER 0x0001    // center rect to monitor
#define MONITOR_CLIP 0x0000      // clip rect to monitor
#define MONITOR_WORKAREA 0x0002  // use monitor work area
#define MONITOR_AREA 0x0000      // use monitor entire area

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp

BEGIN_MESSAGE_MAP(CColorCopApp, CWinApp)
    //{{AFX_MSG_MAP(CColorCopApp) // NOLINT(whitespace/comments)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG // NOLINT(whitespace/comments)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp construction


CColorCopApp::CColorCopApp() {
    m_hMutex = nullptr;
}

CColorCopApp::~CColorCopApp() {
    if (m_hMutex != nullptr) {
        CloseHandle(m_hMutex);
        m_hMutex = nullptr;
    }
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CColorCopApp object

CColorCopApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp initialization

BOOL CColorCopApp::InitInstance() {
    // multiple instances are not allowed?
    if (!(dlg.m_Appflags & MultipleInstances)) {
        // multiple instances are not allowed. check if we have one running
        if (InstanceRunning()) {
            // TODO(j4y): find the current instance and bring forward instead of a msg.  fixes issue #4
            AfxMessageBox(IDS_APP_RUNNING);

            // error instead
            return false;
        }
    }
    // set the main window
    dlg.m_PortableMode = m_PortableMode;
    m_pMainWnd = &dlg;

    int nResponse = dlg.DoModal();  // Launch the color cop dialog

    if ((nResponse == IDOK) || (nResponse == IDCANCEL)) {
        CloseApplication();  // write the data to a file
    }

    return FALSE;
}

// uses a Mutex to figure out if there is an instance of color cop running
bool CColorCopApp::InstanceRunning() {
    m_hMutex = CreateMutex(NULL, TRUE, _T("ColorCop_Mutex"));

    if (m_hMutex) {
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            CloseHandle(m_hMutex);  // Close the duplicate handle
            m_hMutex = nullptr;
            return true;
        }
        return false;
    }

    return false;
}

BOOL CColorCopApp::GetShellFolderPath(LPCTSTR pShellFolder, LPTSTR pShellPath) {
    // pShellFolder can be one of the following
    // AppData, Cache, Cookies, Desktop, Favorites, Fonts, History, NetHood,
    // Personal, Printhood, Programs, Recent, SendTo, Start Menu, Startup,
    // Templates, ShellNew
    DWORD rc;
    DWORD length = MAX_PATH;
    DWORD type = REG_SZ;
    HKEY hkey;

    rc = RegOpenKeyEx(HKEY_CURRENT_USER,
                      _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"),
                      0, KEY_READ, &hkey);

    if (rc == ERROR_SUCCESS) {
        rc = RegQueryValueEx(hkey, pShellFolder, NULL, &type,
                             reinterpret_cast<BYTE*>(pShellPath), &length);
        RegCloseKey(hkey);
    }

    if (rc == ERROR_SUCCESS) {
        return TRUE;
    } else {
        return FALSE;
    }
}

CString CColorCopApp::GetSettingsFolder() {
    CString path;
    if (m_PortableMode) {
        // Portable mode: store settings next to the EXE
        TCHAR exePath[MAX_PATH] = {0};
        GetModuleFileName(NULL, exePath, MAX_PATH);
        PathRemoveFileSpec(exePath);
        path = exePath;
        return path;  // e.g. C:\Tools\ColorCop
    }

    // Installed mode: use LocalAppData\ColorCop
    TCHAR buffer[MAX_PATH] = {0};
    GetShellFolderPath(_T("Local AppData"), buffer);
    path = buffer;

    path += INI_FILE_DIR;  // "\ColorCop"
    return path;           // e.g. C:\Users\...\AppData\Local\ColorCop
}

void CColorCopApp::ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags) {
    RECT rc;
    GetWindowRect(hwnd, &rc);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    dlg.WinLocX = rc.left;
    dlg.WinLocY = rc.top;
}

BOOL CColorCopApp::InitApplication() {
    // Detect portable‑mode command‑line switches (restores pre‑5.3 behavior)
    CString cmd = GetCommandLine();
    cmd.MakeLower();

    if (cmd.Find(_T("-portable")) != -1 || cmd.Find(_T("--portable")) != -1 ||
        cmd.Find(_T("/portable")) != -1 || cmd.Find(_T("/p")) != -1) {
        m_PortableMode = true;
    }
    CString settingsFolder = GetSettingsFolder();
    CString strInitFile = settingsFolder + INI_FILE;  // "\Color_Cop.dat"

    TRACE(_T("Portable mode: %d\n"), m_PortableMode);
    TRACE(_T("INI path: %s\n"), strInitFile);

    CFile file;
    if (file.Open(strInitFile, CFile::modeRead)) {
        CArchive ar(&file, CArchive::load);
        Serialize(ar);
    } else {
        // First time running ColorCop
        if (!m_PortableMode) {
            // Ensure the settings folder exists
            CreateDirectory(settingsFolder, NULL);
        }
        LoadDefaultSettings();

        ClipOrCenterWindowToMonitor(::GetForegroundWindow(), MONITOR_CENTER);
    }

    return CWinApp::InitApplication();
}

void CColorCopApp::LoadDefaultSettings() {
    // Called when no valid .DAT file exists. Load all default settings.

    // Seed color
    const COLORREF seed = kDefaultSeedColor;
    dlg.m_Reddec = GetRValue(seed);
    dlg.m_Greendec = GetGValue(seed);
    dlg.m_Bluedec = GetBValue(seed);

    // Default app flags
    dlg.m_Appflags = AlwaysOnTop | AutoCopytoClip | ModeHTML | EasyMove |
                     Sampling1 | ExpandedDialog | MultipleInstances |
                     MAGWHILEEYEDROP | SpaceRGB;

    // Window defaults
    dlg.WinLocX = kDefaultWinLocX;
    dlg.WinLocY = kDefaultWinLocY;
    dlg.m_iSamplingOffset = kDefaultSamplingOffset;

    // Custom colors
    for (int i = 0; i < kCustomColorCount; ++i)
        dlg.CustColorBank[i] = kDefaultCustomColor;

    // History colors
    for (int i = 0; i < kHistoryCount; ++i)
        dlg.ColorHistory[i] = kDefaultColorHistory[i];
}

void CColorCopApp::CloseApplication() {
    CString settingsFolder = GetSettingsFolder();
    CString strInitFile = settingsFolder + INI_FILE;

    TRACE(_T("Portable mode: %d\n"), m_PortableMode);
    TRACE(_T("INI path: %s\n"), strInitFile);

    if (!m_PortableMode) {
        // Ensure folder exists in installed mode
        CreateDirectory(settingsFolder, NULL);
    }

    CFile file;
    if (file.Open(strInitFile, CFile::modeWrite | CFile::modeCreate)) {
        CArchive ar(&file, CArchive::store);
        Serialize(ar);
    }
}

void CColorCopApp::Serialize(CArchive& ar) {
    if (ar.IsStoring()) {
        // storing code
        try {
            ar << dlg.m_Reddec;
            ar << dlg.m_Greendec;
            ar << dlg.m_Bluedec;
            ar << dlg.m_Appflags;
            ar << dlg.WinLocX;
            ar << dlg.WinLocY;
            for (int j = 0; j < kCustomColorCount; j++)  // load custom color values to array
                ar << dlg.CustColorBank[j];
            for (int w = 0; w < kHistoryCount; w++)  // save color history values
                ar << dlg.ColorHistory[w];

            ar << dlg.m_iSamplingOffset;
        } catch (CArchiveException& e) {
            TRACE("Archive exception caught. Cause = %d, File = %s\n", e.m_cause,
                  e.m_strFileName ? e.m_strFileName : "(unknown)");
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
            // load custom color values to array
            for (int j = 0; j < kCustomColorCount; j++) {
                ar >> dlg.CustColorBank[j];
            }
            // load color history values to array
            for (int w = 0; w < kHistoryCount; w++) {
                ar >> dlg.ColorHistory[w];
            }
            ar >> dlg.m_iSamplingOffset;
        } catch (CArchiveException& e) {
            TRACE("Archive exception caught. Cause = %d, File = %s\n", e.m_cause,
                  e.m_strFileName ? e.m_strFileName : "(unknown)");
            AfxMessageBox(IDS_ERROR_LOADING);
        }
    }
}
