/*******************************************************************************
 * tlx/define/attribute_always_inline.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_ATTRIBUTE_ALWAYS_INLINE_HEADER
#define TLX_DEFINE_ATTRIBUTE_ALWAYS_INLINE_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

/******************************************************************************/
// __attribute__ ((always_inline))

#if defined(__GNUC__) || defined(__clang__)
#define TLX_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#else
#define TLX_ATTRIBUTE_ALWAYS_INLINE
#endif

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_ATTRIBUTE_ALWAYS_INLINE_HEADER

/******************************************************************************/
