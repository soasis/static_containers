//   -----------------
// | phd::fixed_vector |
//   -----------------

// Written in 2019 by ThePhD <phdofthehouse@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#pragma once

#ifndef PHD_FIXED_VECTOR_DETAIL_UNINIT_HPP
#define PHD_FIXED_VECTOR_DETAIL_UNINIT_HPP

#include <utility>

namespace phd { namespace __detail { inline namespace __0 {

	template <typename _Type>
	union alignas(_Type) __uninit {
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

		char _M_dummy;
		_Type _M_value;
	};

}}} // namespace phd::__detail::__0

#endif // PHD_FIXED_VECTOR_DETAIL_UNINIT_HPP
