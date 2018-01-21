/*******************************************************************************
 * tlx/meta/vreduce.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Hung Tran <hung@ae.cs.uni-frankfurt.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_VREDUCE_HEADER
#define TLX_META_VREDUCE_HEADER

#include <tlx/meta/index_sequence.hpp>
#include <tlx/unused.hpp>
#include <tuple>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

/******************************************************************************/
// Variadic Template Expander: run a generic templated functor (like a generic
// lambda) for each component of a tuple and reduce (binary operation) without
// generating an intermediate std::tuple<>.

namespace detail {

//! helper for vreduce_tuple: base case
template <typename Combine, typename Arg>
auto vreduce_impl(Combine&& c, Arg&& arg)
{
    tlx::unused(c);
    return std::forward<Arg>(arg);
}

//! helper for vreduce_tuple: general recursive case
template <typename Combine, typename Arg, typename... MoreArgs>
auto vreduce_impl(Combine&& c, Arg&& arg, MoreArgs&&... rest)
{
    return std::forward<Combine>(c)(std::forward<Arg>(arg),
                                    vreduce_impl(std::forward<Combine>(c), std::forward<MoreArgs>(rest) ...));
}

} // namespace detail

//! Call a generic functor (like a generic lambda) for each tuple component and
//! reduce with given binary operation.
template <typename Combine, typename... Args>
auto vreduce(Combine&& c, Args&&... args)
{
    return detail::vreduce_impl(std::forward<Combine>(c), std::forward<Args>(args) ...);
}

//! \}

} // namespace tlx

#endif // !TLX_META_VREDUCE_HEADER

/******************************************************************************/
