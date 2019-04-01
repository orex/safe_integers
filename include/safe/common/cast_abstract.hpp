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


#ifndef SAFE_COMMON_CAST_ABSTRACT_HPP
#define SAFE_COMMON_CAST_ABSTRACT_HPP

#include <climits>
#include <type_traits>
#include <safe/common/helpers.hpp>

namespace safe {
namespace abstract {

namespace __detail {
template<typename R, typename T, class ExceptionHandler>
class __cast_base {
  static_assert(std::is_integral<R>::value, "Cast result should be integral type.");
 public:
  typedef typename safe::__detail::remove_cvr<R>::type result_type;
  typedef typename std::conditional<(std::is_unsigned<R>::value && std::is_signed<T>::value) ||
      std::numeric_limits<R>::max() < std::numeric_limits<T>::max(), std::true_type, std::false_type>::type
      verification_required_type;
  typedef typename std::conditional<!verification_required_type::value || ExceptionHandler::no_throw_exception_t::value
      , std::true_type, std::false_type>::type cast_noexcept;
};
}


template <class CastBackend>
class cast_handler : private CastBackend {
 public:
  //typedef typename CastBackend::verification_required verification_required;
  //typedef typename CastBackend::result_type result_type;
  using typename CastBackend::verification_required;
  using typename CastBackend::result_type;
  using CastBackend::cast;

};

} } // namespace safe::abstract

#endif //SAFE_COMMON_CAST_ABSTRACT_HPP
