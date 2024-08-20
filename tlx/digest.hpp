/*******************************************************************************
 * tlx/digest.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DIGEST_HEADER
#define TLX_DIGEST_HEADER

//! \defgroup tlx_digest Message Digests
//! Message Digests: MD-5, SHA-256, and SHA-512.

/*[[[perl
foreach (sort glob("tlx/digest/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n"
}
]]]*/
#include <tlx/digest/md5.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/digest/sha1.hpp>   // NOLINT(misc-include-cleaner)
#include <tlx/digest/sha256.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/digest/sha512.hpp> // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_DIGEST_HEADER

/******************************************************************************/
