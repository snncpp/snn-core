// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/result.hh"
#include "snn-core/array_view.hh"
#include "snn-core/json/error.hh"
#include "snn-core/json/node.hh"
#include "snn-core/json/chr/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/pool/append_only.hh"
#include "snn-core/range/unchecked/contiguous.hh"
#include "snn-core/utf8/core.hh"
#include "snn-core/utf8/is_valid.hh"

namespace snn::json::detail
{
    class decoder final
    {
      public:
        constexpr decoder(const strrng rng, pool::append_only<node>& pool, const u32 node_limit,
                          const u16 depth_limit) noexcept
            : rng_{rng},
              first_{rng_.begin()},
              pool_{pool},
              node_count_{0},
              node_limit_{node_limit},
              depth_limit_{depth_limit}
        {
        }

        // Non-copyable
        decoder(const decoder&)            = delete;
        decoder& operator=(const decoder&) = delete;

        // Non-movable
        decoder(decoder&&)            = delete;
        decoder& operator=(decoder&&) = delete;

        [[nodiscard]] constexpr usize byte_position() const noexcept
        {
            return static_cast<usize>(rng_.begin() - first_);
        }

        [[nodiscard]] constexpr result<node&> decode_inplace()
        {
            constexpr u32 depth = 0;

            auto r = decode_recursive_(depth);
            if (!r) return r.error_code();
            node& root = r.value(promise::has_value);

            rng_.pop_front_while(json::chr::is_whitespace);
            if (rng_.is_empty())
            {
                return root;
            }

            return error::trailing_characters;
        }

      private:
        strrng rng_;
        char* first_;
        pool::append_only<node>& pool_;
        u32 node_count_;
        u32 node_limit_;
        u16 depth_limit_;

        [[nodiscard]] constexpr error eof_error_or_(error e) const noexcept
        {
            if (!rng_)
            {
                e = error::unexpected_eof;
            }
            return e;
        }

        template <typename... Args>
        [[nodiscard]] constexpr result<node&> make_node_(Args&&... args)
        {
            if (node_count_ >= node_limit_) [[unlikely]]
            {
                return error::node_limit;
            }
            ++node_count_;
            return pool_.append_inplace(std::forward<Args>(args)...);
        }

        [[nodiscard]] constexpr result<node&> decode_recursive_(const u32 depth)
        {
            if (depth > depth_limit_) [[unlikely]]
            {
                return error::depth_limit;
            }

            rng_.pop_front_while(json::chr::is_whitespace);

            if (rng_)
            {
                const char c = rng_.front(promise::not_empty);

                switch (c)
                {
                    // True
                    case 't':
                    {
                        const auto initial_rng = rng_;
                        if (rng_.drop_front("true"))
                        {
                            return make_node_(
                                cstrview{initial_rng.without_suffix(rng_, promise::is_valid)},
                                type::boolean_true);
                        }
                        break;
                    }

                    // False
                    case 'f':
                    {
                        const auto initial_rng = rng_;
                        rng_.drop_front(promise::not_empty);
                        if (rng_.drop_front("alse"))
                        {
                            return make_node_(
                                cstrview{initial_rng.without_suffix(rng_, promise::is_valid)},
                                type::boolean_false);
                        }
                        break;
                    }

                    // Null
                    case 'n':
                    {
                        const auto initial_rng = rng_;
                        if (rng_.drop_front("null"))
                        {
                            return make_node_(
                                cstrview{initial_rng.without_suffix(rng_, promise::is_valid)},
                                type::null);
                        }
                        break;
                    }

                    // String
                    case '"':
                        rng_.drop_front(promise::not_empty);
                        return decode_string_();

                    // Array
                    case '[':
                        rng_.drop_front(promise::not_empty);
                        return decode_array_(depth);

                    // Object
                    case '{':
                        rng_.drop_front(promise::not_empty);
                        return decode_object_(depth);

                    // Number
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '-':
                        return decode_number_();
                }
            }

            return eof_error_or_(error::unexpected_character);
        }

        [[nodiscard]] constexpr result<node&> decode_array_(const u32 depth)
        {
            auto p = make_node_(type::array);
            if (!p) return p.error_code();
            node& parent = p.value(promise::has_value);

            rng_.pop_front_while(json::chr::is_whitespace);

            if (rng_.drop_front(']'))
            {
                return parent;
            }

            do
            {
                auto n = decode_recursive_(depth + 1);
                if (!n) return n.error_code();
                parent.append(n.value(promise::has_value));

                rng_.pop_front_while(json::chr::is_whitespace);
            } while (rng_.drop_front(','));

            if (rng_.drop_front(']'))
            {
                return parent;
            }

            return eof_error_or_(error::invalid_array);
        }

        [[nodiscard]] constexpr result<range::unchecked::contiguous<char*>> decode_codepoint_(
            range::unchecked::contiguous<char*> write_rng) noexcept
        {
            using ascii::leading_zeros;
            using math::base;

            auto rng = rng_.pop_front_n(4);
            auto p   = rng.pop_front_integral<u32, base::hex, 4>(leading_zeros::allow);
            if (p.count == 4)
            {
                u32 cp = p.value;

                if (unicode::is_leading_surrogate(cp))
                {
                    // Second half of surrogate pair (low).
                    if (rng_.drop_front(R"(\u)"))
                    {
                        rng = rng_.pop_front_n(4);
                        p   = rng.pop_front_integral<u32, base::hex, 4>(leading_zeros::allow);
                        if (p.count == 4 && unicode::is_trailing_surrogate(p.value))
                        {
                            cp = unicode::decode_surrogate_pair(cp, p.value);
                        }
                        else
                        {
                            return eof_error_or_(error::invalid_surrogate_pair);
                        }
                    }
                    else
                    {
                        return eof_error_or_(error::invalid_surrogate_pair);
                    }
                }

                if (utf8::is_valid(cp))
                {
                    snn_should((write_rng.end() - write_rng.begin()) >= 4);
                    return range::unchecked::contiguous<char*>{
                        meta::iterators,
                        utf8::encode_up_to_4_bytes(cp, write_rng.begin(), promise::is_valid),
                        write_rng.end()};
                }
            }

            return eof_error_or_(error::invalid_codepoint_escape);
        }

        [[nodiscard]] constexpr result<node&> decode_number_()
        {
            snn_should(rng_.has_front('-') || rng_.has_front_if(json::chr::is_digit));

            const auto initial_rng = rng_;
            type t                 = type::unsigned_integral;

            if (rng_.drop_front('-'))
            {
                t = type::signed_integral;
            }

            // Leading zero or one or more digits.
            if (rng_.drop_front('0') || rng_.pop_front_while(json::chr::is_digit))
            {
                // Float
                if (rng_.has_front_if(fn::in_array{'.', 'e', 'E'}))
                {
                    t = type::floating_point;

                    // Fraction
                    if (rng_.drop_front('.'))
                    {
                        if (!rng_.pop_front_while(json::chr::is_digit))
                        {
                            return eof_error_or_(error::invalid_number);
                        }
                    }

                    // Exponent
                    if (rng_.drop_front_if(fn::in_array{'e', 'E'}))
                    {
                        rng_.drop_front_if(fn::in_array{'+', '-'});

                        if (!rng_.pop_front_while(json::chr::is_digit))
                        {
                            return eof_error_or_(error::invalid_number);
                        }
                    }
                }

                return make_node_(cstrview{initial_rng.without_suffix(rng_, promise::is_valid)}, t);
            }

            return eof_error_or_(error::invalid_number);
        }

        [[nodiscard]] constexpr result<node&> decode_object_(const u32 depth)
        {
            auto p = make_node_(type::object);
            if (!p) return p.error_code();
            node& parent = p.value(promise::has_value);

            rng_.pop_front_while(json::chr::is_whitespace);

            if (rng_.drop_front('}'))
            {
                return parent;
            }

            while (true)
            {
                if (rng_.drop_front('"'))
                {
                    auto k = decode_string_();
                    if (!k) return k.error_code();
                    node& key = k.value(promise::has_value);

                    rng_.pop_front_while(json::chr::is_whitespace);

                    if (rng_.drop_front(':'))
                    {
                        auto v = decode_recursive_(depth + 1);
                        if (!v) return v.error_code();
                        parent.append(key, v.value(promise::has_value));

                        rng_.pop_front_while(json::chr::is_whitespace);

                        if (rng_.drop_front(','))
                        {
                            rng_.pop_front_while(json::chr::is_whitespace);
                            continue;
                        }
                        else if (rng_.drop_front('}'))
                        {
                            return parent;
                        }
                    }
                }

                break;
            }

            return eof_error_or_(error::invalid_object);
        }

        [[nodiscard]] constexpr result<node&> decode_string_()
        {
            const auto initial_rng = rng_;

            // Optimization for ascii strings that need no decoding or validation.
            rng_.pop_front_while(json::chr::is_non_special_ascii_string);

            if (rng_.has_front('"'))
            {
                const cstrview s{initial_rng.without_suffix(rng_, promise::is_valid)};
                rng_.drop_front(promise::not_empty);
                return make_node_(s);
            }

            // Decoding required or incomplete string.

            rng_.pop_front_while(fn::_not{json::chr::is_special_string});

            range::unchecked::contiguous write_rng{meta::iterators, rng_.begin(), rng_.end()};
            while (rng_)
            {
                snn_should(rng_.has_front_if(json::chr::is_special_string));

                const char c = rng_.pop_front(promise::not_empty);

                if (to_byte(c) < ' ') [[unlikely]]
                {
                    // Control characters U+0000 to U+001F are not allowed.
                    return error::unescaped_control_character;
                }

                snn_should(c == '"' || c == '\\');

                // Quotation mark.
                if (c == '"')
                {
                    const cstrview s{initial_rng.without_suffix(write_rng, promise::is_valid)};
                    if (utf8::is_valid(s))
                    {
                        return make_node_(s);
                    }
                    return error::invalid_utf8;
                }

                // Backslash
                if (rng_)
                {
                    const char escaped = rng_.pop_front(promise::not_empty);

                    switch (escaped)
                    {
                        case '"':
                        case '\\':
                        case '/':
                            write_rng.pop_front(promise::not_empty) = escaped;
                            break;

                        case 'b':
                            write_rng.pop_front(promise::not_empty) = '\b';
                            break;

                        case 'f':
                            write_rng.pop_front(promise::not_empty) = '\f';
                            break;

                        case 'n':
                            write_rng.pop_front(promise::not_empty) = '\n';
                            break;

                        case 'r':
                            write_rng.pop_front(promise::not_empty) = '\r';
                            break;

                        case 't':
                            write_rng.pop_front(promise::not_empty) = '\t';
                            break;

                        case 'u':
                        {
                            auto r = decode_codepoint_(write_rng);
                            if (r)
                            {
                                write_rng = r.value(promise::has_value);
                            }
                            else
                            {
                                return r.error_code();
                            }
                            break;
                        }

                        default:
                            return error::invalid_backslash_escape;
                    }
                }
                else
                {
                    break;
                }

                rng_.pop_front_while(fn::_not{json::chr::is_special_string},
                                     [&write_rng](const char non_special) {
                                         write_rng.pop_front(promise::not_empty) = non_special;
                                     });
            }

            return error::unexpected_eof;
        }
    };
}
