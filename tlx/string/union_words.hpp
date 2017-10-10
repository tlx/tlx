/*******************************************************************************
 * tlx/string/union_words.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_UNION_WORDS_HEADER
#define TLX_STRING_UNION_WORDS_HEADER

#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Return union of two keyword sets.
 */
std::string union_words(const std::string& wordsA, const std::string& wordsB);

//! \}

} // namespace tlx

#endif // !TLX_STRING_UNION_WORDS_HEADER

/******************************************************************************/
