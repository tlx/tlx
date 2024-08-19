/*******************************************************************************
 * tlx/define/attribute_warn_unused_result.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_ATTRIBUTE_WARN_UNUSED_RESULT_HEADER
#define TLX_DEFINE_ATTRIBUTE_WARN_UNUSED_RESULT_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

/******************************************************************************/
// __attribute__ ((warn_unused_result))

#if defined(__GNUC__) || defined(__clang__)
#define TLX_ATTRIBUTE_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define TLX_ATTRIBUTE_WARN_UNUSED_RESULT
#endif

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_ATTRIBUTE_WARN_UNUSED_RESULT_HEADER

/******************************************************************************/
