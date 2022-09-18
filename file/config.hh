// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Config file parser

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/vec.hh"
#include "snn-core/ascii/trim.hh"
#include "snn-core/file/read.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/range/view/reverse.hh"
#include "snn-core/string/range/split.hh"

namespace snn::file
{
    // ## Classes

    // ### config

    class config final
    {
      public:
        config(const transient<null_term<const char*>> path)
            : v_{container::reserve, 10}
        {
            parse_(path);
        }

        explicit operator bool() const noexcept
        {
            return !v_.is_empty();
        }

        // get<String>
        template <constructible_from<const str&> String>
        [[nodiscard]] String get(const transient<cstrview> key, const String def = String{}) const
        {
            for (const auto& kv : range::view::reverse{v_.range()})
            {
                if (kv.key == key.get())
                {
                    return kv.value;
                }
            }
            return def;
        }

        // get<Int>
        template <strict_integral Int>
        [[nodiscard]] Int get(const transient<cstrview> key, const Int def = 0) const
        {
            for (const auto& kv : range::view::reverse{v_.range()})
            {
                if (kv.key == key.get())
                {
                    const auto p = kv.value.template to_prefix<Int>();
                    if (p.count == kv.value.size())
                    {
                        return p.value;
                    }

                    break;
                }
            }
            return def;
        }

        template <any_strcore Str>
        [[nodiscard]] Str to() const
        {
            Str s{container::reserve, v_.count() * 30}; // Only an estimate.
            for (const auto& kv : v_)
            {
                s << kv.key << ": " << kv.value << '\n';
            }
            return s;
        }

      private:
        vec<pair::key_value<str>> v_;

        void parse_(const transient<null_term<const char*>> path)
        {
            auto contents = file::read<strbuf>(path).value();
            for (cstrview line : string::range::split{contents, '\n'})
            {
                ascii::trim_inplace(line);
                if (line && !line.has_front('#'))
                {
                    const usize space_pos = line.find(' ').value_or_npos();
                    if (space_pos != constant::npos)
                    {
                        cstrview key   = line.view(0, space_pos);
                        cstrview value = line.view(space_pos);
                        ascii::trim_inplace(key);
                        ascii::trim_inplace(value);
                        if (key && value)
                        {
                            v_.append_inplace(key, value);
                        }
                    }
                }
            }
        }
    };
}
