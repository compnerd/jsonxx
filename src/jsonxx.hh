/* vim: set et fdm=syntax sts=2 sw=2 ts=8 : */
/**
 * Copyright © 2010,2012,2015 Saleem Abdulrasool <compnerd@compnerd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#ifndef jsonxx_jsonxx_hh_
#define jsonxx_jsonxx_hh_

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace jsonxx {
class value;

struct null {};
typedef bool boolean;
typedef double number;
typedef std::string string;
typedef std::vector<value> array;
typedef std::map<std::string, value> object;

class value {
  friend void parse(std::istream&, value&);

public:
  template <typename type_>
  bool is() const;

  template <typename type_>
  type_& as();

private:
  enum value_type {
    invalid,
    string,
    number,
    object,
    array,
    boolean,
    null,
  };

  jsonxx::string string_;
  jsonxx::number number_;
  jsonxx::object object_;
  jsonxx::array array_;
  jsonxx::boolean boolean_;
  jsonxx::null null_;

  value_type type_;
};

template <>
inline bool value::is<string>() const {
  return type_ == value::string;
}

template <>
inline bool value::is<number>() const {
  return type_ == value::number;
}

template <>
inline bool value::is<object>() const {
  return type_ == value::object;
}

template <>
inline bool value::is<array>() const {
  return type_ == value::array;
}

template <>
inline bool value::is<boolean>() const {
  return type_ == value::boolean;
}

template <>
inline bool value::is<null>() const {
  return type_ == value::null;
}

template <>
inline string& value::as<jsonxx::string>() {
  // TODO(abdulras) throw an exception rather than asserting
  assert(is<jsonxx::string>());
  return string_;
}

template <>
inline number& value::as<jsonxx::number>() {
  // TODO(abdulras) throw an exception rather than asserting
  assert(is<jsonxx::number>());
  return number_;
}

template <>
inline object& value::as<jsonxx::object>() {
  // TODO(abdulras) throw an exception rather than asserting
  assert(is<jsonxx::object>());
  return object_;
}

template <>
inline array& value::as<jsonxx::array>() {
  // TODO(abdulras) throw an exception rather than asserting
  assert(is<jsonxx::array>());
  return array_;
}

template <>
inline boolean& value::as<jsonxx::boolean>() {
  // TODO(abdulras) throw an exception rather than asserting
  assert(is<jsonxx::boolean>());
  return boolean_;
}

template <>
inline null& value::as<jsonxx::null>() {
  // TODO(abdulras) throw an exception rather than asserting
  assert(is<jsonxx::null>());
  return null_;
}

bool allow_comments(void);
void allow_comments(const bool value);

void parse(std::istream& stream, value& value);
};

#endif
