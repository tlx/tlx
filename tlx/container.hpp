/*******************************************************************************
 * tlx/container.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_CONTAINER_HEADER
#define TLX_CONTAINER_HEADER

//! \defgroup tlx_container Containers and Data Structures
//! Containers and Data Structures

/*[[[perl
foreach (sort glob("tlx/container/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n"
}
]]]*/
#include <tlx/container/btree.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/container/btree_map.hpp>      // NOLINT(misc-include-cleaner)
#include <tlx/container/btree_multimap.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/container/btree_multiset.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/container/btree_set.hpp>      // NOLINT(misc-include-cleaner)
#include <tlx/container/d_ary_addressable_int_heap.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/container/d_ary_heap.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/container/loser_tree.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/container/lru_cache.hpp>     // NOLINT(misc-include-cleaner)
#include <tlx/container/radix_heap.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/container/ring_buffer.hpp>   // NOLINT(misc-include-cleaner)
#include <tlx/container/simple_vector.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/container/splay_tree.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/container/string_view.hpp>   // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_CONTAINER_HEADER

/******************************************************************************/
