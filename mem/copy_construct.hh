// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Copy objects to an uninitialized address

#pragma once

#include "snn-core/mem/construct.hh"
#include "snn-core/mem/destruct.hh"
#include "snn-core/mem/raw/copy.hh"

namespace snn::mem
{
    // ## Functions

    // ### copy_construct

    template <typename T>
        requires(std::is_copy_constructible_v<T>)
    constexpr void copy_construct(
        const not_null<const T*> first_copy, const not_null<const T*> last_copy,
        const not_null<T*> first_uninitialized,
        promise::no_overlap_t) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        const T* cur  = first_copy.get();
        const T* last = last_copy.get();

        snn_should(cur <= last);

        if (!std::is_constant_evaluated() && std::is_trivially_copyable_v<T>)
        {
            const auto count = static_cast<usize>(last - cur);
            mem::raw::copy(first_copy, first_uninitialized, byte_size{count * sizeof(T)},
                           promise::no_overlap);
        }
        else
        {
            T* cur_uninit = first_uninitialized.get();
            while (cur != last)
            {
                if constexpr (std::is_nothrow_copy_constructible_v<T>)
                {
                    mem::construct(not_null{cur_uninit}, *cur);
                }
                else
                {
                    try
                    {
                        mem::construct(not_null{cur_uninit}, *cur);
                    }
                    catch (...)
                    {
                        mem::destruct(first_uninitialized.get(), cur_uninit);
                        throw;
                    }
                }

                ++cur;
                ++cur_uninit;
            }
        }
    }
}
