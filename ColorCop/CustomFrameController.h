// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// CustomFrameController.h - Implementation for custom non-client area handling

#pragma once
#include <afxwin.h>

class CustomFrameController {
     public:
    // Constructor takes a pointer to the owner window (the dialog)
    explicit CustomFrameController(CWnd* owner);
    // Sets the color to be displayed in the preview swatch in the title bar
    void SetPreviewColor(COLORREF c);
    // Updates the button rectangles based on the current window size
    void OnNcCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS* params);
    LRESULT OnNcHitTest(CPoint pt);
    void OnNcPaint();
    void OnNcLButtonDown(UINT hitTest, CPoint pt);
    void OnNcMouseMove(UINT hitTest, CPoint pt);
    void OnNcMouseLeave();

     private:
    CWnd* m_owner;

    // Layout
    int m_titleBarHeight = 32;
    CRect m_closeRect;
    CRect m_minRect;
    CRect m_swatchRect;

    // State
    bool m_hoverClose = false;
    bool m_hoverMin = false;

    // Color preview
    COLORREF m_previewColor = RGB(0, 0, 0);

    void UpdateButtonRects();
    void DrawTitleBar(CDC& dc);
    // bool HitCloseButton(CPoint pt) const;
    // bool HitMinButton(CPoint pt) const;
};
