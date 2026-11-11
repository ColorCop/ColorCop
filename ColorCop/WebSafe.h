// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once

constexpr int WEBSAFE_STEP = 51;

inline bool IsWebsafeColor(int r, int g, int b) {
    return (r % WEBSAFE_STEP == 0) &&
           (g % WEBSAFE_STEP == 0) &&
           (b % WEBSAFE_STEP == 0);
}

// Round an RGB channel value to the nearest web-safe value.
// Web-safe values are multiples of 51:
// {0, 51, 102, 153, 204, 255}.
inline int DecimalToWebsafe(int value) {
    const int offset = value % WEBSAFE_STEP;

    if (offset == 0) {
        return value;
    }

    return (offset < WEBSAFE_STEP / 2)
               ? (value - offset)
               : (value + (WEBSAFE_STEP - offset));
}
