/*******************************************************************************
 * tlx/define/attribute_fallthrough.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_ATTRIBUTE_FALLTHROUGH_HEADER
#define TLX_DEFINE_ATTRIBUTE_FALLTHROUGH_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

/******************************************************************************/
// __attribute__ ((fallthrough))

#if defined(__GNUC__) && __GNUC__ >= 7
#define TLX_ATTRIBUTE_FALLTHROUGH __attribute__((fallthrough))
#elif defined(__clang__)
#define TLX_ATTRIBUTE_FALLTHROUGH [[clang::fallthrough]]
#else
#define TLX_ATTRIBUTE_FALLTHROUGH
#endif

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_ATTRIBUTE_FALLTHROUGH_HEADER

/******************************************************************************/
