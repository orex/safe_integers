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

//#define SAFE_REDUCED_LIST
#include "types_list.hpp"

#define BOOST_TEST_MODULE test_assign
#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <safe/backends/native/cast_backend.hpp>
#include <safe/backends/builtin/cast_backend.hpp>
#include <safe/exceptions/std_overflow.hpp>


template <template <typename,typename> class Cast>
void check_value(const precision_type_t &pp) {
  using namespace boost;

  mpl::for_each<types_arithmetic_t>([pp](const auto u) -> void {
    typedef typename std::remove_cv<decltype(u)>::type U;
    if ( fits_in<U>(pp) && precision_saved<U>(pp, pp) ) {
      U ia = static_cast<U>(pp);
      mpl::for_each<types_integer_t>([pp, ia](const auto r) -> void {
        typedef typename std::remove_cv<decltype(r)>::type R;
        typedef typename std::remove_cv<decltype(ia)>::type U;
        if ( fits_in<R>(pp) && precision_saved<R>(pp, pp) ) {
          BOOST_CHECK_NO_THROW((Cast<R, U>::cast(ia)));
          precision_type_t pm = Cast<R, U>::cast(ia);
          BOOST_CHECK_EQUAL(pm, pp);
        } else {
          BOOST_CHECK_THROW((Cast<R,U>::cast(ia)), std::overflow_error);
        }
      });
    }
  });
}

template <template <typename,typename> class Cast>
void check_cast() {
  auto checks = values_to_check();
  for(const auto &v : checks)
    check_value<Cast>(v);
}

template <template <typename,typename> class Cast>
void simple_check() {
    BOOST_CHECK_THROW((Cast<unsigned char, signed char>::cast(-4)), std::overflow_error);
    BOOST_CHECK_THROW((Cast<unsigned char, int>::cast(300)), std::overflow_error);
    BOOST_CHECK_EQUAL((Cast<unsigned char, int>::cast(100)), 100);
    BOOST_CHECK_EQUAL((Cast<unsigned char, int>::cast(200)), 200);
    BOOST_CHECK_THROW((Cast<int, int64_t>::cast(2e10)), std::overflow_error);
    BOOST_CHECK_EQUAL((Cast<signed char, short>::cast(-4)), -4);
};

template <typename R, typename U>
using cast_n = safe::backend::native::cast_backend<R, U, safe::exception::std_safe_exception>;

template <typename R, typename U>
using cast_b = safe::backend::builtin::cast_backend<R, U, safe::exception::std_safe_exception>;

BOOST_AUTO_TEST_CASE( native_cast_name_simple )
{
  simple_check<cast_n>();
}

BOOST_AUTO_TEST_CASE( native_cast_name_extended )
{
  check_cast<cast_n>();
}

BOOST_AUTO_TEST_CASE( bultin_cast_name_simple )
{
  simple_check<cast_b>();
}

BOOST_AUTO_TEST_CASE( bultin_cast_name_extended )
{
  check_cast<cast_b>();
}
