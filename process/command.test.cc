// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/process/command.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                str filename{"one'two"};

                process::command cmd;
                cmd << "touch " << filename;
                snn_require(cmd.to<cstrview>() == R"(touch 'one'\''two')");
            }
            {
                process::command cmd;

                str filename{"one two"};

                cmd << "chmod " << as_num(644) << " " << filename;
                snn_require(cmd.to<cstrview>() == R"(chmod 644 'one two')");

                cmd.clear();
                cmd << "chmod ";
                cmd.append_integral<math::base::octal>(0b110'100'100, 4);
                cmd << " " << filename;
                snn_require(cmd.to<cstrview>() == R"(chmod 0644 'one two')");

                str echo{"echo"};

                cmd.clear();
                cmd.append_command(echo, promise::is_valid);
                cmd << " " << str{"One"} << " " << strbuf{} << " " << cstrview{"Three'''"};
                snn_require(cmd.to<cstrview>() == R"(echo 'One' '' 'Three'\'\'\''')");

                cmd.clear();
                cmd.append_command(echo, promise::is_valid);
                cmd << " " << cstrview{"'"};
                cmd << " " << cstrview{"''"};
                cmd << " " << cstrview{"'''"};
                snn_require(cmd.to<cstrview>() == R"(echo ''\''' ''\'\''' ''\'\'\''')");

                cmd.clear();
                cmd.append_command(echo, promise::is_valid);
                cmd << " " << cstrview{"a'b''c'''d"};
                snn_require(cmd.to<cstrview>() == R"(echo 'a'\''b'\'\''c'\'\'\''d')");

                static_assert(noexcept(cmd.to<cstrview>()));
                static_assert(!noexcept(cmd.to<strbuf>()));
            }
            {
                // This command will not run.
                process::command cmd;
                cmd << "touch " << str{"one\0two"};
                snn_require(cmd.to<cstrview>() == "touch 'one\0two'");
                snn_require_throws_code(cmd.null_terminated(),
                                        generic::error::unexpected_null_character);
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
    }
}
