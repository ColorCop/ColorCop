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

#define AlwaysOnTop            (1u << 2)
#define UpperCaseHex           (1u << 3)
#define OmitPound              (1u << 4)
#define SnaptoWebsafe          (1u << 5)
#define AutoCopytoClip         (1u << 6)
#define MimimizetoTray         (1u << 7)
#define EasyMove               (1u << 8)
#define MinimizeonStart        (1u << 9)
#define ExpandedDialog         (1u << 10)
#define ModeHTML               (1u << 11)
#define ModeDelphi             (1u << 12)
#define ModePowerBuilder       (1u << 13)
#define ModeVisualBasic        (1u << 14)
#define ModeVisualC            (1u << 15)
#define Sampling1              (1u << 16)
#define Sampling3x3            (1u << 17)
#define Sampling5x5            (1u << 18)
#define MultipleInstances      (1u << 19)
#define DetectWebsafeColors    (1u << 20)
#define RGBINT                 (1u << 21)
#define RGBFLOAT               (1u << 22)
#define MAGWHILEEYEDROP        (1u << 23)
#define USECROSSHAIR           (1u << 24)
#define SETCURSORONEYEDROP     (1u << 25)
#define MULTIPIXELSAMPLE       (1u << 26)
#define SamplingMULTI          (1u << 27)
#define SpaceRGB               (1u << 28)
#define SpaceCMYK              (1u << 29)
#define ModeClarion            (1u << 30)

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
