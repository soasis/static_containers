//   -----------------
// | ztd::fixed_vector |
//   -----------------

// Written 2019 - 2021 by ThePhD <phdofthehouse@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#pragma once

#ifndef ZTD_FIXED_VECTOR_DETAIL_MEMORY_HPP
#define ZTD_FIXED_VECTOR_DETAIL_MEMORY_HPP

#include <ztd/fixed_vector/version.hpp>

#include <ztd/fixed_vector/detail/type_traits.hpp>

#include <memory>

namespace ztd {

	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_OPEN_I_

	namespace __fv_detail { namespace __adl {

		template <typename _Type>
		using __detect_to_address = decltype(::std::pointer_traits<_Type>::to_address(::std::declval<_Type&>()));

		template <typename _Type>
		constexpr _Type* __adl_to_address(_Type* __ptr) noexcept {
			static_assert(!::std::is_function_v<_Type>, "the pointer shall not be function pointer type");
			return __ptr;
		}

		template <typename _Pointer, ::std::enable_if_t<!::std::is_pointer_v<_Pointer>>* = nullptr>
		auto __adl_to_address(const _Pointer& p) noexcept {
			if constexpr (__is_detected_v<__detect_to_address, _Pointer>) {
				return ::std::pointer_traits<_Pointer>::to_address(p);
			}
			else {
				return __adl_to_address(p.operator->());
			}
		}
	}} // namespace __fv_detail::__adl

	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif // ZTD_FIXED_VECTOR_DETAIL_MEMORY_HPP