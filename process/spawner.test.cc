// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/process/spawner.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/reader_writer.hh"
#include "snn-core/process/pipe.hh"
#include "snn-core/stream/buffered_reader.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            process::spawner spawner{"/bin/echo", {"foo", "bar"}};

            process::pipe out_pipe;

            spawner.set_standard_out(std::move(out_pipe.writing_end()));

            process::handle h = spawner.spawn().value();
            snn_require(h);

            // Close file descriptors that are no longer needed in parent.
            spawner.standard_out().close().or_throw();

            auto child_out =
                stream::buffered_reader{file::reader_writer{std::move(out_pipe.reading_end())}};

            snn_require(child_out.read_line<cstrview>().value() == "foo bar\n");

            snn_require(h);

            process::termination_status term = h.wait().value();

            snn_require(term.with_exit_status());
            snn_require(term.exit_status() == 0);

            snn_require(!h);

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
            // Will search the directories specified in the `PATH` environment variable for a `true`
            // executable.
            str path = "true";

            result<process::handle> spawn_res = process::spawner{path}.spawn();

            // On FreeBSD `spawn_res` will  hold `error_code{ENOENT, system::error_category}` if no
            // executable was found.

            snn_require(spawn_res); // Successful spawn?

            process::handle handle = std::move(spawn_res.value());

            // `handle` will always hold a process id after a successful `spawn()`.
            snn_require(handle);
            snn_require(handle.has_value());
            snn_require(handle.value() > 0); // Process id 0 is reserved for kernel/scheduler.

            result<process::termination_status> wait_res = handle.wait();

            snn_require(wait_res); // Successful wait?

            process::termination_status term = wait_res.value();

            // Check that the process exited normally (not by signal) and that the exit status is
            // what we expect.
            snn_require(term.with_exit_status());
            snn_require(term.exit_status() == 0);

            // The process has terminated, so `handle` no longer holds the process id.
            snn_require(!handle);
            snn_require(!handle.has_value());
        }

        {
            // Will search the directories specified in the `PATH` environment variable for a
            // `false` executable.
            str path = "false";

            process::handle handle = process::spawner{path}.spawn().value();

            snn_require(handle);
            snn_require(handle.has_value());
            snn_require(handle.value() > 0);
            snn_require(handle.value(assume::has_value) > 0);
            snn_require(handle.value_or(-1) > 0);

            auto pid = handle.value();

            // Move constructor.
            process::handle tmp{std::move(handle)};
            snn_require(tmp.has_value());
            snn_require(tmp.value() == pid);
            snn_require(!handle);
            snn_require(!handle.has_value());

            // Move assignment operator.
            handle = std::move(tmp);
            snn_require(handle.has_value());
            snn_require(handle.value() == pid);
            snn_require(!tmp);
            snn_require(!tmp.has_value());

            process::termination_status term = handle.wait().value();

            snn_require(term.with_exit_status());
            snn_require(!term.by_signal());

            snn_require(term.exit_status() == 1);

            snn_require(!handle);
            snn_require(!handle.has_value());
            snn_require(handle.value_or(-1) == -1);
            snn_require(!handle.wait());
        }

        {
            file::reader_writer dev_null{"/dev/null", file::option::write_only};
            process::pipe in_pipe;
            process::pipe out_pipe;

            // Will search the directories specified in the `PATH` environment variable for a `tee`
            // executable.
            process::spawner spawner{"tee"};

            spawner.set_standard_in(std::move(in_pipe.reading_end()));
            spawner.set_standard_out(std::move(out_pipe.writing_end()));
            spawner.set_standard_error(std::move(dev_null.descriptor()));

            process::handle h = spawner.spawn().value();
            snn_require(h);
            snn_require(h.has_value());
            snn_require(h.value() > 0);

            // Close file descriptors that are no longer needed in parent.
            snn_require(spawner.standard_in().close());
            snn_require(spawner.standard_out().close());
            snn_require(spawner.standard_error().close());

            auto child_in = file::reader_writer{std::move(in_pipe.writing_end())};
            auto child_out =
                stream::buffered_reader{file::reader_writer{std::move(out_pipe.reading_end())}};

            child_in << "fooBAR";
            snn_require(child_out.read_until<cstrview>("BAR").value() == "fooBAR");

            child_in << "ONE TWO THREE\n";
            snn_require(child_out.read_line<cstrview>().value() == "ONE TWO THREE\n");

            // Close `stdin` so tee will exit.
            snn_require(child_in.close());

            process::termination_status term = h.wait().value();

            snn_require(term.with_exit_status());
            snn_require(term.exit_status() == 0);

            snn_require(!h);
        }

        {
            process::spawner spawner{"/bin/echo", {"foo", "bar"}};

            const vec<str>& arguments = spawner.arguments();
            snn_require(arguments.count() == 2);
            snn_require(arguments.at(0).value() == "foo");
            snn_require(arguments.at(1).value() == "bar");
        }

        {
            process::spawner spawner{"/bin/echo"};

            spawner.arguments().append("foo");
            spawner.arguments().append("bar");

            const vec<str>& arguments = spawner.arguments();
            snn_require(arguments.count() == 2);
            snn_require(arguments.at(0).value() == "foo");
            snn_require(arguments.at(1).value() == "bar");
        }

        {
            process::spawner spawner{"/something/dangerous\0/bin/cat", {"words.txt"}};
            auto res = spawner.spawn();
            snn_require(!res);
            snn_require(res.error_code() == generic::error::unexpected_null_character);
        }

        {
            process::spawner spawner{"cat", {"/something/secret\0words.txt"}};
            auto res = spawner.spawn();
            snn_require(!res);
            snn_require(res.error_code() == generic::error::unexpected_null_character);
        }
    }
}
