// Copyright (c) 2026 Jay Prall
// SPDX-License-Identifier: MIT

// ColorTypes.h : header file for Color Cop color types
//
#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdint>

// Compatibility shim for non-Windows builds (e.g., macOS/Linux test targets)
// where <windows.h> and <wingdi.h> are unavailable. Mirrors the Win32
// 32-bit COLORREF type and RGB(r, g, b) macro using Windows' 0x00BBGGRR layout.
using COLORREF = std::uint32_t;

#ifndef RGB
constexpr COLORREF RGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept {
    return static_cast<COLORREF>(r) |
           (static_cast<COLORREF>(g) << 8) |
           (static_cast<COLORREF>(b) << 16);
}
#endif
#endif
