/*******************************************************************************
 * tlx/string/ends_with.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_ENDS_WITH_HEADER
#define TLX_STRING_ENDS_WITH_HEADER

#include <tlx/container/string_view.hpp>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/******************************************************************************/
// ends_with()

/*!
 * Checks if the given match string is located at the end of this string.
 */
bool ends_with(const char* str, const char* match);

/*!
 * Checks if the given match string is located at the end of this string.
 */
bool ends_with(const char* str, tlx::string_view match);

/*!
 * Checks if the given match string is located at the end of this string.
 */
bool ends_with(tlx::string_view str, const char* match);

/*!
 * Checks if the given match string is located at the end of this string.
 */
bool ends_with(tlx::string_view str, tlx::string_view match);

/******************************************************************************/
// ends_with_icase()

/*!
 * Checks if the given match string is located at the end of this
 * string. Compares the characters case-insensitively.
 */
bool ends_with_icase(const char* str, const char* match);

/*!
 * Checks if the given match string is located at the end of this
 * string. Compares the characters case-insensitively.
 */
bool ends_with_icase(const char* str, tlx::string_view match);

/*!
 * Checks if the given match string is located at the end of this
 * string. Compares the characters case-insensitively.
 */
bool ends_with_icase(tlx::string_view str, const char* match);

/*!
 * Checks if the given match string is located at the end of this
 * string. Compares the characters case-insensitively.
 */
bool ends_with_icase(tlx::string_view str, tlx::string_view match);

/******************************************************************************/

//! \}

} // namespace tlx

#endif // !TLX_STRING_ENDS_WITH_HEADER

/******************************************************************************/
