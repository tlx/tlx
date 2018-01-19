/*******************************************************************************
 * tlx/meta/vmap_foreach.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Hung Tran <hung@ae.cs.uni-frankfurt.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_VMAP_FOREACH_TUPLE_H
#define TLX_META_VMAP_FOREACH_TUPLE_H

#include <tlx/meta/index_sequence.hpp>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

/******************************************************************************/
// Variadic Template Expander: run a generic templated functor (like a generic
// lambda) for each of the variadic template parameters of a tuple, and collect
// the return values in a generic std::tuple.

namespace detail {

//! helper for vmap_foreach_tuple_with_index_impl: base case
template <typename Functor, typename Arg>
auto vmap_foreach_tuple_impl(Functor&& f, Arg&& arg)
{
    return std::make_tuple(std::forward<Functor>(f)(std::forward<Arg>(arg)));
}

//! helper for vmap_foreach_tuple_with_index_impl: general recursive case
template <typename Functor, typename Arg, typename... MoreArgs>
auto vmap_foreach_tuple_impl(Functor&& f, Arg&& arg, MoreArgs&&... rest)
{
    return std::tuple_cat(std::make_tuple(std::forward<Functor>(f)(std::forward<Arg>(arg))),
                          vmap_collect_impl(std::forward<Functor>(f), std::forward<MoreArgs>(rest) ...)
    );
}

//! helper for vmap_foreach_tuple: forwards tuple entries
template <typename Functor, typename Tuple, std::size_t... Is>
auto vmap_foreach_tuple_with_index_impl(Functor&& f, Tuple&& t, tlx::index_sequence<Is...>)
{
    return vmap_foreach_tuple_impl(std::forward<Functor>(f), std::get<Is>(std::forward<Tuple>(t)) ...);
}

} // namespace detail

//! Call a generic functor (like a generic lambda) for each variadic template
//! argument and collect the result in a std::tuple<>.
template <typename Functor, typename Tuple>
auto vmap_foreach_tuple(Functor&& f, Tuple&& t)
{
    using Indices = tlx::make_index_sequence<
        std::tuple_size<typename std::decay<Tuple>::type>::value>;
    return detail::vmap_foreach_tuple_with_index_impl(
        std::forward<Functor>(f), std::forward<Tuple>(t), Indices());
}

//! \}

} // namespace tlx

#endif // !TLX_META_VMAP_FOREACH_TUPLE_H
