/*******************************************************************************
 * tlx/string/escape_uri.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_ESCAPE_URI_HEADER
#define TLX_STRING_ESCAPE_URI_HEADER

#include <tlx/container/string_view.hpp>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Escape a string into a URI-encoding. This maps all non A-Z0-9 characters to
 * %HH hex representation.
 */
std::string escape_uri(tlx::string_view str);

//! \}

} // namespace tlx

#endif // !TLX_STRING_ESCAPE_URI_HEADER

/******************************************************************************/
