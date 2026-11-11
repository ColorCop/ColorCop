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

TEST_CASE("DecimalToWebsafe preserves websafe values",
          "[unit][websafe]") {
    REQUIRE(DecimalToWebsafe(0) == 0);
    REQUIRE(DecimalToWebsafe(51) == 51);
    REQUIRE(DecimalToWebsafe(102) == 102);
    REQUIRE(DecimalToWebsafe(153) == 153);
    REQUIRE(DecimalToWebsafe(204) == 204);
    REQUIRE(DecimalToWebsafe(255) == 255);
}

TEST_CASE("DecimalToWebsafe rounds down when closer to lower boundary",
          "[unit][websafe]") {
    REQUIRE(DecimalToWebsafe(1) == 0);
    REQUIRE(DecimalToWebsafe(24) == 0);

    REQUIRE(DecimalToWebsafe(52) == 51);
    REQUIRE(DecimalToWebsafe(75) == 51);

    REQUIRE(DecimalToWebsafe(103) == 102);
    REQUIRE(DecimalToWebsafe(126) == 102);
}

TEST_CASE("DecimalToWebsafe rounds up when closer to upper boundary",
          "[unit][websafe]") {
    REQUIRE(DecimalToWebsafe(26) == 51);
    REQUIRE(DecimalToWebsafe(50) == 51);

    REQUIRE(DecimalToWebsafe(77) == 102);
    REQUIRE(DecimalToWebsafe(101) == 102);

    REQUIRE(DecimalToWebsafe(128) == 153);
    REQUIRE(DecimalToWebsafe(152) == 153);
}

TEST_CASE("DecimalToWebsafe rounds midpoint values upward",
          "[unit][websafe]") {
    REQUIRE(DecimalToWebsafe(25) == 51);
    REQUIRE(DecimalToWebsafe(76) == 102);
    REQUIRE(DecimalToWebsafe(127) == 153);
    REQUIRE(DecimalToWebsafe(178) == 204);
    REQUIRE(DecimalToWebsafe(229) == 255);
}

TEST_CASE("DecimalToWebsafe always produces a websafe value",
          "[unit][websafe]") {
    for (int value = 0; value <= 255; ++value) {
        const int result = DecimalToWebsafe(value);

        INFO("value=" << value << ", result=" << result);

        REQUIRE(result >= 0);
        REQUIRE(result <= 255);
        REQUIRE(result % WEBSAFE_STEP == 0);
    }
}

TEST_CASE("channels can be individually converted to websafe values",
          "[unit][websafe]") {
    REQUIRE(DecimalToWebsafe(128) == 153);
    REQUIRE(DecimalToWebsafe(125) == 102);
    REQUIRE(DecimalToWebsafe(230) == 255);

    REQUIRE(IsWebsafeColor(
        DecimalToWebsafe(128),
        DecimalToWebsafe(125),
        DecimalToWebsafe(230)));
}
