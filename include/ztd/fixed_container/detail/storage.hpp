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

#ifndef ZTD_FIXED_CONTAINER_DETAIL_STORAGE_HPP
#define ZTD_FIXED_CONTAINER_DETAIL_STORAGE_HPP

#include <ztd/fixed_container/version.hpp>

#include <ztd/fixed_container/forward.hpp>

#include <ztd/idk/type_traits.hpp>
#include <ztd/idk/uninit.hpp>

#include <cstddef>
#include <climits>

namespace ztd {
	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_OPEN_I_

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


		template <typename _Ty, ::std::size_t _Capacity, bool = ::std::is_trivial_v<_Ty>>
		class __storage {
		private:
			template <typename, ::std::size_t>
			friend class ::ztd::fixed_vector;
			template <typename, ::std::size_t>
			friend class ::ztd::fixed_basic_string;

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

			using __storage_layout = ::std::conditional_t<sizeof(__size_first) < sizeof(__size_last),
			     __size_first, // size first is better compacted
			     __size_last   // otherwise, size last is better / evenly sized
			     >;

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
			friend class ::ztd::fixed_vector;
			template <typename, ::std::size_t>
			friend class ::ztd::fixed_basic_string;

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

			using __storage_layout = ::std::conditional_t<sizeof(__size_first) < sizeof(__size_last),
			     __size_first, // size first is better compacted
			     __size_last   // otherwise, size last is better / evenly computed
			     >;

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

	ZTD_FIXED_CONTAINER_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif // ZTD_FIXED_CONTAINER_DETAIL_STORAGE_HPP
