/*******************************************************************************
 * tlx/define/attribute_format_printf.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_ATTRIBUTE_FORMAT_PRINTF_HEADER
#define TLX_DEFINE_ATTRIBUTE_FORMAT_PRINTF_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

/******************************************************************************/
// __attribute__ ((format(printf, #, #))

#if defined(__GNUC__) || defined(__clang__)
#define TLX_ATTRIBUTE_FORMAT_PRINTF(X, Y) __attribute__((format(printf, X, Y)))
#else
#define TLX_ATTRIBUTE_FORMAT_PRINTF(X, Y)
#endif

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_ATTRIBUTE_FORMAT_PRINTF_HEADER

/******************************************************************************/
