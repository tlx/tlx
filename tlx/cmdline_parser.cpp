/*******************************************************************************
 * tlx/cmdline_parser.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/cmdline_parser.hpp>
#include <tlx/container/string_view.hpp>
#include <tlx/define/visibility_hidden.hpp>
#include <tlx/string/parse_si_iec_units.hpp>
#include <tlx/unused.hpp>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace tlx {

/******************************************************************************/
// Argument and Struct Hierarchy below it.

//! base class of all options and parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::Argument
{
public:
    //! single letter short option, or 0 is none
    char key_;
    //! long option key or name for parameters
    std::string longkey_;
    //! option type description, e.g. "<#>" to indicate numbers
    std::string keytype_;
    //! longer description, which will be wrapped
    std::string desc_;
    //! required, process() fails if the option/parameter is not found.
    bool required_;
    //! found during processing of command line
    bool found_ = false;
    //! repeated argument, i.e. std::vector<std::string>
    bool repeated_ = false;

public:
    //! contructor filling most attributes
    Argument(char key, tlx::string_view longkey, tlx::string_view keytype,
             tlx::string_view desc, bool required)
        : key_(key),
          longkey_(longkey),
          keytype_(keytype),
          desc_(desc),
          required_(required)
    {
    }

    //! empty virtual destructor
    virtual ~Argument() = default;

    //! return formatted type name to user
    virtual const char* type_name() const = 0;

    //! process one item from command line for this argument
    virtual bool process(int& argc, const char* const*& argv) = 0;

    //! format value to ostream
    virtual void print_value(std::ostream& os) const = 0;

    //! return 'longkey [keytype]'
    std::string param_text() const
    {
        std::string s = longkey_;
        if (!keytype_.empty())
            s += ' ' + keytype_;
        return s;
    }

    //! return '-s, --longkey [keytype]'
    std::string option_text() const
    {
        std::string s;
        if (key_ != 0)
            s += '-', s += key_, s += ", ";
        else
            s += "    ";
        s += "--", s += longkey_;
        if (!keytype_.empty())
            s += ' ' + keytype_;
        return s;
    }
};

//! specialization of argument for boolean flags (can only be set to true).
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentBool final : public Argument
{
private:
    //! reference to boolean to set to true
    bool& dest_;

public:
    //! contructor filling most attributes
    ArgumentBool(char key, tlx::string_view longkey, tlx::string_view keytype,
                 tlx::string_view desc, bool required, bool& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "bool";
    }

    //! "process" argument: just set to true, no argument is used.
    bool process(int& argc, const char* const*& argv) final
    {
        unused(argc), unused(argv);
        dest_ = true;
        return true;
    }

    void print_value(std::ostream& os) const final
    {
        os << (dest_ ? "true" : "false");
    }
};

//! specialization of argument for integer options or parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentInt final : public Argument
{
private:
    int& dest_;

public:
    //! contructor filling most attributes
    ArgumentInt(char key, tlx::string_view longkey, tlx::string_view keytype,
                tlx::string_view desc, bool required, int& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "integer";
    }

    //! parse signed integer using sscanf.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        char* endptr;
        long x = strtol(argv[0], &endptr, 10);
        if (endptr != nullptr && *endptr == 0 &&
            x <= std::numeric_limits<int>::max())
        {
            --argc, ++argv;
            dest_ = static_cast<int>(x);
            return true;
        }
        return false;
    }

    void print_value(std::ostream& os) const final
    {
        os << dest_;
    }
};

//! specialization of argument for unsigned integer options or parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentUnsigned final
    : public Argument
{
private:
    unsigned int& dest_;

public:
    //! contructor filling most attributes
    ArgumentUnsigned(char key, tlx::string_view longkey,
                     tlx::string_view keytype, tlx::string_view desc,
                     bool required, unsigned int& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "unsigned";
    }

    //! parse unsigned integer using sscanf.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        char* endptr;
        unsigned long x = strtoul(argv[0], &endptr, 10);
        if (endptr != nullptr && *endptr == 0 &&
            x <= std::numeric_limits<unsigned int>::max())
        {
            --argc, ++argv;
            dest_ = static_cast<unsigned int>(x);
            return true;
        }
        return false;
    }

    void print_value(std::ostream& os) const final
    {
        os << dest_;
    }
};

//! specialization of argument for size_t options or parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentSizeT final : public Argument
{
private:
    size_t& dest_;

public:
    //! contructor filling most attributes
    ArgumentSizeT(char key, tlx::string_view longkey, tlx::string_view keytype,
                  tlx::string_view desc, bool required, size_t& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "size_t";
    }

    //! parse size_t using sscanf.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        char* endptr;
        unsigned long long x = strtoull(argv[0], &endptr, 10);
        if (endptr != nullptr && *endptr == 0 &&
            x <= std::numeric_limits<size_t>::max())
        {
            --argc, ++argv;
            dest_ = x;
            return true;
        }
        return false;
    }

    void print_value(std::ostream& os) const final
    {
        os << dest_;
    }
};

//! specialization of argument for float options or parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentFloat final : public Argument
{
private:
    float& dest_;

public:
    //! contructor filling most attributes
    ArgumentFloat(char key, tlx::string_view longkey, tlx::string_view keytype,
                  tlx::string_view desc, bool required, float& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "float";
    }

    //! parse unsigned integer using sscanf.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        char* endptr;
        dest_ = strtof(argv[0], &endptr);
        if (endptr != nullptr && *endptr == 0)
        {
            --argc, ++argv;
            return true;
        }
        return false;
    }

    void print_value(std::ostream& os) const final
    {
        os << dest_;
    }
};

//! specialization of argument for double options or parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentDouble final
    : public Argument
{
private:
    double& dest_;

public:
    //! contructor filling most attributes
    ArgumentDouble(char key, tlx::string_view longkey, tlx::string_view keytype,
                   tlx::string_view desc, bool required, double& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "double";
    }

    //! parse unsigned integer using sscanf.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        char* endptr;
        dest_ = strtod(argv[0], &endptr);
        if (endptr != nullptr && *endptr == 0)
        {
            --argc, ++argv;
            return true;
        }
        return false;
    }

    void print_value(std::ostream& os) const final
    {
        os << dest_;
    }
};

//! specialization of argument for SI/IEC suffixes byte size options or
//! parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentBytes32 final
    : public Argument
{
private:
    std::uint32_t& dest_;

public:
    //! contructor filling most attributes
    ArgumentBytes32(char key, tlx::string_view longkey,
                    tlx::string_view keytype, tlx::string_view desc,
                    bool required, std::uint32_t& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "bytes";
    }

    //! parse byte size using SI/IEC parser.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        std::uint64_t dest;
        if (parse_si_iec_units(argv[0], &dest) &&
            static_cast<std::uint64_t>(
                dest_ = static_cast<std::uint32_t>(dest)) == dest)
        {
            --argc, ++argv;
            return true;
        }
        return false;
    }

    void print_value(std::ostream& os) const final
    {
        os << dest_;
    }
};

//! specialization of argument for SI/IEC suffixes byte size options or
//! parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentBytes64 final
    : public Argument
{
private:
    std::uint64_t& dest_;

public:
    //! contructor filling most attributes
    ArgumentBytes64(char key, tlx::string_view longkey,
                    tlx::string_view keytype, tlx::string_view desc,
                    bool required, std::uint64_t& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "bytes";
    }

    //! parse byte size using SI/IEC parser.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        if (parse_si_iec_units(argv[0], &dest_))
        {
            --argc, ++argv;
            return true;
        }
        return false;
    }

    void print_value(std::ostream& os) const final
    {
        os << dest_;
    }
};

//! specialization of argument for string options or parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentString final
    : public Argument
{
private:
    std::string& dest_;

public:
    //! contructor filling most attributes
    ArgumentString(char key, tlx::string_view longkey, tlx::string_view keytype,
                   tlx::string_view desc, bool required, std::string& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
    }

    const char* type_name() const final
    {
        return "string";
    }

    //! "process" string argument just by storing it.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        dest_ = argv[0];
        --argc, ++argv;
        return true;
    }

    void print_value(std::ostream& os) const final
    {
        os << '"' << dest_ << '"';
    }
};

//! specialization of argument for multiple string options or parameters
class TLX_VISIBILITY_HIDDEN CmdlineParser::ArgumentStringlist final
    : public Argument
{
private:
    std::vector<std::string>& dest_;

public:
    //! contructor filling most attributes
    ArgumentStringlist(char key, tlx::string_view longkey,
                       tlx::string_view keytype, tlx::string_view desc,
                       bool required, std::vector<std::string>& dest)
        : Argument(key, longkey, keytype, desc, required), dest_(dest)
    {
        repeated_ = true;
    }

    const char* type_name() const final
    {
        return "string list";
    }

    //! "process" string argument just by storing it in vector.
    bool process(int& argc, const char* const*& argv) final
    {
        if (argc == 0)
            return false;
        dest_.emplace_back(argv[0]);
        --argc, ++argv;
        return true;
    }

    void print_value(std::ostream& os) const final
    {
        os << '[';
        for (size_t i = 0; i < dest_.size(); ++i)
        {
            if (i != 0)
                os << ',';
            os << '"' << dest_[i] << '"';
        }
        os << ']';
    }
};

/******************************************************************************/

void CmdlineParser::calc_option_max(const Argument* arg)
{
    option_max_width_ =
        std::max(arg->option_text().size() + 2, option_max_width_);
}

void CmdlineParser::calc_param_max(const Argument* arg)
{
    param_max_width_ = std::max(arg->param_text().size() + 2, param_max_width_);
}

/******************************************************************************/

void CmdlineParser::output_wrap(std::ostream& os, tlx::string_view text,
                                size_t wraplen, size_t indent_first,
                                size_t indent_rest, size_t current,
                                size_t indent_newline)
{
    std::string::size_type t = 0;
    size_t indent = indent_first;

    while (t != text.size())
    {
        std::string::size_type to = t, lspace = t;

        // scan forward in text until we hit a newline or wrap point
        while (to != text.size() && to + current + indent < t + wraplen &&
               text[to] != '\n')
        {
            if (text[to] == ' ')
                lspace = to;
            ++to;
        }

        // go back to last space
        if (to != text.size() && text[to] != '\n' && lspace != t)
            to = lspace + 1;

        // output line
        os << std::string(indent, ' ') << text.substr(t, to - t) << '\n';

        current = 0;
        indent = indent_rest;

        // skip over last newline
        if (to != text.size() && text[to] == '\n')
        {
            indent = indent_newline;
            ++to;
        }

        t = to;
    }
}

/******************************************************************************/

CmdlineParser::CmdlineParser()
{
}

CmdlineParser::~CmdlineParser()
{
    for (size_t i = 0; i < option_list_.size(); ++i)
        delete option_list_[i];
    option_list_.clear();

    for (size_t i = 0; i < param_list_.size(); ++i)
        delete param_list_[i];
    param_list_.clear();
}

void CmdlineParser::set_description(tlx::string_view description)
{
    description_.assign(description.data(), description.size());
}

void CmdlineParser::set_author(tlx::string_view author)
{
    author_.assign(author.data(), author.size());
}

void CmdlineParser::set_verbose_process(bool verbose_process)
{
    verbose_process_ = verbose_process;
}

/******************************************************************************/

void CmdlineParser::add_bool(char key, tlx::string_view longkey,
                             tlx::string_view keytype, bool& dest,
                             tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentBool(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_flag(char key, tlx::string_view longkey,
                             tlx::string_view keytype, bool& dest,
                             tlx::string_view desc)
{
    return add_bool(key, longkey, keytype, dest, desc);
}

void CmdlineParser::add_int(char key, tlx::string_view longkey,
                            tlx::string_view keytype, int& dest,
                            tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentInt(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_unsigned(char key, tlx::string_view longkey,
                                 tlx::string_view keytype, unsigned int& dest,
                                 tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentUnsigned(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_uint(char key, tlx::string_view longkey,
                             tlx::string_view keytype, unsigned int& dest,
                             tlx::string_view desc)
{
    return add_unsigned(key, longkey, keytype, dest, desc);
}

void CmdlineParser::add_size_t(char key, tlx::string_view longkey,
                               tlx::string_view keytype, size_t& dest,
                               tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentSizeT(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_float(char key, tlx::string_view longkey,
                              tlx::string_view keytype, float& dest,
                              tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentFloat(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_double(char key, tlx::string_view longkey,
                               tlx::string_view keytype, double& dest,
                               tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentDouble(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_bytes(char key, tlx::string_view longkey,
                              tlx::string_view keytype, std::uint32_t& dest,
                              tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentBytes32(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_bytes(char key, tlx::string_view longkey,
                              tlx::string_view keytype, std::uint64_t& dest,
                              tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentBytes64(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_string(char key, tlx::string_view longkey,
                               tlx::string_view keytype, std::string& dest,
                               tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentString(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

void CmdlineParser::add_stringlist(char key, tlx::string_view longkey,
                                   tlx::string_view keytype,
                                   std::vector<std::string>& dest,
                                   tlx::string_view desc)
{
    option_list_.emplace_back(
        new ArgumentStringlist(key, longkey, keytype, desc, false, dest));
    calc_option_max(option_list_.back());
}

/******************************************************************************/

void CmdlineParser::add_bool(char key, tlx::string_view longkey, bool& dest,
                             tlx::string_view desc)
{
    return add_bool(key, longkey, "", dest, desc);
}

void CmdlineParser::add_flag(char key, tlx::string_view longkey, bool& dest,
                             tlx::string_view desc)
{
    return add_bool(key, longkey, dest, desc);
}

void CmdlineParser::add_int(char key, tlx::string_view longkey, int& dest,
                            tlx::string_view desc)
{
    return add_int(key, longkey, "", dest, desc);
}

void CmdlineParser::add_unsigned(char key, tlx::string_view longkey,
                                 unsigned int& dest, tlx::string_view desc)
{
    return add_unsigned(key, longkey, "", dest, desc);
}

void CmdlineParser::add_uint(char key, tlx::string_view longkey,
                             unsigned int& dest, tlx::string_view desc)
{
    return add_unsigned(key, longkey, dest, desc);
}

void CmdlineParser::add_size_t(char key, tlx::string_view longkey, size_t& dest,
                               tlx::string_view desc)
{
    return add_size_t(key, longkey, "", dest, desc);
}

void CmdlineParser::add_float(char key, tlx::string_view longkey, float& dest,
                              tlx::string_view desc)
{
    return add_float(key, longkey, "", dest, desc);
}

void CmdlineParser::add_double(char key, tlx::string_view longkey, double& dest,
                               tlx::string_view desc)
{
    return add_double(key, longkey, "", dest, desc);
}

void CmdlineParser::add_bytes(char key, tlx::string_view longkey,
                              std::uint32_t& dest, tlx::string_view desc)
{
    return add_bytes(key, longkey, "", dest, desc);
}

void CmdlineParser::add_bytes(char key, tlx::string_view longkey,
                              std::uint64_t& dest, tlx::string_view desc)
{
    return add_bytes(key, longkey, "", dest, desc);
}

void CmdlineParser::add_string(char key, tlx::string_view longkey,
                               std::string& dest, tlx::string_view desc)
{
    return add_string(key, longkey, "", dest, desc);
}

void CmdlineParser::add_stringlist(char key, tlx::string_view longkey,
                                   std::vector<std::string>& dest,
                                   tlx::string_view desc)
{
    return add_stringlist(key, longkey, "", dest, desc);
}

/******************************************************************************/

void CmdlineParser::add_bool(tlx::string_view longkey, bool& dest,
                             tlx::string_view desc)
{
    return add_bool(0, longkey, "", dest, desc);
}

void CmdlineParser::add_flag(tlx::string_view longkey, bool& dest,
                             tlx::string_view desc)
{
    return add_bool(0, longkey, dest, desc);
}

void CmdlineParser::add_int(tlx::string_view longkey, int& dest,
                            tlx::string_view desc)
{
    return add_int(0, longkey, "", dest, desc);
}

void CmdlineParser::add_unsigned(tlx::string_view longkey, unsigned int& dest,
                                 tlx::string_view desc)
{
    return add_unsigned(0, longkey, "", dest, desc);
}

void CmdlineParser::add_uint(tlx::string_view longkey, unsigned int& dest,
                             tlx::string_view desc)
{
    return add_unsigned(0, longkey, dest, desc);
}

void CmdlineParser::add_size_t(tlx::string_view longkey, size_t& dest,
                               tlx::string_view desc)
{
    return add_size_t(0, longkey, "", dest, desc);
}

void CmdlineParser::add_float(tlx::string_view longkey, float& dest,
                              tlx::string_view desc)
{
    return add_float(0, longkey, "", dest, desc);
}

void CmdlineParser::add_double(tlx::string_view longkey, double& dest,
                               tlx::string_view desc)
{
    return add_double(0, longkey, "", dest, desc);
}

void CmdlineParser::add_bytes(tlx::string_view longkey, std::uint32_t& dest,
                              tlx::string_view desc)
{
    return add_bytes(0, longkey, "", dest, desc);
}

void CmdlineParser::add_bytes(tlx::string_view longkey, std::uint64_t& dest,
                              tlx::string_view desc)
{
    return add_bytes(0, longkey, "", dest, desc);
}

void CmdlineParser::add_string(tlx::string_view longkey, std::string& dest,
                               tlx::string_view desc)
{
    return add_string(0, longkey, "", dest, desc);
}

void CmdlineParser::add_stringlist(tlx::string_view longkey,
                                   std::vector<std::string>& dest,
                                   tlx::string_view desc)
{
    return add_stringlist(0, longkey, "", dest, desc);
}

/******************************************************************************/

void CmdlineParser::add_param_int(tlx::string_view name, int& dest,
                                  tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentInt(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_unsigned(tlx::string_view name,
                                       unsigned int& dest,
                                       tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentUnsigned(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_uint(tlx::string_view name, unsigned int& dest,
                                   tlx::string_view desc)
{
    add_param_unsigned(name, dest, desc);
}

void CmdlineParser::add_param_size_t(tlx::string_view name, size_t& dest,
                                     tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentSizeT(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_float(tlx::string_view name, float& dest,
                                    tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentFloat(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_double(tlx::string_view name, double& dest,
                                     tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentDouble(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_bytes(tlx::string_view name, std::uint32_t& dest,
                                    tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentBytes32(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_bytes(tlx::string_view name, std::uint64_t& dest,
                                    tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentBytes64(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_string(tlx::string_view name, std::string& dest,
                                     tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentString(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_param_stringlist(tlx::string_view name,
                                         std::vector<std::string>& dest,
                                         tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentStringlist(0, name, "", desc, true, dest));
    calc_param_max(param_list_.back());
}

/******************************************************************************/

void CmdlineParser::add_opt_param_int(tlx::string_view name, int& dest,
                                      tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentInt(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_unsigned(tlx::string_view name,
                                           unsigned int& dest,
                                           tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentUnsigned(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_uint(tlx::string_view name,
                                       unsigned int& dest,
                                       tlx::string_view desc)
{
    return add_opt_param_unsigned(name, dest, desc);
}

void CmdlineParser::add_opt_param_size_t(tlx::string_view name, size_t& dest,
                                         tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentSizeT(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_float(tlx::string_view name, float& dest,
                                        tlx::string_view desc)
{
    param_list_.emplace_back(new ArgumentFloat(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_double(tlx::string_view name, double& dest,
                                         tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentDouble(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_bytes(tlx::string_view name,
                                        std::uint32_t& dest,
                                        tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentBytes32(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_bytes(tlx::string_view name,
                                        std::uint64_t& dest,
                                        tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentBytes64(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_string(tlx::string_view name,
                                         std::string& dest,
                                         tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentString(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

void CmdlineParser::add_opt_param_stringlist(tlx::string_view name,
                                             std::vector<std::string>& dest,
                                             tlx::string_view desc)
{
    param_list_.emplace_back(
        new ArgumentStringlist(0, name, "", desc, false, dest));
    calc_param_max(param_list_.back());
}

/******************************************************************************/

CmdlineParser& CmdlineParser::sort()
{
    std::sort(option_list_.begin(), option_list_.end(),
              [](const Argument* a, Argument* b) {
                  return a->longkey_ < b->longkey_;
              });
    return *this;
}

void CmdlineParser::print_usage(std::ostream& os)
{
    std::ios::fmtflags flags(os.flags());

    os << "Usage: " << program_name_
       << (!option_list_.empty() ? " [options]" : "");

    for (ArgumentList::const_iterator it = param_list_.begin();
         it != param_list_.end(); ++it)
    {
        const Argument* arg = *it;

        os << (arg->required_ ? " <" : " [") << arg->longkey_
           << (arg->repeated_ ? " ..." : "") << (arg->required_ ? '>' : ']');
    }

    os << '\n';

    if (!description_.empty())
    {
        os << '\n';
        output_wrap(os, description_, line_wrap_);
    }
    if (!author_.empty())
    {
        os << "Author: " << author_ << '\n';
    }

    if (!description_.empty() || !author_.empty())
        os << '\n';

    if (!param_list_.empty())
    {
        os << "Parameters:\n";

        for (ArgumentList::const_iterator it = param_list_.begin();
             it != param_list_.end(); ++it)
        {
            const Argument* arg = *it;

            os << "  " << std::setw(static_cast<int>(param_max_width_))
               << std::left << arg->param_text();
            output_wrap(os, arg->desc_, line_wrap_, 0, param_max_width_ + 2,
                        param_max_width_ + 2, 8);
        }
    }

    if (!option_list_.empty())
    {
        os << "Options:\n";

        for (ArgumentList::const_iterator it = option_list_.begin();
             it != option_list_.end(); ++it)
        {
            const Argument* arg = *it;

            os << "  " << std::setw(static_cast<int>(option_max_width_))
               << std::left << arg->option_text();
            output_wrap(os, arg->desc_, line_wrap_, 0, option_max_width_ + 2,
                        option_max_width_ + 2, 8);
        }
    }

    os.flags(flags);
}

void CmdlineParser::print_usage()
{
    return print_usage(std::cout);
}

void CmdlineParser::print_option_error(int argc, const char* const* argv,
                                       const Argument* arg, std::ostream& os)
{
    os << "Error: argument ";
    if (argc != 0)
        os << '"' << argv[0] << '"';

    os << " for " << arg->type_name() << " option " << arg->option_text()
       << (argc == 0 ? " is missing!" : " is invalid!") << '\n'
       << '\n';

    print_usage(os);
}

void CmdlineParser::print_param_error(int argc, const char* const* argv,
                                      const Argument* arg, std::ostream& os)
{
    os << "Error: argument ";
    if (argc != 0)
        os << '"' << argv[0] << '"';

    os << " for " << arg->type_name() << " parameter " << arg->param_text()
       << (argc == 0 ? " is missing!" : " is invalid!") << '\n'
       << '\n';

    print_usage(os);
}

bool CmdlineParser::process(int argc, const char* const* argv, std::ostream& os)
{
    program_name_ = argv[0];
    --argc, ++argv;

    // search for help string and output help
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_usage(os);
            return false;
        }
    }

    // current argument in param_list_
    ArgumentList::iterator argi = param_list_.begin();
    bool end_optlist = false;

    while (argc != 0)
    {
        const char* arg = argv[0];

        if (arg[0] == '-' && !end_optlist)
        {
            // option, advance to argument
            --argc, ++argv;
            if (arg[1] == '-')
            {
                if (arg[2] == '-')
                {
                    end_optlist = true;
                }
                else
                {
                    // long option
                    ArgumentList::const_iterator oi = option_list_.begin();
                    for (; oi != option_list_.end(); ++oi)
                    {
                        if ((arg + 2) == (*oi)->longkey_)
                        {
                            if (!(*oi)->process(argc, argv))
                            {
                                print_option_error(argc, argv, *oi, os);
                                return false;
                            }

                            if (verbose_process_)
                            {
                                os << "Option " << (*oi)->option_text()
                                   << " set to ";
                                (*oi)->print_value(os);
                                os << '.' << '\n';
                            }
                            break;
                        }
                    }
                    if (oi == option_list_.end())
                    {
                        os << "Error: unknown option \"" << arg << "\".\n\n";
                        print_usage(os);
                        return false;
                    }
                }
            }
            else
            {
                // short option
                if (arg[1] == 0)
                {
                    os << "Invalid option \"" << arg << "\".\n";
                }
                else
                {
                    size_t offset = 1, arg_length = strlen(arg);
                    int old_argc = argc;
                    // Arguments will increase argc, so abort if it increases,
                    // while flags won't, so increase offset and parse next
                    while (offset < arg_length && argc == old_argc)
                    {
                        ArgumentList::const_iterator oi = option_list_.begin();
                        for (; oi != option_list_.end(); ++oi)
                        {
                            if (arg[offset] == (*oi)->key_)
                            {
                                ++offset;
                                if (!(*oi)->process(argc, argv))
                                {
                                    print_option_error(argc, argv, *oi, os);
                                    return false;
                                }

                                if (verbose_process_)
                                {
                                    os << "Option " << (*oi)->option_text()
                                       << " set to ";
                                    (*oi)->print_value(os);
                                    os << '.' << '\n';
                                }
                                break;
                            }
                        }
                        if (oi == option_list_.end())
                        {
                            os << "Error: unknown option \"";
                            if (arg_length > 2)
                            {
                                // multiple short options combined
                                os << "-" << arg[offset] << "\" at position "
                                   << offset << " in option sequence \"";
                            }
                            os << arg << "\".\n\n";
                            print_usage(os);
                            return false;
                        }
                    }
                }
            }
        }
        else
        {
            if (argi != param_list_.end())
            {
                if (!(*argi)->process(argc, argv))
                {
                    print_param_error(argc, argv, *argi, os);
                    return false;
                }

                if (verbose_process_)
                {
                    os << "Parameter " << (*argi)->param_text() << " set to ";
                    (*argi)->print_value(os);
                    os << '.' << '\n';
                }
                (*argi)->found_ = true;
                if (!(*argi)->repeated_)
                    ++argi;
            }
            else
            {
                os << "Error: unexpected extra argument "
                   << "\"" << argv[0] << "\".\n\n";
                --argc, ++argv;
                print_usage(os);
                return false;
            }
        }
    }

    bool good = true;

    for (ArgumentList::const_iterator it = param_list_.begin();
         it != param_list_.end(); ++it)
    {
        if ((*it)->required_ && !(*it)->found_)
        {
            os << "Error: argument for parameter " << (*it)->longkey_
               << " is required!\n";
            good = false;
        }
    }

    if (!good)
    {
        os << '\n';
        print_usage(os);
    }

    return good;
}

bool CmdlineParser::process(int argc, const char* const* argv)
{
    return process(argc, argv, std::cout);
}

void CmdlineParser::print_result(std::ostream& os)
{
    std::ios::fmtflags flags(os.flags());

    size_t maxlong = std::max(param_max_width_, option_max_width_);

    if (!param_list_.empty())
    {
        os << "Parameters:\n";

        for (ArgumentList::const_iterator it = param_list_.begin();
             it != param_list_.end(); ++it)
        {
            const Argument* arg = *it;

            os << "  " << std::setw(static_cast<int>(maxlong)) << std::left
               << arg->param_text();

            std::string typestr;
            typestr += '(';
            typestr += arg->type_name();
            typestr += ')';
            os << std::setw(max_type_name_ + 4) << typestr;

            arg->print_value(os);

            os << '\n';
        }
    }

    if (!option_list_.empty())
    {
        os << "Options:\n";

        for (ArgumentList::const_iterator it = option_list_.begin();
             it != option_list_.end(); ++it)
        {
            const Argument* arg = *it;

            os << "  " << std::setw(static_cast<int>(maxlong)) << std::left
               << arg->option_text();

            std::string typestr;
            typestr += '(';
            typestr += arg->type_name();
            typestr += ')';
            os << std::setw(max_type_name_ + 4) << std::left << typestr;

            arg->print_value(os);

            os << '\n';
        }
    }

    os.flags(flags);
}

void CmdlineParser::print_result()
{
    return print_result(std::cout);
}

} // namespace tlx

/******************************************************************************/
