/*******************************************************************************
 * tlx/string/escape_html.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_ESCAPE_HTML_HEADER
#define TLX_STRING_ESCAPE_HTML_HEADER

#include <tlx/container/string_view.hpp>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Escape characters for inclusion in HTML documents: replaces the characters <,
 * >, & and " with HTML entities.
 */
std::string escape_html(tlx::string_view str);

//! \}

} // namespace tlx

#endif // !TLX_STRING_ESCAPE_HTML_HEADER

/******************************************************************************/
