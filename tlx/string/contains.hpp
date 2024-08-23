/*******************************************************************************
 * tlx/string/contains.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_CONTAINS_HEADER
#define TLX_STRING_CONTAINS_HEADER

#include <tlx/container/string_view.hpp>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/******************************************************************************/
// contains()

//! Tests of string contains pattern
bool contains(tlx::string_view str, tlx::string_view pattern);

//! Tests of string contains character
bool contains(tlx::string_view str, const char ch);

//! \}

} // namespace tlx

#endif // !TLX_STRING_CONTAINS_HEADER

/******************************************************************************/
