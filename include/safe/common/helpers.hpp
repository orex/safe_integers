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


#ifndef SAFE_COMMON_HELPERS_HPP
#define SAFE_COMMON_HELPERS_HPP

namespace safe {
namespace __detail {

template<class T>
using remove_cvr = std::remove_reference<typename std::remove_cv<T>::type>;

template<class T, class U>
using is_same_cvr = std::is_same<typename remove_cvr<T>::type, typename remove_cvr<U>::type>;

} } // namespace safe::_detail

#endif //SAFE_COMMON_HELPERS_HPP
