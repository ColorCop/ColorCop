// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// CustomFrameController.h - Implementation for custom non-client area handling

#include "pch.h"
#include "CustomFrameController.h"

CustomFrameController::CustomFrameController(CWnd* owner)
    : m_owner(owner) {
}

void CustomFrameController::UpdateButtonRects() {
    CRect wnd;
    m_owner->GetWindowRect(&wnd);
    int width = wnd.Width();

    int btnSize = 32;
    m_closeRect = CRect(width - btnSize, 0, width, m_titleBarHeight);
    m_minRect   = CRect(width - btnSize*2, 0, width - btnSize, m_titleBarHeight);
    m_swatchRect = CRect(width - btnSize*3, 0, width - btnSize*2, m_titleBarHeight);
}

void CustomFrameController::OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*) {
    // Full client area
}

LRESULT CustomFrameController::OnNcHitTest(CPoint pt) {
    CRect wnd;
    m_owner->GetWindowRect(&wnd);

    CPoint clientPt = pt;
    clientPt.Offset(-wnd.left, -wnd.top);

    UpdateButtonRects();

    if (m_closeRect.PtInRect(clientPt))
        return HTCLOSE;

    if (m_minRect.PtInRect(clientPt))
        return HTMINBUTTON;

    if (clientPt.y < m_titleBarHeight)
        return HTCAPTION;

    return HTCLIENT;
}

void CustomFrameController::OnNcPaint() {
    CWindowDC dc(m_owner);
    DrawTitleBar(dc);
}

void CustomFrameController::DrawTitleBar(CDC& dc) {
    CRect wnd;
    m_owner->GetWindowRect(&wnd);
    int width = wnd.Width();

    UpdateButtonRects();

    // Background
    dc.FillSolidRect(0, 0, width, m_titleBarHeight, RGB(32, 32, 32));

    // Title
    CString title;
    m_owner->GetWindowText(title);

    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(240, 240, 240));

    CRect textRect(8, 0, width - 112, m_titleBarHeight);
    dc.DrawText(title, textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Buttons
    auto drawBtn = [&](CRect r, bool hover, COLORREF base) {
        dc.FillSolidRect(r, hover ? RGB(64, 64, 64) : base);
        dc.DrawEdge(r, EDGE_RAISED, BF_RECT);
    };

    drawBtn(m_minRect, m_hoverMin, RGB(48, 48, 48));
    drawBtn(m_closeRect, m_hoverClose, RGB(48, 48, 48));

    // Glyphs
    dc.SetTextColor(RGB(220, 220, 220));
    dc.DrawText(_T("—"), m_minRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    dc.DrawText(_T("X"), m_closeRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // color preview
    dc.FillSolidRect(m_swatchRect, m_previewColor);
    dc.DrawEdge(m_swatchRect, EDGE_SUNKEN, BF_RECT);
}

void CustomFrameController::OnNcLButtonDown(UINT hitTest, CPoint) {
    if (hitTest == HTCLOSE)
        m_owner->SendMessage(WM_CLOSE);

    if (hitTest == HTMINBUTTON)
        m_owner->ShowWindow(SW_MINIMIZE);
}

void CustomFrameController::OnNcMouseMove(UINT hitTest, CPoint) {
    bool newHoverClose = (hitTest == HTCLOSE);
    bool newHoverMin   = (hitTest == HTMINBUTTON);

    if (newHoverClose != m_hoverClose || newHoverMin != m_hoverMin) {
        m_hoverClose = newHoverClose;
        m_hoverMin   = newHoverMin;
        m_owner->SendMessage(WM_NCPAINT);
    }

    TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
    tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
    tme.hwndTrack = m_owner->GetSafeHwnd();
    TrackMouseEvent(&tme);
}

void CustomFrameController::OnNcMouseLeave() {
    m_hoverClose = false;
    m_hoverMin = false;
    m_owner->SendMessage(WM_NCPAINT);
}

void CustomFrameController::SetPreviewColor(COLORREF c) {
    m_previewColor = c;
    m_owner->SendMessage(WM_NCPAINT);
}
