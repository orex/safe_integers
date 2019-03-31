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


#ifndef SAFE_EXCEPTIONS_STD_OVERFLOW_HPP
#define SAFE_EXCEPTIONS_STD_OVERFLOW_HPP

#include <typeinfo>
#include <stdexcept>
#include <string>

namespace safe {
namespace exception {

class std_safe_exception {
 public:
  typedef std::false_type no_throw_exception_t;
  template <typename R, typename U>
  inline static void cast_error(U u) noexcept(false) {
    throw std::overflow_error("Invalid cast from value '" + std::to_string(u) +
          "' with type '" + typeid(U).name() + "' to type '" + typeid(R).name() + "'");
  }
};

}
}


#endif //SAFE_EXCEPTIONS_STD_OVERFLOW_HPP
