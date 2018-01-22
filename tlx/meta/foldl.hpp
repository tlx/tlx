/*******************************************************************************
 * tlx/meta/foldl.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Hung Tran <hung@ae.cs.uni-frankfurt.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_FOLDL_HEADER
#define TLX_META_FOLDL_HEADER

#include <tlx/meta/index_sequence.hpp>
#include <tlx/unused.hpp>
#include <tuple>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

/******************************************************************************/
// Variadic Template Expander: Implements foldl on the variadic template
// arguments. Implements (init ... op ... pack) of C++17.

namespace detail {

//! helper for foldl: base case
template <typename Reduce, typename Initial, typename Arg>
auto foldl_impl(Reduce&& r, Initial&& init, Arg&& arg) {
    return std::forward<Reduce>(r)(
        std::forward<Initial>(init), std::forward<Arg>(arg));
}

//! helper for foldl: general recursive case
template <typename Reduce, typename Initial, typename Arg, typename... MoreArgs>
auto foldl_impl(Reduce&& r, Initial&& init, Arg&& arg, MoreArgs&& ... rest) {
    return foldl_impl(
        std::forward<Reduce>(r),
        std::forward<Reduce>(r)(
            std::forward<Initial>(init), std::forward<Arg>(arg)),
        std::forward<MoreArgs>(rest) ...);
}

} // namespace detail

//! Implements foldl with binary Reduce operation and initial value.
template <typename Reduce, typename Initial, typename... Args>
auto foldl(Reduce&& r, Initial&& init, Args&& ... args) {
    return detail::foldl_impl(
        std::forward<Reduce>(r),
        std::forward<Initial>(init),
        std::forward<Args>(args) ...);
}

//! \}

} // namespace tlx

#endif // !TLX_META_FOLDL_HEADER

/******************************************************************************/
