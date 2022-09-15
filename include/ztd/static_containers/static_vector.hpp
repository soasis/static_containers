// =============================================================================
// ztd.static_containers
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

#ifndef ZTD_STATIC_CONTAINERS_STATIC_VECTOR_HPP
#define ZTD_STATIC_CONTAINERS_STATIC_VECTOR_HPP

#include <ztd/static_containers/version.hpp>

#include <ztd/static_containers/forward.hpp>
#include <ztd/static_containers/detail/storage.hpp>

#include <ztd/idk/type_traits.hpp>
#include <ztd/idk/to_address.hpp>
#include <ztd/idk/construct_destroy.hpp>
#include <ztd/idk/assert.hpp>
#include <ztd/idk/to_mutable_iter.hpp>
#include <ztd/ranges/wrapped_pointer.hpp>

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <memory>
#include <cassert>
#include <iterator>

namespace ztd {
	ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_OPEN_I_

	template <typename _Ty, ::std::size_t _Capacity>
	class static_vector : private __fv_detail::__storage<_Ty, _Capacity> {
	private:
		template <typename, ::std::size_t>
		friend class ::ztd::static_basic_string;

		using __base_storage = __fv_detail::__storage<_Ty, _Capacity>;

	public:
		using value_type             = _Ty;
		using reference              = ::std::add_lvalue_reference_t<_Ty>;
		using const_reference        = ::std::add_lvalue_reference_t<const _Ty>;
		using pointer                = ::std::add_pointer_t<_Ty>;
		using const_pointer          = ::std::add_pointer_t<const _Ty>;
		using iterator               = ::ztd::ranges::wrapped_pointer<typename __base_storage::__storage_type>;
		using const_iterator         = ::ztd::ranges::wrapped_pointer<const typename __base_storage::__storage_type>;
		using reverse_iterator       = ::std::reverse_iterator<iterator>;
		using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;
		using size_type              = ::std::size_t;
		using difference_type        = ::std::ptrdiff_t;
		inline static constexpr const ::std::size_t inline_capacity = _Capacity;

	private:
		template <typename... _Args>
		static constexpr bool _S_emplace_noexcept() noexcept {
			return ::std::is_nothrow_constructible_v<value_type, _Args...> // cf
			     && (::std::is_nothrow_move_assignable_v<value_type>       // cf
			          || ::std::is_nothrow_copy_assignable_v<value_type>);
		}

	public:
		constexpr static_vector() noexcept : __base_storage() {
		}

		constexpr static_vector(::std::initializer_list<value_type> __values) noexcept : __base_storage() {
			this->insert(this->cend(), ::std::move(__values));
		}

		constexpr static_vector(size_type __count) noexcept : __base_storage() {
			this->insert(this->cend(), __count);
		}

		constexpr static_vector(size_type __count, const value_type& __value) noexcept : __base_storage() {
			this->insert(this->cend(), __count, __value);
		}

		template <typename _First, typename _Last,
		     ::std::enable_if_t<!::std::is_integral_v<_First>                    // cf
		          && !::std::is_same_v<::ztd::remove_cvref_t<_Last>, value_type> // cf
		          >* = nullptr>
		constexpr static_vector(_First __first, _Last __last) noexcept : __base_storage() {
			this->insert(this->cend(), ::std::move(__first), ::std::move(__last));
		}

		template <typename... _Args>
		constexpr reference emplace_back(_Args&&... __args) noexcept(
		     ::std::is_nothrow_constructible_v<value_type, _Args...>) {
			auto* __target_ptr = this->_M_data(this->_M_layout._M_size);
			::ztd::construct_at(__target_ptr, ::std::forward<_Args>(__args)...);
			++this->_M_layout._M_size;
			return *__target_ptr;
		}

		constexpr reference push_back(const value_type& __value) noexcept(
		     ::std::is_nothrow_copy_constructible_v<value_type>) {
			auto* __target_ptr = this->_M_data(this->_M_layout._M_size);
			::ztd::construct_at(__target_ptr, __value);
			++this->_M_layout._M_size;
			return *__target_ptr;
		}

		constexpr reference push_back(value_type&& __value) noexcept(
		     ::std::is_nothrow_move_constructible_v<value_type>) {
			auto* __target_ptr = this->_M_data(this->_M_layout._M_size);
			::ztd::construct_at(__target_ptr, ::std::move(__value));
			++this->_M_layout._M_size;
			return *__target_ptr;
		}

		template <typename... _Args>
		constexpr reference emplace_front(_Args&&... __args) noexcept(
		     ::std::is_nothrow_constructible_v<value_type, _Args...>) {
			return *this->emplace(this->cbegin(), ::std::forward<_Args>(__args)...);
		}

		constexpr reference push_front(const value_type& __value) noexcept(
		     ::std::is_nothrow_copy_constructible_v<value_type>) {
			return *this->insert(this->cbegin(), __value);
		}

		constexpr reference push_front(value_type&& __value) noexcept(
		     ::std::is_nothrow_move_constructible_v<value_type>) {
			return *this->insert(this->cbegin(), ::std::move(__value));
		}

		template <typename... _Args>
		constexpr iterator emplace(const_iterator __where, _Args&&... __args) noexcept(
		     _S_emplace_noexcept<_Args...>()) {
			ZTD_ASSERT(inline_capacity > this->size());
			return this->_M_emplace_no_capacity_check(::std::move(__where), ::std::forward<_Args>(__args)...);
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

		constexpr iterator insert(const_iterator __where, size_type __count, const value_type& __value) noexcept {
			if (__count == 0) {
				return iterator(const_cast<pointer>(__where.base()));
			}
			ZTD_ASSERT(inline_capacity > this->size());
			difference_type __where_dist = __where - this->begin();
			ZTD_ASSERT(__where_dist < this->size());
			ZTD_ASSERT(static_cast<size_type>(__count + __where_dist) <= this->capacity());
			return this->_M_unchecked_multi_insert_count(::std::move(__where), __where_dist, __count, __value);
		}

		template <typename _First, typename _Last,
		     ::std::enable_if_t<!::std::is_integral_v<_First>                    // cf
		          && !::std::is_same_v<::ztd::remove_cvref_t<_Last>, value_type> // cf
		          >* = nullptr>
		constexpr iterator insert(const_iterator __where, _First __first, _Last __last) {
			if (__first == __last) {
				return to_mutable_iter(::std::move(__where), *this);
			}
			if (this->empty()) {
				// simply push all directly in
				this->_M_unchecked_multi_insert_empty(::std::move(__first), ::std::move(__last));
				return this->begin();
			}
			ZTD_ASSERT(inline_capacity > this->size());
			difference_type __where_dist = __where - this->cbegin();
			ZTD_ASSERT(static_cast<size_type>(__where_dist) < this->size());
			// FIXME: lazy dependent-false
			// idiom-check iterators
			if constexpr (::std::is_void_v<value_type>) {
				auto __fl_dist = __last - __first;
				ZTD_ASSERT(static_cast<size_type>(__fl_dist + __where_dist) <= this->capacity());
				return this->_M_unchecked_multi_insert_random_access(
				     ::std::move(__where), __where_dist, ::std::move(__first), ::std::move(__last));
			}
			else {
				iterator __where_first = this->emplace(::std::move(__where), *__first);
				++__first;
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
			ZTD_ASSERT(__where_dist < this->_M_layout._M_size);
			if (__where_dist == this->_M_layout._M_size) {
				return to_mutable_iter(*this, ::std::move(__where));
			}
			if (__where_dist == 0) {
				pop_front();
				return this->begin();
			}
			return this->_M_unchecked_erase(::std::move(__where));
		}

		constexpr iterator erase(const_iterator __where_first, const_iterator __where_last) noexcept {
			if (__where_first == __where_last) {
				return to_mutable_iter(*this, ::std::move(__where_last));
			}
			difference_type __where_diff = __where_last - __where_first;
			ZTD_ASSERT(__where_diff < this->_M_size);
			if (__where_diff == this->size()) {
				this->clear();
				return this->begin();
			}
			return this->_M_unchecked_multi_erase(
			     ::std::move(__where_first), ::std::move(__where_last), __where_diff);
		}

		constexpr void pop_back() noexcept {
			ZTD_ASSERT(this->_M_layout._M_size > 0);
			::ztd::destroy_at(::std::addressof(this->_M_data(this->_M_layout._M_size - 1).value));
			--this->_M_layout._M_size;
		}

		constexpr void pop_front() noexcept {
			ZTD_ASSERT(this->_M_layout._M_size != 0);
			this->_M_unchecked_erase(this->begin());
		}

		constexpr reference front() noexcept {
			ZTD_ASSERT(this->_M_layout._M_size > 0);
			return *this->_M_data(0);
		}

		constexpr const_reference front() const noexcept {
			ZTD_ASSERT(this->_M_layout._M_size > 0);
			return *this->_M_data(0);
		}

		constexpr reference back() noexcept {
			ZTD_ASSERT(this->_M_layout._M_size > 0);
			return *this->_M_data(this->_M_size - 1);
		}

		constexpr const_reference back() const noexcept {
			ZTD_ASSERT(this->_M_layout._M_size > 0);
			return *this->_M_data(this->_M_layout._M_size - 1);
		}

		constexpr bool empty() const noexcept {
			return this->_M_layout._M_size == 0;
		}

		constexpr bool is_empty() const noexcept {
			return this->_M_layout._M_size == 0;
		}

		constexpr void clear() noexcept {
			this->_M_destroy_all();
		}

		constexpr size_type size() const noexcept {
			return this->_M_layout._M_size;
		}

		constexpr size_type capacity() const noexcept {
			return _Capacity;
		}

		constexpr size_type max_size() const noexcept {
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
			return iterator(this->_M_storage_data_at(this->_M_layout._M_size));
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(this->_M_storage_data_at(this->_M_layout._M_size));
		}

		constexpr const_iterator cend() const noexcept {
			return const_iterator(this->_M_storage_data_at(this->_M_layout._M_size));
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
		template <typename... _Args>
		constexpr iterator _M_emplace_no_capacity_check(const_iterator __where, _Args&&... __args) noexcept(
		     _S_emplace_noexcept<_Args...>()) {
			iterator __where_last = this->end();
			if (this->empty() || __where == __where_last) {
				reference __element = this->emplace_back(::std::forward<_Args>(__args)...);
				return iterator(::std::addressof(__element));
			}
			return this->_M_unchecked_emplace(
			     ::std::move(__where), ::std::move(__where_last), ::std::forward<_Args>(__args)...);
		}

		template <typename... _Args>
		constexpr iterator _M_unchecked_emplace(const_iterator __where, iterator __where_last,
		     _Args&&... __args) noexcept(_S_emplace_noexcept<_Args...>()) {
			difference_type __where_dist = __where - this->cbegin();
			ZTD_ASSERT(static_cast<::std::size_t>(__where_dist) < this->size());
			iterator __where_first = to_mutable_iter(__where, *this);

			// Step 0: create a new object off-to-the-right
			auto __where_middle_last = ::std::prev(__where_last);
			::ztd::construct_at(::ztd::to_address(__where_last), *__where_middle_last);
			// Step 1: shift everything to the right, if size is large enough to need it
			if (__where_middle_last != __where_first) {
				[[maybe_unused]] auto __destination_last
				     = ::std::move_backward(__where_first, __where_middle_last, __where_last);
			}
			// Step 2: construct the new value in place
			::ztd::construct_at(::ztd::to_address(__where_first), ::std::forward<_Args>(__args)...);
			++this->_M_layout._M_size;
			return __where_first;
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
					     "it impossible to properly copy in a constexpr constext in a pre-C++20 world.");
				}
			}
			::ztd::destroy_at(::ztd::to_address(__where_last));
			--this->_M_layout._M_size;
			return __where_first;
		}

		template <typename _First, typename _Last>
		constexpr void _M_unchecked_multi_insert_empty(_First __first, _Last __last) {
			for (; __first != __last; ++__first) {
				::ztd::construct_at(this->_M_data(this->_M_layout._M_size), *__first);
				++this->_M_layout._M_size;
			}
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
				::ztd::destroy_at(__where_p);
				if (__where_p == __where_diff_last) {
					break;
				}
			}
			--this->_M_layout._M_size;
			return __where_first;
		}
	};

	template <typename _Ty>
	class static_vector<_Ty, 0> {
	private:
		template <typename, ::std::size_t>
		friend class ::ztd::static_basic_string;

	public:
		using value_type                                            = _Ty;
		using reference                                             = ::std::add_lvalue_reference_t<_Ty>;
		using const_reference                                       = ::std::add_lvalue_reference_t<const _Ty>;
		using pointer                                               = ::std::add_pointer_t<_Ty>;
		using const_pointer                                         = ::std::add_pointer_t<const _Ty>;
		using iterator                                              = ::ztd::ranges::wrapped_pointer<_Ty>;
		using const_iterator                                        = ::ztd::ranges::wrapped_pointer<_Ty>;
		using size_type                                             = ::std::size_t;
		using difference_type                                       = ::std::ptrdiff_t;
		inline static constexpr const ::std::size_t inline_capacity = 0;

		constexpr static_vector() noexcept {
		}

		constexpr static_vector(::std::initializer_list<value_type> __values) noexcept {
			ZTD_ASSERT_MESSAGE("there must be no values", __values.size() < size_type(1));
		}

		constexpr static_vector(size_type __count) noexcept {
			ZTD_ASSERT_MESSAGE("there must be no values", __count < size_type(1));
		}

		template <typename... _Args>
		reference emplace_back(_Args&&...) noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		reference push_back(const value_type&) noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		reference push_back(value_type&&) noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		template <typename... _Args>
		reference emplace_front(_Args&&...) noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		reference push_front(const value_type&) noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		reference push_front(value_type&&) noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		template <typename... _Args>
		iterator emplace(const_iterator, _Args&&...) noexcept {
			ZTD_ASSERT_MESSAGE("nothing can be emplaced in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, value_type&&) noexcept {
			ZTD_ASSERT_MESSAGE("nothing can be inserted in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, const value_type&) noexcept {
			ZTD_ASSERT_MESSAGE("nothing can be inserted in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, ::std::initializer_list<value_type>) noexcept {
			ZTD_ASSERT_MESSAGE("nothing can be inserted in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, size_type) noexcept {
			ZTD_ASSERT_MESSAGE("nothing can be inserted in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr iterator insert(const_iterator, size_type, const value_type&) noexcept {
			ZTD_ASSERT_MESSAGE("nothing can be inserted in this static_vector", false);
			return iterator(nullptr);
		}

		template <typename _First, typename _Last,
		     ::std::enable_if_t<!::std::is_integral_v<_First>                    // cf
		          && !::std::is_same_v<::ztd::remove_cvref_t<_Last>, value_type> // cf
		          >* = nullptr>
		constexpr iterator insert(const_iterator, _First, _Last) {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr iterator erase(const_iterator) noexcept {
			ZTD_ASSERT_MESSAGE("there is nothing to erase in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr iterator erase(const_iterator, const_iterator) noexcept {
			ZTD_ASSERT_MESSAGE("there is nothing to erase in this static_vector", false);
			return iterator(nullptr);
		}

		constexpr void pop_back() noexcept {
			ZTD_ASSERT_MESSAGE("there is nothing to erase in this static_vector", false);
		}

		constexpr void pop_front() noexcept {
			ZTD_ASSERT_MESSAGE("there is nothing to erase in this static_vector", false);
		}

		constexpr reference front() noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		constexpr const_reference front() const noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<const_pointer>(nullptr);
		}

		constexpr reference back() noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		constexpr const_reference back() const noexcept {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
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

		constexpr size_type max_size() const noexcept {
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
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<pointer>(nullptr);
		}

		constexpr const_reference operator[](size_type) const {
			ZTD_ASSERT_MESSAGE("there is no capacity in this static_vector", false);
			return *static_cast<const_pointer>(nullptr);
		}

		constexpr void resize(size_type __count) noexcept {
			ZTD_ASSERT_MESSAGE(
			     "there is no capacity in this static_vector to resize to anything but 0", __count < size_type(1));
		}

		constexpr void resize(size_type __count, const value_type&) noexcept {
			ZTD_ASSERT_MESSAGE(
			     "there is no capacity in this static_vector to resize to anything but 0", __count < size_type(1));
		}
	};

	ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_CLOSE_I_

} // namespace ztd

#endif // ZTD_STATIC_CONTAINERS_STATIC_VECTOR_HPP
