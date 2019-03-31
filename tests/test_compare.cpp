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

#define BOOST_TEST_MODULE test_compare
#include <boost/test/included/unit_test.hpp>


#include <cstdint>
#include <iostream>
#include <limits>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/number.hpp>
#include "safe/backends/native/safe_compare.hpp"


void check_compare(const precision_type_t &v1, const precision_type_t &v2) {
  using namespace boost;
  mpl::for_each<types_arithmetic_t>([&v1, &v2](const auto u) -> void {
    typedef typename std::remove_cv<decltype(u)>::type U;
    if ( fits_in<U>(v1) && precision_saved<U>(v1, v2) ) {
      mpl::for_each<types_arithmetic_t>([&v1, &v2](const auto r) -> void {
        typedef typename std::remove_cv<decltype(r)>::type R;
        if ( fits_in<R>(v2) && precision_saved<R>(v2, v1) ) {
          U m1 = static_cast<U>(v1);
          R m2 = static_cast<R>(v2);
          //std::cout << m1 << " - " << m2 << ": " << v1 << " - " << v2 << std::endl;
          using compare_t = safe_compare<U, R>;
          BOOST_CHECK_EQUAL(compare_t::less(m1, m2), v1 < v2);
          BOOST_CHECK_EQUAL(compare_t::greater(m1, m2), v1 > v2);
          BOOST_CHECK_EQUAL(compare_t::leq(m1, m2), v1 <= v2);
          BOOST_CHECK_EQUAL(compare_t::geq(m1, m2), v1 >= v2);
          BOOST_CHECK_EQUAL(compare_t::eq(m1, m2), v1 == v2);
          BOOST_CHECK_EQUAL(compare_t::neq(m1, m2), v1 != v2);
          if (    std::is_signed<U>::value == std::is_signed<R>::value
               || ( m1 >= 0 && m2 >= 0 ) ) {
            BOOST_CHECK_EQUAL(m1 < m2, v1 < v2);
            BOOST_CHECK_EQUAL(m1 > m2, v1 > v2);
            BOOST_CHECK_EQUAL(m1 <= m2, v1 <= v2);
            BOOST_CHECK_EQUAL(m1 >= m2, v1 >= v2);
            BOOST_CHECK_EQUAL(m1 == m2, v1 == v2);
            BOOST_CHECK_EQUAL(m1 != m2, v1 != v2);
          }
        }
      });
    }
  });

}

BOOST_AUTO_TEST_CASE( compare_check )
{
  auto checks = values_to_check();
  for(const auto &v1 : checks) {
    for(const auto &v2: checks) {
      check_compare(v1, v2);
    }
  }
}




