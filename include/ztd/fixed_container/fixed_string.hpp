// =============================================================================
// ztd::fixed_container
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

#ifndef ZTD_FIXED_CONTAINER_FIXED_STRING_HPP
#define ZTD_FIXED_CONTAINER_FIXED_STRING_HPP

#include <ztd/fixed_container/version.hpp>

#include <ztd/fixed_container/fixed_basic_string.hpp>

#include <ztd/idk/charN_t.hpp>

namespace ztd {
	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_OPEN_I_

	template <::std::size_t _Capacity>
	using fixed_string = ::ztd::fixed_basic_string<char, _Capacity>;

	template <::std::size_t _Capacity>
	using fixed_wstring = ::ztd::fixed_basic_string<::ztd::uwchar_t, _Capacity>;

	template <::std::size_t _Capacity>
	using fixed_u8string = ::ztd::fixed_basic_string<::ztd::uchar8_t, _Capacity>;

	template <::std::size_t _Capacity>
	using fixed_u16string = ::ztd::fixed_basic_string<::ztd::uchar16_t, _Capacity>;

	template <::std::size_t _Capacity>
	using fixed_u32string = ::ztd::fixed_basic_string<::ztd::uchar32_t, _Capacity>;

	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif // ZTD_FIXED_CONTAINER_FIXED_STRING_HPP
