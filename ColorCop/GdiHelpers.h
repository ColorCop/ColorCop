// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once
//
// GdiHelpers.h
#include <windows.h>

class WindowDC {
 public:
    explicit WindowDC(HWND hwnd)
        : m_hwnd(hwnd), m_hdc(::GetDC(hwnd)) {}

    ~WindowDC() {
        if (m_hdc) {
            ::ReleaseDC(m_hwnd, m_hdc);
        }
    }

    operator HDC() const { return m_hdc; }

 private:
    HWND m_hwnd;
    HDC  m_hdc;
};
