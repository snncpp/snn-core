// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encoder

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/json/document.hh"
#include "snn-core/json/encode.hh"
#include "snn-core/json/node.hh"
#include "snn-core/json/type.hh"

namespace snn::json
{
    // ## Classes

    // ### encoder

    class encoder final
    {
      public:
        static constexpr usize default_reserve_capacity = 256;

        template <typename Buf>
        constexpr void encode(const document& doc, strcore<Buf>& append_to, assume::no_overlap_t)
        {
            encode_(doc.root(), append_to);
        }

        template <any_strcore Str = str>
        [[nodiscard]] constexpr Str encode(const document& doc,
                                           const usize reserve_capacity = default_reserve_capacity)
        {
            Str append_to{init::reserve, reserve_capacity};
            encode_(doc.root(), append_to);
            return append_to;
        }

        template <typename Buf>
        constexpr void pretty_encode(const document& doc, strcore<Buf>& append_to,
                                     assume::no_overlap_t)
        {
            constexpr usize depth = 0;
            pretty_encode_(doc.root(), append_to, depth);
        }

        template <any_strcore Str = str>
        [[nodiscard]] constexpr Str pretty_encode(
            const document& doc, const usize reserve_capacity = default_reserve_capacity)
        {
            Str append_to{init::reserve, reserve_capacity};
            constexpr usize depth = 0;
            pretty_encode_(doc.root(), append_to, depth);
            return append_to;
        }

        [[nodiscard]] constexpr json::option options() const noexcept
        {
            return options_;
        }

        constexpr void set_options(const json::option options) noexcept
        {
            options_ = options;
        }

      private:
        json::option options_ = json::option::none;

        template <typename Str>
        constexpr void encode_(const node& n, Str& append_to)
        {
            switch (n.type())
            {
                case type::boolean_false:
                case type::boolean_true:
                case type::floating_point:
                case type::null:
                case type::signed_integral:
                case type::unsigned_integral:
                    append_to << n.to<cstrview>();
                    break;

                case type::array:
                    encode_array_(n, append_to);
                    break;

                case type::object:
                    encode_object_(n, append_to);
                    break;

                case type::string:
                    json::encode(n.to<cstrview>(), options_, append_to, assume::no_overlap);
                    break;

                case type::empty:
                    break;
            }
        }

        template <typename Str>
        constexpr void pretty_encode_(const node& n, Str& append_to, const usize depth)
        {
            switch (n.type())
            {
                case type::boolean_false:
                case type::boolean_true:
                case type::floating_point:
                case type::null:
                case type::signed_integral:
                case type::unsigned_integral:
                    append_to << n.to<cstrview>();
                    break;

                case type::array:
                    encode_array_pretty_(n, append_to, depth);
                    break;

                case type::object:
                    encode_object_pretty_(n, append_to, depth);
                    break;

                case type::string:
                    json::encode(n.to<cstrview>(), options_, append_to, assume::no_overlap);
                    break;

                case type::empty:
                    break;
            }
        }

        template <typename Str>
        constexpr void encode_array_(const node& node_parent, Str& append_to)
        {
            append_to << '[';

            auto rng = node_parent.range();
            if (rng)
            {
                const node& node_first = rng.front(promise::not_empty);
                rng.drop_front(promise::not_empty);

                encode_(node_first, append_to);

                for (const node& n : rng)
                {
                    append_to << ',';
                    encode_(n, append_to);
                }
            }

            append_to << ']';
        }

        template <typename Str>
        constexpr void encode_array_pretty_(const node& node_parent, Str& append_to, usize depth)
        {
            auto rng = node_parent.range();
            if (rng)
            {
                const node& node_first = rng.front(promise::not_empty);
                rng.drop_front(promise::not_empty);

                append_to << "[\n";
                ++depth;
                append_to.append_for_overwrite(2 * depth).fill(' ');

                pretty_encode_(node_first, append_to, depth);

                for (const node& n : rng)
                {
                    append_to << ",\n";
                    append_to.append_for_overwrite(2 * depth).fill(' ');
                    pretty_encode_(n, append_to, depth);
                }

                append_to << '\n';
                --depth;
                append_to.append_for_overwrite(2 * depth).fill(' ');
                append_to << ']';
            }
            else
            {
                append_to << "[]";
            }
        }

        template <typename Str>
        constexpr void encode_object_(const node& node_parent, Str& append_to)
        {
            append_to << '{';

            auto rng = node_parent.range();
            if (rng)
            {
                const node& node_first = rng.front(promise::not_empty);
                rng.drop_front(promise::not_empty);

                // Key
                json::encode(node_first.to<cstrview>(), options_, append_to, assume::no_overlap);
                append_to << ':';
                // Value
                encode_(node_first.child(), append_to);

                for (const node& n : rng)
                {
                    append_to << ',';
                    // Key
                    json::encode(n.to<cstrview>(), options_, append_to, assume::no_overlap);
                    append_to << ':';
                    // Value
                    encode_(n.child(), append_to);
                }
            }

            append_to << '}';
        }

        template <typename Str>
        constexpr void encode_object_pretty_(const node& node_parent, Str& append_to, usize depth)
        {
            auto rng = node_parent.range();
            if (rng)
            {
                const node& node_first = rng.front(promise::not_empty);
                rng.drop_front(promise::not_empty);

                append_to << "{\n";
                ++depth;
                append_to.append_for_overwrite(2 * depth).fill(' ');

                // Key
                json::encode(node_first.to<cstrview>(), options_, append_to, assume::no_overlap);
                append_to << ": ";
                // Value
                pretty_encode_(node_first.child(), append_to, depth);

                for (const node& n : rng)
                {
                    append_to << ",\n";
                    append_to.append_for_overwrite(2 * depth).fill(' ');
                    // Key
                    json::encode(n.to<cstrview>(), options_, append_to, assume::no_overlap);
                    append_to << ": ";
                    // Value
                    pretty_encode_(n.child(), append_to, depth);
                }

                append_to << '\n';
                --depth;
                append_to.append_for_overwrite(2 * depth).fill(' ');
                append_to << '}';
            }
            else
            {
                append_to << "{}";
            }
        }
    };
}
