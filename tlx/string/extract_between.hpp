/*******************************************************************************
 * tlx/string/extract_between.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_EXTRACT_BETWEEN_HEADER
#define TLX_STRING_EXTRACT_BETWEEN_HEADER

#include <tlx/container/string_view.hpp>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Search the string for given start and end separators and extract all
 * characters between the both, if they are found. Otherwise return an empty
 * string.
 *
 * \param str   string to search in
 * \param sep1  start boundary
 * \param sep2  end boundary
 */
std::string extract_between(tlx::string_view str, tlx::string_view sep1,
                            tlx::string_view sep2);

//! \}

} // namespace tlx

#endif // !TLX_STRING_EXTRACT_BETWEEN_HEADER

/******************************************************************************/
