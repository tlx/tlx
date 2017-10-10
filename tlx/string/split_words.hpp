/*******************************************************************************
 * tlx/string/split_words.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_SPLIT_WORDS_HEADER
#define TLX_STRING_SPLIT_WORDS_HEADER

#include <string>
#include <vector>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Split the given string by whitespaces into distinct words. Multiple
 * consecutive whitespaces are considered as one split point. Whitespaces are
 * space, tab, newline and carriage-return.
 *
 * \param str   string to split
 * \param limit maximum number of parts returned
 * \return      vector containing each split substring
 */
std::vector<std::string> split_words(
    const std::string& str, std::string::size_type limit = std::string::npos);

//! \}

} // namespace tlx

#endif // !TLX_STRING_SPLIT_WORDS_HEADER

/******************************************************************************/
