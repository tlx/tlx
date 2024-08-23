/*******************************************************************************
 * tlx/string/less_icase.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_LESS_ICASE_HEADER
#define TLX_STRING_LESS_ICASE_HEADER

#include <tlx/container/string_view.hpp>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/******************************************************************************/
// less_icase()

//! returns true if a < b without regard for letter case
bool less_icase(const char* a, const char* b);

//! returns true if a < b without regard for letter case
bool less_icase(const char* a, tlx::string_view b);

//! returns true if a < b without regard for letter case
bool less_icase(tlx::string_view a, const char* b);

//! returns true if a < b without regard for letter case
bool less_icase(tlx::string_view a, tlx::string_view b);

/******************************************************************************/
// order_less_icase: case-insensitive order relation functional classes

//! Case-insensitive less order relation functional class for std::map, etc.
struct less_icase_asc
{
    bool operator()(tlx::string_view a, tlx::string_view b) const
    {
        return less_icase(a, b);
    }
};

//! Descending case-insensitive less order relation functional class for
//! std::map, etc.
struct less_icase_desc
{
    bool operator()(tlx::string_view a, tlx::string_view b) const
    {
        return !less_icase(a, b);
    }
};

//! \}

} // namespace tlx

#endif // !TLX_STRING_LESS_ICASE_HEADER

/******************************************************************************/
