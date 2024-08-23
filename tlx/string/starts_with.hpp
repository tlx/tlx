/*******************************************************************************
 * tlx/string/starts_with.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_STARTS_WITH_HEADER
#define TLX_STRING_STARTS_WITH_HEADER

#include <tlx/container/string_view.hpp>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/******************************************************************************/

/*!
 * Checks if the given match string is located at the start of this string.
 */
bool starts_with(tlx::string_view str, tlx::string_view match);

/******************************************************************************/

/*!
 * Checks if the given match string is located at the start of this
 * string. Compares the characters case-insensitively.
 */
bool starts_with_icase(tlx::string_view str, tlx::string_view match);

/******************************************************************************/

//! \}

} // namespace tlx

#endif // !TLX_STRING_STARTS_WITH_HEADER

/******************************************************************************/
