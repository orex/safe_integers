// Copyright (C) 2019 Kirill Okhotnikov.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//
// You are welcome to contact the author at:
//  kirill.okhotnikov@gmail.com
//


#ifndef SAFE_SAFE_CAST_HPP
#define SAFE_SAFE_CAST_HPP

#include <type_traits>
#include <climits>

#include <safe/backends/native/cast_backend.hpp>
#include <safe/exceptions/std_overflow.hpp>

namespace safe {

template <typename R, typename T>
constexpr R safe_cast(const T t) {
  using namespace safe;
  static_assert(std::is_integral<R>::value, "Result should be an integral value.");
  static_assert(std::is_arithmetic<T>::value, "Input variable should have an arithmetic type.");
  return backend::native::cast_backend<R, T, exception::std_safe_exception>::cast(t);
}


} // namespace safe

#endif //SAFE_INT_SAFE_CAST_HPP
