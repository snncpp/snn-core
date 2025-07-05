// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/chr/common.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            usize count = 0;
            for (const char c : range::integral<char>{})
            {
                // Alpha
                if (c >= 'A' && c <= 'Z')
                {
                    snn_require(chr::is_alpha(c));
                    snn_require(!chr::is_alpha_lower(c));
                    snn_require(chr::is_alpha_upper(c));

                    snn_require(chr::is_alphanumeric(c));
                    snn_require(!chr::is_alphanumeric_lower(c));
                    snn_require(chr::is_alphanumeric_upper(c));
                }
                else if (c >= 'a' && c <= 'z')
                {
                    snn_require(chr::is_alpha(c));
                    snn_require(chr::is_alpha_lower(c));
                    snn_require(!chr::is_alpha_upper(c));

                    snn_require(chr::is_alphanumeric(c));
                    snn_require(chr::is_alphanumeric_lower(c));
                    snn_require(!chr::is_alphanumeric_upper(c));
                }
                else
                {
                    snn_require(!chr::is_alpha(c));
                    snn_require(!chr::is_alpha_lower(c));
                    snn_require(!chr::is_alpha_upper(c));

                    if (!chr::is_digit(c))
                    {
                        snn_require(!chr::is_alphanumeric(c));
                        snn_require(!chr::is_alphanumeric_lower(c));
                        snn_require(!chr::is_alphanumeric_upper(c));
                    }
                }

                // Ascii
                if (to_byte(c) <= 0b0111'1111)
                {
                    snn_require(chr::is_ascii(c));
                }
                else
                {
                    snn_require(!chr::is_ascii(c));
                }

                // Binary
                if (c == '0' || c == '1')
                {
                    snn_require(chr::is_binary(c));
                    snn_require(chr::is_base<math::base::binary>(c));
                }
                else
                {
                    snn_require(!chr::is_binary(c));
                    snn_require(!chr::is_base<math::base::binary>(c));
                }

                // Octal
                if (c >= '0' && c <= '7')
                {
                    snn_require(chr::is_octal(c));
                    snn_require(chr::is_base<math::base::octal>(c));
                }
                else
                {
                    snn_require(!chr::is_octal(c));
                    snn_require(!chr::is_base<math::base::octal>(c));
                }

                // Ascii control.
                if (to_byte(c) < 32)
                {
                    snn_require(chr::is_ascii_control(c));
                }
                else if (to_byte(c) == 127)
                {
                    snn_require(chr::is_ascii_control(c));
                }
                else
                {
                    snn_require(!chr::is_ascii_control(c));
                }

                // Ascii control or space.
                if (to_byte(c) <= 32)
                {
                    snn_require(chr::is_ascii_control_or_space(c));
                }
                else if (to_byte(c) == 127)
                {
                    snn_require(chr::is_ascii_control_or_space(c));
                }
                else
                {
                    snn_require(!chr::is_ascii_control_or_space(c));
                }

                // Ascii printable.
                if (to_byte(c) >= 32 && to_byte(c) < 127)
                {
                    snn_require(chr::is_ascii_printable(c));
                }
                else
                {
                    snn_require(!chr::is_ascii_printable(c));
                }

                // Digit (decimal).
                if (c >= '0' && c <= '9')
                {
                    snn_require(chr::is_digit(c));
                    snn_require(chr::is_base<math::base::decimal>(c));

                    snn_require(chr::is_alphanumeric(c));
                    snn_require(chr::is_alphanumeric_lower(c));
                    snn_require(chr::is_alphanumeric_upper(c));
                }
                else
                {
                    snn_require(!chr::is_digit(c));
                    snn_require(!chr::is_base<math::base::decimal>(c));

                    if (!chr::is_alpha(c))
                    {
                        snn_require(!chr::is_alphanumeric(c));
                        snn_require(!chr::is_alphanumeric_lower(c));
                        snn_require(!chr::is_alphanumeric_upper(c));
                    }
                }

                // Non-zero digit (decimal).
                if (c >= '1' && c <= '9')
                {
                    snn_require(chr::is_nonzero_digit(c));
                }
                else
                {
                    snn_require(!chr::is_nonzero_digit(c));
                }

                // Hex
                if (c >= 'A' && c <= 'F')
                {
                    snn_require(chr::is_hex(c));
                    snn_require(chr::is_base<math::base::hex>(c));
                    snn_require(chr::decode_hex(c) != chr::invalid);
                    snn_require(chr::decode_base<math::base::hex>(c) != chr::invalid);
                }
                else if (c >= 'a' && c <= 'f')
                {
                    snn_require(chr::is_hex(c));
                    snn_require(chr::is_base<math::base::hex>(c));
                    snn_require(chr::decode_hex(c) != chr::invalid);
                    snn_require(chr::decode_base<math::base::hex>(c) != chr::invalid);
                }
                else if (c >= '0' && c <= '9')
                {
                    snn_require(chr::is_hex(c));
                    snn_require(chr::is_base<math::base::hex>(c));
                    snn_require(chr::decode_hex(c) != chr::invalid);
                    snn_require(chr::decode_base<math::base::hex>(c) != chr::invalid);
                }
                else
                {
                    snn_require(!chr::is_hex(c));
                    snn_require(!chr::is_base<math::base::hex>(c));
                    snn_require(chr::decode_hex(c) == chr::invalid);
                    snn_require(chr::decode_base<math::base::hex>(c) == chr::invalid);
                }

                ++count;
            }
            snn_require(count == 256);

            // Binary
            snn_require(chr::decode_binary('0') == 0);
            snn_require(chr::decode_binary('1') == 1);

            snn_require(chr::decode_base<math::base::binary>('0') == 0);
            snn_require(chr::decode_base<math::base::binary>('1') == 1);

            // Octal
            snn_require(chr::decode_octal('0') == 0);
            snn_require(chr::decode_octal('1') == 1);
            snn_require(chr::decode_octal('2') == 2);
            snn_require(chr::decode_octal('3') == 3);
            snn_require(chr::decode_octal('4') == 4);
            snn_require(chr::decode_octal('5') == 5);
            snn_require(chr::decode_octal('6') == 6);
            snn_require(chr::decode_octal('7') == 7);

            snn_require(chr::decode_base<math::base::octal>('0') == 0);
            snn_require(chr::decode_base<math::base::octal>('1') == 1);
            snn_require(chr::decode_base<math::base::octal>('2') == 2);
            snn_require(chr::decode_base<math::base::octal>('3') == 3);
            snn_require(chr::decode_base<math::base::octal>('4') == 4);
            snn_require(chr::decode_base<math::base::octal>('5') == 5);
            snn_require(chr::decode_base<math::base::octal>('6') == 6);
            snn_require(chr::decode_base<math::base::octal>('7') == 7);

            // Decimal
            snn_require(chr::decode_digit('0') == 0);
            snn_require(chr::decode_digit('1') == 1);
            snn_require(chr::decode_digit('2') == 2);
            snn_require(chr::decode_digit('3') == 3);
            snn_require(chr::decode_digit('4') == 4);
            snn_require(chr::decode_digit('5') == 5);
            snn_require(chr::decode_digit('6') == 6);
            snn_require(chr::decode_digit('7') == 7);
            snn_require(chr::decode_digit('8') == 8);
            snn_require(chr::decode_digit('9') == 9);

            snn_require(chr::decode_base<math::base::decimal>('0') == 0);
            snn_require(chr::decode_base<math::base::decimal>('1') == 1);
            snn_require(chr::decode_base<math::base::decimal>('2') == 2);
            snn_require(chr::decode_base<math::base::decimal>('3') == 3);
            snn_require(chr::decode_base<math::base::decimal>('4') == 4);
            snn_require(chr::decode_base<math::base::decimal>('5') == 5);
            snn_require(chr::decode_base<math::base::decimal>('6') == 6);
            snn_require(chr::decode_base<math::base::decimal>('7') == 7);
            snn_require(chr::decode_base<math::base::decimal>('8') == 8);
            snn_require(chr::decode_base<math::base::decimal>('9') == 9);

            // Hex
            snn_require(chr::decode_hex('0') == 0);
            snn_require(chr::decode_hex('1') == 1);
            snn_require(chr::decode_hex('2') == 2);
            snn_require(chr::decode_hex('3') == 3);
            snn_require(chr::decode_hex('4') == 4);
            snn_require(chr::decode_hex('5') == 5);
            snn_require(chr::decode_hex('6') == 6);
            snn_require(chr::decode_hex('7') == 7);
            snn_require(chr::decode_hex('8') == 8);
            snn_require(chr::decode_hex('9') == 9);

            snn_require(chr::decode_hex('a') == 10);
            snn_require(chr::decode_hex('b') == 11);
            snn_require(chr::decode_hex('c') == 12);
            snn_require(chr::decode_hex('d') == 13);
            snn_require(chr::decode_hex('e') == 14);
            snn_require(chr::decode_hex('f') == 15);

            snn_require(chr::decode_hex('A') == 10);
            snn_require(chr::decode_hex('B') == 11);
            snn_require(chr::decode_hex('C') == 12);
            snn_require(chr::decode_hex('D') == 13);
            snn_require(chr::decode_hex('E') == 14);
            snn_require(chr::decode_hex('F') == 15);

            snn_require(chr::decode_base<math::base::hex>('0') == 0);
            snn_require(chr::decode_base<math::base::hex>('1') == 1);
            snn_require(chr::decode_base<math::base::hex>('2') == 2);
            snn_require(chr::decode_base<math::base::hex>('3') == 3);
            snn_require(chr::decode_base<math::base::hex>('4') == 4);
            snn_require(chr::decode_base<math::base::hex>('5') == 5);
            snn_require(chr::decode_base<math::base::hex>('6') == 6);
            snn_require(chr::decode_base<math::base::hex>('7') == 7);
            snn_require(chr::decode_base<math::base::hex>('8') == 8);
            snn_require(chr::decode_base<math::base::hex>('9') == 9);

            snn_require(chr::decode_base<math::base::hex>('a') == 10);
            snn_require(chr::decode_base<math::base::hex>('b') == 11);
            snn_require(chr::decode_base<math::base::hex>('c') == 12);
            snn_require(chr::decode_base<math::base::hex>('d') == 13);
            snn_require(chr::decode_base<math::base::hex>('e') == 14);
            snn_require(chr::decode_base<math::base::hex>('f') == 15);

            snn_require(chr::decode_base<math::base::hex>('A') == 10);
            snn_require(chr::decode_base<math::base::hex>('B') == 11);
            snn_require(chr::decode_base<math::base::hex>('C') == 12);
            snn_require(chr::decode_base<math::base::hex>('D') == 13);
            snn_require(chr::decode_base<math::base::hex>('E') == 14);
            snn_require(chr::decode_base<math::base::hex>('F') == 15);

            // to_alpha_lower
            // to_alpha_upper
            {
                snn_require(chr::to_alpha_lower('a') == 'a');
                snn_require(chr::to_alpha_lower('A') == 'a');
                snn_require(chr::to_alpha_lower('9') == '9');
                snn_require(chr::to_alpha_upper('z') == 'Z');
                snn_require(chr::to_alpha_upper('Z') == 'Z');
                snn_require(chr::to_alpha_upper('.') == '.');
            }
            {
                const char case_diff = 'a' - 'A';
                snn_require(case_diff == 32);

                count = 0;
                for (const char c : range::integral<char>{})
                {
                    if (c >= 'a' && c <= 'z')
                    {
                        snn_require(chr::to_alpha_lower(c) == c);
                        snn_require(chr::to_alpha_upper(c) == (c - case_diff));
                    }
                    else if (c >= 'A' && c <= 'Z')
                    {
                        snn_require(chr::to_alpha_lower(c) == (c + case_diff));
                        snn_require(chr::to_alpha_upper(c) == c);
                    }
                    else
                    {
                        snn_require(chr::to_alpha_lower(c) == c);
                        snn_require(chr::to_alpha_upper(c) == c);
                    }

                    ++count;
                }
                snn_require(count == 256);
            }

            // to_alpha_lower_inplace
            // to_alpha_upper_inplace
            {
                char c1 = 'a';
                char c2 = '9';
                chr::to_alpha_upper_inplace(c1);
                chr::to_alpha_upper_inplace(c2);
                snn_require(c1 == 'A');
                snn_require(c2 == '9');
                chr::to_alpha_lower_inplace(c1);
                chr::to_alpha_lower_inplace(c2);
                snn_require(c1 == 'a');
                snn_require(c2 == '9');
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
