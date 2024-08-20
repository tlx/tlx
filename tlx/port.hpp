/*******************************************************************************
 * tlx/port.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_PORT_HEADER
#define TLX_PORT_HEADER

//! \defgroup tlx_port Helpers for Portable Code
//! Tools to enable easier writing of portable code.

/*[[[perl
foreach (sort glob("tlx/port/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n";
}
]]]*/
#include <tlx/port/setenv.hpp> // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_PORT_HEADER

/******************************************************************************/
