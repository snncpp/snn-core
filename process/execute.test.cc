// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/process/execute.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            // int execute(const command& cmd)
            {
                process::command cmd;
                cmd << "/usr/bin/true";
                const int exit_status = process::execute(cmd);
                snn_require(exit_status == constant::exit::success);
            }
            {
                process::command cmd;
                cmd << "/usr/bin/false";
                const int exit_status = process::execute(cmd);
                snn_require(exit_status != constant::exit::success);
            }
            {
                process::command cmd;
                cmd << "/bin/echo " << cstrview{"foo\0bar"};
                snn_require_throws_code(process::execute(cmd),
                                        generic::error::unexpected_null_character);
            }

            // output execute_and_consume_output(const command& cmd)
            {
                process::command cmd;
                cmd << "/bin/ls -a1";

                auto output = process::execute_and_consume_output(cmd);

                snn_require(output);

                while (auto res = output.read_line())
                {
                    const auto& line = res.value();
                    snn_require(!line.is_empty());
                }

                snn_require(!output.read_line());
                snn_require(!output);

                snn_require(output.exit_status() == constant::exit::success);
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
    }
}
