// =============================================================================
// ztd::fixed_vector
//
// Written 2019 - 2022 by ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// ============================================================================ //

#pragma once

#ifndef ZTD_FIXED_VECTOR_FIXED_VECTOR_HPP
#define ZTD_FIXED_VECTOR_FIXED_VECTOR_HPP

#include <ztd/fixed_vector/detail/uninit.hpp>
#include <ztd/fixed_vector/detail/wrap_pointer.hpp>
#include <ztd/fixed_vector/forward.hpp>

#include <ztd/idk/type_traits.hpp>
#include <ztd/idk/to_address.hpp>
#include <ztd/idk/assert.hpp>

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <memory>
#include <cassert>
#include <iterator>

namespace ztd {
	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_OPEN_I_

	namespace __fv_detail {
		template <typename _Ty, ::std::size_t _Capacity, bool = ::std::is_trivial_v<_Ty>>
		class __storage {
		private:
			template <typename _TyF, ::std::size_t _CapacityF>
			friend class ::ztd::fixed_vector;

			using __value_type   = _Ty;
			using __storage_type = __fv_detail::__uninit<__value_type>;

		public:
			constexpr __storage() noexcept : _M_size(), _M_elements() {
			}

			constexpr ::std::add_pointer_t<__storage_type> _M_storage_data_at(::std::size_t __index) noexcept {
				return ::std::addressof(this->_M_elements[__index]);
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<__storage_type>> _M_storage_data_at(
			     ::std::size_t __index) const noexcept {
				return (this->_M_elements + __index);
			}

			constexpr ::std::add_pointer_t<__value_type> _M_data(::std::size_t __index) noexcept {
				return ::std::addressof((this->_M_elements + __index)->_M_value);
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<__value_type>> _M_data(
			     ::std::size_t __index) const noexcept {
				return ::std::addressof((this->_M_elements + __index)->_M_value);
			}

			::std::add_pointer_t<__value_type> data() noexcept {
				return ::std::addressof((this->_M_elements + 0)->_M_value);
			}

			::std::add_pointer_t<::std::add_const_t<__value_type>> data() const noexcept {
				return ::std::addressof((this->_M_elements + 0)->_M_value);
			}

			~__storage() {
				this->_M_destroy_all();
			}

		private:
			::std::size_t _M_size;
			__storage_type _M_elements[_Capacity];

			constexpr void _M_destroy_all() noexcept {
				if constexpr (::std::is_nothrow_destructible_v<_Ty>) {
					for (::std::size_t __index = this->_M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
					}
					this->_M_size = 0;
				}
				else {
					for (::std::size_t __index = this->_M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
						this->_M_size -= 1;
					}
				}
			}
		};

		template <typename _Ty, ::std::size_t _Capacity>
		class __storage<_Ty, _Capacity, true> {
		private:
			template <typename _TyF, ::std::size_t _CapacityF>
			friend class ::ztd::fixed_vector;
			using __value_type   = _Ty;
			using __storage_type = __value_type;

		public:
			constexpr __storage() noexcept : _M_size(), _M_elements() {
			}

			constexpr ::std::add_pointer_t<__storage_type> _M_storage_data_at(::std::size_t __index) noexcept {
				return this->_M_elements + __index;
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<__storage_type>> _M_storage_data_at(
			     ::std::size_t __index) const noexcept {
				return this->_M_elements + __index;
			}

			constexpr ::std::add_pointer_t<_Ty> _M_data(::std::size_t __index) noexcept {
				return this->_M_elements + __index;
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<_Ty>> _M_data(::std::size_t __index) const noexcept {
				return this->_M_elements + __index;
			}

			constexpr ::std::add_pointer_t<_Ty> data() noexcept {
				return this->_M_elements + 0;
			}

			constexpr ::std::add_pointer_t<::std::add_const_t<_Ty>> data() const noexcept {
				return this->_M_elements + 0;
			}

		private:
			::std::size_t _M_size;
			__storage_type _M_elements[_Capacity];

			constexpr void _M_destroy_all() noexcept {
				if constexpr (::std::is_nothrow_destructible_v<_Ty>) {
					for (::std::size_t __index = this->_M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
					}
					this->_M_size = 0;
				}
				else {
					for (::std::size_t __index = this->_M_size; __index-- > 0;) {
						this->_M_data(__index)->~__value_type();
						this->_M_size -= 1;
					}
				}
			}
		};
	} // namespace __fv_detail

	template <typename _Ty, ::std::size_t _Capacity>
	class fixed_vector : private __fv_detail::__storage<_Ty, _Capacity> {
	private:
		using __base_storage = __fv_detail::__storage<_Ty, _Capacity>;

	public:
		using value_type             = _Ty;
		using reference              = ::std::add_lvalue_reference_t<_Ty>;
		using const_reference        = ::std::add_lvalue_reference_t<const _Ty>;
		using pointer                = ::std::add_pointer_t<_Ty>;
		using const_pointer          = ::std::add_pointer_t<const _Ty>;
		using iterator               = __fv_detail::__wrap_pointer<typename __base_storage::__storage_type>;
		using const_iterator         = __fv_detail::__wrap_pointer<const typename __base_storage::__storage_type>;
		using reverse_iterator       = ::std::reverse_iterator<iterator>;
		using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;
		using size_type              = ::std::size_t;
		using difference_type        = ::std::ptrdiff_t;
		inline static constexpr const ::std::size_t inline_capacity = _Capacity;

	private:
		using __alloc        = ::std::allocator<_Ty>;
		using __alloc_traits = ::std::allocator_traits<__alloc>;

	public:
		constexpr fixed_vector() noexcept : __base_storage() {
		}

		constexpr fixed_vector(::std::initializer_list<value_type> __values) noexcept : __base_storage() {
			this->insert(this->cend(), ::std::move(__values));
		}

		constexpr fixed_vector(size_type __count) noexcept : __base_storage() {
			this->insert(this->cend(), __count, value_type {});
		}

		template <typename... _Args>
		constexpr void emplace_back(_Args&&... __args) noexcept(
		     ::std::is_nothrow_constructible_v<value_type, _Args...>) {
			__alloc __al;
			__alloc_traits::construct(__al, this->_M_data(this->_M_size), ::std::forward<_Args>(__args)...);
			++this->_M_size;
		}

		constexpr void push_back(const value_type& __value) noexcept(
		     ::std::is_nothrow_copy_constructible_v<value_type>) {
			__alloc __al;
			__alloc_traits::construct(__al, this->_M_data(this->_M_size), __value);
			++this->_M_size;
		}

		constexpr void push_back(value_type&& __value) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
			__alloc __al;
			__alloc_traits::construct(__al, this->_M_data(this->_M_size), ::std::move(__value));
			++this->_M_size;
		}

		template <typename... _Args>
		constexpr void emplace_front(_Args&&... __args) noexcept(
		     ::std::is_nothrow_constructible_v<value_type, _Args...>) {
			this->emplace(this->cbegin(), ::std::forward<_Args>(__args)...);
		}

		constexpr void push_front(const value_type& __value) noexcept(
		     ::std::is_nothrow_copy_constructible_v<value_type>) {
			this->insert(this->cbegin(), __value);
		}

		constexpr void push_front(value_type&& __value) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
			this->insert(this->cbegin(), ::std::move(__value));
		}

		template <typename... _Args>
		constexpr iterator emplace(const_iterator __where, _Args&&... __args) noexcept(
		     ::std::is_nothrow_constructible_v<value_type,
		          _Args...> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			iterator __where_last = this->end();
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
			     __al, ::ztd::idk_adl::adl_to_address(__where_last), ::std::move(*__from_where_p));

			for (; __from_where_p != __where_first; --__where_p, (void)__from_where_p) {
				if constexpr (::std::is_nothrow_move_assignable_v<value_type>) {
					*__where_p = ::std::move(*__from_where_p);
				}
				else {
					*__where_p = *__from_where_p;
				}
			}
			__alloc_traits::construct(
			     __al, ::ztd::idk_adl::adl_to_address(__where_first), ::std::forward<_Args>(__args)...);
			return __where_first;
		}

		constexpr iterator insert(const_iterator __where, value_type&& __value) noexcept(
		     ::std::is_nothrow_move_constructible_v<
		          value_type> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			return this->emplace(::std::move(__where), ::std::move(__value));
		}

		constexpr iterator insert(const_iterator __where, const value_type& __value) noexcept(
		     ::std::is_nothrow_copy_constructible_v<
		          value_type> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			return this->emplace(::std::move(__where), __value);
		}

		constexpr iterator insert(const_iterator __where, ::std::initializer_list<value_type> __values) noexcept(
		     ::std::is_nothrow_copy_constructible_v<
		          value_type> && (::std::is_nothrow_move_assignable_v<value_type> || ::std::is_nothrow_copy_assignable_v<value_type>)) {
			return this->insert(::std::move(__where), __values.begin(), __values.end());
		}

		constexpr iterator insert(const_iterator __where, size_type __count) noexcept {
			return this->insert(::std::move(__where), __count, value_type {});
		}

		constexpr iterator insert(const_iterator __where, size_type __count, const value_type& __value) noexcept {
			if (__count == 0) {
				return iterator(const_cast<pointer>(__where.base()));
			}
			difference_type __where_dist = __where - this->begin();
			assert(__where_dist < this->size());
			assert(static_cast<size_type>(__count + __where_dist) <= this->capacity());
			return this->_M_unchecked_multi_insert_count(::std::move(__where), __where_dist, __count, __value);
		}

		template <typename _First, typename _Last,
		     ::std::enable_if_t<
		          !::std::is_integral_v<
		               _First> && !::std::is_same_v<::ztd::remove_cvref_t<_Last>, value_type>>* = nullptr>
		constexpr iterator insert(const_iterator __where, _First __first, _Last __last) {
			if (__first == __last) {
				return iterator(const_cast<pointer>(__where.base()));
			}
			difference_type __where_dist = __where.base() - this->begin();
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
			assert(this->_M_size > 0);
			this->_M_data(this->_M_size - 1)._M_value.~value_type();
			--this->_M_size;
		}

		constexpr void pop_front() noexcept {
			assert(this->_M_size != 0);
			this->_M_unchecked_erase(this->begin());
		}

		constexpr reference front() noexcept {
			assert(this->_M_size > 0);
			return *this->_M_data(0);
		}

		constexpr const_reference front() const noexcept {
			assert(this->_M_size > 0);
			return *this->_M_data(0);
		}

		constexpr reference back() noexcept {
			assert(this->_M_size > 0);
			return *this->_M_data(this->_M_size - 1);
		}

		constexpr const_reference back() const noexcept {
			assert(this->_M_size > 0);
			return *this->_M_data(this->_M_size - 1);
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

		using __base_storage::data;

		constexpr iterator begin() noexcept {
			return iterator(this->_M_storage_data_at(0));
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator(this->_M_storage_data_at(0));
		}

		constexpr const_iterator cbegin() const noexcept {
			return const_iterator(this->_M_storage_data_at(0));
		}

		constexpr iterator end() noexcept {
			return iterator(this->_M_storage_data_at(this->_M_size));
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(this->_M_storage_data_at(this->_M_size));
		}

		constexpr const_iterator cend() const noexcept {
			return const_iterator(this->_M_storage_data_at(this->_M_size));
		}

		constexpr reverse_iterator rbegin() noexcept {
			return ::std::make_reverse_iterator(this->end());
		}

		constexpr const_reverse_iterator rbegin() const noexcept {
			return ::std::make_reverse_iterator(this->end());
		}

		constexpr const_reverse_iterator crbegin() const noexcept {
			return ::std::make_reverse_iterator(this->cend());
		}

		constexpr reverse_iterator rend() noexcept {
			return ::std::make_reverse_iterator(this->begin());
		}

		constexpr const_reverse_iterator rend() const noexcept {
			return ::std::make_reverse_iterator(this->begin());
		}

		constexpr const_reverse_iterator crend() const noexcept {
			return ::std::make_reverse_iterator(this->cbegin());
		}

		constexpr reference operator[](size_type index) {
			return *this->_M_data(index);
		}

		constexpr const_reference operator[](size_type index) const {
			return *this->_M_data(index);
		}

		constexpr void resize(size_type __count) {
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

		constexpr void resize(size_type __count, const value_type& __value) {
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

	private:
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
			iterator __where_last(const_cast<pointer>(__last.base()));
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
	};

	template <typename _Ty>
	class fixed_vector<_Ty, 0> {
	public:
		using value_type                                            = _Ty;
		using reference                                             = ::std::add_lvalue_reference_t<_Ty>;
		using const_reference                                       = ::std::add_lvalue_reference_t<const _Ty>;
		using pointer                                               = ::std::add_pointer_t<_Ty>;
		using const_pointer                                         = ::std::add_pointer_t<const _Ty>;
		using iterator                                              = __fv_detail::__wrap_pointer<_Ty>;
		using const_iterator                                        = __fv_detail::__wrap_pointer<_Ty>;
		using size_type                                             = ::std::size_t;
		using difference_type                                       = ::std::ptrdiff_t;
		inline static constexpr const ::std::size_t inline_capacity = 0;

	public:
		constexpr fixed_vector() noexcept {
		}

		constexpr fixed_vector(::std::initializer_list<value_type>) noexcept {
		}

		constexpr fixed_vector(size_type) noexcept {
		}

		template <typename... _Args>
		void emplace_back(_Args&&...) noexcept {
		}

		void push_back(const value_type&) noexcept {
		}

		void push_back(value_type&&) noexcept {
		}

		template <typename... _Args>
		void emplace_front(_Args&&...) noexcept {
		}

		void push_front(const value_type&) noexcept {
		}

		void push_front(value_type&&) noexcept {
		}

		template <typename... _Args>
		iterator emplace(const_iterator, _Args&&...) noexcept {
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, value_type&&) noexcept {
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, const value_type&) noexcept {
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, ::std::initializer_list<value_type>) noexcept {
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, size_type) noexcept {
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, size_type, const value_type&) noexcept {
			return iterator(nullptr);
		}

		template <typename _First, typename _Last,
		     ::std::enable_if_t<!::std::is_integral_v<_First>                    // cf-hack
		          && !::std::is_same_v<::ztd::remove_cvref_t<_Last>, value_type> // cf-hack
		          >* = nullptr>
		constexpr iterator insert(const_iterator, _First, _Last) {
			return iterator(nullptr);
		}

		constexpr iterator erase() noexcept {
			return iterator(nullptr);
		}

		constexpr iterator erase(const_iterator, const_iterator) noexcept {
			return iterator(nullptr);
		}

		constexpr void pop_back() noexcept {
		}

		constexpr void pop_front() noexcept {
		}

		constexpr reference front() noexcept {
			return *static_cast<pointer>(nullptr);
		}

		constexpr const_reference front() const noexcept {
			return *static_cast<const_pointer>(nullptr);
		}

		constexpr reference back() noexcept {
			return *static_cast<pointer>(nullptr);
		}

		constexpr const_reference back() const noexcept {
			return *static_cast<const_pointer>(nullptr);
		}

		constexpr bool empty() const noexcept {
			return true;
		}

		constexpr void clear() noexcept {
		}

		constexpr size_type size() const noexcept {
			return 0;
		}

		constexpr size_type capacity() const noexcept {
			return 0;
		}

		constexpr pointer data() noexcept {
			return nullptr;
		}

		constexpr const_pointer data() const noexcept {
			return nullptr;
		}

		constexpr iterator begin() noexcept {
			return iterator(nullptr);
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator(nullptr);
		}

		constexpr const_iterator cbegin() const noexcept {
			return const_iterator(nullptr);
		}

		constexpr iterator end() noexcept {
			return iterator(nullptr);
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(nullptr);
		}

		constexpr const_iterator cend() const noexcept {
			return const_iterator(nullptr);
		}

		constexpr reference operator[](size_type) {
			return *static_cast<pointer>(nullptr);
		}

		constexpr const_reference operator[](size_type) const {
			return *static_cast<const_pointer>(nullptr);
		}

		constexpr void resize(size_type) noexcept {
		}

		constexpr void resize(size_type, const value_type&) noexcept {
		}
	};

	ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif // ZTD_FIXED_VECTOR_FIXED_VECTOR_HPP
