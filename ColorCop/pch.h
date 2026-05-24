// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once

//------------------------------------------------------------------------------
// Platform / Windows configuration
//------------------------------------------------------------------------------

#define NOMINMAX              // Prevent Windows macros from breaking std::min/std::max
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used Windows headers

//------------------------------------------------------------------------------
// Standard library
//------------------------------------------------------------------------------

#include <cstdint>
#include <cmath>

//------------------------------------------------------------------------------
// Application feature flags (bitmask)
//------------------------------------------------------------------------------
//
// NOTE:
// These are intentionally kept as macros for minimal churn.
// A future modernization step would be a strongly-typed enum class.
//

#define AlwaysOnTop (1ull << 2)
#define UpperCaseHex (1ull << 3)
#define OmitPound (1ull << 4)
#define SnaptoWebsafe (1ull << 5)
#define AutoCopytoClip (1ull << 6)
#define MinimizetoTray (1ull << 7)
#define EasyMove (1ull << 8)
#define MinimizeOnStart (1ull << 9)
#define ExpandedDialog (1ull << 10)
#define ModeHTML (1ull << 11)
#define ModeDelphi (1ull << 12)
#define ModePowerBuilder (1ull << 13)
#define ModeVisualBasic (1ull << 14)
#define ModeVisualC (1ull << 15)
#define Sampling1 (1ull << 16)
#define Sampling3x3 (1ull << 17)
#define Sampling5x5 (1ull << 18)
#define MultipleInstances (1ull << 19)
#define DetectWebsafeColors (1ull << 20)
#define RGBINT (1ull << 21)
#define RGBFLOAT (1ull << 22)
#define MAGWHILEEYEDROP (1ull << 23)
#define USECROSSHAIR (1ull << 24)
#define SETCURSORONEYEDROP (1ull << 25)
#define MULTIPIXELSAMPLE (1ull << 26)
#define SamplingMULTI (1ull << 27)
#define SpaceRGB (1ull << 28)
#define SpaceCMYK (1ull << 29)
#define ModeClarion (1ull << 30)
#define DarkMode (1ull << 31)
#define ForceLight (1ull << 32)


//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

constexpr int MULTIPIX_MIN = 1;
constexpr int MULTIPIX_MAX = 15;

// Prefer constexpr over macros for typed constants
constexpr double kPi = 3.14159265358979323846;

//------------------------------------------------------------------------------
// MFC headers
//------------------------------------------------------------------------------

#include <afxwin.h>    // Core MFC components
#include <afxext.h>    // Extensions (controls, OLE)
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>    // Common controls
#endif

//{{AFX_INSERT_LOCATION}} // NOLINT(whitespace/comments)
