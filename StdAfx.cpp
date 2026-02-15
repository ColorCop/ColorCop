// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// This source file exists solely to build the project's precompiled header.
// MSVC requires exactly one .cpp file that includes the PCH header so it can
// compile and cache the expensive system and MFC headers. By isolating this
// work here, all other source files can compile faster and avoid repeatedly
// parsing the same large headers.

#include "stdafx.h"
