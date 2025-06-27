// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Buffered reader for any readable stream

// If any `read*()` call throws, do not attempt any more calls. After an exception has been thrown
// `buffered_reader` is only in a state to clean up it's resources on destruction (basic exception
// guarantee).

#pragma once

#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/raw/copy.hh"
#include "snn-core/stream/common.hh"
#include "snn-core/string/size.hh"

namespace snn::stream
{
    // ## Classes

    // ### buffered_reader

    template <stream::readable Stream>
    class buffered_reader final
    {
      public:
        // #### Explicit constructors

        constexpr explicit buffered_reader(
            Stream stream, const u32 buffer_size = 8192,
            const u32 buffer_max_size = constant::limit<u32>::max) noexcept
            : stream_{std::move(stream)},
              read_size_{math::round_up_to_multiple<512, u32>(buffer_size).get()},
              buffer_max_size_{buffer_max_size}
        {
        }

        // #### Underlying stream

        [[nodiscard]] constexpr Stream& stream() noexcept
        {
            return stream_;
        }

        [[nodiscard]] constexpr const Stream& stream() const noexcept
        {
            return stream_;
        }

        // #### Read

        // Read a chunk up to max size bytes. If the string is returned as a reference (e.g.
        // `cstrview`) it is only valid until the next read call or until the `buffered_reader` is
        // destroyed. Returns an empty string on end-of-file (EOF) or if `max_size` is 0.

        template <typename String = str>
        [[nodiscard]] constexpr result<String> read(const usize max_size = constant::npos)
        {
            // EOF?
            if (offset_ == constant::npos)
            {
                return String{};
            }

            snn_should(offset_ <= buffer_.size());

            // Is the buffer empty?
            if (offset_ >= buffer_.size())
            {
                const auto res = stream_.read_some(buffer_.resize_for_overwrite(read_size_));
                if (res)
                {
                    buffer_.truncate(res.value(assume::has_value));
                    offset_ = 0;
                }
                else
                {
                    buffer_.clear();
                    offset_ = 0;
                    return res.error_code();
                }
            }

            const cstrview chunk{buffer_.view(offset_, max_size)};
            offset_ += chunk.size();
            return String{chunk};
        }

        // #### Read line

        // Read a line up to including the `'\n'` character. Note that the last string returned
        // before end-of-file (EOF) might not end with the `'\n'` character. If the string is
        // returned as a reference (e.g. `cstrview`) it is only valid until the next read call or
        // until the `buffered_reader` is destroyed. Returns an empty string on end-of-file (EOF).

        template <typename String = str>
        [[nodiscard]] constexpr result<String> read_line()
        {
            return read_until<String>('\n');
        }

        // #### Read some

        // Read up to `buffer.size()` bytes. Returns 0 on end-of-file (EOF) or if `buffer` is empty.

        [[nodiscard]] constexpr result<usize> read_some(strview buffer)
        {
            const auto res = read<cstrview>(buffer.size());
            if (res)
            {
                const cstrview chunk = res.value(assume::has_value);
                snn_should(chunk.size() <= buffer.size());
                mem::raw::copy(chunk.data(), buffer.writable(), chunk.byte_size(),
                               assume::no_overlap);
                return chunk.size();
            }
            return res.error_code();
        }

        // #### Read until

        // Read up to including `needle`. Note that the last string returned before end-of-file
        // (EOF) might not end with `needle`. If the string is returned as a reference (e.g.
        // `cstrview`) it is only valid until the next read call or until the `buffered_reader` is
        // destroyed. Returns an empty string on end-of-file (EOF) or if `needle` is empty.

        template <typename String = str>
        [[nodiscard]] constexpr result<String> read_until(const cstrview needle)
        {
            return read_until_<String, cstrview>(needle);
        }

        template <typename String = str, character Char>
        [[nodiscard]] constexpr result<String> read_until(const Char needle)
        {
            return read_until_<String, char>(needle);
        }

      private:
        Stream stream_;
        strbuf buffer_;
        usize offset_{0};
        u32 read_size_;
        u32 buffer_max_size_;

        template <typename String, typename T>
        constexpr result<String> read_until_(const T needle)
        {
            static_assert(std::is_same_v<T, char> || std::is_same_v<T, cstrview>);

            // EOF?
            if (offset_ == constant::npos)
            {
                return String{};
            }

            snn_should(offset_ <= buffer_.size());

            usize start_pos = offset_;
            while (true)
            {
                snn_should(start_pos >= offset_ && start_pos <= buffer_.size());

                // If the needle is in the buffer, return string up to including needle.
                // An empty chunk will be returned here if the needle is empty.
                usize pos = buffer_.find(needle, start_pos).value_or_npos();
                if (pos != constant::npos)
                {
                    pos += string::size(needle); // Include needle.
                    const cstrview chunk = buffer_.view(offset_, pos - offset_);
                    offset_ += chunk.size();
                    return String{chunk};
                }

                // Avoid searching the same text over and over again.
                if constexpr (std::is_same_v<T, cstrview>)
                {
                    snn_should(needle.size() > 0);
                    if ((buffer_.size() - start_pos) >= needle.size())
                    {
                        // Part of the needle might still be in the buffer.
                        start_pos = (buffer_.size() + 1) - needle.size();
                    }
                }

                // Do not allocate more than max size.
                // This can not overflow (`57-bit-virtual-address-space` + `u32`).
                if ((buffer_.size() + read_size_) > buffer_max_size_)
                {
                    return generic::error::capacity_would_exceed_max_capacity;
                }

                // Read and append to the buffer.
                const usize prior_size = buffer_.size();
                const auto res = stream_.read_some(buffer_.append_for_overwrite(read_size_));
                if (res)
                {
                    const usize appended_size = res.value(assume::has_value);

                    buffer_.truncate(prior_size + appended_size);

                    if (appended_size == 0)
                    {
                        // EOF, return what's left in the buffer.
                        const cstrview chunk{buffer_.view(offset_)};
                        offset_ = constant::npos; // EOF state for next `read*()` call.
                        return String{chunk};
                    }
                }
                else
                {
                    buffer_.truncate(prior_size);
                    // `offset_` doesn't change.
                    return res.error_code();
                }
            }
        }
    };
}
