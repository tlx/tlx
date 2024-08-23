/*******************************************************************************
 * tlx/string/union_words.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_UNION_WORDS_HEADER
#define TLX_STRING_UNION_WORDS_HEADER

#include <tlx/container/string_view.hpp>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Return union of two keyword sets.
 */
std::string union_words(tlx::string_view wordsA, tlx::string_view wordsB);

//! \}

} // namespace tlx

#endif // !TLX_STRING_UNION_WORDS_HEADER

/******************************************************************************/
