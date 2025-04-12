// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format function and default formatters

#pragma once

#include "snn-core/array.hh"
#include "snn-core/formatter.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fmt/context.hh"
#include "snn-core/fmt/error.hh"
#include "snn-core/fmt/integral.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/utf8/pad.hh"

namespace snn
{
    // ## Classes

    // ### formatter[_format_as] specializations

    // Default formatters, can be overridden with formatter_override<T>.

    template <>
    struct formatter<bool>
    {
        using type = bool;

        template <typename Buf>
        constexpr void format(const bool b, cstrview, const fmt::context&, strcore<Buf>& append_to,
                              promise::no_overlap_t)
        {
            if (b)
            {
                append_to.append("true");
            }
            else
            {
                append_to.append("false");
            }
        }
    };

    template <>
    struct formatter<char>
    {
        using type = char;

        template <typename Buf>
        constexpr void format(const char c, cstrview, const fmt::context&, strcore<Buf>& append_to,
                              promise::no_overlap_t)
        {
            append_to.append(c);
        }
    };

    template <>
    struct formatter<cstrview>
    {
        using type = cstrview;

        template <typename Buf>
        constexpr void format(const cstrview s, cstrview, const fmt::context&,
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            append_to.append(s);
        }
    };

    template <character Char, usize Count>
    struct formatter_format_as<array_view<Char, Count>>
    {
        using type = cstrview;
    };

    template <typename Buf>
    struct formatter_format_as<strcore<Buf>>
    {
        using type = cstrview;
    };

    template <typename Tuple>
    struct formatter<Tuple, std::void_t<std::tuple_element_t<0, Tuple>>>
    {
      public:
        using type = Tuple;

        static constexpr usize tuple_count = std::tuple_size_v<Tuple>;

        template <typename Buf>
        constexpr void format(const Tuple& tup, const cstrview format_string,
                              const fmt::context& ctx, strcore<Buf>& append_to,
                              promise::no_overlap_t)
        {
            append_to.append('(');
            format_<0>(tup, format_string, ctx, append_to);
            append_to.append(')');
        }

      private:
        template <usize Index, typename Buf>
        constexpr void format_(const Tuple& tup, const cstrview format_string,
                               const fmt::context& ctx, strcore<Buf>& append_to)
        {
            using element_type = std::tuple_element_t<Index, Tuple>;

            formatter_select_t<element_type> f;
            f.format(get<Index>(tup), format_string, ctx, append_to, promise::no_overlap);

            constexpr usize next_index = Index + 1;
            if constexpr (next_index < tuple_count)
            {
                if constexpr (next_index < 3)
                {
                    append_to.append(", ");
                    format_<next_index>(tup, format_string, ctx, append_to);
                }
                else
                {
                    append_to.append(", ...");
                }
            }
        }
    };
}

namespace snn::fmt
{
    namespace detail
    {
        template <typename Buf>
        class argument final
        {
          private:
            enum class type : u8
            {
                unsigned_integral,
                signed_integral,
#if SNN_INT128_ENABLED
                unsigned_128bit_integral,
                signed_128bit_integral,
#endif
                boolean,
                character,
                string,
                other,
            };

          public:
            constexpr argument() noexcept
                : string_{},
                  type_{type::string}
            {
            }

            constexpr explicit argument(const bool& b) noexcept
                : character_{b},
                  type_{type::boolean}
            {
            }

            constexpr explicit argument(const char& c) noexcept
                : character_{c},
                  type_{type::character}
            {
            }

            template <usize N>
            constexpr explicit argument(const char (&s)[N]) noexcept
                : string_{s},
                  type_{type::string}
            {
            }

            template <character Char>
            constexpr explicit argument(const array_view<Char> s) noexcept
                : string_{s},
                  type_{type::string}
            {
            }

            template <typename B>
            constexpr explicit argument(const strcore<B>& s) noexcept
                : string_{s.view()},
                  type_{type::string}
            {
            }

            template <strict_signed_integral SInt>
            constexpr explicit argument(const SInt& i) noexcept
                : signed_integral_{i},
                  type_{type::signed_integral}
            {
            }

            template <strict_unsigned_integral UInt>
            constexpr explicit argument(const UInt& i) noexcept
                : unsigned_integral_{i},
                  type_{type::unsigned_integral}
            {
            }

#if SNN_INT128_ENABLED
            constexpr explicit argument(const i128& i) noexcept
                : signed_128bit_integral_{i},
                  type_{type::signed_128bit_integral}
            {
            }

            constexpr explicit argument(const u128& i) noexcept
                : unsigned_128bit_integral_{i},
                  type_{type::unsigned_128bit_integral}
            {
            }
#endif

            template <typename T>
            constexpr explicit argument(const T& v) noexcept
                : other_{&v, format_other_<T>},
                  type_{type::other}
            {
            }

            constexpr void format(const cstrview format_string, const fmt::context& ctx,
                                  strcore<Buf>& append_to, promise::no_overlap_t) const
            {
                switch (type_)
                {
                    case type::unsigned_integral:
                        format_(unsigned_integral_, format_string, ctx, append_to,
                                promise::no_overlap);
                        break;

                    case type::signed_integral:
                        format_(signed_integral_, format_string, ctx, append_to,
                                promise::no_overlap);
                        break;

#if SNN_INT128_ENABLED
                    case type::unsigned_128bit_integral:
                        format_(unsigned_128bit_integral_, format_string, ctx, append_to,
                                promise::no_overlap);
                        break;

                    case type::signed_128bit_integral:
                        format_(signed_128bit_integral_, format_string, ctx, append_to,
                                promise::no_overlap);
                        break;
#endif

                    case type::boolean:
                        format_(static_cast<bool>(character_), format_string, ctx, append_to,
                                promise::no_overlap);
                        break;

                    case type::character:
                        format_(character_, format_string, ctx, append_to, promise::no_overlap);
                        break;

                    case type::string:
                        format_(string_, format_string, ctx, append_to, promise::no_overlap);
                        break;

                    case type::other:
                        other_.format_func_ptr(other_.value_ptr, format_string, ctx, append_to,
                                               promise::no_overlap);
                        break;
                }
            }

            constexpr cstrview string() const
            {
                if (type_ == type::string)
                {
                    return string_;
                }
                throw_or_abort(error::referenced_argument_must_be_a_string);
            }

            constexpr u64 unsigned_integral(const u64 max_inclusive) const
            {
                if (type_ == type::unsigned_integral)
                {
                    if (unsigned_integral_ <= max_inclusive)
                    {
                        return unsigned_integral_;
                    }
                    throw_or_abort(error::integral_argument_is_too_large);
                }
                else if (type_ == type::signed_integral)
                {
                    if (signed_integral_ >= 0)
                    {
                        const u64 u = to_u64(signed_integral_);
                        if (u <= max_inclusive)
                        {
                            return u;
                        }
                        throw_or_abort(error::integral_argument_is_too_large);
                    }
                    throw_or_abort(error::integral_argument_is_negative);
                }
                throw_or_abort(error::referenced_argument_must_be_an_integral);
            }

          private:
            struct other final
            {
                const void* value_ptr;
                void (*format_func_ptr)(const void*, cstrview, const context&, strcore<Buf>&,
                                        promise::no_overlap_t);
            };

            union
            {
                cstrview string_;
#if SNN_INT128_ENABLED
                u128 unsigned_128bit_integral_;
                i128 signed_128bit_integral_;
#endif
                other other_;
                u64 unsigned_integral_;
                i64 signed_integral_;
                char character_;
            };
            type type_;

            template <typename T>
            static constexpr void format_(const T v, const cstrview format_string,
                                          const fmt::context& ctx, strcore<Buf>& append_to,
                                          promise::no_overlap_t)
            {
                formatter_select_t<T> f;
                f.format(v, format_string, ctx, append_to, promise::no_overlap);
            }

            template <typename T>
            static constexpr void format_other_(const void* const value_ptr,
                                                const cstrview format_string,
                                                const fmt::context& ctx, strcore<Buf>& append_to,
                                                promise::no_overlap_t)
            {
                const T& v = *static_cast<const T*>(value_ptr);
                formatter_select_t<T> f;
                f.format(v, format_string, ctx, append_to, promise::no_overlap);
            }
        };

        enum class alignment : u8
        {
            none = 0,
            left,
            right,
            center,
        };

        constexpr alignment to_alignment(const char c) noexcept
        {
            switch (c)
            {
                case '<':
                    return alignment::left;
                case '>':
                    return alignment::right;
                case '^':
                    return alignment::center;
                default:
                    return alignment::none;
            }
        }

        constexpr usize pop_index(cstrrng& rng, usize& position_index)
        {
            // '{' has already been skipped.

            if (rng.drop_front('}'))
            {
                const usize index = position_index;
                ++position_index;
                return index;
            }

            const auto index_value_count = rng.pop_front_integral<usize, math::base::decimal, 2>();
            if (index_value_count.count && rng.drop_front('}'))
            {
                return index_value_count.value;
            }

            if (rng.is_empty())
            {
                throw_or_abort(error::unexpected_end_of_format_string);
            }

            if (rng.has_front_if(chr::is_digit))
            {
                throw_or_abort(error::invalid_index);
            }

            throw_or_abort(error::unexpected_character_in_format_string);
        }

        template <typename Buf>
        constexpr void format_single(cstrrng& rng, usize& position_index,
                                     const array_view<const argument<Buf>> arguments,
                                     const context& ctx, strcore<Buf>& append_to,
                                     promise::no_overlap_t)
        {
            // '{' has already been skipped.

            cstrview format_string;
            cstrview fill_string;

            alignment align = alignment::left;

            usize index = 0;
            usize width = 0;

            constexpr usize max_width = 9999;

            // Index
            const auto index_value_count = rng.pop_front_integral<usize, math::base::decimal, 2>();
            if (index_value_count.count)
            {
                index = index_value_count.value;
            }
            else
            {
                index = position_index;
                ++position_index;
            }

            // Width, alignment & fill.
            if (rng.drop_front(','))
            {
                // Width
                if (rng.drop_front('{'))
                {
                    const usize width_index = pop_index(rng, position_index);
                    if (width_index < arguments.count())
                    {
                        const auto& arg = arguments.at(width_index, promise::within_bounds);
                        width           = arg.unsigned_integral(max_width);
                    }
                    else
                    {
                        throw_or_abort(error::argument_index_out_of_bounds);
                    }
                }
                else
                {
                    const auto width_value_count =
                        rng.pop_front_integral<usize, math::base::decimal, 4>();
                    if (width_value_count.count && !rng.has_front_if(chr::is_digit))
                    {
                        width = width_value_count.value;
                    }
                    else
                    {
                        throw_or_abort(error::invalid_width_in_format_string);
                    }
                }

                // Alignment
                if (rng)
                {
                    align = to_alignment(rng.front(promise::not_empty));
                    if (align != alignment::none)
                    {
                        rng.drop_front(promise::not_empty);

                        // Fill
                        if (rng.drop_front('{'))
                        {
                            const usize fill_index = pop_index(rng, position_index);
                            if (fill_index < arguments.count())
                            {
                                const auto& arg = arguments.at(fill_index, promise::within_bounds);
                                fill_string     = arg.string();
                            }
                            else
                            {
                                throw_or_abort(error::argument_index_out_of_bounds);
                            }
                        }
                        else
                        {
                            constexpr auto is_fill_char = fn::_not{fn::in_array{':', '}'}};
                            fill_string = cstrview{rng.pop_front_while(is_fill_char)};
                        }
                    }
                    else
                    {
                        align = alignment::left;
                    }
                }
            }

            // Type format string.
            if (rng.drop_front(':'))
            {
                if (rng.drop_front('{'))
                {
                    const usize format_index = pop_index(rng, position_index);
                    if (format_index < arguments.count())
                    {
                        const auto& arg = arguments.at(format_index, promise::within_bounds);
                        format_string   = arg.string();
                    }
                    else
                    {
                        throw_or_abort(error::argument_index_out_of_bounds);
                    }
                }
                else
                {
                    format_string = cstrview{rng.pop_front_while(fn::is{fn::not_equal_to{}, '}'})};
                }
            }

            if (rng.drop_front('}'))
            {
                if (index < arguments.count())
                {
                    const auto& arg = arguments.at(index, promise::within_bounds);

                    if (width == 0)
                    {
                        arg.format(format_string, ctx, append_to, promise::no_overlap);
                    }
                    else
                    {
                        strcore<Buf> align_buf;

                        arg.format(format_string, ctx, align_buf, promise::no_overlap);

                        if (fill_string.is_empty())
                        {
                            fill_string = " ";
                        }

                        switch (align)
                        {
                            case alignment::none:
                                snn_should(false && "Should never happen.");
                                break;

                            case alignment::left:
                                utf8::pad_right_inplace(align_buf, width, fill_string,
                                                        promise::no_overlap);
                                break;

                            case alignment::right:
                                utf8::pad_left_inplace(align_buf, width, fill_string,
                                                       promise::no_overlap);
                                break;

                            case alignment::center:
                                utf8::pad_inplace(align_buf, width, fill_string,
                                                  promise::no_overlap);
                                break;
                        }

                        append_to.append(align_buf);
                    }
                }
                else
                {
                    throw_or_abort(error::argument_index_out_of_bounds);
                }
            }
            else if (rng.is_empty())
            {
                throw_or_abort(error::unexpected_end_of_format_string);
            }
            else if (rng.has_front_if(chr::is_digit))
            {
                throw_or_abort(error::invalid_index);
            }
            else
            {
                throw_or_abort(error::unexpected_character_in_format_string);
            }
        }

        template <typename Buf>
        constexpr void format(const cstrview string,
                              const array_view<const argument<Buf>> arguments, const context& ctx,
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            usize position_index = 0;

            auto rng = string.range();
            while (rng)
            {
                const auto skipped_rng = rng.pop_front_while(fn::_not{fn::in_array{'{', '}'}});
                if (skipped_rng)
                {
                    append_to.append(cstrview{skipped_rng});
                }

                if (rng.drop_front("{}"))
                {
                    if (position_index < arguments.count())
                    {
                        const auto& arg = arguments.at(position_index, promise::within_bounds);
                        arg.format("", ctx, append_to, promise::no_overlap);
                    }
                    else
                    {
                        throw_or_abort(error::argument_index_out_of_bounds);
                    }
                    ++position_index;
                }
                else if (rng.drop_front("{{"))
                {
                    append_to.append('{');
                }
                else if (rng.drop_front("}}"))
                {
                    append_to.append('}');
                }
                else if (rng.drop_front('{'))
                {
                    format_single(rng, position_index, arguments, ctx, append_to,
                                  promise::no_overlap);
                }
                else if (!rng.is_empty())
                {
                    throw_or_abort(error::unexpected_character_in_format_string);
                }
            }
        }
    }

    // ## Functions

    // ### format

    template <any_strcore Str = str, typename... Args>
    [[nodiscard]] constexpr Str format(const transient<cstrview> string, const Args&... args)
    {
        Str append_to;
        using argument_type = detail::argument<typename Str::buffer_type>;
        const array<argument_type, sizeof...(Args)> arguments{argument_type{args}...};
        context ctx;
        detail::format(string.get(), arguments.view(), ctx, append_to, promise::no_overlap);
        return append_to;
    }

    // ### format_append

    template <typename Buf, typename... Args>
    constexpr void format_append(const transient<cstrview> string, strcore<Buf>& append_to,
                                 promise::no_overlap_t, const Args&... args)
    {
        snn_should(std::is_constant_evaluated() || !string.get().overlaps(append_to));
        using argument_type = detail::argument<Buf>;
        const array<argument_type, sizeof...(Args)> arguments{argument_type{args}...};
        context ctx;
        detail::format(string.get(), arguments.view(), ctx, append_to, promise::no_overlap);
    }
}
