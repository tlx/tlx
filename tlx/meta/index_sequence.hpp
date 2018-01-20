/*******************************************************************************
 * tlx/meta/index_sequence.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_INDEX_SEQUENCE_HEADER
#define TLX_META_INDEX_SEQUENCE_HEADER

#include <cstddef>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

// Compile-time integer sequences, an implementation of std::index_sequence and
// std::make_index_sequence, as these are not available in many current
// libraries (MS Visual C++).
template <size_t... Indexes>
struct index_sequence {
    static size_t size() { return sizeof ... (Indexes); }
};

namespace detail {

template <size_t CurrentIndex, size_t... Indexes>
struct make_index_sequence_helper;

template <size_t... Indexes>
struct make_index_sequence_helper<0, Indexes...>{
    using type = index_sequence<Indexes...>;
};

template <size_t CurrentIndex, size_t... Indexes>
struct make_index_sequence_helper {
    using type = typename make_index_sequence_helper<
              CurrentIndex - 1, CurrentIndex - 1, Indexes...>::type;
};

template <size_t Count, size_t... Indexes>
struct make_reverse_index_sequence_helper_impl;

template <size_t... Indexes>
struct make_reverse_index_sequence_helper_impl<0, Indexes...> {
    using type = typename index_sequence<Indexes..., 0>;
};

template <size_t Count, size_t... Indexes, size_t CurrentIndex>
struct make_reverse_index_sequence_helper_impl {
    using type = typename make_reverse_index_sequence_helper_impl<Count - 1, Indexes..., CurrentIndex, CurrentIndex - 1>::type;
};

template <size_t CurrentIndex>
struct make_reverse_index_sequence_helper {
    using type = typename make_reverse_index_sequence_helper_impl<CurrentIndex - 1, CurrentIndex - 1>::type;
};

struct make_reverse_index_sequence_helper<0> {
    using type = index_sequence<>;
};

} // namespace detail

template <size_t Size>
struct make_index_sequence
    : public detail::make_index_sequence_helper<Size>::type { };

template <size_t Size>
struct make_reverse_index_sequence
    : public detail::make_reverse_index_sequence_helper<Size>::type { };
//! \}

} // namespace tlx

#endif // !TLX_META_INDEX_SEQUENCE_HEADER

/******************************************************************************/
