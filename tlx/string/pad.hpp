/*******************************************************************************
 * tlx/string/pad.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_PAD_HEADER
#define TLX_STRING_PAD_HEADER

#include <tlx/container/string_view.hpp>
#include <cstddef>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Truncate or pad string to exactly len characters.
 *
 * \param str      string to process
 * \param len      length of the string to pad to
 * \param pad_char character to pad string with
 * \return         copy of the modified string
 */
std::string pad(tlx::string_view str, size_t len, char pad_char = ' ');

//! \}

} // namespace tlx

#endif // !TLX_STRING_PAD_HEADER

/******************************************************************************/
