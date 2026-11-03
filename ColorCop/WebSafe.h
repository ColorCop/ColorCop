// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once

constexpr int WEBSAFE_STEP = 51;

inline bool IsWebsafeColor(int r, int g, int b) {
    return (r % WEBSAFE_STEP == 0) &&
           (g % WEBSAFE_STEP == 0) &&
           (b % WEBSAFE_STEP == 0);
}
