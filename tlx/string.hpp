/*******************************************************************************
 * tlx/string.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_HEADER
#define TLX_STRING_HEADER

//! \defgroup tlx_string String Algorithms
//! Simple string manipulations

/*[[[perl
my %exclude = ("tlx/string/appendline.hpp" => 1,
               "tlx/string/ssprintf_generic.hpp" => 1);
foreach (grep {!$exclude{$_}} sort glob("tlx/string/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n"
}
]]]*/
#include <tlx/string/base64.hpp>        // NOLINT(misc-include-cleaner)
#include <tlx/string/bitdump.hpp>       // NOLINT(misc-include-cleaner)
#include <tlx/string/compare_icase.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/string/contains.hpp>      // NOLINT(misc-include-cleaner)
#include <tlx/string/contains_word.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/string/ends_with.hpp>     // NOLINT(misc-include-cleaner)
#include <tlx/string/equal_icase.hpp>   // NOLINT(misc-include-cleaner)
#include <tlx/string/erase_all.hpp>     // NOLINT(misc-include-cleaner)
#include <tlx/string/escape_html.hpp>   // NOLINT(misc-include-cleaner)
#include <tlx/string/escape_uri.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/string/expand_environment_variables.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/string/extract_between.hpp>     // NOLINT(misc-include-cleaner)
#include <tlx/string/format_iec_units.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/string/format_si_iec_units.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/string/format_si_units.hpp>     // NOLINT(misc-include-cleaner)
#include <tlx/string/hash_djb2.hpp>           // NOLINT(misc-include-cleaner)
#include <tlx/string/hash_sdbm.hpp>           // NOLINT(misc-include-cleaner)
#include <tlx/string/hexdump.hpp>             // NOLINT(misc-include-cleaner)
#include <tlx/string/index_of.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/string/join.hpp>                // NOLINT(misc-include-cleaner)
#include <tlx/string/join_generic.hpp>        // NOLINT(misc-include-cleaner)
#include <tlx/string/join_quoted.hpp>         // NOLINT(misc-include-cleaner)
#include <tlx/string/less_icase.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/string/levenshtein.hpp>         // NOLINT(misc-include-cleaner)
#include <tlx/string/pad.hpp>                 // NOLINT(misc-include-cleaner)
#include <tlx/string/parse_si_iec_units.hpp>  // NOLINT(misc-include-cleaner)
#include <tlx/string/parse_uri.hpp>           // NOLINT(misc-include-cleaner)
#include <tlx/string/parse_uri_form_data.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/string/replace.hpp>             // NOLINT(misc-include-cleaner)
#include <tlx/string/split.hpp>               // NOLINT(misc-include-cleaner)
#include <tlx/string/split_quoted.hpp>        // NOLINT(misc-include-cleaner)
#include <tlx/string/split_view.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/string/split_words.hpp>         // NOLINT(misc-include-cleaner)
#include <tlx/string/ssprintf.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/string/starts_with.hpp>         // NOLINT(misc-include-cleaner)
#include <tlx/string/to_lower.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/string/to_upper.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/string/trim.hpp>                // NOLINT(misc-include-cleaner)
#include <tlx/string/union_words.hpp>         // NOLINT(misc-include-cleaner)
#include <tlx/string/word_wrap.hpp>           // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_STRING_HEADER

/******************************************************************************/
