// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/stream/buffered_reader.hh"

#include "snn-core/array.hh"
#include "snn-core/array_view.hh"
#include "snn-core/fuzz.hh"
#include "snn-core/math/common.hh"
#include "snn-core/string/size.hh"

namespace snn::app
{
    class mock_reader final
    {
      public:
        constexpr explicit mock_reader(const cstrview contents, const usize max_chunk_size) noexcept
            : contents_{contents},
              max_chunk_size_{math::max(max_chunk_size, 1)}
        {
        }

        [[nodiscard]] constexpr result<usize> read_some(strview buffer) noexcept
        {
            const usize size = math::min(buffer.size(), max_chunk_size_, contents_.size());
            buffer.view(0, size).fill(contents_.view(0, size));
            contents_.drop_front_n(size);
            return size;
        }

      private:
        cstrview contents_;
        usize max_chunk_size_;
    };

    namespace
    {
        void fuzz_read_some(const cstrview input, const usize max_chunk_size)
        {
            stream::buffered_reader br{app::mock_reader{input, max_chunk_size}};

            array<char, 128> buffer;

            usize byte_count = 0;
            while (true)
            {
                auto res = br.read_some(buffer.view());
                static_assert(std::is_same_v<decltype(res), result<usize>>);
                snn_assert(res); // Should never fail with `mock_reader`.
                byte_count += res.value();
                if (res.value() == 0)
                {
                    break; // EOF
                }
            }
            snn_assert(byte_count == input.size());
        }

        template <typename T>
        void fuzz_read_until(const cstrview input, const T needle, const usize max_chunk_size)
        {
            stream::buffered_reader br{app::mock_reader{input, max_chunk_size}};

            usize byte_count        = 0;
            bool next_must_be_empty = false;
            while (true)
            {
                auto res = br.read_until<cstrview>(needle);
                static_assert(std::is_same_v<decltype(res), result<cstrview>>);
                snn_assert(res); // Should never fail with `mock_reader`.
                const cstrview s = res.value();
                if (s)
                {
                    snn_assert(!next_must_be_empty);

                    byte_count += s.size();

                    if (!s.has_back(needle))
                    {
                        // Final chunk (the needle was not found in the remaining data).
                        next_must_be_empty = true;
                    }
                }
                else
                {
                    break; // EOF or empty needle.
                }
            }
            snn_assert(string::size(needle) == 0 || byte_count == input.size());
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        if (input.size() >= 6)
        {
            const auto prefix = input.view_exactly<0, 6>(assume::within_bounds);
            input.drop_front_n(6);

            constexpr usize mask = 0b0001'1111'1111;
            static_assert(mask == 511); // 0-511

            const usize max_chunk_size = prefix.view_exactly<0, 2>().load<u16>() & mask;
            const usize needle_pos     = prefix.view_exactly<2, 2>().load<u16>() & mask;
            const usize needle_size    = prefix.view_exactly<4, 2>().load<u16>() & mask;

            const cstrview needle = input.view(needle_pos, needle_size);

            app::fuzz_read_until<cstrview>(input, needle, max_chunk_size);

            app::fuzz_read_until<char>(input, '\n', max_chunk_size);

            app::fuzz_read_some(input, max_chunk_size);
        }

        return constant::exit::success;
    }
}
