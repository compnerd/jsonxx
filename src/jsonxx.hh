/* vim: set et fdm=syntax sts=4 sw=4 ts=4 : */
/**
 * Copyright © 2010 Saleem Abdulrasool <compnerd@compnerd.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation and/or
 *    other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 **/

#ifndef jsonxx_hh
#define jsonxx_hh

#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

namespace jsonxx
{
    class value;

    struct null { };
    typedef bool boolean;
    typedef double number;
    typedef std::string string;
    typedef std::vector<value> array;
    typedef std::map<std::string, value> object;

    class value
    {
        friend void parse(std::istream &, value &);

        private:
            enum value_type
            {
                invalid,
                string,
                number,
                object,
                array,
                boolean,
                null,
            };

            jsonxx::string  _string;
            jsonxx::number  _number;
            jsonxx::object  _object;
            jsonxx::array   _array;
            jsonxx::boolean _boolean;
            jsonxx::null    _null;

            value_type      _type;

        public:
            template <typename type_>
            bool is(void) const;

            template <typename type_>
            type_ & as(void);
    };

    template <>
    inline bool value::is<jsonxx::string>(void) const { return _type == value::string; }

    template <>
    inline bool value::is<jsonxx::number>(void) const { return _type == value::number; }

    template <>
    inline bool value::is<jsonxx::object>(void) const { return _type == value::object; }

    template <>
    inline bool value::is<jsonxx::array>(void) const { return _type == value::array; }

    template <>
    inline bool value::is<jsonxx::boolean>(void) const { return _type == value::boolean; }

    template <>
    inline bool value::is<jsonxx::null>(void) const { return _type == value::null; }

    template <>
    inline jsonxx::string &
    value::as<jsonxx::string>(void)
    {
        assert(is<jsonxx::string>());
        return _string;
    }

    template <>
    inline jsonxx::number &
    value::as<jsonxx::number>(void)
    {
        assert(is<jsonxx::number>());
        return _number;
    }

    template <>
    inline jsonxx::object &
    value::as<jsonxx::object>(void)
    {
        assert(is<jsonxx::object>());
        return _object;
    }

    template <>
    inline jsonxx::array &
    value::as<jsonxx::array>(void)
    {
        assert(is<jsonxx::array>());
        return _array;
    }

    template <>
    inline jsonxx::boolean &
    value::as<jsonxx::boolean>(void)
    {
        assert(is<jsonxx::boolean>());
        return _boolean;
    }

    template <>
    inline jsonxx::null &
    value::as<jsonxx::null>(void)
    {
        assert(is<jsonxx::null>());
        return _null;
    }

    bool allow_comments(void);
    void allow_comments(const bool value);

    void parse(std::istream & stream, value & value);
};

#endif

