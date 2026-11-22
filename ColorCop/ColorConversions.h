// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cmath>

struct HSV {
    double h{};
    double s{};
    double v{};
};

struct CMYK {
    int c{};
    int m{};
    int y{};
    int k{};
};

constexpr double kRgbMaxD = 255.0;
constexpr int kCmykMax = 100;
constexpr double kHsvEpsilon = 1e-6;

inline HSV RGBToHSV(int red, int green, int blue) {
    const double rf = red / kRgbMaxD;
    const double gf = green / kRgbMaxD;
    const double bf = blue / kRgbMaxD;

    const double maxv = std::max({rf, gf, bf});
    const double minv = std::min({rf, gf, bf});
    const double delta = maxv - minv;

    HSV hsv;

    // Value (0–100)
    hsv.v = maxv * 100.0;

    // Saturation (0–100)
    hsv.s = (maxv == 0.0)
                ? 0.0
                : (delta / maxv) * 100.0;

    // Hue (0–360)
    if (delta <= kHsvEpsilon) {
        hsv.h = 0.0;
    } else if (maxv == rf) {
        hsv.h =
            60.0 * std::fmod(
                       ((gf - bf) / delta),
                       6.0);
    } else if (maxv == gf) {
        hsv.h =
            60.0 * (((bf - rf) / delta) + 2.0);
    } else {
        hsv.h =
            60.0 * (((rf - gf) / delta) + 4.0);
    }

    if (hsv.h < 0.0) {
        hsv.h += 360.0;
    }

    return hsv;
}

inline CMYK RGBToCMYK(int red, int green, int blue) {
    const double rNorm =
        static_cast<double>(red) / kRgbMaxD;

    const double gNorm =
        static_cast<double>(green) / kRgbMaxD;

    const double bNorm =
        static_cast<double>(blue) / kRgbMaxD;

    const double maxRgb =
        std::max({rNorm, gNorm, bNorm});

    const double K = 1.0 - maxRgb;

    double C = 0.0;
    double M = 0.0;
    double Y = 0.0;

    if (K < 1.0) {
        const double denom = 1.0 - K;

        C = (1.0 - rNorm - K) / denom;
        M = (1.0 - gNorm - K) / denom;
        Y = (1.0 - bNorm - K) / denom;
    }

    return {
        static_cast<int>(std::round(C * kCmykMax)),
        static_cast<int>(std::round(M * kCmykMax)),
        static_cast<int>(std::round(Y * kCmykMax)),
        static_cast<int>(std::round(K * kCmykMax))
    };
}
