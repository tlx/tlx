/*******************************************************************************
 * tlx/logger/tuple.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_LOGGER_TUPLE_HEADER
#define TLX_LOGGER_TUPLE_HEADER

#include <tlx/logger/core.hpp>
#include <tlx/meta/call_foreach_tuple_with_index.hpp>

#include <tuple>

namespace tlx {

class LoggerTupleFormatter
{
public:
    explicit LoggerTupleFormatter(std::ostream& os, const bool addSpace) : os_(os), addSpace_(addSpace) { }
    template <typename Index, typename Arg>
    void operator () (const Index&, const Arg& a) const {
        if (Index::index != 0) os_ << ',';
        if (Index::index != 0 && addSpace_) os_ << ' ';
        LoggerFormatter<typename std::decay<Arg>::type>::print(os_, a, addSpace_);
    }
    std::ostream& os_;
    bool addSpace_;
};

template <typename... Args>
class LoggerFormatter<std::tuple<Args...> >
{
public:
    static void print(std::ostream& os, const std::tuple<Args...>& t, const bool addSpace) {
	(void)addSpace;
        os << '(';
        call_foreach_tuple_with_index(LoggerTupleFormatter(os, addSpace), t);
        os << ')';
    }
};

template <>
class LoggerFormatter<std::tuple<> >
{
public:
    static void print(std::ostream& os, const std::tuple<>&, const bool addSpace) {
	(void)addSpace;
        os << '(' << ')';
    }
};

} // namespace tlx

#endif // !TLX_LOGGER_TUPLE_HEADER

/******************************************************************************/
