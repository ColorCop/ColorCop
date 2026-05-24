// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once

class CDarkStatusBar : public CStatusBar {
 public:
    COLORREF m_clrBack = RGB(32, 32, 32);
    COLORREF m_clrText = RGB(220, 220, 220);

    void SetTheme(bool dark);


    DECLARE_DYNAMIC(CDarkStatusBar)



protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
