/*******************************************************************************
 * tlx/define/attribute_packed.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_ATTRIBUTE_PACKED_HEADER
#define TLX_DEFINE_ATTRIBUTE_PACKED_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

/******************************************************************************/
// __attribute__ ((packed))

#if defined(__GNUC__) || defined(__clang__)
#define TLX_ATTRIBUTE_PACKED __attribute__((packed))
#else
#define TLX_ATTRIBUTE_PACKED
#endif

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_ATTRIBUTE_PACKED_HEADER

/******************************************************************************/
