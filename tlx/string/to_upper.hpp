/*******************************************************************************
 * tlx/string/to_upper.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_TO_UPPER_HEADER
#define TLX_STRING_TO_UPPER_HEADER

#include <tlx/container/string_view.hpp>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

//! Transform the given character to upper case without any localization.
char to_upper(char ch);

/*!
 * Transforms the given string to uppercase and returns a reference to it.
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
std::string& to_upper(std::string* str);

/*!
 * Returns a copy of the given string converted to uppercase.
 *
 * \param str   string to process
 * \return      new string uppercased
 */
std::string to_upper(tlx::string_view str);

//! \}

} // namespace tlx

#endif // !TLX_STRING_TO_UPPER_HEADER

/******************************************************************************/
