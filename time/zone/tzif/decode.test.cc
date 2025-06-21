// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/tzif/decode.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            constexpr cstrview tzif_jp =
                "\x54\x5A\x69\x66\x32\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                "\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x03\x00\x00\x00\x00"
                "\x00\x00\x00\x08\x00\x00\x00\x03\x00\x00\x00\x08\xD7\x3E\x02\x70"
                "\xD7\xED\x59\xF0\xD8\xF8\xFA\x70\xD9\xCD\x3B\xF0\xDB\x07\x00\xF0"
                "\xDB\xAD\x1D\xF0\xDC\xE6\xE2\xF0\xDD\x8C\xFF\xF0\x00\x01\x00\x01"
                "\x00\x01\x00\x01\x00\x00\x8C\xA0\x01\x00\x00\x00\x7E\x90\x00\x04"
                "\x00\x00\x7E\x90\x00\x04\x4A\x44\x54\x00\x4A\x53\x54\x00\x00\x00"
                "\x01\x00\x00\x01\x54\x5A\x69\x66\x32\x00\x00\x00\x00\x00\x00\x00"
                "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x04"
                "\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x04\x00\x00\x00\x0C"
                "\xFF\xFF\xFF\xFF\x65\xC2\xA4\x70\xFF\xFF\xFF\xFF\xD7\x3E\x02\x70"
                "\xFF\xFF\xFF\xFF\xD7\xED\x59\xF0\xFF\xFF\xFF\xFF\xD8\xF8\xFA\x70"
                "\xFF\xFF\xFF\xFF\xD9\xCD\x3B\xF0\xFF\xFF\xFF\xFF\xDB\x07\x00\xF0"
                "\xFF\xFF\xFF\xFF\xDB\xAD\x1D\xF0\xFF\xFF\xFF\xFF\xDC\xE6\xE2\xF0"
                "\xFF\xFF\xFF\xFF\xDD\x8C\xFF\xF0\x03\x01\x02\x01\x02\x01\x02\x01"
                "\x02\x00\x00\x83\x03\x00\x00\x00\x00\x8C\xA0\x01\x04\x00\x00\x7E"
                "\x90\x00\x08\x00\x00\x7E\x90\x00\x08\x4C\x4D\x54\x00\x4A\x44\x54"
                "\x00\x4A\x53\x54\x00\x00\x00\x00\x01\x00\x00\x00\x01\x0A\x4A\x53"
                "\x54\x2D\x39\x0A";

            vec<time::zone::offset> offsets;
            vec<time::zone::transition> transitions;
            str footer;

            snn_require(time::zone::tzif::decode(tzif_jp.range(), offsets, transitions, footer));

            snn_require(offsets.count() == 4);

            snn_require(offsets.at(0).value().abbr().view() == "LMT");
            snn_require(offsets.at(0).value().is_dst() == false);
            snn_require(offsets.at(0).value().seconds() == 33539);

            snn_require(offsets.at(1).value().abbr().view() == "JDT");
            snn_require(offsets.at(1).value().is_dst() == true);
            snn_require(offsets.at(1).value().seconds() == 36000);

            snn_require(offsets.at(2).value().abbr().view() == "JST");
            snn_require(offsets.at(2).value().is_dst() == false);
            snn_require(offsets.at(2).value().seconds() == 32400);

            snn_require(offsets.at(3).value().abbr().view() == "JST");
            snn_require(offsets.at(3).value().is_dst() == false);
            snn_require(offsets.at(3).value().seconds() == 32400);

            snn_require(transitions.count() == 9);

            snn_require(transitions.at(0).value().when == -2587712400);
            snn_require(transitions.at(0).value().offset_index == 3);

            snn_require(transitions.at(1).value().when == -683802000);
            snn_require(transitions.at(1).value().offset_index == 1);

            snn_require(transitions.at(2).value().when == -672310800);
            snn_require(transitions.at(2).value().offset_index == 2);

            snn_require(transitions.at(3).value().when == -654771600);
            snn_require(transitions.at(3).value().offset_index == 1);

            snn_require(transitions.at(4).value().when == -640861200);
            snn_require(transitions.at(4).value().offset_index == 2);

            snn_require(transitions.at(5).value().when == -620298000);
            snn_require(transitions.at(5).value().offset_index == 1);

            snn_require(transitions.at(6).value().when == -609411600);
            snn_require(transitions.at(6).value().offset_index == 2);

            snn_require(transitions.at(7).value().when == -588848400);
            snn_require(transitions.at(7).value().offset_index == 1);

            snn_require(transitions.at(8).value().when == -577962000);
            snn_require(transitions.at(8).value().offset_index == 2);

            snn_require(footer == "JST-9");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
