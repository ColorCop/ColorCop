// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// AboutDlg.h : header file for the About dialog
//
#pragma once

#include "pch.h"
#include "Label.h"
#include "resource.h"

class CAboutDlg : public CDialog {
 public:
    CAboutDlg();

    // Dialog Data
    enum { IDD = IDD_ABOUTBOX };

 protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    CLabel m_maillink;
    CLabel m_link;

    DECLARE_MESSAGE_MAP()
};
