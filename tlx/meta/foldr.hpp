/*******************************************************************************
 * tlx/meta/foldr.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Hung Tran <hung@ae.cs.uni-frankfurt.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_FOLDR_HEADER
#define TLX_META_FOLDR_HEADER

#include <tlx/meta/index_sequence.hpp>
#include <tlx/unused.hpp>
#include <tuple>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

/******************************************************************************/
// Variadic Template Expander: Implements foldr on the variadic template
// arguments. Implements (pack ... op ... init) of C++17.

namespace detail {

//! helper for foldr: base case
template <typename Reduce, typename Initial, typename Arg>
auto foldr_impl(Reduce&& r, Initial&& init, Arg&& arg) {
    return std::forward<Reduce>(r)(
        std::forward<Arg>(arg), std::forward<Initial>(init));
}

//! helper for foldr: general recursive case
template <typename Reduce, typename Initial, typename Arg, typename... MoreArgs>
auto foldr_impl(Reduce&& r, Initial&& init, Arg&& arg, MoreArgs&& ... rest) {
    return std::forward<Reduce>(r)(
        std::forward<Arg>(arg),
        foldr_impl(std::forward<Reduce>(r),
                   std::forward<Initial>(init),
                   std::forward<MoreArgs>(rest) ...));
}

} // namespace detail

//! Implements foldr with binary Reduce operation and initial value.
template <typename Reduce, typename Initial, typename... Args>
auto foldr(Reduce&& r, Initial&& init, Args&& ... args) {
    return detail::foldr_impl(
        std::forward<Reduce>(r), std::forward<Initial>(init),
        std::forward<Args>(args) ...);
}

//! \}

} // namespace tlx

#endif // !TLX_META_FOLDR_HEADER

/******************************************************************************/
