// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

TEST_CASE("sanity") {
    REQUIRE(true);
}

TEST_CASE("basic math") {
    REQUIRE(2 + 2 == 4);
}
