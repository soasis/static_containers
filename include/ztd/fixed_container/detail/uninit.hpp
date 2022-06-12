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

#ifndef ZTD_FIXED_CONTAINER_DETAIL_UNINIT_HPP
#define ZTD_FIXED_CONTAINER_DETAIL_UNINIT_HPP

#include <ztd/fixed_container/version.hpp>

#include <utility>

namespace ztd {

	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_OPEN_I_

	namespace __fv_detail {
		template <typename _Type>
		struct alignas(_Type) __uninit {
			constexpr __uninit() : _M_dummy() {
			}

			template <typename... _Args>
			constexpr __uninit(::std::in_place_t, _Args&&... __args) : _M_value(::std::forward<_Args>(__args)...) {
			}

			friend _Type& __unwrap_value(__uninit& __wrapped_value) noexcept {
				return __wrapped_value._M_value;
			}

			friend const _Type& __unwrap_value(const __uninit& __wrapped_value) noexcept {
				return __wrapped_value._M_value;
			}

			friend _Type&& __unwrap_value(__uninit&& __wrapped_value) noexcept {
				return ::std::move(__wrapped_value._M_value);
			}

			union {
				char _M_dummy;
				_Type _M_value;
			};

			~__uninit() {
			}
		};

	} // namespace __fv_detail

	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif // ZTD_FIXED_CONTAINER_DETAIL_UNINIT_HPP
