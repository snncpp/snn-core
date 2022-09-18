// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Node

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/strcore.fwd.hh"
#include "snn-core/json/is_floating_point.hh"
#include "snn-core/json/is_integral.hh"
#include "snn-core/json/type.hh"
#include "snn-core/range/forward.hh"
#include "snn-core/range/view/enumerate.hh"

namespace snn::json
{
    // ## Classes

    // ### node

    class node final
    {
      public:
        class iterator final
        {
          public:
            using difference_type   = iptrdiff;
            using value_type        = node;
            using pointer           = const node*;
            using reference         = const node&;
            using iterator_category = std::forward_iterator_tag;

            constexpr explicit iterator(const node* const child) noexcept
                : child_{child}
            {
            }

            constexpr const node& operator*() const noexcept
            {
                snn_should(child_ != nullptr);
                return *child_;
            }

            constexpr iterator& operator++() noexcept
            {
                snn_should(child_ != nullptr);
                child_ = child_->sibling_next_;
                return *this;
            }

            constexpr bool operator==(const iterator& other) const noexcept
            {
                return child_ == other.child_;
            }

          private:
            const node* child_;
        };

        constexpr explicit node() noexcept
            : val_{},
              type_{type::empty}
        {
        }

        constexpr explicit node(const enum type t) noexcept
            : val_{},
              type_{t}
        {
            snn_should(type_ == type::empty || type_ == type::array || type_ == type::object);
        }

        constexpr explicit node(const cstrview val, const enum type t = type::string) noexcept
            : val_{val},
              type_{t}
        {
            snn_should(is_valid_(val_, type_));
        }

        constexpr ~node() = default;

        // Non-copyable
        node(const node&)            = delete;
        node& operator=(const node&) = delete;

        // Non-movable
        node(node&&)            = delete;
        node& operator=(node&&) = delete;

        [[nodiscard]] constexpr const node& get(const usize index) const noexcept
        {
            for (const auto p : range::view::enumerate{range()})
            {
                if (p.index == index)
                {
                    return p.value;
                }
            }
            return empty_node_();
        }

        [[nodiscard]] constexpr const node& get(const cstrview key) const noexcept
        {
            for (const node& n : range())
            {
                if (n.is_string() && n.val_ == key)
                {
                    return n.child();
                }
            }
            return empty_node_();
        }

        template <strict_integral Int, math::base Base = math::base::decimal>
        [[nodiscard]] constexpr optional<Int> to() const noexcept
        {
            return val_.to<Int, Base>();
        }

        template <same_as<cstrview> CStrView>
        [[nodiscard]] constexpr CStrView to() const noexcept
        {
            return val_;
        }

        template <any_strcore Str>
        [[nodiscard]] constexpr Str to() const
        {
            return Str{val_};
        }

        [[nodiscard]] constexpr iterator begin() const noexcept
        {
            return iterator{child_first_};
        }

        [[nodiscard]] constexpr iterator end() const noexcept
        {
            return iterator{nullptr};
        }

        [[nodiscard]] constexpr iterator cbegin() const noexcept
        {
            return begin();
        }

        [[nodiscard]] constexpr iterator cend() const noexcept
        {
            return end();
        }

        [[nodiscard]] constexpr const node& child() const noexcept
        {
            if (child_first_ != nullptr)
            {
                return *child_first_;
            }
            return empty_node_();
        }

        [[nodiscard]] constexpr enum type type() const noexcept
        {
            return type_;
        }

        [[nodiscard]] constexpr bool is_array() const noexcept
        {
            return type_ == type::array;
        }

        [[nodiscard]] constexpr bool is_boolean() const noexcept
        {
            return type_ == type::boolean_false || type_ == type::boolean_true;
        }

        [[nodiscard]] constexpr bool is_false() const noexcept
        {
            return type_ == type::boolean_false;
        }

        [[nodiscard]] constexpr bool is_true() const noexcept
        {
            return type_ == type::boolean_true;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return type_ == type::empty;
        }

        [[nodiscard]] constexpr bool is_floating_point() const noexcept
        {
            return type_ == type::floating_point;
        }

        [[nodiscard]] constexpr bool is_integral() const noexcept
        {
            return type_ == type::signed_integral || type_ == type::unsigned_integral;
        }

        [[nodiscard]] constexpr bool is_null() const noexcept
        {
            return type_ == type::null;
        }

        [[nodiscard]] constexpr bool is_number() const noexcept
        {
            return type_ == type::floating_point || type_ == type::signed_integral ||
                   type_ == type::unsigned_integral;
        }

        [[nodiscard]] constexpr bool is_object() const noexcept
        {
            return type_ == type::object;
        }

        [[nodiscard]] constexpr bool is_signed_integral() const noexcept
        {
            return type_ == type::signed_integral;
        }

        [[nodiscard]] constexpr bool is_string() const noexcept
        {
            return type_ == type::string;
        }

        [[nodiscard]] constexpr bool is_unsigned_integral() const noexcept
        {
            return type_ == type::unsigned_integral;
        }

        [[nodiscard]] constexpr bool has_children() const noexcept
        {
            return child_first_ != nullptr;
        }

        constexpr void append(node& n) noexcept
        {
            snn_should(type_ == type::array);
            append_(&n);
        }

        constexpr void append(node& key, node& value) noexcept
        {
            snn_should(type_ == type::object);
            snn_should(key.is_string());
            key.child_first_ = &value;
            key.child_last_  = &value;
            append_(&key);
        }

        [[nodiscard]] constexpr range::forward<iterator> range() const noexcept
        {
            return range::forward<iterator>{meta::iterators, cbegin(), cend()};
        }

      private:
        cstrview val_;
        node* sibling_next_ = nullptr;
        node* child_first_  = nullptr;
        node* child_last_   = nullptr;
        enum type type_;

        constexpr void append_(node* const n) noexcept
        {
            if (child_last_ == nullptr)
            {
                child_first_ = n;
                child_last_  = n;
            }
            else
            {
                child_last_->sibling_next_ = n;
                child_last_                = n;
            }
        }

        constexpr const node& empty_node_() const noexcept
        {
            return constant::value_initialized<node>;
        }

#if SNN_SHOULD_BOOL
        static constexpr bool is_valid_(const cstrview s, const enum type t) noexcept
        {
            return t == type::string ||
                   (t == type::unsigned_integral && json::is_unsigned_integral(s)) ||
                   (t == type::signed_integral && json::is_signed_integral(s)) ||
                   (t == type::boolean_false && s == "false") ||
                   (t == type::boolean_true && s == "true") || (t == type::null && s == "null") ||
                   (t == type::floating_point && json::is_floating_point(s));
        }
#endif
    };
}
