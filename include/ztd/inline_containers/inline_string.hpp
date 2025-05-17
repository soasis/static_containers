// =============================================================================
// ztd.inline_containers
//
// © ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#pragma once

#ifndef ZTD_INLINE_CONTAINERS_INLINE_STRING_HPP
#define ZTD_INLINE_CONTAINERS_INLINE_STRING_HPP

#include <ztd/inline_containers/version.hpp>

#include <ztd/inline_containers/inline_basic_string.hpp>

#include <ztd/idk/charN_t.hpp>

namespace ztd {
	ZTD_INLINE_CONTAINERS_INLINE_ABI_NAMESPACE_OPEN_I_

	template <::std::size_t _Capacity>
	using inline_string = ::ztd::inline_basic_string<char, _Capacity>;

	template <::std::size_t _Capacity>
	using inline_wstring = ::ztd::inline_basic_string<wchar_t, _Capacity>;

	template <::std::size_t _Capacity>
	using inline_u8string = ::ztd::inline_basic_string<::ztd::uchar8_t, _Capacity>;

	template <::std::size_t _Capacity>
	using inline_u16string = ::ztd::inline_basic_string<::ztd::uchar16_t, _Capacity>;

	template <::std::size_t _Capacity>
	using inline_u32string = ::ztd::inline_basic_string<::ztd::uchar32_t, _Capacity>;

	ZTD_INLINE_CONTAINERS_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif
