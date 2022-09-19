// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Country codes (array)

// Two-letter uppercase country/territory codes from IANA Time Zone Database.
// https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2

// Sorted to match country names.

#pragma once

#include "snn-core/array.hh"
#include "snn-core/country/code.hh"

namespace snn::country
{
    // ## Arrays

    // ### codes

    // Generated by: detail/codes-names.gen.cc

    inline constexpr array<country::code, 249> codes{
        "AF", "AX", "AL", "DZ", "AD", "AO", "AI", "AQ", "AG", "AR", "AM", "AW", "AU", "AT", "AZ",
        "BS", "BH", "BD", "BB", "BY", "BE", "BZ", "BJ", "BM", "BT", "BO", "BA", "BW", "BV", "BR",
        "GB", "IO", "BN", "BG", "BF", "BI", "KH", "CM", "CA", "CV", "BQ", "KY", "CF", "TD", "CL",
        "CN", "CX", "CC", "CO", "KM", "CD", "CG", "CK", "CR", "CI", "HR", "CU", "CW", "CY", "CZ",
        "DK", "DJ", "DM", "DO", "TL", "EC", "EG", "SV", "GQ", "ER", "EE", "SZ", "ET", "FK", "FO",
        "FJ", "FI", "FR", "GF", "PF", "TF", "GA", "GM", "GE", "DE", "GH", "GI", "GR", "GL", "GD",
        "GP", "GU", "GT", "GG", "GN", "GW", "GY", "HT", "HM", "HN", "HK", "HU", "IS", "IN", "ID",
        "IR", "IQ", "IE", "IM", "IL", "IT", "JM", "JP", "JE", "JO", "KZ", "KE", "KI", "KP", "KR",
        "KW", "KG", "LA", "LV", "LB", "LS", "LR", "LY", "LI", "LT", "LU", "MO", "MG", "MW", "MY",
        "MV", "ML", "MT", "MH", "MQ", "MR", "MU", "YT", "MX", "FM", "MD", "MC", "MN", "ME", "MS",
        "MA", "MZ", "MM", "NA", "NR", "NP", "NL", "NC", "NZ", "NI", "NE", "NG", "NU", "NF", "MK",
        "MP", "NO", "OM", "PK", "PW", "PS", "PA", "PG", "PY", "PE", "PH", "PN", "PL", "PT", "PR",
        "QA", "RE", "RO", "RU", "RW", "AS", "WS", "SM", "ST", "SA", "SN", "RS", "SC", "SL", "SG",
        "SK", "SI", "SB", "SO", "ZA", "GS", "SS", "ES", "LK", "BL", "SH", "KN", "LC", "SX", "MF",
        "PM", "VC", "SD", "SR", "SJ", "SE", "CH", "SY", "TW", "TJ", "TZ", "TH", "TG", "TK", "TO",
        "TT", "TN", "TR", "TM", "TC", "TV", "UG", "UA", "AE", "US", "UY", "UM", "UZ", "VU", "VA",
        "VE", "VN", "VG", "VI", "WF", "EH", "YE", "ZM", "ZW",
    };
}