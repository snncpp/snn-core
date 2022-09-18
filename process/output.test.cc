// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/process/output.hh"

#include "snn-core/unittest.hh"
#include "snn-core/process/execute.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            process::command cmd;
            cmd << "/bin/echo -n " << cstrview{"Foo\n\nBar"};

            // "-n Do not print the trailing newline character."

            snn_require(cmd.to<cstrview>() == "/bin/echo -n 'Foo\n\nBar'");

            process::output output = process::execute_and_consume_output(cmd);

            snn_require(output);

            auto res = output.read_line();
            snn_require(res);
            snn_require(res.value() == "Foo\n");

            res = output.read_line();
            snn_require(res);
            snn_require(res.value() == "\n");

            res = output.read_line();
            snn_require(res);
            snn_require(res.value() == "Bar");

            res = output.read_line();
            snn_require(!res);
            snn_require(res.error_code() == process::error::no_more_data);

            snn_require(!output);

            snn_require(output.exit_status() == constant::exit::success);

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
            process::command cmd;
            cmd << "/bin/ls -a1";

            auto output = process::execute_and_consume_output(cmd);

            snn_require(output);

            while (auto res = output.read_line())
            {
                str& line = res.value();
                snn_require(!line.is_empty());
            }

            snn_require(!output.read_line());
            snn_require(!output);

            snn_require(output.exit_status() == constant::exit::success);
        }

        {
            process::command cmd;
            cmd << "/bin/ls -a1";

            auto output = process::execute_and_consume_output(cmd);

            snn_require(output);

            bool found_self = false;

            // Return a view of the internal buffer, this view is only valid until the next
            // read_line() call or until the output object is destroyed, whichever comes first.
            while (const auto res = output.read_line<cstrview>())
            {
                const cstrview line = res.value();
                snn_require(!line.is_empty());

                if (line == "output.test.cc\n")
                {
                    found_self = true;
                }
            }

            snn_require(found_self);

            snn_require(!output.read_line());
            snn_require(!output);

            snn_require(output.exit_status() == constant::exit::success);

            static_assert(noexcept(output.read_line<cstrview>()));
            static_assert(!noexcept(output.read_line()));
            static_assert(!noexcept(output.read_line<strbuf>()));
        }
    }
}
