// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include "WebSafe.h"

TEST_CASE("websafe colors are detected", "[unit][websafe]") {
    REQUIRE(IsWebsafeColor(0, 0, 0));
    REQUIRE(IsWebsafeColor(255, 255, 255));
    REQUIRE(IsWebsafeColor(102, 153, 204));
}

TEST_CASE("all websafe color combinations are accepted",
          "[unit][websafe]") {
    const int values[] = {0, 51, 102, 153, 204, 255};

    for (int r : values) {
        for (int g : values) {
            for (int b : values) {
                CAPTURE(r, g, b);
                REQUIRE(IsWebsafeColor(r, g, b));
            }
        }
    }
}

TEST_CASE("non-websafe colors are rejected", "[unit][websafe]") {
    REQUIRE_FALSE(IsWebsafeColor(1, 0, 0));
    REQUIRE_FALSE(IsWebsafeColor(128, 128, 128));
    REQUIRE_FALSE(IsWebsafeColor(255, 254, 255));
}

TEST_CASE("nearby invalid values are rejected",
          "[unit][websafe]") {
    REQUIRE_FALSE(IsWebsafeColor(50, 0, 0));
    REQUIRE_FALSE(IsWebsafeColor(52, 0, 0));

    REQUIRE_FALSE(IsWebsafeColor(0, 50, 0));
    REQUIRE_FALSE(IsWebsafeColor(0, 52, 0));

    REQUIRE_FALSE(IsWebsafeColor(0, 0, 50));
    REQUIRE_FALSE(IsWebsafeColor(0, 0, 52));
}
