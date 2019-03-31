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


#ifndef SAFE_NATIVE_CAST_BACKEND_HPP
#define SAFE_NATIVE_CAST_BACKEND_HPP

#include <safe/common/cast_abstract.hpp>

namespace safe {
namespace backend {
namespace native {

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

  typedef typename std::conditional<(std::numeric_limits<R>::max() < std::numeric_limits<T>::max())
      , std::true_type, std::false_type>::type __check_max;

  typedef typename std::conditional<std::is_unsigned<R>::value && std::is_signed<T>::value
      , std::true_type, std::false_type>::type __check_zero;

  typedef typename std::conditional<std::is_signed<R>::value && std::is_signed<T>::value &&
      (std::numeric_limits<R>::lowest() > std::numeric_limits<T>::lowest())
      , std::true_type, std::false_type>::type __check_min;

  static_assert(!__check_min::value || __check_max::value, "Min check should go with max check." );
  static_assert((__check_min::value || __check_zero::value || __check_max::value) == verification_required_type::value,
                "Wrong verification conditions");

  template <typename U = T, std::enable_if_t<safe::__detail::is_same_cvr<U, T>::value &&
      !__check_max::value && !__check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type __cast(U u) {
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<safe::__detail::is_same_cvr<U, T>::value &&
      !__check_max::value && __check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type __cast(U u) {
    if( u < 0 ) { ExceptionHandler::template cast_error<result_type, U>(u); };
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<safe::__detail::is_same_cvr<U, T>::value &&
      __check_max::value && !__check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type __cast(U u) {
    if( u > std::numeric_limits<result_type>::max() ) { ExceptionHandler::template cast_error<result_type, U>(u); };
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<safe::__detail::is_same_cvr<U, T>::value &&
      __check_max::value && __check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type __cast(U u) {
    if( u < 0 || u > std::numeric_limits<result_type>::max() ) { ExceptionHandler::template cast_error<result_type, U>(u); };
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<safe::__detail::is_same_cvr<U, T>::value &&
      __check_max::value && !__check_zero::value && __check_min::value, int> = 0 >
  constexpr static result_type __cast(U u) {
    if( u < std::numeric_limits<result_type>::lowest() || u > std::numeric_limits<result_type>::max() )
    { ExceptionHandler::template cast_error<result_type, U>(u); };
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

#endif //SAFE_NATIVE_CAST_BACKEND_HPP
