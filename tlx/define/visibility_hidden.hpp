/*******************************************************************************
 * tlx/define/visibility_hidden.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_VISIBILITY_HIDDEN_HEADER
#define TLX_DEFINE_VISIBILITY_HIDDEN_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

/******************************************************************************/
// __attribute__ ((visibility ("hidden")))

#if defined(__GNUC__) || defined(__clang__)
#define TLX_VISIBILITY_HIDDEN __attribute__((visibility("hidden")))
#else
#define TLX_VISIBILITY_HIDDEN
#endif

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_VISIBILITY_HIDDEN_HEADER

/******************************************************************************/
