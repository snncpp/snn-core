// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encoder

#pragma once

#include "snn-core/json/encode.hh"
#include "snn-core/json/is_number.hh"

namespace snn::json::stream
{
    // ## Enums

    // ### Separators

    enum separator_array
    {
        array
    };

    enum separator_array_end
    {
        array_end
    };

    enum separator_colon
    {
        colon
    };

    enum separator_comma
    {
        comma
    };

    enum separator_object
    {
        object
    };

    enum separator_object_end
    {
        object_end
    };

    // ## Functions

    namespace detail
    {
        template <typename T>
        struct as_helper final
        {
            T value;

            constexpr explicit as_helper(T v) noexcept
                : value{v}
            {
            }
        };
    }

    // ### as_bool

    constexpr auto as_bool(const same_as<bool> auto b) noexcept
    {
        if (b)
        {
            return detail::as_helper<cstrview>{"true"};
        }
        return detail::as_helper<cstrview>{"false"};
    }

    constexpr auto as_bool(const cstrview s) noexcept
    {
        snn_should(s == "true" || s == "false");
        return detail::as_helper<cstrview>{s};
    }

    // ### as_null

    constexpr auto as_null(const std::nullptr_t) noexcept
    {
        return detail::as_helper<cstrview>{"null"};
    }

    constexpr auto as_null(const cstrview s) noexcept
    {
        snn_should(s == "null");
        return detail::as_helper<cstrview>{s};
    }

    // ### as_num

    template <strict_integral Int>
    constexpr auto as_num(const Int i) noexcept
    {
        return detail::as_helper<Int>{i};
    }

    constexpr auto as_num(const cstrview s) noexcept
    {
        snn_should(json::is_number(s));
        return detail::as_helper<cstrview>{s};
    }

    // ## Classes

    // ### encoder

    template <any_strcore Str>
    class encoder final
    {
      public:
        // Promise up front that nothing added to this object will overlap the buffer.
        constexpr explicit encoder(Str& s, promise::no_overlap_t) noexcept
            : append_to_{s}
        {
        }

        constexpr encoder& operator<<(separator_array)
        {
            return append_('[');
        }

        constexpr encoder& operator<<(separator_array_end)
        {
            return replace_trailing_comma_or_append_(']');
        }

        constexpr encoder& operator<<(separator_colon)
        {
            return append_(':');
        }

        constexpr encoder& operator<<(separator_comma)
        {
            return append_(',');
        }

        constexpr encoder& operator<<(separator_object)
        {
            return append_('{');
        }

        constexpr encoder& operator<<(separator_object_end)
        {
            return replace_trailing_comma_or_append_('}');
        }

        constexpr encoder& operator<<(const cstrview s)
        {
            json::encode(s, json::option::none, append_to_, promise::no_overlap);
            return *this;
        }

        constexpr encoder& operator<<(const same_as<char> auto c)
        {
            json::encode(cstrview{as_ref(c)}, json::option::none, append_to_, promise::no_overlap);
            return *this;
        }

        constexpr encoder& operator<<(const detail::as_helper<cstrview> h)
        {
            append_to_.append(h.value);
            return *this;
        }

        template <strict_integral Int>
        constexpr encoder& operator<<(const detail::as_helper<Int> h)
        {
            append_to_.append_integral(h.value);
            return *this;
        }

      private:
        Str& append_to_;

        constexpr encoder& append_(const char c)
        {
            append_to_.append(c);
            return *this;
        }

        constexpr encoder& replace_trailing_comma_or_append_(const char c)
        {
            if (append_to_.has_back(','))
            {
                append_to_.back(promise::not_empty) = c;
                return *this;
            }

            append_to_.append(c);
            return *this;
        }
    };
}
