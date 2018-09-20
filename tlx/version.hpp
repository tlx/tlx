/*******************************************************************************
 * tlx/version.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_VERSION_HEADER
#define TLX_VERSION_HEADER

namespace tlx {

//! TLX_MAJOR is the a mahor version number currently always zero.
#define TLX_MAJOR_VERSION     0

/*[[[perl
  return "keep" if $ENV{USER} ne "tb";
  use POSIX qw(strftime);
  my $date = strftime("%Y%m%d", localtime);
  print "//! TLX_MINOR is the date of the last commit.\n";
  print "#define TLX_MINOR_VERSION     $date\n";
]]]*/
//! TLX_MINOR is the date of the last commit.
#define TLX_MINOR_VERSION     20180920
// [[[end]]]

//! TLX_VERSION is a combination of TLX_MAJOR_VERSION and TLX_MINOR_VERSION
#define TLX_VERSION (TLX_MAJOR_VERSION * 100000000lu + TLX_MINOR_VERSION)

} // namespace tlx

#endif // !TLX_VERSION_HEADER

/******************************************************************************/
