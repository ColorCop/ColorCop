// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once

#include "ColorTypes.h"


//
// ColorHistory.h

// Inserts a color at the front of the history list.
// The oldest history entry is discarded.
inline void PushColorHistory(COLORREF* history, COLORREF color) {
    // Duplicate current colors are ignored.
    if (history[0] == color)
        return;

    for (int i = 6; i > 0; --i) {
        history[i] = history[i - 1];
    }

    history[0] = color;
}
