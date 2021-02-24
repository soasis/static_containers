//   -----------------
// | ztd::fixed_vector |
//   -----------------

// Written 2019 - 2021 by ThePhD <phdofthehouse@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include <ztd/fixed_vector.hpp>

#include <string>

inline namespace ztd_fixed_vector_tests_basic_compile_time {
	struct empty_struct { };
} // namespace ztd_fixed_vector_tests_basic_compile_time

static_assert(sizeof(ztd::fixed_vector<int, 0>) == sizeof(empty_struct),
     "Zero-sized fixed_vector should be as small as possible.");
static_assert(sizeof(ztd::fixed_vector<double, 0>) == sizeof(empty_struct),
     "Zero-sized fixed_vector should be as small as possible.");
static_assert(sizeof(ztd::fixed_vector<std::string, 0>) == sizeof(empty_struct),
     "Zero-sized fixed_vector should be as small as possible.");
static_assert(sizeof(ztd::fixed_vector<char, 0>) == sizeof(empty_struct),
     "Zero-sized fixed_vector should be as small as possible.");
static_assert(sizeof(ztd::fixed_vector<void*, 0>) == sizeof(empty_struct),
     "Zero-sized fixed_vector should be as small as possible.");
static_assert(sizeof(ztd::fixed_vector<short, 0>) == sizeof(empty_struct),
     "Zero-sized fixed_vector should be as small as possible.");
