// Copyright 2021 René Ferdinand Rivera Morell
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_SCOPED_HPP
#define LYRA_DETAIL_SCOPED_HPP

#include <functional>

namespace lyra { namespace detail {
class scoped
{
	public:
	scoped(std::function<void()> entry, std::function<void()> && exit)
		: exit_function(std::move(exit))
	{
		entry();
	}

	~scoped() { exit_function(); }

	private:
	std::function<void()> exit_function;
};
}} // namespace lyra::detail

#endif
