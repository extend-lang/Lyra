/*
Copyright 2020 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>
#include <sstream>
#include <string>

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		bool a = false;
		bool show_help = false;
		auto cli = lyra::cli() | opt(a)["-a"] | help(show_help);
		std::ostringstream help_text;
		auto result = cli.parse({ "TestApp", "-?" });
		help_text << cli;
		test(REQUIRE(result));
		test(REQUIRE(help_text.str().find("USAGE:") != std::string::npos));
	}
	{
		bool a = false;
		bool show_help = false;
		auto cli = lyra::cli() | opt(a)["-a"] | help(show_help);
		std::ostringstream help_text;
		auto result = cli.parse({ "", "-?" });
		help_text << cli;
		test(REQUIRE(result));
		test(REQUIRE(help_text.str().find("USAGE:") == std::string::npos));
	}
	{
		std::string named_required;
		auto opt_required
			= lyra::opt(named_required, "required-arg")["--required"](
				"You must supply this arg");
		test(REQUIRE(
			opt_required.get_usage_text().find("--required")
			!= std::string::npos));
		test(REQUIRE(
			opt_required.get_usage_text().find("<required-arg>")
			!= std::string::npos));
	}
	{
		bool show_help = false;
		bool show_subcmd_help = false;
		std::ostringstream help_text;
		const auto sub = [&](const group& g)
		{
			if (show_subcmd_help) help_text << g;
		};
		auto cli = lyra::cli()
			| command("subcmd", sub)
				.help("SUBCMD DESCRIPTION")
				.add_argument(help(show_subcmd_help)
					.description("SUBCMD DESCRIPTION"))
			| help(show_help)
			;
		{
			auto result = cli.parse({ "TestApp", "--help" });
			test(REQUIRE(result));
			test(REQUIRE(help_text.str().find("SUBCMD DESCRIPTION")
				== std::string::npos));
		}
		{
			auto result = cli.parse({ "TestApp", "subcmd", "--help" });
			test(REQUIRE(result));
			test(REQUIRE(help_text.str().find("SUBCMD DESCRIPTION")
				!= std::string::npos));
		}
	}

	return test;
}
