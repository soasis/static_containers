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

#ifndef ZTD_FIXED_VECTOR_VERSION_HPP
#define ZTD_FIXED_VECTOR_VERSION_HPP

#include <cstddef>
#include <cstdint>
#include <climits>

#define ZTD_FIXED_VECTOR_VERSION_MAJOR 0
#define ZTD_FIXED_VECTOR_VERSION_MINOR 0
#define ZTD_FIXED_VECTOR_VERSION_PATCH 0
#define ZTD_FIXED_VECTOR_VERSION_STRING "0.0.0"
#define ZTD_FIXED_VECTOR_VERSION                                                          \
	((ZTD_FIXED_VECTOR_VERSION_MAJOR * 100'000) + (ZTD_FIXED_VECTOR_VERSION_MINOR * 100) \
	     + (ZTD_FIXED_VECTOR_VERSION_PATCH))

#define ZTD_FIXED_VECTOR_IS_ON(OP_SYMBOL) ((3 OP_SYMBOL 3) != 0)
#define ZTD_FIXED_VECTOR_IS_OFF(OP_SYMBOL) ((3 OP_SYMBOL 3) == 0)
#define ZTD_FIXED_VECTOR_IS_DEFAULT_ON(OP_SYMBOL) ((3 OP_SYMBOL 3) > 3)
#define ZTD_FIXED_VECTOR_IS_DEFAULT_OFF(OP_SYMBOL) ((3 OP_SYMBOL 3 OP_SYMBOL 3) < 0)

#define ZTD_FIXED_VECTOR_ON |
#define ZTD_FIXED_VECTOR_OFF ^
#define ZTD_FIXED_VECTOR_DEFAULT_ON +
#define ZTD_FIXED_VECTOR_DEFAULT_OFF -

// clang-format off

#if defined(__has_include)
	#define ZTD_FIXED_VECTOR_COMPILER_WITH_HAS_INCLUDE_I_ ZTD_FIXED_VECTOR_ON
	#define ZTD_FIXED_VECTOR_HAS_INCLUDE_I_(...) __has_include(__VA_ARGS__)
#else
	#define ZTD_FIXED_VECTOR_COMPILER_WITH_HAS_INCLUDE_I_ ZTD_FIXED_VECTOR_OFF
	#define ZTD_FIXED_VECTOR_HAS_INCLUDE_I_(...) 0
#endif

#if ZTD_FIXED_VECTOR_HAS_INCLUDE_I_(<version>)
	// Can check __has_include and we have the C++20 <version> header
	#include <version>
#endif

#if defined(__clang__)
	#define ZTD_FIXED_VECTOR_COMPILER_CLANG_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_COMPILER_CLANG_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined(__GNUC__)
	#define ZTD_FIXED_VECTOR_COMPILER_GCC_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_COMPILER_GCC_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined (_MSC_VER)
	#define ZTD_FIXED_VECTOR_COMPILER_VCXX_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_COMPILER_VCXX_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if ZTD_FIXED_VECTOR_IS_ON(ZTD_FIXED_VECTOR_COMPILER_VCXX_I_) && ZTD_FIXED_VECTOR_IS_ON(ZTD_FIXED_VECTOR_COMPILER_CLANG_I_)
	#define ZTD_FIXED_VECTOR_COMPILER_VCXX_CLANG_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_COMPILER_VCXX_CLANG_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined(__MINGW32__)
	#define ZTD_FIXED_VECTOR_COMPILER_MINGW_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_COMPILER_MINGW_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined(__GLIBCXX__)
	#define ZTD_FIXED_VECTOR_LIBSTDCXX_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_LIBSTDCXX_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined(_LIBCPP_VERSION)
	#define ZTD_FIXED_VECTOR_LIBCXX_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_LIBCXX_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined (_YVALS_CORE_H_) // Volatile definition; would be nice if VC++ exposed some documented macros...
	#define ZTD_FIXED_VECTOR_LIBVCXX_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_LIBVCXX_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined(_WIN32)
	#define ZTD_FIXED_VECTOR_PLATFORM_WINDOWS_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_WINDOWS_I_ ZTD_FIXED_VECTOR_OFF
#endif // Windows

#if defined(__CYGWIN__)
	#define ZTD_FIXED_VECTOR_PLATFORM_CYGWIN_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_CYGWIN_I_ ZTD_FIXED_VECTOR_OFF
#endif // Cygwin shenanigans

#if defined(__MVS__)
	#define ZTD_FIXED_VECTOR_PLATFORM_ZEDOS_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_ZEDOS_I_ ZTD_FIXED_VECTOR_OFF
#endif // IBM's Z/OS

#if defined(__STDC_HOSTED__)
	#define ZTD_FIXED_VECTOR_PLATFORM_FREESTANDING_I_ ZTD_FIXED_VECTOR_OFF
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_FREESTANDING_I_ ZTD_FIXED_VECTOR_ON
#endif

#if defined(_AIX)
	// We are on the AIX platform
	#define ZTD_FIXED_VECTOR_PLATFORM_AIX_I_ ZTD_FIXED_VECTOR_ON
#else
	// We are not on the AIX platform
	#define ZTD_FIXED_VECTOR_PLATFORM_AIX_I_ ZTD_FIXED_VECTOR_OFF
#endif // IBM's AIX

#if defined(ZTD_FIXED_VECTOR_PLATFORM_UNKNOWN_CHECK)
	#if (ZTD_FIXED_VECTOR_PLATFORM_UNKNOWN_CHECK != 0)
		#define ZTD_FIXED_VECTOR_PLATFORM_UNKNOWN_CHECK_I_ ZTD_FIXED_VECTOR_ON
	#else
		#define ZTD_FIXED_VECTOR_PLATFORM_UNKNOWN_CHECK_I_ ZTD_FIXED_VECTOR_OFF
	#endif
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_UNKNOWN_CHECK_I_ ZTD_FIXED_VECTOR_OFF
#endif

#if defined(sun) || defined(__sun)
	#if defined(__SVR4) || defined(__svr4__)
		// Solaris
		#define ZTD_FIXED_VECTOR_PLATFORM_SUNOS_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
		#define ZTD_FIXED_VECTOR_PLATFORM_SOLARIS_I_ ZTD_FIXED_VECTOR_DEFAULT_ON
	#else
		// SunOS
		#define ZTD_FIXED_VECTOR_PLATFORM_SUNOS_I_ ZTD_FIXED_VECTOR_DEFAULT_ON
		#define ZTD_FIXED_VECTOR_PLATFORM_SOLARIS_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
	#endif
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_SUNOS_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
	#define ZTD_FIXED_VECTOR_PLATFORM_SOLARIS_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif // SunOS and Solaris

#if defined(__unix) || defined(__unix__)
	#define ZTD_FIXED_VECTOR_PLATFORM_UNIX_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_UNIX_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif // UNIX platforms

#if defined(__linux) || defined(__linux__) || defined(linux)
	#define ZTD_FIXED_VECTOR_PLATFORM_LINUX_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_PLATFORM_LINUX_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif // Linux platforms

#if defined(__STDC_ISO_10646__) && (__STDC_ISO_10646__ != 0)
	#define ZTD_FIXED_VECTOR_WCHAR_T_UTF32_COMPATIBLE_I_ ZTD_FIXED_VECTOR_DEFAULT_ON
#else
	#define ZTD_FIXED_VECTOR_WCHAR_T_UTF32_COMPATIBLE_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif

#if defined(ZTD_FIXED_VECTOR_CHAR8_T)
	#define ZTD_FIXED_VECTOR_USE_USER_CHAR8_T_I_ ZTD_FIXED_VECTOR_ON
	#define ZTD_FIXED_VECTOR_CHAR8_T_I_ ZTD_FIXED_VECTOR_CHAR8_T
#elif defined(__cpp_char8_t)
	#define ZTD_FIXED_VECTOR_USE_USER_CHAR8_T_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
	#define ZTD_FIXED_VECTOR_CHAR8_T_I_ char8_t
#else
	#define ZTD_FIXED_VECTOR_USE_USER_CHAR8_T_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
	#define ZTD_FIXED_VECTOR_CHAR8_T_I_ unsigned char
#endif // char8_t defined by the user

#if defined(__cpp_char8_t)
	#define ZTD_FIXED_VECTOR_NATIVE_CHAR8_T_I_ ZTD_FIXED_VECTOR_DEFAULT_ON
#else
	#define ZTD_FIXED_VECTOR_NATIVE_CHAR8_T_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif // char8_t exists natively (C++) - C proposal in WG14 still in progress

#if defined(ZTD_FIXED_VECTOR_BUILD)
	#if (ZTD_FIXED_VECTOR_BUILD != 0)
		#define ZTD_FIXED_VECTOR_BUILD_I_ ZTD_FIXED_VECTOR_ON
	#else
		#define ZTD_FIXED_VECTOR_BUILD_I_ ZTD_FIXED_VECTOR_OFF
	#endif
#else
	#define ZTD_FIXED_VECTOR_BUILD_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif // Building or not

#if defined(ZTD_FIXED_VECTOR_DLL) && (ZTD_FIXED_VECTOR_DLL != 0)
	#define ZTD_FIXED_VECTOR_DLL_I_ ZTD_FIXED_VECTOR_ON
#else
	#define ZTD_FIXED_VECTOR_DLL_I_ ZTD_FIXED_VECTOR_OFF
#endif // Building or not

#if defined(ZTD_FIXED_VECTOR_HEADER_ONLY)
	#if (ZTD_FIXED_VECTOR_HEADER_ONLY != 0)
		#define ZTD_FIXED_VECTOR_HEADER_ONLY_I_ ZTD_FIXED_VECTOR_ON
	#else
		#define ZTD_FIXED_VECTOR_HEADER_ONLY_I_ ZTD_FIXED_VECTOR_OFF
	#endif
#else
	#define ZTD_FIXED_VECTOR_HEADER_ONLY_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif // Header only library

#if defined(ZTD_FIXED_VECTOR_INLINE_BUILD)
	#if (ZTD_FIXED_VECTOR_INLINE_BUILD != 0)
		#define ZTD_FIXED_VECTOR_INLINE_BUILD_I_ ZTD_FIXED_VECTOR_ON
	#else
		#define ZTD_FIXED_VECTOR_INLINE_BUILD_I_ ZTD_FIXED_VECTOR_OFF
	#endif
#else
	#define ZTD_FIXED_VECTOR_INLINE_BUILD_I_ ZTD_FIXED_VECTOR_DEFAULT_OFF
#endif // Header only library

#if defined(ZTD_FIXED_VECTOR_ABI_NAMESPACE)
	#define ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_OPEN_I_ inline namespace ZTD_FIXED_VECTOR_ABI_NAMESPACE {
	#define ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_CLOSE_I_ }
#else
	#define ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_OPEN_I_ inline namespace __v0 {
	#define ZTD_FIXED_VECTOR_INLINE_ABI_NAMESPACE_CLOSE_I_ }
#endif

// clang-format on

#endif // ZTD_FIXED_VECTOR_VERSION_HPP
