// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/trivial_allocator.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_trivial_allocator()
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            {
                mem::trivial_allocator<int> alloc;

                constexpr usize count = 3;

                int* ptr = alloc.allocate(not_zero{count}).value();

                ptr[0] = 123;
                ptr[1] = 456;
                ptr[2] = 789;

                alloc.deallocate(ptr, count);
            }
            {
                mem::trivial_allocator<int> alloc;

                constexpr usize count = 3;

                // Allocate using `reallocate`.
                int* ptr = alloc.reallocate(nullptr, 0, not_zero{count}, 0).value();

                ptr[0] = 123;
                ptr[1] = 456;
                ptr[2] = 789;

                alloc.deallocate(ptr, count);
            }
            {
                mem::trivial_allocator<int> alloc;

                constexpr usize initial_count = 3;

                int* ptr = alloc.allocate(not_zero{initial_count}).value();

                ptr[0] = 123;
                ptr[1] = 456;
                // ptr[2] is not initialized.

                constexpr usize use_count   = 2;
                constexpr usize final_count = 5;

                // Grow allocation.
                auto opt = alloc.reallocate(ptr, initial_count, not_zero{final_count}, use_count);

                snn_require(opt);
                ptr = opt.value();

                snn_require(ptr[0] == 123);
                snn_require(ptr[1] == 456);

                ptr[2] = 789;
                ptr[3] = 333;
                ptr[4] = 444;

                alloc.deallocate(ptr, final_count);
            }
            {
                mem::trivial_allocator<int> alloc;

                constexpr usize initial_count = 4;

                int* ptr = alloc.allocate(not_zero{initial_count}).value();

                ptr[0] = 123;
                ptr[1] = 456;
                ptr[2] = 789;
                // ptr[3] is not initialized.

                constexpr usize use_count   = 3;
                constexpr usize final_count = 2;

                // Shrink allocation.
                auto opt = alloc.reallocate(ptr, initial_count, not_zero{final_count}, use_count);

                snn_require(opt);
                ptr = opt.value();

                snn_require(ptr[0] == 123);
                snn_require(ptr[1] == 456);

                alloc.deallocate(ptr, final_count);
            }

            SNN_DIAGNOSTIC_POP

            {
                mem::trivial_allocator<int> alloc;

                constexpr usize count = 3;

                auto opt = alloc.allocate(not_zero{count});

                snn_require(opt);
                snn_require(opt.has_value());
                snn_require(opt.value() != nullptr);
                snn_require(opt.value(promise::has_value) != nullptr);
                snn_require(opt.value_or(nullptr) != nullptr);
                snn_require(opt.value_or_nullptr() != nullptr);

                alloc.deallocate(opt.value(), count);
            }
            {
                mem::trivial_allocator<int> alloc;

                auto opt = alloc.allocate(not_zero{constant::limit<usize>::max});

                snn_require(!opt);
                snn_require(!opt.has_value());
                snn_require_throws_code(opt.value(), generic::error::memory_allocation_failure);
                snn_require(opt.value_or(nullptr) == nullptr);
                snn_require(opt.value_or_nullptr() == nullptr);
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_trivial_allocator());
    }
}
