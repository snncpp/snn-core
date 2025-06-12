// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/set/sorted.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            set::sorted<str> set = {"One", "Two", "Three"};

            snn_require(set);
            snn_require(!set.is_empty());
            snn_require(set.count() == 3);

            snn_require(set.contains("One"));
            snn_require(set.contains("Two"));
            snn_require(set.contains("Three"));

            snn_require(!set.contains("ONE"));
            snn_require(!set.contains("Four"));

            snn_require(!set.insert("One"));

            snn_require(set.remove("One"));
            snn_require(!set.remove("One"));
            snn_require(!set.contains("One"));

            auto ins_res = set.insert("One");
            snn_require(ins_res.key() == "One");
            snn_require(ins_res);
            snn_require(ins_res.was_inserted());

            ins_res = set.insert("One");
            snn_require(ins_res.key() == "One");
            snn_require(!ins_res);
            snn_require(!ins_res.was_inserted());

            auto rng = set.range();
            static_assert(bidirectional_range<decltype(rng)>);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == "One");
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(rng.front(promise::not_empty) == "Three");
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(rng.front(promise::not_empty) == "Two");
            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

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
            set::sorted<str> set;

            snn_require(set.is_empty());
            snn_require(set.count() == 0);
            snn_require(!set);

            snn_require(!set.contains("One"));

            {
                const auto res = set.insert("One");
                snn_require(res);
                snn_require(res.was_inserted());
                snn_require(res.key() == "One");
            }

            snn_require(set.contains("One"));
            set.insert("One");         // Does nothing.
            set.insert_inplace("One"); // Does nothing.

            snn_require(set.remove("One"));
            set.insert("One"); // Inserted

            snn_require(set.contains("One"));

            {
                const auto res = set.insert("One");
                snn_require(!res);
                snn_require(!res.was_inserted());
                snn_require(res.key() == "One");
            }

            snn_require(set.contains("One"));

            snn_require(!set.is_empty());
            snn_require(set.count() == 1);
            snn_require(set);

            snn_require(set.insert("Two"));
            snn_require(set.insert_inplace("Three"));

            snn_require(!set.is_empty());
            snn_require(set.count() == 3);
            snn_require(set);

            snn_require(!set.insert("Two"));
            snn_require(!set.insert_inplace("Three"));

            snn_require(!set.is_empty());
            snn_require(set.count() == 3);

            snn_require(set.contains("One"));
            snn_require(set.contains("Two"));
            snn_require(set.contains("Three"));
            snn_require(!set.contains("Four"));

            str buf;
            for (const auto& s : set)
            {
                buf << s << '\n';
            }
            snn_require(buf == "One\nThree\nTwo\n");

            buf.clear();
            for (const auto& s : set.range())
            {
                buf << s << '\n';
            }
            snn_require(buf == "One\nThree\nTwo\n");

            set.clear();
            snn_require(set.is_empty());
            snn_require(set.count() == 0);

            // Generic container interface.
            set.append_inplace("One");
            snn_require(!set.is_empty());
            snn_require(set.count() == 1);
            snn_require(set.contains("One"));
        }

        {
            set::sorted<str> set = {{"One"}, {"Two"}, {"Three"}};

            snn_require(!set.is_empty());
            snn_require(set.count() == 3);

            str buf;
            for (const auto& s : set)
            {
                buf << s << '\n';
            }
            snn_require(buf == "One\nThree\nTwo\n");

            snn_require(set.remove("One"));
            snn_require(!set.remove("One"));
            snn_require(set.remove("Two"));
            snn_require(!set.remove("Two"));
            snn_require(set.remove("Three"));
            snn_require(!set.remove("Three"));

            snn_require(!set.remove("Four"));

            snn_require(set.is_empty());
            snn_require(set.count() == 0);
        }

        {
            // Generic container interface.

            set::sorted<str> set{init::reserve, 1'000};

            snn_require(set.is_empty());
            snn_require(set.count() == 0);

            set.reserve(2'000);
            set.reserve_append(500);

            snn_require(set.is_empty());
            snn_require(set.count() == 0);
        }

        {
            set::sorted<str> set1 = {{"One"}, {"Two"}, {"Three"}};
            set::sorted<str> set2;

            snn_require(set1.contains("One"));
            snn_require(set1.contains("Two"));
            snn_require(set1.contains("Three"));

            snn_require(set2.is_empty());

            swap(set1, set2);

            snn_require(set1.is_empty());

            snn_require(set2.contains("One"));
            snn_require(set2.contains("Two"));
            snn_require(set2.contains("Three"));
        }

        // Swap with self.
        {
            set::sorted<str> set = {{"One"}, {"Two"}, {"Three"}};

            std::swap(set, set);

            snn_require(set.contains("One"));
            snn_require(set.contains("Two"));
            snn_require(set.contains("Three"));

            set.swap(set);

            snn_require(set.contains("One"));
            snn_require(set.contains("Two"));
            snn_require(set.contains("Three"));
        }
    }
}
