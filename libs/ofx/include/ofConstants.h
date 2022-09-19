#pragma once

#define OF_EXIT_APP(val)		std::exit(val);

// core: ---------------------------
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <implot.h>
#include <imnodes.h>
//------------------------------------------------ make_unique
// This is a helper method for make unique on platforms that support C++11, but not C++14.
#if !defined(NO_OF_MAKE_UNIQUE) && (defined(_MSC_VER) && _MSC_VER < 1800) || (!defined(_MSC_VER) && __cplusplus <= 201103L)

// Implementation for C++11 platforms that do not yet have std::make_unique.
// Implementation from http://stackoverflow.com/a/13512344/1518329
namespace std {


template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
	static_assert(std::extent<T>::value == 0,
				  "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

	typedef typename std::remove_extent<T>::type U;
	return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}


} // namespace std

#endif
