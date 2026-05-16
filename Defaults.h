// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// Defaults.h : Centralized constants for array sizes and other shared defaults.
//

#pragma once

constexpr int kCustomColorCount = 16;
constexpr int kHistoryCount     = 7;

constexpr int kDefaultWinLocX = 200;
constexpr int kDefaultWinLocY = 200;
constexpr int kDefaultSamplingOffset = 3;

// Default color history (COLORREF format: 0x00BBGGRR)
constexpr COLORREF kDefaultColorHistory[kHistoryCount] = {
    0x00223300,
    0x000000FF,
    0x0000FF99,
    0x009999FF,
    0x00FF9900,
    0x00FFFF99,
    0x00999900
};

constexpr COLORREF kDefaultSeedColor = RGB(171, 208, 188);

constexpr COLORREF kDefaultCustomColor = 0x00FFFFFF;
