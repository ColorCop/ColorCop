// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// AboutDlg.h : implementation file for the About dialog
//
#include "pch.h"
#include "AboutDlg.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    // no message handlers
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MAILLINK, m_maillink);
    DDX_Control(pDX, IDC_LINK, m_link);
}

BOOL CAboutDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    m_link.SetLink(TRUE)
        .SetTextColor(RGB(0, 0, 255))
        .SetFontUnderline(TRUE)
        .SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDPOINTER));

    m_maillink.SetLink(TRUE)
        .SetTextColor(RGB(0, 0, 255))
        .SetFontUnderline(TRUE)
        .SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDPOINTER));

    return TRUE;
}
