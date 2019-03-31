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


#ifndef SAFE_BUILTIN_CAST_BACKEND_HPP
#define SAFE_BUILTIN_CAST_BACKEND_HPP

#include <safe/common/cast_abstract.hpp>

namespace safe {
namespace backend {
namespace builtin {

template <typename R, typename T, class ExceptionHandler>
class cast_backend {
 public:
  typedef typename
      safe::abstract::__detail::__cast_base<R, T, ExceptionHandler>::result_type result_type;
  typedef typename
      safe::abstract::__detail::__cast_base<R, T, ExceptionHandler>::verification_required_type verification_required_type;

 private:
  typedef typename
      safe::abstract::__detail::__cast_base<R, T, ExceptionHandler>::cast_noexcept cast_noexcept;

  template <typename U = T, std::enable_if_t<std::is_fundamental<U>::value && !verification_required_type::value, int> = 0 >
  constexpr static result_type __cast(U u) {
   return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<verification_required_type::value &&
      std::is_integral<U>::value, int> = 0 >
  constexpr static result_type __cast(U u) {
    result_type result = 0;
    if( __builtin_add_overflow(u, static_cast<result_type>(0), &result) ) {
      ExceptionHandler::template cast_error<result_type, U>(u);
    };
    return result;
  }

  template <typename U = T, std::enable_if_t<verification_required_type::value &&
      std::is_floating_point<U>::value, int> = 0 >
  constexpr static result_type __cast(U u) {
    if( u > std::numeric_limits<result_type>::max() || u < std::numeric_limits<result_type>::lowest()  ) {
      ExceptionHandler::template cast_error<result_type, U>(u);
    };
    return static_cast<result_type>(u);
  }

 public:
  inline
  constexpr static result_type cast(T t) noexcept(cast_noexcept::value) {
    return __cast(t);
  }

};

}
}
}

#endif //SAFE_BUILTIN_CAST_BACKEND_HPP
