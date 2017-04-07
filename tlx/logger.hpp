/*******************************************************************************
 * tlx/logger.hpp
 *
 * Simple logging methods using ostream output.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_LOGGER_HEADER
#define TLX_LOGGER_HEADER

#include <sstream>
#include <string>

namespace tlx {

/*!

\brief LOG and sLOG for development and debugging

This is a short description of how to use \ref LOG and \ref sLOG for rapid
development of modules with debug output, and how to **keep it afterwards**.

There are two classes Logger and SpacingLogger, but one does not use these
directly.

Instead there are the macros: \ref LOG and \ref sLOG that can be used as such:
\code
LOG << "This will be printed with a newline";
sLOG << "Print variables a" << a << "b" << b << "c" << c;
\endcode

There macros only print the lines if the boolean variable **debug** is
true. This variable is searched for in the scope of the LOG, which means it can
be set or overridden in the function scope, the class scope, from **inherited
classes**, or even the global scope.

\code
class MyClass
{
    static constexpr bool debug = true;

    void func1()
    {
        LOG << "Hello World";

        LOG0 << "This is temporarily disabled.";
    }

    void func2()
    {
        static constexpr bool debug = false;
        LOG << "This is not printed any more.";

        LOG1 << "But this is forced.";
    }
};
\endcode

There are two variation of \ref LOG and \ref sLOG : append 0 or 1 for
temporarily disabled or enabled debug lines. These macros are then \ref LOG0,
\ref LOG1, \ref sLOG0, and \ref sLOG1. The suffix overrides the debug variable's
setting.

After a module works as intended, one can just set `debug = false`, and all
debug output will disappear and be optimized out.
 */
class Logger
{
private:
    //! collector stream
    std::ostringstream oss_;

public:
    //! mutex synchronized output to std::cout
    static void Output(const char* str);
    //! mutex synchronized output to std::cout
    static void Output(const std::string& str);

    //! output any type, including io manipulators
    template <typename AnyType>
    Logger& operator << (const AnyType& at) {
        oss_ << at;
        return *this;
    }

    //! destructor: output a newline
    ~Logger();
};

/*!
 * A logging class which outputs spaces between elements pushed via
 * operator<<. Depending on the real parameter the output may be suppressed.
 */
class SpacingLogger
{
private:
    //! true until the first element it outputted.
    bool first_ = true;

    //! collector stream
    std::ostringstream oss_;

public:
    //! output any type, including io manipulators
    template <typename AnyType>
    SpacingLogger& operator << (const AnyType& at) {
        if (!first_) oss_ << ' ';
        else first_ = false;

        oss_ << at;

        return *this;
    }

    //! destructor: output a newline
    ~SpacingLogger();
};

class LoggerVoidify
{
public:
    void operator & (Logger&) { }
    void operator & (SpacingLogger&) { }
};

//! Explicitly specify the condition for logging
#define LOGC(cond) \
    !(cond) ? (void)0 : ::tlx::LoggerVoidify() & ::tlx::Logger()

//! Default logging method: output if the local debug variable is true.
#define LOG LOGC(debug)

//! Override default output: never or always output log.
#define LOG0 LOGC(false)
#define LOG1 LOGC(true)

//! Explicitly specify the condition for logging
#define sLOGC(cond) \
    !(cond) ? (void)0 : ::tlx::LoggerVoidify() & ::tlx::SpacingLogger()

//! Default logging method: output if the local debug variable is true.
#define sLOG sLOGC(debug)

//! Override default output: never or always output log.
#define sLOG0 sLOGC(false)
#define sLOG1 sLOGC(true)

} // namespace tlx

#endif // !TLX_LOGGER_HEADER

/******************************************************************************/
