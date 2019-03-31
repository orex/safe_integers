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


template <typename L, typename R, bool IsLSigned, bool IsRSigned, bool IsSafeCmp>
class __safe_compare_base {
};

template <typename L, typename R, bool IsLSigned, bool IsRSigned>
class __safe_compare_base<L, R, IsLSigned, IsRSigned, true>  {
// Trivial comparator
 public:
  typedef std::true_type trivial_compare_t;

  constexpr static bool less(L left, R right) noexcept {
    return left < right;
  }

  constexpr static bool leq(L left, R right) noexcept {
    return left <= right;
  }

  constexpr static bool greater(L left, R right) noexcept {
    return left > right;
  }

  constexpr static bool geq(L left, R right) noexcept {
    return left >= right;
  }

  constexpr static bool eq(L left, R right) noexcept {
    return left == right;
  }

  constexpr static bool neq(L left, R right) noexcept {
    return left != right;
  }

};

template <typename L, typename R>
class __safe_compare_base<L, R, true, false, false>  {
// Left signed and right unsigned comparator
 public:
  typedef std::false_type trivial_compare_t;

  constexpr static bool less(L left, R right) noexcept {
    return (left < 0) || (left < right);
  }

  constexpr static bool leq(L left, R right) noexcept {
    return (left <= 0) || (left <= right);
  }

  constexpr static bool greater(L left, R right) noexcept {
    return (left > 0) && (left > right);
  }

  constexpr static bool geq(L left, R right) noexcept {
    return (left >= 0) && (left >= right);
  }

  constexpr static bool eq(L left, R right) noexcept {
    return (left >= 0) && (left == right);
  }

  constexpr static bool neq(L left, R right) noexcept {
    return (left < 0) || (left != right);
  }

};

template <typename L, typename R>
class __safe_compare_base<L, R, false, true, false>  {
// Left unsigned and right signed comparator
 public:
  typedef std::false_type trivial_compare_t;

  constexpr static bool less(L left, R right) noexcept {
    return (right > 0) && (left < right);
  }

  constexpr static bool leq(L left, R right) noexcept {
    return (right >= 0) && (left <= right);
  }

  constexpr static bool greater(L left, R right) noexcept {
    return (right < 0) || (left > right);
  }

  constexpr static bool geq(L left, R right) noexcept {
    return (right <= 0) || (left >= right);
  }

  constexpr static bool eq(L left, R right) noexcept {
    return (right >= 0) && (left == right);
  }

  constexpr static bool neq(L left, R right) noexcept {
    return (right < 0) || (left != right);
  }
};



template <typename L, typename R, class Base>
class safe_compare_abstract : public Base {
 private:
   typedef Base  __base_class_t;
 public:
   using typename __base_class_t::trivial_compare_t;

   using __base_class_t::less;
   using __base_class_t::leq;
   using __base_class_t::greater;
   using __base_class_t::geq;
   using __base_class_t::eq;
   using __base_class_t::neq;
};

template <typename L, typename R>
using safe_compare = safe_compare_abstract<L, R, __safe_compare_base<L, R, std::is_signed<L>::value, std::is_signed<R>::value, 
                                                (std::is_signed<L>::value == std::is_signed<R>::value) || 
                                                 std::is_floating_point<L>::value || std::is_floating_point<R>::value > >;


#endif //SAFE_INT_CAST_DEFAULT_HPP
