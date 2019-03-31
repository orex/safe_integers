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


#ifndef SAFE_INT_CAST_DEFAULT_HPP
#define SAFE_INT_CAST_DEFAULT_HPP

#include <type_traits>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <string>

template<class T>
using remove_cvr = std::remove_reference<typename std::remove_cv<T>::type>;

template<class T, class U>
using is_same_cvr = std::is_same<typename remove_cvr<T>::type, typename remove_cvr<U>::type>;

template <typename R, typename T>
class cast_base {
 public:
  typedef typename remove_cvr<R>::type result_type;
  typedef typename std::conditional<std::is_unsigned<R>::value && std::is_signed<T>::value ||
                                    std::numeric_limits<R>::max() < std::numeric_limits<T>::max()
      , std::true_type, std::false_type>::type verification_required;

  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value && !verification_required::value, int> = 0 >
  constexpr static result_type cast(U u) {
    return static_cast<result_type>(u);
  }
};

class safe_int_exception_default {
 public:
  template <typename R, typename U>
  inline static void cast_error(U u) {
    throw std::overflow_error("Invalid cast from value '" + std::to_string(u) + 
                              "' with type '" + typeid(U).name() + "' to type '" + typeid(R).name() + "'");
  }
};

template <typename R, typename T, class ExceptionHandler = safe_int_exception_default >
class cast_canonical : public cast_base<R, T> {
 private:
  typedef cast_base<R, T> __base_type;
 public:
  typedef typename __base_type::verification_required verification_required;
  typedef typename __base_type::result_type result_type;

 protected:
  typedef typename std::conditional<(std::numeric_limits<R>::max() < std::numeric_limits<T>::max())
      , std::true_type, std::false_type>::type __check_max;

  typedef typename std::conditional<std::is_unsigned<R>::value && std::is_signed<T>::value
      , std::true_type, std::false_type>::type __check_zero;

  typedef typename std::conditional<std::is_signed<R>::value && std::is_signed<T>::value &&
      (std::numeric_limits<R>::min() > std::numeric_limits<T>::min())
      , std::true_type, std::false_type>::type __check_min;

  static_assert(!__check_min::value || __check_max::value, "Min check should go with max check." );
  static_assert((__check_min::value || __check_zero::value || __check_max::value) == verification_required::value,
                "Wrong verification conditions");
 public:

  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value &&
      !__check_max::value && !__check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type cast(U u) {
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value &&
      !__check_max::value && __check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type cast(U u) {
    if( u < 0 ) { ExceptionHandler::template cast_error<result_type, U>(u); };
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value &&
      __check_max::value && !__check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type cast(U u) {
    if( u > std::numeric_limits<result_type>::max() ) { ExceptionHandler::template cast_error<result_type, U>(u); };
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value &&
      __check_max::value && __check_zero::value && !__check_min::value, int> = 0 >
  constexpr static result_type cast(U u) {
    if( u < 0 || u > std::numeric_limits<result_type>::max() ) { ExceptionHandler::template cast_error<result_type, U>(u); };
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value &&
      __check_max::value && !__check_zero::value && __check_min::value, int> = 0 >
  constexpr static result_type cast(U u) {
    if( u < std::numeric_limits<result_type>::min() || u > std::numeric_limits<result_type>::max() )
      { ExceptionHandler::template cast_error<result_type, U>(u); };
    return static_cast<result_type>(u);
  }
};

template <typename R, typename T, class ExceptionHandler = safe_int_exception_default >
class cast_builtin : public cast_base<R, T> {
 private:
  typedef cast_base<R, T> __base_type;
 public:
  typedef typename __base_type::verification_required verification_required;
  typedef typename __base_type::result_type result_type;

 public:
  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value &&
      !verification_required::value, int> = 0 >
  constexpr static result_type cast(U u) {
    return static_cast<result_type>(u);
  }

  template <typename U = T, std::enable_if_t<is_same_cvr<U, T>::value &&
      verification_required::value, int> = 0 >
  constexpr static result_type cast(U u) {
    result_type result;
    if( __builtin_add_overflow(u, static_cast<typename remove_cvr<T>::type>(0), &result) ) { ExceptionHandler::template cast_error<result_type, U>(u); };
    return result;
  }

};


#endif //SAFE_INT_CAST_DEFAULT_HPP
