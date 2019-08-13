/*******************************************************************************
 * tlx/container/string_view.hpp
 *
 * A simplified string_view implementation for portability.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_CONTAINER_STRING_VIEW_HEADER
#define TLX_CONTAINER_STRING_VIEW_HEADER

#include <algorithm>
#include <ostream>
#include <string>

#include <tlx/string/hash_djb2.hpp>

namespace tlx {

//! \addtogroup tlx_container
//! \{

/*!
 * StringView is a reference to a part of a string, consisting of only a char
 * pointer and a length. It does not have ownership of the substring and is used
 * mainly for temporary objects.
 */
class StringView
{
public:
    using iterator = const char*;

    //! Default constructor for a StringView. Doesn't initialize anything.
    StringView() = default;

    /*!
     * Creates a new StringView, given a const char* and the size.
     *
     * \param data pointer to start of data
     * \param size size of data in bytes.
     * \return new StringView object.
     */
    StringView(const char* data, size_t size) noexcept
        : data_(data), size_(size) { }

    /*!
     * Creates a new StringView, given a const iterator to a std::string and the
     * size.
     *
     * \param data iterator to start of data
     * \param size size of data in character.
     * \return new StringView object.
     */
    StringView(const std::string::const_iterator& data, size_t size) noexcept
        : data_(&(*data)), size_(size) { }

    /*!
     * Creates a new reference StringView, given two const iterators to a
     * std::string.
     *
     * \param begin iterator to start of data
     * \param end iterator to the end of data.
     * \return new StringView object.
     */
    StringView(const std::string::const_iterator& begin,
               const std::string::const_iterator& end) noexcept
        : StringView(begin, end - begin) { }

    //! Construct a StringView to the whole std::string.
    explicit StringView(const std::string& str) noexcept
        : StringView(str.begin(), str.end()) { }

    //! Returns a pointer to the start of the data.
    const char * data() const noexcept {
        return data_;
    }

    //! Returns a pointer to the beginning of the data.
    iterator begin() const noexcept {
        return data_;
    }

    //! Returns a pointer beyond the end of the data.
    iterator end() const noexcept {
        return data_ + size_;
    }

    //! Returns the size of this StringView
    size_t size() const noexcept {
        return size_;
    }

    //! Equality operator to compare a StringView with another StringView
    bool operator == (const StringView& other) const noexcept {
        return size_ == other.size_ &&
               std::equal(data_, data_ + size_, other.data_);
    }

    //! Inequality operator to compare a StringView with another StringView
    bool operator != (const StringView& other) const noexcept {
        return !(operator == (other));
    }

    //! Less operator to compare a StringView with another StringView
    //! lexicographically
    bool operator < (const StringView& other) const noexcept {
        return std::lexicographical_compare(
            data_, data_ + size_, other.data_, other.data_ + other.size_);
    }

    //! Equality operator to compare a StringView with a std::string
    bool operator == (const std::string& other) const noexcept {
        return size_ == other.size() &&
               std::equal(data_, data_ + size_, other.data());
    }

    //! Inequality operator to compare a StringView with a std::string
    bool operator != (const std::string& other) const noexcept {
        return !(operator == (other));
    }

    //! Less operator to compare a StringView with a std::string
    //! lexicographically
    bool operator < (const std::string& other) const noexcept {
        return std::lexicographical_compare(
            data_, data_ + size_, other.data(), other.data() + other.size());
    }

    //! make StringView ostreamable
    friend std::ostream& operator << (std::ostream& os, const StringView& sv) {
        return os.write(sv.data(), sv.size());
    }

    //! Returns the data of this StringView as a std::string
    std::string to_string() const {
        return std::string(data_, size_);
    }

    // operator std::string () const { return to_string(); }

private:
    //! pointer to character data
    const char* data_;
    //! size of data
    size_t size_;
};

//! Equality operator to compare a std::string with a StringView
static inline
bool operator == (const std::string& a, const StringView& b) noexcept {
    return b == a;
}

//! Inequality operator to compare a std::string with a StringView
static inline
bool operator != (const std::string& a, const StringView& b) noexcept {
    return b != a;
}

//! Less operator to compare a std::string with a StringView lexicographically
static inline
bool operator < (const std::string& a, const StringView& b) noexcept {
    return std::lexicographical_compare(
        a.data(), a.data() + a.size(), b.data(), b.data() + b.size());
}

//! make alias due to STL similarity
using string_view = StringView;

//! \}

} // namespace tlx

namespace std {
template <>
struct hash<tlx::StringView> {
    size_t operator () (const tlx::StringView& sv) const {
        return tlx::hash_djb2(sv.data(), sv.size());
    }
};

} // namespace std

#endif // !TLX_CONTAINER_STRING_VIEW_HEADER

/******************************************************************************/
