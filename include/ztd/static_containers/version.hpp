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

#ifndef ZTD_STATIC_CONTAINERS_VERSION_HPP
#define ZTD_STATIC_CONTAINERS_VERSION_HPP

#include <ztd/version.hpp>

#define ZTD_STATIC_CONTAINERS_VERSION_MAJOR 0
#define ZTD_STATIC_CONTAINERS_VERSION_MINOR 0
#define ZTD_STATIC_CONTAINERS_VERSION_PATCH 0
#define ZTD_STATIC_CONTAINERS_VERSION_STRING "0.0.0"
#define ZTD_STATIC_CONTAINERS_VERSION                                                               \
	((ZTD_STATIC_CONTAINERS_VERSION_MAJOR * 100'000) + (ZTD_STATIC_CONTAINERS_VERSION_MINOR * 100) \
	     + (ZTD_STATIC_CONTAINERS_VERSION_PATCH))

// clang-format off

#if defined(ZTD_STATIC_CONTAINERS_ABI_NAMESPACE)
	#define ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_OPEN_I_ inline namespace ZTD_STATIC_CONTAINERS_ABI_NAMESPACE {
	#define ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_CLOSE_I_ }
#else
	#define ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_OPEN_I_ inline namespace __fv_v0 {
	#define ZTD_STATIC_CONTAINERS_INLINE_ABI_NAMESPACE_CLOSE_I_ }
#endif

// clang-format on

#endif
