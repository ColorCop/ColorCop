// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#include "pch.h"
#include "DarkStatusBar.h"

IMPLEMENT_DYNAMIC(CDarkStatusBar, CStatusBar)

BEGIN_MESSAGE_MAP(CDarkStatusBar, CStatusBar)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CDarkStatusBar::SetTheme(bool dark) {
    if (dark) {
        m_clrBack = RGB(32, 32, 32);
        m_clrText = RGB(220, 220, 220);
    } else {
        m_clrBack = RGB(240, 240, 240);  // standard dialog background
        m_clrText = RGB(0, 0, 0);        // standard text
    }

    Invalidate();
}

void CDarkStatusBar::OnPaint() {
    CPaintDC dc(this);

    CRect rc;
    GetClientRect(&rc);

    // Fill dark background
    dc.FillSolidRect(rc, m_clrBack);

    // Draw each pane manually
    int nParts = GetCount();
    for (int i = 0; i < nParts; i++) {
        CRect part;
        GetItemRect(i, &part);

        CString text;
        GetPaneText(i, text);

        dc.SetTextColor(m_clrText);
        dc.SetBkColor(m_clrBack);
        dc.DrawText(text, part, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
}
