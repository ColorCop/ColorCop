// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// SamplingBounds.h
//
// Computes the pixel-sampling region used by ColorCop's averaged color
// sampling modes. The bounds are clamped to the virtual desktop so that
// sampling works correctly on multi-monitor configurations, including
// monitors positioned at negative coordinates.

#pragma once

#include <algorithm>

struct SampleBounds {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
};

// Compute the rectangular sampling area centered on (x, y).
//
// The returned bounds are clamped to the virtual desktop, preventing
// callers from sampling outside the valid screen area when the target
// point is near a screen edge or on a monitor that uses negative
// virtual-screen coordinates.
inline SampleBounds ComputeSampleBounds(
    int x,
    int y,
    int offset,
    int virtualLeft,
    int virtualTop,
    int virtualRight,
    int virtualBottom) {
    return {
        std::max(x - offset, virtualLeft),
        std::max(y - offset, virtualTop),
        std::min(x + offset, virtualRight),
        std::min(y + offset, virtualBottom)};
}
