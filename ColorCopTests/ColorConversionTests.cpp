// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ColorConversions.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("RGBToHSV converts primary colors") {
    SECTION("red") {
        const auto hsv = RGBToHSV(255, 0, 0);

        REQUIRE_THAT(hsv.h, WithinAbs(0.0, 0.001));
        REQUIRE_THAT(hsv.s, WithinAbs(100.0, 0.001));
        REQUIRE_THAT(hsv.v, WithinAbs(100.0, 0.001));
    }

    SECTION("green") {
        const auto hsv = RGBToHSV(0, 255, 0);

        REQUIRE_THAT(hsv.h, WithinAbs(120.0, 0.001));
        REQUIRE_THAT(hsv.s, WithinAbs(100.0, 0.001));
        REQUIRE_THAT(hsv.v, WithinAbs(100.0, 0.001));
    }

    SECTION("blue") {
        const auto hsv = RGBToHSV(0, 0, 255);

        REQUIRE_THAT(hsv.h, WithinAbs(240.0, 0.001));
        REQUIRE_THAT(hsv.s, WithinAbs(100.0, 0.001));
        REQUIRE_THAT(hsv.v, WithinAbs(100.0, 0.001));
    }
}

TEST_CASE("RGBToHSV converts grayscale colors") {
    const auto hsv = RGBToHSV(128, 128, 128);

    REQUIRE_THAT(hsv.h, WithinAbs(0.0, 0.001));
    REQUIRE_THAT(hsv.s, WithinAbs(0.0, 0.001));

    // 128 / 255 * 100
    REQUIRE_THAT(hsv.v, WithinAbs(50.196078, 0.001));
}

TEST_CASE("RGBToHSV converts black") {
    const auto hsv = RGBToHSV(0, 0, 0);

    REQUIRE_THAT(hsv.h, WithinAbs(0.0, 0.001));
    REQUIRE_THAT(hsv.s, WithinAbs(0.0, 0.001));
    REQUIRE_THAT(hsv.v, WithinAbs(0.0, 0.001));
}

TEST_CASE("RGBToHSV converts white") {
    const auto hsv = RGBToHSV(255, 255, 255);

    REQUIRE_THAT(hsv.h, WithinAbs(0.0, 0.001));
    REQUIRE_THAT(hsv.s, WithinAbs(0.0, 0.001));
    REQUIRE_THAT(hsv.v, WithinAbs(100.0, 0.001));
}

TEST_CASE("RGBToHSV stays within expected ranges") {
    for (int r = 0; r <= 255; r += 51) {
        for (int g = 0; g <= 255; g += 51) {
            for (int b = 0; b <= 255; b += 51) {
                const auto hsv = RGBToHSV(r, g, b);

                REQUIRE(hsv.h >= 0.0);
                REQUIRE(hsv.h <= 360.0);

                REQUIRE(hsv.s >= 0.0);
                REQUIRE(hsv.s <= 100.0);

                REQUIRE(hsv.v >= 0.0);
                REQUIRE(hsv.v <= 100.0);
            }
        }
    }
}

TEST_CASE("RGBToCMYK converts black") {
    const auto cmyk = RGBToCMYK(0, 0, 0);

    REQUIRE(cmyk.c == 0);
    REQUIRE(cmyk.m == 0);
    REQUIRE(cmyk.y == 0);
    REQUIRE(cmyk.k == 100);
}

TEST_CASE("RGBToCMYK converts white") {
    const auto cmyk = RGBToCMYK(255, 255, 255);

    REQUIRE(cmyk.c == 0);
    REQUIRE(cmyk.m == 0);
    REQUIRE(cmyk.y == 0);
    REQUIRE(cmyk.k == 0);
}

TEST_CASE("RGBToCMYK converts primary colors") {
    SECTION("red") {
        const auto cmyk = RGBToCMYK(255, 0, 0);

        REQUIRE(cmyk.c == 0);
        REQUIRE(cmyk.m == 100);
        REQUIRE(cmyk.y == 100);
        REQUIRE(cmyk.k == 0);
    }

    SECTION("green") {
        const auto cmyk = RGBToCMYK(0, 255, 0);

        REQUIRE(cmyk.c == 100);
        REQUIRE(cmyk.m == 0);
        REQUIRE(cmyk.y == 100);
        REQUIRE(cmyk.k == 0);
    }

    SECTION("blue") {
        const auto cmyk = RGBToCMYK(0, 0, 255);

        REQUIRE(cmyk.c == 100);
        REQUIRE(cmyk.m == 100);
        REQUIRE(cmyk.y == 0);
        REQUIRE(cmyk.k == 0);
    }
}

TEST_CASE("RGBToCMYK converts gray") {
    const auto cmyk = RGBToCMYK(128, 128, 128);

    REQUIRE(cmyk.c == 0);
    REQUIRE(cmyk.m == 0);
    REQUIRE(cmyk.y == 0);
    REQUIRE(cmyk.k >= 49);
    REQUIRE(cmyk.k <= 50);
}

TEST_CASE("RGBToCMYK stays within valid ranges") {
    for (int r = 0; r <= 255; r += 51) {
        for (int g = 0; g <= 255; g += 51) {
            for (int b = 0; b <= 255; b += 51) {
                const auto cmyk = RGBToCMYK(r, g, b);

                REQUIRE(cmyk.c >= 0);
                REQUIRE(cmyk.c <= 100);

                REQUIRE(cmyk.m >= 0);
                REQUIRE(cmyk.m <= 100);

                REQUIRE(cmyk.y >= 0);
                REQUIRE(cmyk.y <= 100);

                REQUIRE(cmyk.k >= 0);
                REQUIRE(cmyk.k <= 100);
            }
        }
    }
}
