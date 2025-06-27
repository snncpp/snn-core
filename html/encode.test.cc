// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/html/encode.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"
#include "snn-core/utf8/encode.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(html::encode("") == "");
            snn_require(html::encode("a") == "a");
            snn_require(html::encode("abc") == "abc");
            snn_require(html::encode("åäö") == "åäö");

            snn_require(html::encode("&") == "&amp;");
            snn_require(html::encode("<<<") == "&lt;&lt;&lt;");
            snn_require(html::encode(R"(<"&'>)") == "&lt;&quot;&amp;&#39;&gt;");

            snn_require(html::encode("<span>") == "&lt;span&gt;");

            return true;
        }

        // Encodes 5 characters: < > & " '
        constexpr strbuf encode_ref(const cstrview s)
        {
            strbuf enc{init::reserve, s.size()};
            for (const char c : s)
            {
                switch (c)
                {
                    case '<':
                        enc << "&lt;";
                        break;
                    case '>':
                        enc << "&gt;";
                        break;
                    case '&':
                        enc << "&amp;";
                        break;
                    case '"':
                        enc << "&quot;";
                        break;
                    case '\'':
                        enc << "&#39;";
                        break;
                    default:
                        enc << c;
                        break;
                }
            }
            return enc;
        }

        // Encodes 3 characters: & " U+00A0
        constexpr strbuf encode_attribute_value_ref(const cstrview s)
        {
            strbuf enc{init::reserve, s.size()};
            auto rng = s.range();
            while (rng)
            {
                const char c = rng.pop_front(assume::not_empty);

                switch (c)
                {
                    case '"':
                        enc << "&quot;";
                        break;

                    case '&':
                        enc << "&amp;";
                        break;

                    case to_char(0xC2):
                        if (rng.drop_front(to_char(0xA0)))
                        {
                            enc << "&nbsp;";
                        }
                        else
                        {
                            enc << c;
                        }
                        break;

                    default:
                        enc << c;
                        break;
                }
            }
            return enc;
        }

        // Encodes 4 characters: < > & U+00A0
        constexpr strbuf encode_text_value_ref(const cstrview s)
        {
            strbuf enc{init::reserve, s.size()};
            auto rng = s.range();
            while (rng)
            {
                const char c = rng.pop_front(assume::not_empty);

                switch (c)
                {
                    case '&':
                        enc << "&amp;";
                        break;

                    case '<':
                        enc << "&lt;";
                        break;

                    case '>':
                        enc << "&gt;";
                        break;

                    case to_char(0xC2):
                        if (rng.drop_front(to_char(0xA0)))
                        {
                            enc << "&nbsp;";
                        }
                        else
                        {
                            enc << c;
                        }
                        break;

                    default:
                        enc << c;
                        break;
                }
            }
            return enc;
        }

        constexpr bool test_encode()
        {
            {
                snn_require(html::encode("") == "");
                snn_require(html::encode("a") == "a");
                snn_require(html::encode("abc") == "abc");
                snn_require(html::encode("åäö") == "åäö");

                strbuf s;
                html::encode("abc", s, assume::no_overlap);
                html::encode(cstrview{"åäö"}, s, assume::no_overlap);
                snn_require(s == "abcåäö");

                snn_require(html::encode("&") == "&amp;");
                snn_require(html::encode("<<<") == "&lt;&lt;&lt;");
                snn_require(html::encode(R"(<"&'>)") == "&lt;&quot;&amp;&#39;&gt;");

                snn_require(html::encode("<br />") == "&lt;br /&gt;");
                snn_require(html::encode("<span>") == "&lt;span&gt;");
                snn_require(html::encode("<åäö>") == "&lt;åäö&gt;");
                snn_require(html::encode(R"(<span class="name">)") ==
                            "&lt;span class=&quot;name&quot;&gt;");
                snn_require(
                    html::encode(R"(<span class="name" onclick="return confirm('Go?');">)") ==
                    "&lt;span class=&quot;name&quot; onclick=&quot;return "
                    "confirm(&#39;Go?&#39;);&quot;&gt;");
                snn_require(html::encode("one & <b>two</b>") == "one &amp; &lt;b&gt;two&lt;/b&gt;");
                snn_require(html::encode("one & <b>two</b> & three") ==
                            "one &amp; &lt;b&gt;two&lt;/b&gt; &amp; three");
            }

            {
                str dest{"One two & three"};
                snn_require(dest.capacity() == str::default_capacity());
                const str src{"<åäö>"};
                html::encode(src, dest, assume::no_overlap);
                snn_require(dest == "One two & three&lt;åäö&gt;");
                snn_require(dest.capacity() > str::default_capacity());
            }

            {
                const cstrview all{"<\"&'>\xC2\xA0"};
                snn_require(html::encode(all) == "&lt;&quot;&amp;&#39;&gt;\xC2\xA0");
                snn_require(html::encode_attribute_value(all, assume::is_utf8) ==
                            "<&quot;&amp;'>&nbsp;");
                snn_require(html::encode_text_value(all, assume::is_utf8) ==
                            "&lt;\"&amp;'&gt;&nbsp;");
            }

            {
                strbuf s{init::reserve, 384};
                snn_require(s.size() == 0);
                snn_require(s.capacity() >= 384);
                for (const auto b : range::integral<byte>{})
                {
                    utf8::encode(u32{b}, s, promise::is_valid);
                }
                snn_require(s.size() == 384);

                snn_require(html::encode<strbuf>(s) == encode_ref(s));
                snn_require(html::encode_attribute_value<strbuf>(s, assume::is_utf8) ==
                            encode_attribute_value_ref(s));
                snn_require(html::encode_text_value<strbuf>(s, assume::is_utf8) ==
                            encode_text_value_ref(s));

                snn_require(html::encode<strbuf>(s) != encode_attribute_value_ref(s));
                snn_require(html::encode_attribute_value<strbuf>(s, assume::is_utf8) !=
                            encode_text_value_ref(s));
                snn_require(html::encode_text_value<strbuf>(s, assume::is_utf8) != encode_ref(s));
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_encode());
    }
}
