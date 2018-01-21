/*******************************************************************************
 * tlx/meta/vreduce_tuple.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Hung Tran <hung@ae.cs.uni-frankfurt.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_VREDUCE_TUPLE_HEADER
#define TLX_META_VREDUCE_TUPLE_HEADER

#include <tlx/meta/index_sequence.hpp>
#include <tlx/meta/vreduce.hpp>
#include <tuple>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

/******************************************************************************/
// Variadic Template Expander: run a generic templated binary operation on the
// tuple to reduce.

namespace detail {

//! helper for vreduce_tuple: forwards tuple entries
template <typename Combine, typename Tuple, std::size_t... Is>
auto vreduce_tuple_impl(Combine&& c, Tuple&& t, tlx::index_sequence<Is...>)
{
    return vreduce(std::forward<Combine>(c), std::get<Is>(std::forward<Tuple>(t)) ...);
}

} // namespace detail

//! Call a generic functor (like a generic lambda) for each tuple component and
//! reduce with given binary operation.
template <typename Combine, typename Tuple>
auto vreduce_tuple(Combine&& c, Tuple&& t)
{
    using Indices = tlx::make_index_sequence<
        std::tuple_size<typename std::decay<Tuple>::type>::value>;
    return detail::vreduce_tuple_impl(
        std::forward<Combine>(c),
        std::forward<Tuple>(t), Indices());
}

//! \}

} // namespace tlx

#endif // !TLX_META_VREDUCE_TUPLE_HEADER

/******************************************************************************/
