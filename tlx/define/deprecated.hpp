/*******************************************************************************
 * tlx/define/deprecated.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2008-2009 Andreas Beckmann <beckmann@cs.uni-frankfurt.de>
 * Copyright (C) 2013 Timo Bingmann <tb@panthema.net>
 * Copyright (C) 2018 Manuel Penschuck <tlx@manuel.jetzt>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_DEPRECATED_HEADER
#define TLX_DEFINE_DEPRECATED_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

#if TLX_NO_DEPRECATED
#define TLX_DEPRECATED(x) x
#elif defined(_MSC_VER)
#define TLX_DEPRECATED(x) __declspec(deprecated) x
#elif defined(__clang__) || defined(__GNUG__)
#define TLX_DEPRECATED(x) x __attribute__((deprecated))
#endif

#define TLX_DEPRECATED_FUNC_DEF(x)                                             \
    TLX_DEPRECATED(x);                                                         \
    x

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_DEPRECATED_HEADER

/******************************************************************************/
