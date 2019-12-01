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

#ifndef PHD_FIXED_VECTOR_DETAIL_WRAP_POINTER_HPP
#define PHD_FIXED_VECTOR_DETAIL_WRAP_POINTER_HPP

#include <phd/FIXED_VECTOR/detail/type_traits.hpp>

#include <utility>
#include <iterator>
#include <memory>

namespace phd { namespace __detail { inline namespace __0 {

	class __contiguous_iterator_tag : public ::std::random_access_iterator_tag {};

	template <typename _Type>
	using __is_unwrappable_value_test = decltype(__unwrap_value(::std::declval<_Type>()));

	template <typename _Type>
	using __is_unwrappable_ptr_value_test = decltype(__unwrap_iterator_value(::std::declval<_Type>()));

	template <typename _Type>
	inline constexpr bool __is_unwrappable_value_v = __is_detected_v<__is_unwrappable_value_test, _Type>;

	template <typename _Type>
	inline constexpr bool __is_unwrappable_iterator_value_v = __is_detected_v<__is_unwrappable_ptr_value_test, _Type>;

	template <typename _Type>
	constexpr decltype(auto) __unwrap(_Type&& __value) noexcept {
		if constexpr (__is_unwrappable_value_v<_Type>) {
			return __unwrap_value(::std::forward<_Type>(__value));
		}
		else {
			return ::std::forward<_Type>(__value);
		}
	}

	template <typename _Type>
	constexpr decltype(auto) __unwrap_ptr(_Type&& __value) noexcept {
		if constexpr (__is_unwrappable_iterator_value_v<_Type>) {
			return __unwrap_iterator_value(::std::forward<_Type>(__value));
		}
		else if constexpr (__is_unwrappable_value_v<_Type>) {
			return ::std::addressof(__unwrap_value(*::std::forward<_Type>(__value)));
		}
		else {
			return ::std::forward<_Type>(__value);
		}
	}

	template <typename _Type>
	class __wrap_pointer {
	private:
		using __unwrapped_type = decltype(__unwrap(::std::declval<_Type&>()));

	public:
		using iterator_category = __contiguous_iterator_tag;
		using value_type        = ::std::remove_cvref_t<__unwrapped_type>;
		using element_type      = ::std::remove_reference_t<__unwrapped_type>;
		using reference         = ::std::add_lvalue_reference_t<__unwrapped_type>;
		using pointer           = ::std::add_pointer_t<element_type>;
		using difference_type   = decltype(::std::declval<const pointer&>() - ::std::declval<const pointer&>());

		constexpr __wrap_pointer() : __wrap_pointer(nullptr) {
		}
		constexpr __wrap_pointer(pointer __ptr) : _M_ptr(__ptr) {
		}
		constexpr __wrap_pointer(const __wrap_pointer&) = default;
		constexpr __wrap_pointer(__wrap_pointer&&)      = default;
		constexpr __wrap_pointer& operator=(const __wrap_pointer&) = default;
		constexpr __wrap_pointer& operator=(__wrap_pointer&&) = default;

		constexpr explicit operator bool() const noexcept {
			return this->_M_ptr != nullptr;
		}

		constexpr pointer base() const noexcept {
			return this->_M_ptr;
		}

		constexpr reference operator[](difference_type __index) const noexcept {
			pointer __ptr = this->_M_ptr + __index;
			return __unwrap(*__ptr);
		}

		constexpr reference operator*() const noexcept {
			return __unwrap(*this->_M_ptr);
		}

		constexpr pointer operator->() const noexcept {
			return ::std::addressof(__unwrap(*this->_M_ptr));
		}

		constexpr __wrap_pointer& operator++() noexcept {
			++this->_M_ptr;
			return *this;
		}

		constexpr __wrap_pointer operator++(int) noexcept {
			auto __copy = *this;
			++__copy;
			return __copy;
		}

		constexpr __wrap_pointer& operator--() noexcept {
			--this->_M_ptr;
			return *this;
		}

		constexpr __wrap_pointer operator--(int) noexcept {
			auto __copy = *this;
			--__copy;
			return __copy;
		}

		constexpr __wrap_pointer& operator+=(difference_type __right) noexcept {
			this->_M_ptr += __right;
			return *this;
		}

		constexpr __wrap_pointer& operator-=(difference_type __right) noexcept {
			this->_M_ptr -= __right;
			return *this;
		}

		constexpr __wrap_pointer operator+(difference_type __right) const noexcept {
			return __wrap_pointer(this->_M_ptr + __right);
		}

		constexpr __wrap_pointer operator-(difference_type __right) const noexcept {
			return __wrap_pointer(this->_M_ptr - __right);
		}

		constexpr difference_type operator-(const __wrap_pointer& __right) const noexcept {
			return __right.base() - this->_M_ptr;
		}

		friend constexpr pointer to_address(const __wrap_pointer& __wrapped) noexcept {
			return __unwrap_iterator_value(__wrapped._M_ptr);
		}

	private:
		pointer _M_ptr;
	};

	template <typename _LeftType, typename _RightType>
	constexpr bool operator==(const __wrap_pointer<_LeftType>& __left, const __wrap_pointer<_RightType>& __right) {
		return __left.base() == __right.base();
	}

	template <typename _RightType>
	constexpr bool operator==(::std::nullptr_t __left, const __wrap_pointer<_RightType>& __right) {
		return __left == __right.base();
	}

	template <typename _LeftType>
	constexpr bool operator==(const __wrap_pointer<_LeftType>& __left, ::std::nullptr_t __right) {
		return __left.base() == __right;
	}

	template <typename _LeftType, typename _RightType>
	constexpr bool operator!=(const __wrap_pointer<_LeftType>& __left, const __wrap_pointer<_RightType>& __right) {
		return __left.base() != __right.base();
	}

	template <typename _RightType>
	constexpr bool operator!=(::std::nullptr_t __left, const __wrap_pointer<_RightType>& __right) {
		return __left != __right.base();
	}

	template <typename _LeftType>
	constexpr bool operator!=(const __wrap_pointer<_LeftType>& __left, ::std::nullptr_t __right) {
		return __left.base() != __right;
	}

}}} // namespace phd::__detail::__0

namespace std {
	template <typename _Type>
	struct pointer_traits<::phd::__detail::__wrap_pointer<_Type>> {
		using pointer         = typename ::phd::__detail::__wrap_pointer<_Type>::pointer;
		using element_type    = typename ::phd::__detail::__wrap_pointer<_Type>::element_type;
		using difference_type = typename ::phd::__detail::__wrap_pointer<_Type>::difference_type;

		static constexpr pointer pointer_to(element_type& __element) {
			return ::std::addressof(__element);
		}

		static constexpr pointer to_address(const ::phd::__detail::__wrap_pointer<_Type>& __element) {
			return to_address(__element);
		}
	};

} // namespace std

#endif // PHD_FIXED_VECTOR_DETAIL_WRAP_POINTER_HPP
