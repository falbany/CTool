/**
 * @file unit_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Unit tests for ctool::unit engineering notation and unit conversions.
 * @version 1.0
 * @date 2026-05-30
 */

#include <gtest/gtest.h>
#include "../../ctool/unit.hpp"
#include <string>

/* ============================================================================
 * Format Tests
 * ============================================================================ */

TEST(CtUnitFormatTest, ZeroValues) {
    EXPECT_EQ(ctool::unit::format(0.0), "0 ");
    EXPECT_EQ(ctool::unit::format(0.0, 2, "V"), "0 V");
    EXPECT_EQ(ctool::unit::format(-0.0), "0 ");
}

TEST(CtUnitFormatTest, NoPrefix) {
    EXPECT_EQ(ctool::unit::format(1.234, 2), "1.23");
    EXPECT_EQ(ctool::unit::format(1.234, 1, "V"), "1.2 V");
    EXPECT_EQ(ctool::unit::format(0.5, 0), "500 m");
}

TEST(CtUnitFormatTest, EngineeringPrefixes) {
    EXPECT_EQ(ctool::unit::format(1200.0, 0, "Hz"), "1 kHz");
    EXPECT_EQ(ctool::unit::format(1500000.0, 1, "W"), "1.5 MW");
    EXPECT_EQ(ctool::unit::format(1500000000.0, 2, "Hz"), "1.50 GHz");
    EXPECT_EQ(ctool::unit::format(0.0015, 2, "A"), "1.50 mA");
    EXPECT_EQ(ctool::unit::format(0.0000022, 1, "F"), "2.2 uF");
    EXPECT_EQ(ctool::unit::format(1.5e-9, 1, "s"), "1.5 ns");
    EXPECT_EQ(ctool::unit::format(1.5e-12, 1, "F"), "1.5 pF");
    EXPECT_EQ(ctool::unit::format(1.5e-15, 1, "s"), "1.5 fs");
}

TEST(CtUnitFormatTest, EdgeCaseValues) {
    // Beyond Giga
    EXPECT_EQ(ctool::unit::format(1.5e12, 1, "W"), "1500.0 GW");
    // Beyond Femto
    EXPECT_EQ(ctool::unit::format(1.5e-18, 2, "s"), "0.00 s");
}

TEST(CtUnitFormatTest, PrecisionAndRounding) {
    EXPECT_EQ(ctool::unit::format(1234.5678, 0, "Hz"), "1 kHz");
    EXPECT_EQ(ctool::unit::format(1234.5678, 4, "Hz"), "1.2346 kHz");
}

TEST(CtUnitFormatTest, NegativeValues) {
    EXPECT_EQ(ctool::unit::format(-1500.0, 1, "V"), "-1.5 kV");
    EXPECT_EQ(ctool::unit::format(-0.000001, 0, "A"), "-1 uA");
}

/* ============================================================================
 * Parse Tests
 * ============================================================================ */

TEST(CtUnitParseTest, BasicBasics) {
    EXPECT_DOUBLE_EQ(ctool::unit::parse("1.5"), 1.5);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("  1.5  "), 1.5);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("-42.5"), -42.5);
}

TEST(CtUnitParseTest, PrefixedValues) {
    EXPECT_DOUBLE_EQ(ctool::unit::parse("1.5k"), 1500.0);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("1.5 k"), 1500.0);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("100M"), 100e6);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("2.2G"), 2.2e9);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("10m"), 0.01);
    EXPECT_NEAR(ctool::unit::parse("2.2u"), 2.2e-6, 1e-12);
    EXPECT_NEAR(ctool::unit::parse("10n"), 1e-8, 1e-14);
    EXPECT_NEAR(ctool::unit::parse("1p"), 1e-12, 1e-18);
    EXPECT_NEAR(ctool::unit::parse("1f"), 1e-15, 1e-21);
}

TEST(CtUnitParseTest, RobustnessWithUnits) {
    EXPECT_DOUBLE_EQ(ctool::unit::parse("1.5mV"), 0.0015);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("100mA"), 0.1);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("100A"), 100.0);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("100 A"), 100.0);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("2.2 kOhm"), 2200.0);
    EXPECT_NEAR(ctool::unit::parse("10uF"), 1e-5, 1e-11);
}

TEST(CtUnitParseTest, InvalidInputs) {
    EXPECT_DOUBLE_EQ(ctool::unit::parse(""), 0.0);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("   "), 0.0);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("abc"), 0.0);
    EXPECT_DOUBLE_EQ(ctool::unit::parse("k"), 0.0);    // Only prefix
    // Note: With the new parsing logic, std::stod("1.2.3") returns 1.2
    // and the suffix becomes ".3k". Since '.' is not a prefix, it returns 1.2.
    EXPECT_DOUBLE_EQ(ctool::unit::parse("1.2.3k"), 1.2);
}

/* ============================================================================
 * Temperature Conversion Tests
 * ============================================================================ */

TEST(CtUnitConvertTest, CelsiusToKelvin) {
    EXPECT_DOUBLE_EQ(ctool::unit::cToK(0.0), 273.15);
    EXPECT_DOUBLE_EQ(ctool::unit::cToK(100.0), 373.15);
    EXPECT_DOUBLE_EQ(ctool::unit::cToK(-273.15), 0.0);
}

TEST(CtUnitConvertTest, KelvinToCelsius) {
    EXPECT_DOUBLE_EQ(ctool::unit::kToC(273.15), 0.0);
    EXPECT_DOUBLE_EQ(ctool::unit::kToC(373.15), 100.0);
    EXPECT_DOUBLE_EQ(ctool::unit::kToC(0.0), -273.15);
}
