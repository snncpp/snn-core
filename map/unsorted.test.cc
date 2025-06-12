// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/map/unsorted.hh"

#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            map::unsorted<str, int> m = {{"One", 1}, {"Two", 22}, {"Three", 333}};

            snn_require(!m.is_empty());
            snn_require(m);
            snn_require(m.count() == 3);

            snn_require(m.contains("One"));
            snn_require(m.contains("Two"));
            snn_require(m.contains("Three"));

            snn_require(!m.contains("ONE"));
            snn_require(!m.contains("Four"));

            snn_require(m.get("Two").value() == 22);
            snn_require(m.get("Two").value_or_default() == 22);

            snn_require(m.get("Four").value_or_default() == 0);

            // Insert (if unique).
            {
                auto ins_res = m.insert("Two", 123);
                snn_require(ins_res.key() == "Two");
                snn_require(ins_res.value() == 22); // Not changed.
                snn_require(!ins_res);
                snn_require(!ins_res.was_inserted());
            }

            // Insert (if unique).
            {
                auto ins_res = m.insert("Four", 4444);
                snn_require(ins_res.key() == "Four");
                snn_require(ins_res.value() == 4444);
                snn_require(ins_res);
                snn_require(ins_res.was_inserted());
            }

            // Insert or assign.
            {
                auto ins_res = m.insert_or_assign("Two", 123);
                snn_require(ins_res.key() == "Two");
                snn_require(ins_res.value() == 123); // Assigned
                snn_require(!ins_res.was_inserted());
                snn_require(ins_res.was_assigned());
            }

            snn_require(m.get("Two").value() == 123);

            static_assert(std::is_same_v<decltype(m.get("Two")), optional<int&>>);
            static_assert(
                std::is_same_v<decltype(std::as_const(m).get("Two")), optional<const int&>>);

            static_assert(std::is_same_v<decltype(m.get<int>("Two")), optional<int>>);
            static_assert(
                std::is_same_v<decltype(std::as_const(m).get<int>("Two")), optional<int>>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        {
            map::unsorted<str, int> m;

            snn_require(!m);
            snn_require(m.is_empty());
            snn_require(m.count() == 0);

            snn_require(!m.get("One"));
            snn_require(!m.contains("One"));

            {
                const auto ins_res = m.insert("One", 1111);
                snn_require(ins_res);
                snn_require(ins_res.was_inserted());
                snn_require(ins_res.key() == "One");
                snn_require(ins_res.value() == 1111);
            }

            snn_require(m.get("One").value() == 1111);
            m.insert("One", 123456789);         // Does nothing.
            m.insert_inplace("One", 123456789); // Does nothing.
            snn_require(m.get("One").value() == 1111);

            snn_require(m.remove("One"));
            m.insert("One", 1111); // Inserted

            snn_require(m.get("One").value() == 1111);

            {
                auto ins_res = m.insert("One", 123456789);
                snn_require(!ins_res);
                snn_require(!ins_res.was_inserted());
                snn_require(ins_res.key() == "One");
                snn_require(ins_res.value() == 1111); // Old value.

                ins_res.value() = 123; // Assign new value.
            }

            snn_require(!m.insert("One", 123456789));         // Key already exists.
            snn_require(!m.insert_inplace("One", 123456789)); // Key already exists.

            snn_require(m);
            snn_require(!m.is_empty());
            snn_require(m.count() == 1);

            {
                auto opt = m.get("One");
                static_assert(std::is_same_v<decltype(opt), optional<int&>>);
                snn_require(opt);
                snn_require(opt.value() == 123);
            }
            {
                auto opt = m.get<int>("One");
                static_assert(std::is_same_v<decltype(opt), optional<int>>);
                snn_require(opt);
                snn_require(opt.value() == 123);
            }

            {
                auto ins_res = m.insert_or_assign("One", 11);
                snn_require(ins_res.key() == "One");
                snn_require(ins_res.value() == 11);
                snn_require(!ins_res.was_inserted());
                snn_require(ins_res.was_assigned());
            }

            snn_require(m);
            snn_require(!m.is_empty());
            snn_require(m.count() == 1);

            snn_require(m.insert_inplace("Two", 2222));
            snn_require(m.get("Two").value() == 2222);
            snn_require(m.insert_or_assign("Two", 22).was_assigned());
            snn_require(m.get("Two").value() == 22);

            snn_require(m.insert_or_assign("Three", 3333).was_inserted());
            snn_require(m.get("Three").value() == 3333);
            snn_require(m.insert_or_assign("Three", 33).was_assigned());
            snn_require(m.get("Three").value() == 33);

            snn_require(m);
            snn_require(!m.is_empty());
            snn_require(m.count() == 3);

            snn_require(!m.get("Four"));

            snn_require(m.contains("One"));
            snn_require(m.contains("Two"));
            snn_require(m.contains("Three"));
            snn_require(!m.contains("Four"));

            str buf;
            for (const auto& p : m)
            {
                buf << p.first << ':' << as_num(p.second) << '\n';
            }
            snn_require(buf.size() == 23);
            snn_require(buf.contains("One:11\n"));
            snn_require(buf.contains("Two:22\n"));
            snn_require(buf.contains("Three:33\n"));
            snn_require(!buf.contains("Four:44\n"));

            m.clear();
            snn_require(m.is_empty());
            snn_require(m.count() == 0);

            // Generic container interface.
            m.append_inplace("One", 111);
            snn_require(m.get("One").value() == 111);
        }

        {
            map::unsorted<str, int> m = {{"One", 11}, {"Two", 22}, {"Three", 33}};

            snn_require(!m.is_empty());
            snn_require(m.count() == 3);

            {
                auto opt = m.get("One");
                snn_require(opt);
                snn_require(opt.value() == 11);
            }
            {
                auto opt = m.get("Two");
                snn_require(opt);
                snn_require(opt.value() == 22);
            }
            {
                auto opt = m.get("Three");
                snn_require(opt);
                snn_require(opt.value() == 33);
            }

            snn_require(!m.get("Four"));

            snn_require(m.remove("One"));
            snn_require(!m.remove("One"));
            snn_require(m.remove("Two"));
            snn_require(!m.remove("Two"));
            snn_require(m.remove("Three"));
            snn_require(!m.remove("Three"));

            snn_require(!m.remove("Four"));

            snn_require(m.is_empty());
            snn_require(m.count() == 0);
        }

        {
            map::unsorted<str, int> m;

            snn_require(m.is_empty());
            snn_require(m.count() == 0);

            const usize remove_count = m.remove_if(fn::ret{true});
            snn_require(remove_count == 0);

            snn_require(m.is_empty());
            snn_require(m.count() == 0);
        }
        {
            map::unsorted<str, int> m = {{"One", 11}, {"Two", 22}, {"Three", 33}};

            snn_require(!m.is_empty());
            snn_require(m.count() == 3);

            const usize remove_count = m.remove_if(fn::ret{true});
            snn_require(remove_count == 3);

            snn_require(m.is_empty());
            snn_require(m.count() == 0);
        }
        {
            map::unsorted<str, int> m = {{"One", 11}, {"Two", 22}, {"Three", 33}};

            snn_require(!m.is_empty());
            snn_require(m.count() == 3);

            const usize remove_count =
                m.remove_if(fn::element{meta::index<1>, fn::is{fn::eq{}, 22}});
            snn_require(remove_count == 1);

            snn_require(!m.is_empty());
            snn_require(m.count() == 2);

            snn_require(m.contains("One"));
            snn_require(!m.contains("Two"));
            snn_require(m.contains("Three"));
        }

        {
            // Generic container interface.

            map::unsorted<str, int> m{init::reserve, 1'000};

            snn_require(m.is_empty());
            snn_require(m.count() == 0);

            m.reserve(2'000);
            m.reserve_append(500);

            snn_require(m.is_empty());
            snn_require(m.count() == 0);
        }

        {
            map::unsorted<str, str> m = {{"One", "1"}, {"Two", "22"}, {"Three", "33"}};

            auto opt = m.get("One");
            snn_require(opt);
            snn_require(opt.value() == "1");
            opt.value() = "abcdefghijklmnopqrstuvwxyz";

            snn_require(m.get("One").value() == "abcdefghijklmnopqrstuvwxyz");
        }

        {
            map::unsorted<str, int> m1 = {{"One", 11}, {"Two", 22}, {"Three", 33}};
            map::unsorted<str, int> m2;

            snn_require(m1.contains("One"));
            snn_require(m1.contains("Two"));
            snn_require(m1.contains("Three"));

            snn_require(m2.is_empty());

            swap(m1, m2);

            snn_require(m1.is_empty());

            snn_require(m2.contains("One"));
            snn_require(m2.contains("Two"));
            snn_require(m2.contains("Three"));
        }

        {
            map::unsorted<str, int> m;

            snn_require(!m.contains("One"));
            snn_require(m.insert_inplace("One"));
            snn_require(m.contains("One"));
            snn_require(m.get("One").value() == 0);
            snn_require(!m.insert("One", 111));
            snn_require(m.get("One").value() == 0);

            snn_require(!m.contains("Two"));
            int& i = m.insert("Two", 222).value();
            snn_require(m.contains("Two"));
            snn_require(m.get("Two").value() == 222);
            snn_require(i == 222);
            i = 222'222;
            snn_require(m.get("Two").value() == 222'222);
            snn_require(!m.insert("Two", 222)); // Does nothing.
            snn_require(i == 222'222);
            snn_require(!m.insert_inplace("Two")); // Does nothing.
            snn_require(i == 222'222);
            snn_require(m.get("Two").value() == 222'222);
        }

        // range
        {
            map::unsorted<str, int> m = {{"One", 11}, {"Two", 22}, {"Three", 33}};

            static_assert(forward_range<decltype(m.range())>);
            static_assert(!bidirectional_range<decltype(m.range())>);

            str buf;

            buf.clear();
            for (auto& p : m.range())
            {
                static_assert(std::is_same_v<decltype(p), std::pair<const str, int>&>);
                buf << p.first << ':' << as_num(p.second) << '\n';
            }
            snn_require(buf.size() == 23);
            snn_require(buf.contains("One:11\n"));
            snn_require(buf.contains("Two:22\n"));
            snn_require(buf.contains("Three:33\n"));

            buf.clear();
            for (auto& p : std::as_const(m).range())
            {
                static_assert(std::is_same_v<decltype(p), const std::pair<const str, int>&>);
                buf << p.first << ':' << as_num(p.second) << '\n';
            }
            snn_require(buf.size() == 23);
            snn_require(buf.contains("One:11\n"));
            snn_require(buf.contains("Two:22\n"));
            snn_require(buf.contains("Three:33\n"));
        }
    }
}
