// =============================================================================
// ztd.static_containers
//
// Written 2019 - 2022 by ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#pragma once

#ifndef ZTD_STATIC_CONTAINERS_FORWARD_HPP
#define ZTD_STATIC_CONTAINERS_FORWARD_HPP

#include <ztd/static_containers/version.hpp>

#include <cstddef>

namespace ztd {
	ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_OPEN_I_

	template <typename, ::std::size_t>
	class static_vector;

	template <typename, ::std::size_t>
	class static_basic_string;

	ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_CLOSE_I_
} // namespace ztd

#endif
