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


#ifndef SAFE_INT_TYPES_LIST_HPP
#define SAFE_INT_TYPES_LIST_HPP


#ifndef SAFE_REDUCED_LIST
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 40
#endif

#include <cstdint>
#include <type_traits>
#include <boost/mpl/list.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/number.hpp>


#ifdef SAFE_REDUCED_LIST
typedef boost::mpl::list<signed char, unsigned char,
                         unsigned short, signed short,
                         signed int, unsigned int,
                         signed long, unsigned long,
                         signed long long, unsigned long long
                        > __types_integer_raw_t;
#else
typedef boost::mpl::list<signed char, unsigned char,
                         unsigned short, signed short,
                         signed int, unsigned int,
                         signed long, unsigned long,
                         signed long long, unsigned long long,
                         std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                         std::int_fast8_t, std::int_fast16_t, std::int_fast32_t, std::int_fast64_t,
                         std::int_least8_t, std::int_least16_t, std::int_least32_t, std::int_least64_t,
                         std::intmax_t,
                         std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                         std::uint_fast8_t, std::uint_fast16_t, std::uint_fast32_t, std::uint_fast64_t,
                         std::uint_least8_t, std::uint_least16_t, std::uint_least32_t, std::uint_least64_t,
                         std::uintmax_t
                        > __types_integer_raw_t;
#endif


//typedef boost::mpl::list<signed char, unsigned char> __result_types_raw_t;

typedef boost::mpl::list<float, double, long double> types_floating_t;


namespace __details {

template <typename List, typename T>
struct check_element
    : std::is_same<
        typename boost::mpl::find_if<List, std::is_same<boost::mpl::_1, T> >::type,
      typename boost::mpl::end<List>::type>
{};

typedef boost::mpl::fold<
    __types_integer_raw_t, boost::mpl::list<> ,
    boost::mpl::if_<
      boost::mpl::lambda<check_element<boost::mpl::_1, boost::mpl::_2> >,
      boost::mpl::push_front<boost::mpl::_1, boost::mpl::_2>, boost::mpl::_1>

      >::type types_integer_t;

}

typedef __details::types_integer_t types_integer_t;
typedef boost::mpl::copy<types_integer_t, boost::mpl::front_inserter< types_floating_t> >::type types_arithmetic_t;

//typedef boost::mpl::list<int>::type types_integer_t;
//typedef boost::mpl::list<float>::type types_arithmetic_t;

typedef boost::multiprecision::cpp_int precision_type_t;

inline
std::vector<precision_type_t> values_to_check() {
  std::vector<precision_type_t> result;
  precision_type_t cv = 1;
  for(int i = 0; i <= 68; i++) {
    for(const auto j : {-5, -2, -1, 0, 1, 2, 5}) {
      for(const auto k : {1, -1, 3, -3}) {
        result.emplace_back(k*cv + j);
      }
    }
    cv *= 2;
  }
  return result;
}

template <typename U>
bool fits_in(const precision_type_t &pp) {
  return    static_cast<precision_type_t>(std::numeric_limits<U>::max()) >= pp
         && static_cast<precision_type_t>(std::numeric_limits<U>::lowest()) <= pp;
}

template <typename T>
T __abs(const T t) {
  return t >= 0 ? t : -t;
};

template <typename U>
typename std::enable_if<std::is_floating_point<U>::value, bool>::type
precision_saved(const precision_type_t &v1, const precision_type_t &v2) {
  precision_type_t dv = v1 - v2;
  return __abs(dv.convert_to<U>()) + 1 > std::numeric_limits<U>::epsilon()
          * (__abs(v1.convert_to<U>()) + __abs(v2.convert_to<U>()));
}

template <typename U>
typename std::enable_if<!std::is_floating_point<U>::value, bool>::type
precision_saved(const precision_type_t &v1, const precision_type_t &v2) {
  return true;
}


#endif //SAFE_INT_TYPES_LIST_HPP
