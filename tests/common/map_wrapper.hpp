// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2020, Intel Corporation */

/*
 * map_wrapper.cpp -- wrapper for sharing tests between map-like data
 * structures
 */

#ifndef LIBPMEMOBJ_CPP_TESTS_MAP_WRAPPER_HPP
#define LIBPMEMOBJ_CPP_TESTS_MAP_WRAPPER_HPP

/* if concurrent map is defined */
#ifdef LIBPMEMOBJ_CPP_TESTS_CONCURRENT_MAP

#include <libpmemobj++/experimental/concurrent_map.hpp>

namespace nvobj = pmem::obj;
namespace nvobjex = pmem::obj::experimental;

template <typename T, typename U, typename Comparator = std::less<T>>
using container_t = nvobjex::concurrent_map<T, U, Comparator>;

template <typename C, typename... Args>
auto
erase(C &m, Args &&... args)
	-> decltype(m.unsafe_erase(std::forward<Args>(args)...))
{
	return m.unsafe_erase(std::forward<Args>(args)...);
}

/* if radix tree is defined */
#elif defined LIBPMEMOBJ_CPP_TESTS_RADIX

#include <libpmemobj++/experimental/bytes_view.hpp>
#include <libpmemobj++/experimental/radix.hpp>
namespace nvobj = pmem::obj;
namespace nvobjex = pmem::obj::experimental;

template <typename T, typename Enable = void>
struct test_bytes_view;

template <typename T>
struct test_bytes_view<
	T, typename std::enable_if<!std::is_signed<T>::value>::type> {
	using type = nvobjex::big_endian_bytes_view<T>;
};

struct test_bytes_view_int {
	test_bytes_view_int(const int *v)
	    : v((unsigned)(*v + std::numeric_limits<int>::max() + 1))
	{
	}

	size_t
	size() const
	{
		return sizeof(int);
	}

	char operator[](std::ptrdiff_t p) const
	{
		return reinterpret_cast<const char *>(
			&v)[(ptrdiff_t)(size()) - p - 1];
	}

	unsigned v;
};

template <typename T>
struct test_bytes_view<
	T, typename std::enable_if<std::is_signed<T>::value>::type> {
	using type = test_bytes_view_int;
};

/* The third param is comparator but radix does not support that */
template <typename T, typename U, typename Ignore = void>
using container_t =
	nvobjex::radix_tree<T, U, typename test_bytes_view<T>::type>;

template <typename C, typename... Args>
auto
erase(C &m, Args &&... args) -> decltype(m.erase(std::forward<Args>(args)...))
{
	return m.erase(std::forward<Args>(args)...);
}

#endif

#endif
