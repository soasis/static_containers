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

#ifndef ZTD_FIXED_VECTOR_FIXED_VECTOR_HPP
#define ZTD_FIXED_VECTOR_FIXED_VECTOR_HPP

#include <phd/fixed_vector/detail/uninit.hpp>
#include <phd/fixed_vector/detail/wrap_pointer.hpp>

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <memory>
#include <cassert>

namespace ztd {
	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_OPEN_I_

	template <typename _Ty, ::std::size_t _Capacity>
	class fixed_vector {
	public:
		using value_type      = _Ty;
		using reference       = ::std::add_lvalue_reference_t<_Ty>;
		using const_reference = ::std::add_lvalue_reference_t<const _Ty>;
		using pointer         = ::std::add_pointer_t<_Ty>;
		using const_pointer   = ::std::add_pointer_t<const _Ty>;
		using iterator        = __fv_detail::__wrap_pointer<__fv_detail::__uninit<_Ty>>;
		using const_iterator  = __fv_detail::__wrap_pointer<const __fv_detail::__uninit<_Ty>>;
		using size_type       = ::std::size_t;
		using difference_type = ::std::ptrdiff_t;
		inline static constexpr const ::std::size_t inline_capacity = _Capacity;

	private:
		using __alloc        = ::std::allocator<_Ty>;
		using __alloc_traits = ::std::allocator_traits<__alloc>;

	public:
		constexpr fixed_vector() noexcept : _M_size(), _M_elements() {
		}

		fixed_vector(::std::initializer_list<value_type> __values) noexcept : _M_size(), _M_elements() {
			this->insert(this->cend(), ::std::move(__values));
		}

		fixed_vector(size_type __count) noexcept : _M_size(), _M_elements() {
			this->insert(this->cend(), __count, value_type {});
		}

		template <typename... _Args>
		void emplace_back(_Args&&... __args) noexcept(::std::is_nothrow_constructible_v<value_type, _Args...>) {
			__alloc __al;
			__alloc_traits::construct(
			     __al, ::std::addressof(this->_M_elements[this->_M_size]), ::std::forward<_Args>(__args)...);
			++this->_M_size;
		}

		void push_back(const value_type& __value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>) {
			__alloc __al;
			__alloc_traits::construct(__al, ::std::addressof(this->_M_elements[this->_M_size]), __value);
			++this->_M_size;
		}

		void push_back(value_type&& __value) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
			__alloc __al;
			__alloc_traits::construct(
			     __al, ::std::addressof(this->_M_elements[this->_M_size]), ::std::move(__value));
			++this->_M_size;
		}

		template <typename... _Args>
		void emplace_front(_Args&&... __args) noexcept(::std::is_nothrow_constructible_v<value_type, _Args...>) {
			this->emplace(this->cbegin(), ::std::forward<_Args>(__args)...);
		}

		void push_front(const value_type& __value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>) {
			this->insert(this->cbegin(), __value);
		}

		void push_front(value_type&& __value) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
			this->insert(this->cbegin(), ::std::move(__value));
		}

		template <typename... _Args>
		iterator emplace(const_iterator __where, _Args&&... __args) noexcept(
		     ::std::is_nothrow_constructible_v<value_type,
		          _Args...> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			iterator __where_last = this->cend();
			if (this->empty() || __where == __where_last) {
				this->emplace_back(::std::forward<_Args>(__args)...);
			}
			difference_type __where_dist = __where - this->cbegin();
			assert(__where_dist < this->size());
			__alloc __al;

			iterator __where_first(const_cast<pointer>(__where.base()));
			iterator __where_p      = __where_first - 2;
			iterator __from_where_p = __where_last - 1;

			__alloc_traits::construct(
			     __al, __fv_detail::__adl::__adl_to_address(__where_last), ::std::move(*__from_where_p));

			for (; __from_where_p != __where_first; --__where_p, (void)__from_where_p) {
				if constexpr (::std::is_nothrow_move_assignable_v<value_type>) {
					*__where_p = ::std::move(*__from_where_p);
				}
				else {
					*__where_p = *__from_where_p;
				}
			}
			__alloc_traits::construct(
			     __al, __fv_detail::__adl::__adl_to_address(__where_first), ::std::forward<_Args>(__args)...);
		}

		iterator insert(const_iterator __where, value_type&& __value) noexcept(
		     ::std::is_nothrow_move_constructible_v<
		          value_type> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			return this->emplace(::std::move(__where), ::std::move(__value));
		}

		iterator insert(const_iterator __where, const value_type& __value) noexcept(
		     ::std::is_nothrow_copy_constructible_v<
		          value_type> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			return this->emplace(::std::move(__where), __value);
		}

		iterator insert(const_iterator __where, ::std::initializer_list<value_type> __values) noexcept(
		     ::std::is_nothrow_copy_constructible_v<
		          value_type> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			return this->insert(::std::move(__where), __values.begin(), __values.end());
		}

		iterator insert(const_iterator __where, size_type __count) noexcept {
			return this->insert(::std::move(__where), __count, value_type {});
		}

		iterator insert(const_iterator __where, size_type __count, const value_type& __value) noexcept {
			if (__count == 0) {
				return iterator(const_cast<pointer>(__where.base()));
			}
			difference_type __where_dist = __where.base() - this->data();
			assert(__where_dist < this->size());
			assert(static_cast<size_type>(__count + __where_dist) <= this->capacity());
			return this->_M_unchecked_multi_insert_count(::std::move(__where), __where_dist, __count, __value);
		}

		template <typename _First, typename _Last,
		     ::std::enable_if_t<
		          !::std::is_integral_v<
		               _First> && !::std::is_same_v<::std::remove_cvref_t<_Last>, value_type>>* = nullptr>
		iterator insert(const_iterator __where, _First __first, _Last __last) {
			if (__first == __last) {
				return iterator(const_cast<pointer>(__where.base()));
			}
			difference_type __where_dist = __where.base() - this->data();
			assert(__where_dist < this->size());
			// FIXME: lazy dependent-false
			// idiom-check iterators
			if constexpr (::std::is_void_v<value_type>) {
				auto __fl_dist = __last - __first;
				assert(static_cast<size_type>(__fl_dist + __where_dist) <= this->capacity());
				return this->_M_unchecked_multi_insert_random_access(
				     ::std::move(__where), __where_dist, ::std::move(__first), ::std::move(__last));
			}
			else {
				iterator __where_first     = this->emplace(::std::move(__where), *__first);
				iterator __where_currently = __where_first;
				for (; __first != __last; ++__first) {
					++__where_currently;
					__where_currently = this->emplace(::std::move(__where_currently), *__first);
				}
				return __where_first;
			}
		}

		constexpr iterator erase(const_iterator __where) noexcept {
			difference_type __where_dist = __where - this->begin();
			assert(__where_dist < this->_M_size);
			if (__where_dist == this->_M_size) {
				return iterator(const_cast<pointer>(::std::move(__where).base()));
			}
			if (__where_dist == 0) {
				pop_front();
				return this->begin();
			}
			return this->_M_unchecked_erase(::std::move(__where));
		}

		constexpr iterator erase(const_iterator __where_first, const_iterator __where_last) noexcept {
			if (__where_first == __where_last) {
				return iterator(const_cast<pointer>(::std::move(__where_first).base()));
			}
			difference_type __where_diff = __where_last - __where_first;
			assert(__where_diff < this->_M_size);
			if (__where_diff == this->size()) {
				this->clear();
				return this->begin();
			}
			return this->_M_unchecked_multi_erase(
			     ::std::move(__where_first), ::std::move(__where_last), __where_diff);
		}

		constexpr void pop_back() noexcept {
			assert(this->_M_size != 0);
			this->_M_elements[this->_M_size - 1]._M_value.~value_type();
			--this->_M_size;
		}

		constexpr void pop_front() noexcept {
			assert(this->_M_size != 0);
			this->_M_unchecked_erase(this->begin());
		}

		constexpr reference front() noexcept {
			return this->_M_elements[0]._M_value;
		}

		constexpr const_reference front() const noexcept {
			return this->_M_elements[0]._M_value;
		}

		constexpr reference back() noexcept {
			return this->_M_elements[this->_M_size - 1]._M_value;
		}

		constexpr const_reference back() const noexcept {
			return this->_M_elements[this->_M_size - 1]._M_value;
		}

		constexpr bool empty() const noexcept {
			return this->_M_size == 0;
		}

		constexpr void clear() noexcept {
			this->_M_destroy_all();
		}

		constexpr size_type size() const noexcept {
			return this->_M_size;
		}

		constexpr size_type capacity() const noexcept {
			return _Capacity;
		}

		constexpr pointer data() noexcept {
			return ::std::addressof(this->_M_elements[0]._M_value);
		}

		constexpr const_pointer data() const noexcept {
			return ::std::addressof(this->_M_elements[0]._M_value);
		}

		constexpr iterator begin() noexcept {
			return iterator(this->data());
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator(this->data());
		}

		constexpr const_iterator cbegin() const noexcept {
			return const_iterator(this->data());
		}

		constexpr iterator end() noexcept {
			return iterator(this->_M_data_end());
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(this->_M_data_end());
		}

		constexpr const_iterator cend() const noexcept {
			return const_iterator(this->_M_data_end());
		}

		void resize(size_type __count) {
			if (__count == 0) {
				this->clear();
				return;
			}

			if (this->size() < __count) {
				size_type __diff = __count - this->size();
				this->insert(this->cend(), __diff, value_type {});
			}
			else if (this->size() > __count) {
				size_type __diff = this->size() - __count;
				this->erase(this->begin() + __diff, this->cend());
			}
		}

		void resize(size_type __count, const value_type& __value) {
			if (__count == 0) {
				this->clear();
				return;
			}

			if (this->size() < __count) {
				size_type __diff = __count - this->size();
				this->insert(this->cend(), __diff, __value);
			}
			else if (this->size() > __count) {
				size_type __diff = this->size() - __count;
				this->erase(this->begin() + __diff, this->cend());
			}
		}

		~fixed_vector() {
			this->_M_destroy_all();
		}

	private:
		size_type _M_size;
		__fv_detail::__uninit<value_type> _M_elements[_Capacity];

		constexpr pointer _M_data_end() {
			return ::std::addressof(this->_M_elements[this->_M_size]._M_value);
		}

		constexpr const_pointer _M_data_end() const {
			return ::std::addressof(this->_M_elements[this->_M_size]._M_value);
		}

		constexpr iterator _M_unchecked_erase(const_pointer __where_cp) noexcept {
			iterator __where_first(const_cast<pointer>(__where_cp));
			iterator __where_last = this->begin() + this->size();
			for (iterator __where_p = __where_first, __from_where_p = __where_first + 1;
			     __from_where_p != __where_last; ++__where_p, (void)++__from_where_p) {
				if constexpr (::std::is_nothrow_move_assignable_v<value_type>) {
					*__where_p = ::std::move(*__from_where_p);
				}
				else if constexpr (::std::is_nothrow_copy_assignable_v<value_type>) {
					*__where_p = *__from_where_p;
				}
				else {
					static_assert(::std::is_nothrow_copy_assignable_v<value_type>,
					     "this type is neither nothrow copy-assignable or nothrow move-constructible, making "
					     "it "
					     "impossible to properly copy in a constexpr constext in a pre-C++20 world.");
				}
			}
			__where_last->~value_type();
			--this->_M_size;
			return __where_first;
		}

		constexpr iterator _M_unchecked_multi_erase(
		     const_iterator __first, const_iterator __last, difference_type __where_diff) noexcept {
			iterator __where_first(const_cast<pointer>(__first.base()));
			iterator __where_last      = this->end();
			iterator __where_diff_last = __where_last - __where_diff;
			if constexpr (::std::is_nothrow_move_assignable_v<value_type>) {
				for (iterator __where_p = __where_first, __from_where_p = __where_diff_last;
				     __from_where_p != __where_first; ++__where_p, (void)++__from_where_p) {
					*__where_p = ::std::move(*__from_where_p);
				}
			}
			else if (::std::is_nothrow_copy_assignable_v<value_type>) {
				for (iterator __where_p = __where_first, __from_where_p = __where_diff_last;
				     __from_where_p != __where_first; ++__where_p, (void)++__from_where_p) {
					*__where_p = *__from_where_p;
				}
			}
			// FIXME: when construct_at and destroy_at are valid on automatic storage
			// duration variables, we can constexpr destruct then constexpr-construct
#if 0
			else if (::std::is_nothrow_move_constructible_v<value_type) {
			}
			else if (::std::is_nothrow_copy_constructible_v<value_type>) {
			}
#endif
			else {
				static_assert(::std::is_nothrow_copy_assignable_v<value_type>,
				     "this type is neither nothrow copy-assignable or nothrow move-constructible, making it "
				     "impossible to properly copy in a constexpr constext in a pre-C++20 world.");
			}
			for (pointer __where_p = __where_last - 1;; --__where_p) {
				__where_p->~value_type();
				if (__where_p == __where_diff_last) {
					break;
				}
			}
			--this->_M_size;
			return __where_first;
		}

		void _M_destroy_all() noexcept {
			if constexpr (::std::is_nothrow_destructible_v<value_type>) {
				for (size_type __index = this->_M_size; __index-- > 0;) {
					this->_M_elements[__index]._M_value.~value_type();
				}
				this->_M_size = 0;
			}
			else {
				for (size_type __index = this->_M_size; __index-- > 0;) {
					this->_M_elements[__index]._M_value.~value_type();
					this->_M_size -= 1;
				}
			}
		}
	};

	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif // ZTD_FIXED_VECTOR_FIXED_VECTOR_HPP
