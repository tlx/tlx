/*******************************************************************************
 * tlx/define.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_HEADER
#define TLX_DEFINE_HEADER

//! \defgroup tlx_define Defines and Macros
//! Attribute macros and other defines

/*[[[perl
foreach (sort glob("tlx/define/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n"
}
]]]*/
#include <tlx/define/attribute_always_inline.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/define/attribute_fallthrough.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/define/attribute_format_printf.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/define/attribute_packed.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/define/attribute_warn_unused_result.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/define/constexpr.hpp>         // NOLINT(misc-include-cleaner)
#include <tlx/define/deprecated.hpp>        // NOLINT(misc-include-cleaner)
#include <tlx/define/endian.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/define/likely.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/define/visibility_hidden.hpp> // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_DEFINE_HEADER

/******************************************************************************/
