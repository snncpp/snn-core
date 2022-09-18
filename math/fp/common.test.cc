// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/math/fp/common.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            snn_require(static_cast<int>(math::fp::abs(-5.0)) == 5);
            snn_require(static_cast<int>(math::fp::abs(5.0)) == 5);

            snn_require(math::fp::is_within_bounds<i64>(9007199254740991.0));
            snn_require(!math::fp::is_within_bounds<i64>(9007199254740992.0));

            snn_require(math::fp::is_within_bounds<double>(u64{9007199254740991}));
            snn_require(!math::fp::is_within_bounds<double>(u64{9007199254740992}));

            snn_require(math::fp::is_almost_equal(123.456, 123.457, 0.001));
            snn_require(!math::fp::is_almost_equal(123.456, 123.457, 0.0001));

            snn_require(math::fp::is_bitwise_equal(123.456, 123.456));
            snn_require(!math::fp::is_bitwise_equal(0.0, -0.0));

            snn_require(math::fp::is_infinity(constant::fp::limit<double>::infinity));
            snn_require(!math::fp::is_infinity(123.456));

            snn_require(math::fp::is_nan(constant::fp::limit<double>::nan));
            snn_require(!math::fp::is_nan(123.456));

            snn_require(math::fp::is_negative(-123.456));
            snn_require(!math::fp::is_negative(123.456));

            snn_require(static_cast<int>(math::fp::round(-0.5)) == -1);
            snn_require(static_cast<int>(math::fp::round(-0.4)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.4)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.5)) == 1);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        // constexpr double abs(const double d)
        {
            static_assert(static_cast<int>(-5.0) == -5);
            static_assert(static_cast<int>(math::fp::abs(-5.0)) == 5);
            static_assert(static_cast<int>(math::fp::abs(5.0)) == 5);
        }

        // constexpr float abs(const float f)
        {
            static_assert(static_cast<int>(-5.0f) == -5);
            static_assert(static_cast<int>(math::fp::abs(-5.0f)) == 5);
            static_assert(static_cast<int>(math::fp::abs(5.0f)) == 5);
        }

        // constexpr long double abs(const long double d)
        {
            static_assert(static_cast<int>(-5.0l) == -5);
            static_assert(static_cast<int>(math::fp::abs(-5.0l)) == 5);
            static_assert(static_cast<int>(math::fp::abs(5.0l)) == 5);
        }

        // bool is_within_bounds(const Fp fp)
        {
            // float

            // Max
            snn_require(math::fp::is_within_bounds<i64>(16777215.0f));
            snn_require(math::fp::is_within_bounds<i64>(-16777215.0f));
            snn_require(math::fp::is_within_bounds<u64>(16777215.0f));

            // Max, but not supported by `Int`.
            snn_require(!math::fp::is_within_bounds<u64>(-16777215.0f));
            snn_require(!math::fp::is_within_bounds<i16>(16777215.0f));
            snn_require(!math::fp::is_within_bounds<i16>(-16777215.0f));
            snn_require(!math::fp::is_within_bounds<u16>(16777215.0f));
            snn_require(!math::fp::is_within_bounds<u16>(-16777215.0f));

            // Greater than max.
            snn_require(!math::fp::is_within_bounds<i64>(16777216.0f));
            snn_require(!math::fp::is_within_bounds<i64>(-16777216.0f));
            snn_require(!math::fp::is_within_bounds<u64>(16777216.0f));
            snn_require(!math::fp::is_within_bounds<u64>(-16777216.0f));

            // double

            // Max
            snn_require(math::fp::is_within_bounds<i64>(9007199254740991.0));
            snn_require(math::fp::is_within_bounds<i64>(-9007199254740991.0));
            snn_require(math::fp::is_within_bounds<u64>(9007199254740991.0));

            // Max, but not supported by `Int`.
            snn_require(!math::fp::is_within_bounds<u64>(-9007199254740991.0));
            snn_require(!math::fp::is_within_bounds<i32>(9007199254740991.0));
            snn_require(!math::fp::is_within_bounds<i32>(-9007199254740991.0));
            snn_require(!math::fp::is_within_bounds<u32>(9007199254740991.0));
            snn_require(!math::fp::is_within_bounds<u32>(-9007199254740991.0));

            // Greater than max.
            snn_require(!math::fp::is_within_bounds<i64>(9007199254740992.0));
            snn_require(!math::fp::is_within_bounds<i64>(-9007199254740992.0));
            snn_require(!math::fp::is_within_bounds<u64>(9007199254740992.0));
            snn_require(!math::fp::is_within_bounds<u64>(-9007199254740992.0));

#if SNN_INT128_BOOL
            // long double

            // Max
            snn_require(math::fp::is_within_bounds<i128>(18446744073709551615.0l));
            snn_require(math::fp::is_within_bounds<i128>(-18446744073709551615.0l));
            snn_require(math::fp::is_within_bounds<u128>(18446744073709551615.0l));
            snn_require(math::fp::is_within_bounds<u64>(18446744073709551615.0l));

            // Max, but not supported by `Int`.
            snn_require(!math::fp::is_within_bounds<u128>(-18446744073709551615.0l));
            snn_require(!math::fp::is_within_bounds<i64>(18446744073709551615.0l));
            snn_require(!math::fp::is_within_bounds<i64>(-18446744073709551615.0l));
            snn_require(!math::fp::is_within_bounds<u64>(-18446744073709551615.0l));

            // Greater than max.
            snn_require(!math::fp::is_within_bounds<i128>(18446744073709551616.0l));
            snn_require(!math::fp::is_within_bounds<i128>(-18446744073709551616.0l));
            snn_require(!math::fp::is_within_bounds<u128>(18446744073709551616.0l));
            snn_require(!math::fp::is_within_bounds<u128>(-18446744073709551616.0l));
#endif

            // Small integrals (make sure integral promotion doesn't break things).

            snn_require(math::fp::is_within_bounds<i8>(127.0));
            snn_require(!math::fp::is_within_bounds<i8>(128.0));

            snn_require(math::fp::is_within_bounds<i8>(-128.0));
            snn_require(!math::fp::is_within_bounds<i8>(-129.0));

            snn_require(math::fp::is_within_bounds<u8>(255.0));
            snn_require(!math::fp::is_within_bounds<u8>(256.0));

            // Test extremes.

            using math::fp::is_within_bounds;

            snn_require(!is_within_bounds<i64>(constant::fp::limit<float>::min_negative));
            snn_require(!is_within_bounds<i64>(constant::fp::limit<float>::max));
            snn_require(!is_within_bounds<i64>(constant::fp::limit<float>::infinity));
            snn_require(!is_within_bounds<i64>(constant::fp::limit<float>::nan));

            snn_require(!is_within_bounds<i64>(constant::fp::limit<double>::min_negative));
            snn_require(!is_within_bounds<i64>(constant::fp::limit<double>::max));
            snn_require(!is_within_bounds<i64>(constant::fp::limit<double>::infinity));
            snn_require(!is_within_bounds<i64>(constant::fp::limit<double>::nan));

            snn_require(!is_within_bounds<u64>(constant::fp::limit<float>::min_negative));
            snn_require(!is_within_bounds<u64>(constant::fp::limit<float>::max));
            snn_require(!is_within_bounds<u64>(constant::fp::limit<float>::infinity));
            snn_require(!is_within_bounds<u64>(constant::fp::limit<float>::nan));

            snn_require(!is_within_bounds<u64>(constant::fp::limit<double>::min_negative));
            snn_require(!is_within_bounds<u64>(constant::fp::limit<double>::max));
            snn_require(!is_within_bounds<u64>(constant::fp::limit<double>::infinity));
            snn_require(!is_within_bounds<u64>(constant::fp::limit<double>::nan));
        }

        // constexpr bool is_within_bounds(const Int i)
        {
            // float

            // Max
            static_assert(math::fp::is_within_bounds<float>(16777215));
            static_assert(math::fp::is_within_bounds<float>(-16777215));
            static_assert(math::fp::is_within_bounds<float>(u32{16777215}));

            // Greater than max.
            static_assert(!math::fp::is_within_bounds<float>(16777216));
            static_assert(!math::fp::is_within_bounds<float>(-16777216));
            static_assert(!math::fp::is_within_bounds<float>(u32{16777216}));
            static_assert(!math::fp::is_within_bounds<float>(constant::limit<i32>::min));
            static_assert(!math::fp::is_within_bounds<float>(constant::limit<i32>::max));
            static_assert(!math::fp::is_within_bounds<float>(constant::limit<i64>::min));
            static_assert(!math::fp::is_within_bounds<float>(constant::limit<i64>::max));

            // double

            // Max
            static_assert(math::fp::is_within_bounds<double>(9007199254740991));
            static_assert(math::fp::is_within_bounds<double>(-9007199254740991));
            static_assert(math::fp::is_within_bounds<double>(u64{9007199254740991}));

            // Greater than max.
            static_assert(!math::fp::is_within_bounds<double>(9007199254740992));
            static_assert(!math::fp::is_within_bounds<double>(-9007199254740992));
            static_assert(!math::fp::is_within_bounds<double>(u64{9007199254740992}));
            static_assert(!math::fp::is_within_bounds<double>(constant::limit<i64>::min));
            static_assert(!math::fp::is_within_bounds<double>(constant::limit<i64>::max));

#if SNN_INT128_BOOL
            // long double

            constexpr i128 max = constant::limit<u64>::max;

            // Max
            static_assert(math::fp::is_within_bounds<long double>(max));
            static_assert(math::fp::is_within_bounds<long double>(-max));

            // Greater than max.
            static_assert(!math::fp::is_within_bounds<long double>(max + 1));
            static_assert(!math::fp::is_within_bounds<long double>(-max - 1));
#endif

            // Small integrals (make sure integral promotion doesn't break things).

            static_assert(math::fp::is_within_bounds<float>(constant::limit<i8>::min));
            static_assert(math::fp::is_within_bounds<float>(constant::limit<i8>::max));
            static_assert(math::fp::is_within_bounds<float>(constant::limit<u8>::min));
            static_assert(math::fp::is_within_bounds<float>(constant::limit<u8>::max));
        }

        // constexpr bool is_almost_equal(const F a, const not_deduced_t<F> b,
        //     const not_deduced_t<F> max_diff) noexcept
        {
            static_assert(math::fp::is_almost_equal(123.456, 123.456, 0.0));
            static_assert(math::fp::is_almost_equal(123.456, 123.456, 0.000001));
            static_assert(math::fp::is_almost_equal(-123.456, -123.456, 0.0));
            static_assert(math::fp::is_almost_equal(-123.456, -123.456, 0.000001));

            static_assert(math::fp::is_almost_equal(123.456, 123.457, 0.001));
            static_assert(math::fp::is_almost_equal(123.457, 123.456, 0.001));
            static_assert(!math::fp::is_almost_equal(123.456, 123.457, 0.0001));
            static_assert(!math::fp::is_almost_equal(123.457, 123.456, 0.0001));

            static_assert(math::fp::is_almost_equal(-123.456, -123.457, 0.001));
            static_assert(math::fp::is_almost_equal(-123.457, -123.456, 0.001));
            static_assert(!math::fp::is_almost_equal(-123.456, -123.457, 0.0001));
            static_assert(!math::fp::is_almost_equal(-123.457, -123.456, 0.0001));

            constexpr auto min      = constant::fp::limit<double>::min_negative;
            constexpr auto max      = constant::fp::limit<double>::max;
            constexpr auto infinity = constant::fp::limit<double>::infinity;
            constexpr auto nan      = constant::fp::limit<double>::nan;

            static_assert(math::fp::is_almost_equal(min, min, 0.000001));
            static_assert(math::fp::is_almost_equal(max, max, 0.000001));
            snn_require(!math::fp::is_almost_equal(min, max, 0.000001));
            snn_require(!math::fp::is_almost_equal(max, min, 0.000001));

            static_assert(!math::fp::is_almost_equal(infinity, max, 0.000001));
            static_assert(!math::fp::is_almost_equal(max, infinity, 0.000001));

            snn_require(!math::fp::is_almost_equal(infinity, infinity, 0.000001));

            snn_require(!math::fp::is_almost_equal(nan, nan, 0.1));
            snn_require(!math::fp::is_almost_equal(0.0, nan, 0.1));
            snn_require(!math::fp::is_almost_equal(nan, 0.0, 0.1));
            snn_require(!math::fp::is_almost_equal(0.0, 0.0, nan));
            snn_require(!math::fp::is_almost_equal(nan, nan, nan));
        }

        // bool is_bitwise_equal(const F a, const not_deduced_t<F> b)
        {
            constexpr auto infinity = constant::fp::limit<double>::infinity;
            constexpr auto nan      = constant::fp::limit<double>::nan;

            snn_require(math::fp::is_bitwise_equal(nan, nan));
            snn_require(math::fp::is_bitwise_equal(123.456, 123.456));

            snn_require(!math::fp::is_bitwise_equal(0.0, -0.0));
            snn_require(!math::fp::is_bitwise_equal(123.456, 123.457));
            snn_require(!math::fp::is_bitwise_equal(infinity, -infinity));
        }

        // bool is_infinity(const F f)
        {
            snn_require(math::fp::is_infinity(constant::fp::limit<double>::infinity));
            snn_require(math::fp::is_infinity(-constant::fp::limit<double>::infinity));

            snn_require(!math::fp::is_infinity(123.456));
            snn_require(!math::fp::is_infinity(constant::fp::limit<double>::nan));
            snn_require(!math::fp::is_infinity(constant::fp::limit<double>::max));
        }

        // constexpr bool is_nan(const F f)
        {
            static_assert(math::fp::is_nan(constant::fp::limit<double>::nan));
            static_assert(!math::fp::is_nan(123.456));
            static_assert(!math::fp::is_nan(constant::fp::limit<double>::max));
            static_assert(!math::fp::is_nan(constant::fp::limit<double>::infinity));
        }

        // bool is_negative(const F f)
        {
            snn_require(math::fp::is_negative(-0.0));
            snn_require(math::fp::is_negative(-1.0));
            snn_require(math::fp::is_negative(-0.00001));
            snn_require(math::fp::is_negative(-123.456));

            snn_require(!math::fp::is_negative(0.0));
            snn_require(!math::fp::is_negative(1.0));
            snn_require(!math::fp::is_negative(0.00001));
            snn_require(!math::fp::is_negative(123.456));

            snn_require(math::fp::is_negative(constant::fp::limit<double>::min_negative));

            snn_require(!math::fp::is_negative(constant::fp::limit<double>::min_positive));
            snn_require(!math::fp::is_negative(constant::fp::limit<double>::max));
        }
        {
            snn_require(math::fp::is_negative(-0.0f));
            snn_require(math::fp::is_negative(-1.0f));
            snn_require(math::fp::is_negative(-0.00001f));
            snn_require(math::fp::is_negative(-123.456f));

            snn_require(!math::fp::is_negative(0.0f));
            snn_require(!math::fp::is_negative(1.0f));
            snn_require(!math::fp::is_negative(0.00001f));
            snn_require(!math::fp::is_negative(123.456f));

            snn_require(math::fp::is_negative(constant::fp::limit<float>::min_negative));

            snn_require(!math::fp::is_negative(constant::fp::limit<float>::min_positive));
            snn_require(!math::fp::is_negative(constant::fp::limit<float>::max));
        }

        // double round(const double d)
        {
            snn_require(static_cast<int>(math::fp::round(-0.6)) == -1);
            snn_require(static_cast<int>(math::fp::round(-0.5)) == -1);
            snn_require(static_cast<int>(math::fp::round(-0.4)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.3)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.2)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.1)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.0)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.0)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.1)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.2)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.3)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.4)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.5)) == 1);
            snn_require(static_cast<int>(math::fp::round(0.6)) == 1);

            // Special cases that fail if round() uses Int(abs(dbl) + 0.5).
            snn_require(static_cast<int>(math::fp::round(-0.49999999999999994)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.49999999999999994)) == 0);
        }

        // float round(const float f)
        {
            snn_require(static_cast<int>(math::fp::round(-0.6f)) == -1);
            snn_require(static_cast<int>(math::fp::round(-0.5f)) == -1);
            snn_require(static_cast<int>(math::fp::round(-0.4f)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.3f)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.2f)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.1f)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.0f)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.0f)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.1f)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.2f)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.3f)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.4f)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.5f)) == 1);
            snn_require(static_cast<int>(math::fp::round(0.6f)) == 1);
        }

        // long double round(const long double d)
        {
            snn_require(static_cast<int>(math::fp::round(-0.6l)) == -1);
            snn_require(static_cast<int>(math::fp::round(-0.5l)) == -1);
            snn_require(static_cast<int>(math::fp::round(-0.4l)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.3l)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.2l)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.1l)) == 0);
            snn_require(static_cast<int>(math::fp::round(-0.0l)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.0l)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.1l)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.2l)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.3l)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.4l)) == 0);
            snn_require(static_cast<int>(math::fp::round(0.5l)) == 1);
            snn_require(static_cast<int>(math::fp::round(0.6l)) == 1);
        }
    }
}
