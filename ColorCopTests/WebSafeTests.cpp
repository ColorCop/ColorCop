// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include "WebSafe.h"

TEST_CASE("websafe colors are detected") {
    REQUIRE(IsWebsafeColor(0, 0, 0));
    REQUIRE(IsWebsafeColor(255, 255, 255));
    REQUIRE(IsWebsafeColor(102, 153, 204));
}

TEST_CASE("non-websafe colors are rejected") {
    REQUIRE_FALSE(IsWebsafeColor(1, 0, 0));
    REQUIRE_FALSE(IsWebsafeColor(128, 128, 128));
    REQUIRE_FALSE(IsWebsafeColor(255, 254, 255));
}
