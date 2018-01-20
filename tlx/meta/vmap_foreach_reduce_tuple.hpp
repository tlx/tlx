/*******************************************************************************
 * tlx/meta/vmap_foreach_reduce_tuple.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Hung Tran <hung@ae.cs.uni-frankfurt.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_VMAP_FOREACH_REDUCE_TUPLE_HEADER
#define TLX_META_VMAP_FOREACH_REDUCE_TUPLE_HEADER

#include <tlx/meta/index_sequence.hpp>
#include <tlx/unused.hpp>
#include <tuple>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

/******************************************************************************/
// Variadic Template Expander: run a generic templated functor (like a generic
// lambda) for each component of a tuple and reduce (binary operation) in order
// without generating an intermediate std::tuple<>.

namespace detail {

//! helper for reduce_vmap_foreach_tuple_with_index_impl: base case
template <typename Functor, typename Combine, typename Arg>
auto vmap_foreach_reduce_tuple_impl(Functor&& f, Combine&& c, Arg&& arg)
{
    tlx::unused(std::forward<Combine>(c));
    return std::forward<Functor>(f)(std::forward<Arg>(arg));
}

//! helper for reduce_vmap_foreach_tuple_with_index_impl: general recursive case
template <typename Functor, typename Combine, typename Arg, typename... MoreArgs>
auto vmap_foreach_reduce_tuple_impl(Functor&& f, Combine&& c, Arg&& arg, MoreArgs&&... rest)
{
    return std::forward<Combine>(c)
        (std::forward<Functor>(f)(std::forward<Arg>(arg)),
         vmap_foreach_reduce_tuple_impl(std::forward<Functor>(f), std::forward<Combine>(c), std::forward<MoreArgs>(rest)...));
}

//! helper for reduce_vmap_foreach_tuple: forwards tuple entries
template <typename Functor, typename Combine, typename Tuple, std::size_t... Is>
auto vmap_foreach_reduce_tuple_with_index_impl(Functor&& f, Combine&& c, Tuple&& t, tlx::index_sequence<Is...>)
{
    return vmap_foreach_reduce_tuple_impl(std::forward<Functor>(f),
                                          std::forward<Combine>(c),
                                          std::get<Is>(std::forward<Tuple>(t)) ...);
}

} // namespace detail

//! Call a generic functor (like a generic lambda) for each variadic template
//! argument and unfold for reduce.
template <typename Functor, typename Combine, typename Tuple>
auto vmap_foreach_reduce_tuple(Functor&& f, Combine&& c, Tuple&& t)
{
    using Indices = tlx::make_index_sequence<
        std::tuple_size<typename std::decay<Tuple>::type>::value>;
    return detail::vmap_foreach_reduce_tuple_with_index_impl(
        std::forward<Functor>(f),
        std::forward<Combine>(c),
        std::forward<Tuple>(t), Indices());
}

//! \}

} // namespace tlx

#endif // !TLX_META_VMAP_FOREACH_REDUCE_TUPLE_HEADER

/******************************************************************************/
