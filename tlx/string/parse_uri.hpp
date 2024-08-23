/*******************************************************************************
 * tlx/string/parse_uri.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2020-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_PARSE_URI_HEADER
#define TLX_STRING_PARSE_URI_HEADER

#include <tlx/container/string_view.hpp>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Parse a URI like "/path1/path2?query=string&submit=submit#fragment" into the
 * parts path, query_string, and fragment. The parts are returned as
 * tlx::string_views to avoid copying data.
 */
static inline void parse_uri(tlx::string_view uri, tlx::string_view* path,
                             tlx::string_view* query_string,
                             tlx::string_view* fragment)
{
    tlx::string_view::const_iterator c = uri.begin(), e = uri.end();

    // find path part
    tlx::string_view::const_iterator begin = c;
    while (c != e && *c != '?' && *c != '#')
        ++c;

    *path = tlx::string_view(begin, c - begin);

    // find query string
    begin = c;
    if (*c == '?')
    {
        begin = ++c;
        while (c != e && *c != '#')
            ++c;
    }
    *query_string = tlx::string_view(begin, c - begin);

    // find fragment
    begin = c;
    if (*c == '#')
    {
        begin = ++c;
        while (c != e)
            ++c;
    }
    *fragment = tlx::string_view(begin, c - begin);
}

//! \}

} // namespace tlx

#endif // !TLX_STRING_PARSE_URI_HEADER

/******************************************************************************/
