/*******************************************************************************
 * tlx/meta.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_HEADER
#define TLX_META_HEADER

//! \defgroup tlx_meta Meta-Template Programming
//! Tools for easier meta-template programming

/*[[[perl
foreach (sort glob("tlx/meta/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n"
}
]]]*/
#include <tlx/meta/apply_tuple.hpp>        // NOLINT(misc-include-cleaner)
#include <tlx/meta/call_for_range.hpp>     // NOLINT(misc-include-cleaner)
#include <tlx/meta/call_foreach.hpp>       // NOLINT(misc-include-cleaner)
#include <tlx/meta/call_foreach_tuple.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/meta/call_foreach_tuple_with_index.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/meta/call_foreach_with_index.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/meta/enable_if.hpp>               // NOLINT(misc-include-cleaner)
#include <tlx/meta/fold_left.hpp>               // NOLINT(misc-include-cleaner)
#include <tlx/meta/fold_left_tuple.hpp>         // NOLINT(misc-include-cleaner)
#include <tlx/meta/fold_right.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/meta/fold_right_tuple.hpp>        // NOLINT(misc-include-cleaner)
#include <tlx/meta/function_chain.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/meta/function_stack.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/meta/has_member.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/meta/has_method.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/meta/index_sequence.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/meta/is_std_array.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/meta/is_std_pair.hpp>             // NOLINT(misc-include-cleaner)
#include <tlx/meta/is_std_tuple.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/meta/is_std_vector.hpp>           // NOLINT(misc-include-cleaner)
#include <tlx/meta/log2.hpp>                    // NOLINT(misc-include-cleaner)
#include <tlx/meta/no_operation.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/meta/static_index.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/meta/vexpand.hpp>                 // NOLINT(misc-include-cleaner)
#include <tlx/meta/vmap_for_range.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/meta/vmap_foreach.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/meta/vmap_foreach_tuple.hpp>      // NOLINT(misc-include-cleaner)
#include <tlx/meta/vmap_foreach_tuple_with_index.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/meta/vmap_foreach_with_index.hpp> // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_META_HEADER

/******************************************************************************/
