// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/allocator.hh"

#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include "snn-core/mem/construct.hh"
#include "snn-core/mem/destruct_n.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_allocator()
        {
            static_assert(is_trivially_relocatable_v<str>);

            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            {
                mem::allocator<str> alloc;

                constexpr usize count = 3;

                str* ptr = alloc.allocate(not_zero{count}).value();

                // WARNING: This is not exception safe. `mem::allocator` should not be used without
                // RAII: https://en.cppreference.com/w/cpp/language/raii

                mem::construct(not_null{&ptr[0]}, "Test test");
                mem::construct(not_null{&ptr[1]}, "A longer string that goes on the heap.");
                // &ptr[2] is not constructed.

                snn_require(ptr[1].has_front("A longer string"));

                constexpr usize use_count = 2;

                mem::destruct_n(ptr, use_count);

                alloc.deallocate(ptr, count);
            }
            {
                mem::allocator<str> alloc;

                constexpr usize count = 3;

                // Allocate using `reallocate`.
                str* ptr = alloc.reallocate(nullptr, 0, not_zero{count}, 0).value();

                // WARNING: This is not exception safe. `mem::allocator` should not be used without
                // RAII: https://en.cppreference.com/w/cpp/language/raii

                mem::construct(not_null{&ptr[0]}, "Test test");
                mem::construct(not_null{&ptr[1]}, "A longer string that goes on the heap.");
                // &ptr[2] is not constructed.

                snn_require(ptr[1].has_front("A longer string"));

                constexpr usize use_count = 2;

                mem::destruct_n(ptr, use_count);

                alloc.deallocate(ptr, count);
            }
            {
                mem::allocator<str> alloc;

                constexpr usize initial_count = 3;

                str* ptr = alloc.allocate(not_zero{initial_count}).value();

                // WARNING: This is not exception safe. `mem::allocator` should not be used without
                // RAII: https://en.cppreference.com/w/cpp/language/raii

                mem::construct(not_null{&ptr[0]}, "Test test");
                mem::construct(not_null{&ptr[1]}, "A longer string that goes on the heap.");
                // `&ptr[2]` is not constructed.

                usize use_count             = 2;
                constexpr usize final_count = 5;

                // Grow allocation.
                auto opt = alloc.reallocate(ptr, initial_count, not_zero{final_count}, use_count);

                snn_require(opt);
                ptr = opt.value();

                snn_require(ptr[0] == "Test test");
                snn_require(ptr[1].has_front("A longer string"));

                mem::construct(not_null{&ptr[2]}, "...");
                mem::construct(not_null{&ptr[3]}, "Another long string that goes on the heap.");
                // `&ptr[4]` is not constructed.

                use_count = 4;

                mem::destruct_n(ptr, use_count);

                alloc.deallocate(ptr, final_count);
            }
            {
                mem::allocator<str> alloc;

                constexpr usize initial_count = 4;

                str* ptr = alloc.allocate(not_zero{initial_count}).value();

                // WARNING: This is not exception safe. `mem::allocator` should not be used without
                // RAII: https://en.cppreference.com/w/cpp/language/raii

                mem::construct(not_null{&ptr[0]}, "Test test");
                mem::construct(not_null{&ptr[1]}, "A longer string that goes on the heap.");
                mem::construct(not_null{&ptr[2]}, "Another long string that goes on the heap.");
                // `&ptr[3]` is not constructed.

                usize use_count             = 3;
                constexpr usize final_count = 2;

                // Shrink allocation (`reallocate` must move 2 objects and destroy 1 object).
                auto opt = alloc.reallocate(ptr, initial_count, not_zero{final_count}, use_count);

                snn_require(opt);
                ptr = opt.value();

                snn_require(ptr[0] == "Test test");
                snn_require(ptr[1].has_front("A longer string"));

                use_count = 2;

                mem::destruct_n(ptr, use_count);

                alloc.deallocate(ptr, final_count);
            }

            SNN_DIAGNOSTIC_POP

            {
                mem::allocator<str> alloc;

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
                mem::allocator<str> alloc;

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
        snn_static_require(app::test_allocator());
    }
}
