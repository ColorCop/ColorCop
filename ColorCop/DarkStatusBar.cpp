// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#include "pch.h"
#include "DarkStatusBar.h"

IMPLEMENT_DYNAMIC(CDarkStatusBar, CStatusBar)

BEGIN_MESSAGE_MAP(CDarkStatusBar, CStatusBar)
    ON_WM_PAINT()
END_MESSAGE_MAP()

//
// CDarkStatusBar
//
// Lightweight owner-drawn status bar used by ColorCop to support dark mode.
//
// The standard MFC status bar does not automatically adapt its background
// and text colors for custom application themes, so this control paints its
// own background and pane text using theme-provided colors.
//
// Text is rendered using the status bar's assigned font rather than the
// default memory-DC font. This preserves visual consistency with the rest
// of the UI while remaining defensive against invalid or unavailable font
// handles during initialization.
//
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

    // Paint the entire status bar background using the active theme color.
    dc.FillSolidRect(rc, m_clrBack);

    // Draw each pane individually so text colors remain consistent with the
    // selected light or dark theme.
    int nParts = GetCount();
    for (int i = 0; i < nParts; i++) {
        CRect part;
        GetItemRect(i, &part);

        CString text;
        GetPaneText(i, text);

        dc.SetTextColor(m_clrText);
        dc.SetBkColor(m_clrBack);

        // Draw using the status bar's font rather than the default font
        // selected into the paint DC. Validate the font before selecting it
        // because status bar initialization order can vary during startup.
        CFont* pOldFont = nullptr;

        if (CFont* pFont = GetFont();
            pFont != nullptr &&
            pFont->GetSafeHandle() != nullptr) {
            pOldFont = dc.SelectObject(pFont);
        }

        dc.DrawText(
            text,
            part,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE |
                DT_END_ELLIPSIS);

        // Restore the original DC font before continuing.
        if (pOldFont != nullptr) {
            dc.SelectObject(pOldFont);
        }
    }
}
