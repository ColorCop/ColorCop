// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once

// Feature flags (bitmask values)
#define AlwaysOnTop            (1 << 2)
#define UpperCaseHex           (1 << 3)
#define OmitPound              (1 << 4)
#define SnaptoWebsafe          (1 << 5)
#define AutoCopytoClip         (1 << 6)
#define MimimizetoTray         (1 << 7)
#define EasyMove               (1 << 8)
#define MinimizeonStart        (1 << 9)
#define ExpandedDialog         (1 << 10)
#define ModeHTML               (1 << 11)
#define ModeDelphi             (1 << 12)
#define ModePowerBuilder       (1 << 13)
#define ModeVisualBasic        (1 << 14)
#define ModeVisualC            (1 << 15)
#define Sampling1              (1 << 16)
#define Sampling3x3            (1 << 17)
#define Sampling5x5            (1 << 18)
#define MultipleInstances      (1 << 19)
#define DetectWebsafeColors    (1 << 20)
#define RGBINT                 (1 << 21)
#define RGBFLOAT               (1 << 22)
#define MAGWHILEEYEDROP        (1 << 23)
#define USECROSSHAIR           (1 << 24)
#define SETCURSORONEYEDROP     (1 << 25)
#define MULTIPIXELSAMPLE       (1 << 26)
#define SamplingMULTI          (1 << 27)
#define SpaceRGB               (1 << 28)
#define SpaceCMYK              (1 << 29)
#define ModeClarion            (1 << 30)

#define MULTIPIX_MIN 1
#define MULTIPIX_MAX 15

#define PI (3.1415926535897932384626433832795)

// MFC includes
#include <afxwin.h>
#include <afxext.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

//{{AFX_INSERT_LOCATION}}
