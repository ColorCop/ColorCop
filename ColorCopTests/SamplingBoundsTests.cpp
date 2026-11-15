// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// SamplingBoundsTests.cpp : Unit tests for ComputeSampleBounds() function.

#include <catch2/catch_test_macros.hpp>

#include "SamplingBounds.h"

TEST_CASE("ComputeSampleBounds supports negative monitor coordinates") {
    const auto bounds = ComputeSampleBounds(
        -1918,
        100,
        2,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == -1920);
    REQUIRE(bounds.xmax == -1916);
    REQUIRE(bounds.ymin == 98);
    REQUIRE(bounds.ymax == 102);
}

TEST_CASE("ComputeSampleBounds clips left edge") {
    const auto bounds = ComputeSampleBounds(
        -1920,
        100,
        3,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == -1920);
    REQUIRE(bounds.xmax == -1917);
    REQUIRE(bounds.ymin == 97);
    REQUIRE(bounds.ymax == 103);
}

TEST_CASE("ComputeSampleBounds clips right edge") {
    const auto bounds = ComputeSampleBounds(
        1919,
        100,
        3,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == 1916);
    REQUIRE(bounds.xmax == 1919);
    REQUIRE(bounds.ymin == 97);
    REQUIRE(bounds.ymax == 103);
}

TEST_CASE("ComputeSampleBounds clips top edge") {
    const auto bounds = ComputeSampleBounds(
        100,
        0,
        2,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == 98);
    REQUIRE(bounds.xmax == 102);
    REQUIRE(bounds.ymin == 0);
    REQUIRE(bounds.ymax == 2);
}

TEST_CASE("ComputeSampleBounds clips bottom edge") {
    const auto bounds = ComputeSampleBounds(
        100,
        1079,
        2,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == 98);
    REQUIRE(bounds.xmax == 102);
    REQUIRE(bounds.ymin == 1077);
    REQUIRE(bounds.ymax == 1079);
}

TEST_CASE("ComputeSampleBounds clips top-left corner") {
    const auto bounds = ComputeSampleBounds(
        -1920,
        0,
        2,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == -1920);
    REQUIRE(bounds.xmax == -1918);
    REQUIRE(bounds.ymin == 0);
    REQUIRE(bounds.ymax == 2);
}

TEST_CASE("ComputeSampleBounds clips bottom-right corner") {
    const auto bounds = ComputeSampleBounds(
        1919,
        1079,
        2,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == 1917);
    REQUIRE(bounds.xmax == 1919);
    REQUIRE(bounds.ymin == 1077);
    REQUIRE(bounds.ymax == 1079);
}

TEST_CASE("ComputeSampleBounds leaves interior regions unchanged") {
    const auto bounds = ComputeSampleBounds(
        500,
        500,
        2,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == 498);
    REQUIRE(bounds.xmax == 502);
    REQUIRE(bounds.ymin == 498);
    REQUIRE(bounds.ymax == 502);
}

TEST_CASE("ComputeSampleBounds supports a zero sampling offset") {
    const auto bounds = ComputeSampleBounds(
        100,
        200,
        0,
        -1920,
        0,
        1919,
        1079);

    REQUIRE(bounds.xmin == 100);
    REQUIRE(bounds.xmax == 100);
    REQUIRE(bounds.ymin == 200);
    REQUIRE(bounds.ymax == 200);
}
