// =============================================================================
// ztd.static_containers
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

#ifndef ZTD_STATIC_CONTAINERS_DETAIL_STORAGE_HPP
#define ZTD_STATIC_CONTAINERS_DETAIL_STORAGE_HPP

#include <ztd/static_containers/version.hpp>

#include <ztd/static_containers/forward.hpp>

#include <ztd/idk/type_traits.hpp>
#include <ztd/idk/uninit.hpp>

#include <cstddef>
#include <climits>

namespace ztd {
	ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_OPEN_I_

	namespace __fv_detail {

		template <::std::size_t _Capacity>
		struct __optimal_storage_type {
			using type = ::std::conditional_t<_Capacity <= UCHAR_MAX, unsigned char,              // unsigned char
			     ::std::conditional_t<_Capacity <= USHRT_MAX, unsigned short,                     // unsigned short
			          ::std::conditional_t<_Capacity <= UINT_MAX, unsigned int,                   // unsigned int
			               ::std::conditional_t<_Capacity <= ULONG_MAX, unsigned long,            // unsigned long
			                    ::std::conditional_t<_Capacity <= ULLONG_MAX, unsigned long long, // unsigned long
			                                                                                      // long
			                         ::std::size_t // just pick std::size_t if nothing else matches
			                         >             // end unsigned long long choice
			                    >                  // end unsigned long choice
			               >                       // end unsigned int choice
			          >                            // end unsigned short choice
			     >;                                // end unsigned char choice
		};

		template <::std::size_t _Capacity>
		using __optimal_storage_type_t = typename __optimal_storage_type<_Capacity>::type;

		template <typename _LayoutFirst, typename _LayoutB>
		constexpr auto __get_optiomal_layout() noexcept {
			// Cache is measured in 8-bit bytes, versus CHAR_BIT-sized structures
			constexpr ::std::size_t __cache_bit_size        = ZTD_L1_CACHE_LINE_SIZE_I_ * 8;
			constexpr ::std::size_t __layout_first_bit_size = (sizeof(_LayoutFirst) * CHAR_BIT);
			constexpr ::std::size_t __layout_b_bit_size     = (sizeof(_LayoutB) * CHAR_BIT);
			constexpr bool __layout_first_cache_smaller     = __layout_first_bit_size <= __cache_bit_size;
			constexpr bool __layout_b_cache_smaller         = __layout_b_bit_size <= __cache_bit_size;
			if constexpr (__layout_first_cache_smaller && __layout_b_cache_smaller) {
				// if both layouts are smaller than cache
				// pick the one with the optimal size
				if constexpr (sizeof(_LayoutFirst) < sizeof(_LayoutB)) {
					return _LayoutFirst {};
				}
				else {
					return _LayoutB {};
				}
			}
			else if constexpr (__layout_first_cache_smaller) {
				// prefer the one that fits in cache (size first better)
				return _LayoutFirst {};
			}
			else if constexpr (__layout_b_cache_smaller) {
				// prefer the one that fits in cache (size last if it gets us inside of cache)
				return _LayoutB {};
			}
			else {
				// There are 2 intepretations.
				// (1) is always prefer size first when we're beyond cache size.
				// That would mostly be to prevent cache hits, albeit due to alignment it CAN blow up size
				// of a structure considerable....
				// (2) would be to keep smallest, since that would make storage better.
				// Fixed-size vectors are often used to create better storage situations,
				// and we don't have a guarantee this won't be put inside of classes (it often is).
				//
				// Therefore, we pick (2) to prioritize the more general use cases.
#if 0
				// (1)
				return _LayoutFirst{};
#else
				// (2)
				if constexpr (sizeof(_LayoutFirst) < sizeof(_LayoutB)) {
					return _LayoutFirst {};
				}
				else {
					return _LayoutB {};
				}
#endif
			}
		}

		template <typename _Ty, ::std::size_t _Capacity, bool = ::std::is_trivial_v<_Ty>>
		class __storage {
		private:
			template <typename, ::std::size_t>
			friend class ::ztd::static_vector;
			template <typename, ::std::size_t>
			friend class ::ztd::static_basic_string;

			using __value_type   = _Ty;
			using __storage_type = ztd::uninit<__value_type>;

			struct __size_first {
				__optimal_storage_type_t<_Capacity> _M_size;
				__storage_type _M_elements[_Capacity];
			};
			struct __size_last {
				__storage_type _M_elements[_Capacity];
				__optimal_storage_type_t<_Capacity> _M_size;
			};

			using __storage_layout = decltype(__get_optiomal_layout<__size_first, __size_last>());

		public:
			constexpr __storage() noexcept : _M_layout() {
			}

			constexpr ::std::add_pointer_t<__storage_type> _M_storage_data_at(::std::size_t __index) noexcept {
				return ::std::addressof(this->_M_layout._M_elements[__index]);
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<__storage_type>> _M_storage_data_at(
			     ::std::size_t __index) const noexcept {
				return (this->_M_layout._M_elements + __index);
			}

			constexpr ::std::add_pointer_t<__value_type> _M_data(::std::size_t __index) noexcept {
				return ::std::addressof((this->_M_layout._M_elements + __index)->value);
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<__value_type>> _M_data(
			     ::std::size_t __index) const noexcept {
				return ::std::addressof((this->_M_layout._M_elements + __index)->value);
			}

			::std::add_pointer_t<__value_type> data() noexcept {
				return ::std::addressof((this->_M_layout._M_elements + 0)->value);
			}

			::std::add_pointer_t<::std::add_const_t<__value_type>> data() const noexcept {
				return ::std::addressof((this->_M_layout._M_elements + 0)->value);
			}

			~__storage() {
				this->_M_destroy_all();
			}

		private:
			__storage_layout _M_layout;

			constexpr void _M_destroy_all() noexcept {
				if constexpr (::std::is_nothrow_destructible_v<_Ty>) {
					for (::std::size_t __index = this->_M_layout._M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
					}
					this->_M_layout._M_size = 0;
				}
				else {
					for (::std::size_t __index = this->_M_layout._M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
						this->_M_layout._M_size -= 1;
					}
				}
			}
		};

		template <typename _Ty, ::std::size_t _Capacity>
		class __storage<_Ty, _Capacity, true> {
		private:
			template <typename, ::std::size_t>
			friend class ::ztd::static_vector;
			template <typename, ::std::size_t>
			friend class ::ztd::static_basic_string;

			using __value_type   = _Ty;
			using __storage_type = __value_type;

			struct __size_first {
				__optimal_storage_type_t<_Capacity> _M_size;
				__storage_type _M_elements[_Capacity];
			};
			struct __size_last {
				__storage_type _M_elements[_Capacity];
				__optimal_storage_type_t<_Capacity> _M_size;
			};

			using __storage_layout = decltype(__get_optiomal_layout<__size_first, __size_last>());

		public:
			constexpr __storage() noexcept : _M_layout() {
			}

			constexpr ::std::add_pointer_t<__storage_type> _M_storage_data_at(::std::size_t __index) noexcept {
				return this->_M_layout._M_elements + __index;
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<__storage_type>> _M_storage_data_at(
			     ::std::size_t __index) const noexcept {
				return this->_M_layout._M_elements + __index;
			}

			constexpr ::std::add_pointer_t<_Ty> _M_data(::std::size_t __index) noexcept {
				return this->_M_layout._M_elements + __index;
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<_Ty>> _M_data(::std::size_t __index) const noexcept {
				return this->_M_layout._M_elements + __index;
			}

			constexpr ::std::add_pointer_t<_Ty> data() noexcept {
				return this->_M_layout._M_elements + 0;
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<_Ty>> data() const noexcept {
				return this->_M_layout._M_elements + 0;
			}

		private:
			__storage_layout _M_layout;

			constexpr void _M_destroy_all() noexcept {
				if constexpr (::std::is_nothrow_destructible_v<_Ty>) {
					for (::std::size_t __index = this->_M_layout._M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
					}
					this->_M_layout._M_size = 0;
				}
				else {
					for (::std::size_t __index = this->_M_layout._M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
						this->_M_layout._M_size -= 1;
					}
				}
			}
		};
	} // namespace __fv_detail

	ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif
