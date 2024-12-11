/*******************************************************************************
 * tlx/string/trim.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_TRIM_HEADER
#define TLX_STRING_TRIM_HEADER

#include <tlx/container/string_view.hpp>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{
//! \name Trim
//! \{

/******************************************************************************/

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters in the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
std::string& trim(std::string* str);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters in the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      reference to the modified string
 */
std::string& trim(std::string* str, tlx::string_view drop);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      reference to the modified string
 */
std::string& trim(std::string* str, char drop);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters in the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
tlx::string_view& trim(tlx::string_view* str);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters in the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      reference to the modified string
 */
tlx::string_view& trim(tlx::string_view* str, tlx::string_view drop);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      reference to the modified string
 */
tlx::string_view& trim(tlx::string_view* str, char drop);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters in the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
tlx::string_view trim(tlx::string_view str);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters in the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      reference to the modified string
 */
tlx::string_view trim(tlx::string_view str, tlx::string_view drop);

/*!
 * Trims the given string in-place on the left and right. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      reference to the modified string
 */
tlx::string_view trim(tlx::string_view str, char drop);

/******************************************************************************/

/*!
 * Trims the given string in-place only on the right. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
std::string& trim_right(std::string* str);

/*!
 * Trims the given string in-place only on the right. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      reference to the modified string
 */
std::string& trim_right(std::string* str, tlx::string_view drop);

/*!
 * Trims the given string in-place only on the right. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      reference to the modified string
 */
std::string& trim_right(std::string* str, char drop);

/*!
 * Trims the given string in-place only on the right. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
tlx::string_view& trim_right(tlx::string_view* str);

/*!
 * Trims the given string in-place only on the right. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      reference to the modified string
 */
tlx::string_view& trim_right(tlx::string_view* str, tlx::string_view drop);

/*!
 * Trims the given string in-place only on the right. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      reference to the modified string
 */
tlx::string_view& trim_right(tlx::string_view* str, char drop);

/*!
 * Trims the given string only on the right. Removes all characters in the
 * given drop array, which defaults to " \r\n\t". Returns a copy of the string.
 *
 * \param str   string to process
 * \return      new trimmed string
 */
tlx::string_view trim_right(tlx::string_view str);

/*!
 * Trims the given string only on the right. Removes all characters in the
 * given drop array, which defaults to " \r\n\t". Returns a copy of the string.
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      new trimmed string
 */
tlx::string_view trim_right(tlx::string_view str, tlx::string_view drop);

/*!
 * Trims the given string only on the right. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      new trimmed string
 */
tlx::string_view trim_right(tlx::string_view str, char drop);

/******************************************************************************/

/*!
 * Trims the given string in-place only on the left. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
std::string& trim_left(std::string* str);

/*!
 * Trims the given string in-place only on the left. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      reference to the modified string
 */
std::string& trim_left(std::string* str, tlx::string_view drop);

/*!
 * Trims the given string in-place only on the left. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      reference to the modified string
 */
std::string& trim_left(std::string* str, char drop);

/*!
 * Trims the given string in-place only on the left. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \return      reference to the modified string
 */
tlx::string_view& trim_left(tlx::string_view* str);

/*!
 * Trims the given string in-place only on the left. Removes all characters in
 * the given drop array, which defaults to " \r\n\t".
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      reference to the modified string
 */
tlx::string_view& trim_left(tlx::string_view* str, tlx::string_view drop);

/*!
 * Trims the given string in-place only on the left. Removes all
 * characters equal to the given drop character.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      reference to the modified string
 */
tlx::string_view& trim_left(tlx::string_view* str, char drop);

/*!
 * Trims the given string only on the left. Removes all characters in the given
 * drop array, which defaults to " \r\n\t". Returns a copy of the string.
 *
 * \param str   string to process
 * \return      new trimmed string
 */
tlx::string_view trim_left(tlx::string_view str);

/*!
 * Trims the given string only on the left. Removes all characters in the given
 * drop array, which defaults to " \r\n\t". Returns a copy of the string.
 *
 * \param str   string to process
 * \param drop  remove these characters
 * \return      new trimmed string
 */
tlx::string_view trim_left(tlx::string_view str, tlx::string_view drop);

/*!
 * Trims the given string only on the left. Removes all characters equal to the
 * given drop character. Returns a copy of the string.
 *
 * \param str   string to process
 * \param drop  remove this character
 * \return      new trimmed string
 */
tlx::string_view trim_left(tlx::string_view str, char drop);

//! \}
//! \}

} // namespace tlx

#endif // !TLX_STRING_TRIM_HEADER

/******************************************************************************/
