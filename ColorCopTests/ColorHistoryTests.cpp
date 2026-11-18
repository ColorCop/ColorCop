// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

#include "ColorHistory.h"

TEST_CASE("PushColorHistory inserts current color at front") {
    COLORREF history[7] = {};

    PushColorHistory(history, RGB(10, 20, 30));

    REQUIRE(history[0] == RGB(10, 20, 30));
}

TEST_CASE("PushColorHistory shifts existing history entries down") {
    COLORREF history[7] = {};

    history[0] = RGB(1, 1, 1);
    history[1] = RGB(2, 2, 2);
    history[2] = RGB(3, 3, 3);
    history[3] = RGB(4, 4, 4);

    PushColorHistory(history, RGB(100, 110, 120));

    REQUIRE(history[0] == RGB(100, 110, 120));
    REQUIRE(history[1] == RGB(1, 1, 1));
    REQUIRE(history[2] == RGB(2, 2, 2));
    REQUIRE(history[3] == RGB(3, 3, 3));
    REQUIRE(history[4] == RGB(4, 4, 4));
}

TEST_CASE("PushColorHistory ignores duplicate current color") {
    COLORREF history[7] = {};

    history[0] = RGB(10, 20, 30);
    history[1] = RGB(40, 50, 60);

    PushColorHistory(history, RGB(10, 20, 30));

    REQUIRE(history[0] == RGB(10, 20, 30));
    REQUIRE(history[1] == RGB(40, 50, 60));
}

TEST_CASE("PushColorHistory discards oldest history entry") {
    COLORREF history[7] = {};

    for (int i = 0; i < 7; ++i) {
        history[i] = RGB(i, i, i);
    }

    PushColorHistory(history, RGB(200, 201, 202));

    REQUIRE(history[0] == RGB(200, 201, 202));

    REQUIRE(history[1] == RGB(0, 0, 0));
    REQUIRE(history[2] == RGB(1, 1, 1));
    REQUIRE(history[3] == RGB(2, 2, 2));
    REQUIRE(history[4] == RGB(3, 3, 3));
    REQUIRE(history[5] == RGB(4, 4, 4));
    REQUIRE(history[6] == RGB(5, 5, 5));
}
