// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/stream/buffered_reader.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/reader_writer.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            stream::buffered_reader br{
                file::reader_writer{"buffered_reader.test.cc", file::option::read_only}};
            snn_require(br.read_line<cstrview>().value().has_front("// Copyright"));
            snn_require(br.read_line<cstrview>().value().has_front("// SPDX-License-Identifier: "));
            snn_require(br.read_line<cstrview>().value() == "\n");
            snn_require(br.read_line<cstrview>().value() == "#include \"snn-core/stream/"
                                                            "buffered_reader.hh\"\n");

            return true;
        }

        class mock_reader final
        {
          public:
            constexpr explicit mock_reader(const cstrview contents,
                                           const usize max_chunk_size) noexcept
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

        constexpr bool test_buffered_reader()
        {
            {
                stream::buffered_reader br{mock_reader{"abc", 1}};
                snn_require(br.read().value() == "a");
                snn_require(br.read().value() == "b");
                snn_require(br.read().value() == "c");
                snn_require(br.read().value() == ""); // End-of-file
            }

            {
                stream::buffered_reader br{mock_reader{"abc", 1}};
                snn_require(br.read<cstrview>().value() == "a");
                snn_require(br.read<cstrview>().value() == "b");
                snn_require(br.read<cstrview>().value() == "c");
                snn_require(br.read<cstrview>().value() == ""); // End-of-file
            }

            {
                stream::buffered_reader br{mock_reader{"abc", 99}};
                snn_require(br.read<cstrview>().value() == "abc");
                snn_require(br.read<cstrview>().value() == ""); // End-of-file
            }

            {
                stream::buffered_reader br{mock_reader{"", 99}};
                snn_require(br.read<cstrview>().value() == ""); // End-of-file
            }

            {
                for (const auto max_chunk_size : init_list<usize>{1, 2, 3, 4, 5, 9, 99})
                {
                    stream::buffered_reader br{
                        mock_reader{"one\ntwo\n\nfour\nfive", max_chunk_size}};
                    snn_require(br.read_line<cstrview>().value() == "one\n");
                    snn_require(br.read_until<cstrview>('\n').value() == "two\n");
                    snn_require(br.read_line<cstrview>().value() == "\n");
                    snn_require(br.read_line<cstrview>().value() == "four\n");
                    snn_require(br.read_line<cstrview>().value() == "five");
                    snn_require(br.read_line<cstrview>().value() == ""); // End-of-file
                }

                for (const auto max_chunk_size : init_list<usize>{1, 2, 3, 4, 5, 9, 99})
                {
                    stream::buffered_reader br{mock_reader{"one\ntwo\n\none\ntwo", max_chunk_size}};
                    snn_require(br.read_until<cstrview>("one").value() == "one");
                    snn_require(br.read_until<cstrview>("one").value() == "\ntwo\n\none");
                    snn_require(br.read_until<cstrview>("one").value() == "\ntwo");
                    snn_require(br.read_until<cstrview>("one").value() == ""); // End-of-file
                }
            }

            {
                stream::buffered_reader br{mock_reader{"abc", 1}};

                array<char, 4> buffer;

                snn_require(br.read_some(buffer.view()).value() == 1);
                snn_require(buffer.view() == "a\0\0\0");

                snn_require(br.read_some(buffer.view()).value() == 1);
                snn_require(buffer.view() == "b\0\0\0");

                snn_require(br.read_some(buffer.view()).value() == 1);
                snn_require(buffer.view() == "c\0\0\0");

                snn_require(br.read_some(buffer.view()).value() == 0); // End-of-file
            }

            {
                stream::buffered_reader br{mock_reader{"abcdefg", 3}};

                array<char, 4> buffer;

                snn_require(br.read_some(buffer.view()).value() == 3);
                snn_require(buffer.view() == "abc\0");

                snn_require(br.read_some(buffer.view()).value() == 3);
                snn_require(buffer.view() == "def\0");

                snn_require(br.read_some(buffer.view()).value() == 1);
                snn_require(buffer.view() == "gef\0");

                snn_require(br.read_some(buffer.view()).value() == 0); // End-of-file
            }

            {
                stream::buffered_reader br{mock_reader{"abcdefg", 99}};

                array<char, 4> buffer;

                snn_require(br.read_some(buffer.view(0, 0)).value() == 0); // Empty buffer.
                snn_require(buffer.view() == "\0\0\0\0");

                snn_require(br.read_some(buffer.view()).value() == 4);
                snn_require(buffer.view() == "abcd");

                snn_require(br.read_some(buffer.view()).value() == 3);
                snn_require(buffer.view() == "efgd");

                snn_require(br.read_some(buffer.view()).value() == 0); // End-of-file
            }

            {
                stream::buffered_reader br{mock_reader{"", 99}};

                static_assert(std::is_same_v<decltype(br.read()), result<str>>);
                static_assert(std::is_same_v<decltype(br.read<str>()), result<str>>);
                static_assert(std::is_same_v<decltype(br.read<strbuf>()), result<strbuf>>);
                static_assert(std::is_same_v<decltype(br.read<cstrview>()), result<cstrview>>);

                static_assert(std::is_same_v<decltype(br.read_line()), result<str>>);
                static_assert(std::is_same_v<decltype(br.read_line<str>()), result<str>>);
                static_assert(std::is_same_v<decltype(br.read_line<strbuf>()), result<strbuf>>);
                static_assert(std::is_same_v<decltype(br.read_line<cstrview>()), result<cstrview>>);

                static_assert(std::is_same_v<decltype(br.read_until("foo")), result<str>>);
                static_assert(std::is_same_v<decltype(br.read_until<str>("foo")), result<str>>);
                static_assert(
                    std::is_same_v<decltype(br.read_until<strbuf>("foo")), result<strbuf>>);
                static_assert(
                    std::is_same_v<decltype(br.read_until<cstrview>("foo")), result<cstrview>>);

                static_assert(std::is_same_v<decltype(br.read_until('x')), result<str>>);
                static_assert(std::is_same_v<decltype(br.read_until<str>('x')), result<str>>);
                static_assert(std::is_same_v<decltype(br.read_until<strbuf>('x')), result<strbuf>>);
                static_assert(
                    std::is_same_v<decltype(br.read_until<cstrview>('x')), result<cstrview>>);
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
        snn_static_require(app::test_buffered_reader());

        {
            stream::buffered_reader br{
                file::reader_writer{"buffered_reader.test.cc", file::option::read_only}};
            snn_require(br.read<cstrview>(12).value() == "// Copyright");
            snn_require(br.read<cstrview>(5).value() == " (c) ");
            snn_require(br.read<cstrview>().value().size() > 0);
        }
        {
            stream::buffered_reader br{
                file::reader_writer{"buffered_reader.test.cc", file::option::read_only}};
            snn_require(br.read_until<cstrview>("right").value() == "// Copyright");
            snn_require(br.read_until<cstrview>("").value() == ""); // Undistinguishable from EOF.
            snn_require(br.read_until<cstrview>("buffered_reader.hh").value().size() > 0);
        }
        {
            stream::buffered_reader br{
                file::reader_writer{"buffered_reader.test.cc", file::option::read_only}, 512, 1024};
            auto res = br.read_until<cstrview>("\n\n\n"); // Should not exists.
            snn_require(!res);
            snn_require(res.error_code() == generic::error::capacity_would_exceed_max_capacity);
        }
    }
}
