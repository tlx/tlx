/*******************************************************************************
 * tlx/string/parse_uri_form_data.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2020-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_PARSE_URI_FORM_DATA_HEADER
#define TLX_STRING_PARSE_URI_FORM_DATA_HEADER

#include <tlx/container/string_view.hpp>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Helper function to decode %20 and + in urlencoded form data like
 * "query=string+with+spaces&submit=yes%21&".
 */
static inline std::string parse_uri_form_data_decode(tlx::string_view str)
{
    tlx::string_view::const_iterator s = str.begin();
    const tlx::string_view::const_iterator end = str.end();

    std::string out;
    out.reserve(str.size());

    while (s != end)
    {
        if (*s == '%' && s + 2 < end)
        {
            char a = s[1], b = s[2];
            if (a >= '0' && a <= '9')
                a -= '0';
            else if (a >= 'a' && a <= 'f')
                a -= 'a' - 10;
            else if (a >= 'A' && a <= 'F')
                a -= 'A' - 10;
            else
            {
                // invalid hex digits, copy '%' and continue
                out += *s++;
                continue;
            }

            if (b >= '0' && b <= '9')
                b -= '0';
            else if (b >= 'a' && b <= 'f')
                b -= 'a' - 10;
            else if (b >= 'A' && b <= 'F')
                b -= 'A' - 10;
            else
            {
                // invalid hex digits, copy '%' and continue
                out += *s++;
                continue;
            }

            out += static_cast<char>(16 * a + b);
            s += 3;
        }
        else if (*s == '+')
        {
            out += ' ';
            s++;
        }
        else
        {
            out += *s++;
        }
    }
    return out;
}

/*!
 * Parse a urlencoded form data like "query=string+with+spaces&submit=yes%21&"
 * into a list of keys and values. The keys and values are returned as pairs in
 * the two vectors, to avoid using std::pair or another struct.
 */
static inline void parse_uri_form_data(tlx::string_view query_string,
                                       std::vector<std::string>* key,
                                       std::vector<std::string>* value)
{
    key->clear(), value->clear();
    tlx::string_view::const_iterator c = query_string.begin(),
                                     e = query_string.end();

    while (c != e)
    {
        tlx::string_view::const_iterator begin = c;
        while (c != e && *c != '=')
            ++c;

        if (c == begin)
            return;

        std::string k = parse_uri_form_data_decode(tlx::string_view(begin, c));

        if (c == e)
        {
            key->emplace_back(std::move(k));
            value->emplace_back(std::string());
            return;
        }

        begin = ++c;
        while (c != e && *c != '&')
            ++c;

        std::string v = parse_uri_form_data_decode(tlx::string_view(begin, c));

        key->emplace_back(std::move(k));
        value->emplace_back(std::move(v));

        if (*c != '&')
            break;
        ++c;
    }
}

//! \}

} // namespace tlx

#endif // !TLX_STRING_PARSE_URI_FORM_DATA_HEADER

/******************************************************************************/
