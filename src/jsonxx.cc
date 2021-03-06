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

#include <string.h>
#include <strings.h>

#include "jsonxx.hh"

#include <cassert>
#include <sstream>

namespace jsonxx {
namespace internals {
struct whitespace_traits {
  bool allow_comments;
};
whitespace_traits whitespace = { 0 };

std::istream& operator>>(std::istream& stream, whitespace_traits&) {
  char ch;

  stream >> std::ws;

  if (!stream)
    return stream;

  if (stream.peek() != '/')
    return stream;

  stream.get(ch);

  if (!stream)
    return stream;

  switch (stream.peek()) {
  case '/':
    while (stream) {
      stream.get(ch);
      if (ch == '\n')
        break;
    }
    break;
  case '*':
    while (stream) {
      stream.get(ch);
      if (ch == '*' && stream.peek() == '/')
        break;
    }
    if (stream)
      stream.get(ch);
    break;
  default:
    stream.unget();
    break;
  }

  return stream >> whitespace;
}

void consume_string(std::istream& stream, string& value) {
  std::ostringstream buffer;
  char ch;

  stream.get(ch);
  if (ch != '"') {
    stream.setstate(stream.rdstate() | std::ios_base::failbit);
    // TODO(abdulras) throw an exception
    return;
  }

  while (stream) {
    stream.get(ch);
    if (ch == '"')
      break;

    switch (ch) {
    case '\\':
      stream.get(ch);
      switch (ch) {
      case '"':
      case '\\':
      case '/':
        buffer << ch;
        break;
      case 'b':
        buffer << '\b';
        break;
      case 'f':
        buffer << '\f';
        break;
      case 'n':
        buffer << '\n';
        break;
      case 'r':
        buffer << '\r';
        break;
      case 't':
        buffer << '\t';
        break;
      case 'u':
        // TODO(abdulras) implement unicode handling
        assert(false);
        break;
      default:
        buffer << '\\' << ch;
        break;
      }
      break;
    default:
      buffer << ch;
      break;
    }
  }

  if (ch != '"') {
    stream.setstate(stream.rdstate() | std::ios_base::failbit);
    // TODO(abdulras) throw an exception
    return;
  }

  value = buffer.str();
}

void consume_number(std::istream& stream, number& value) {
  stream >> value;
}

void consume_array(std::istream& stream, array& value) {
  array array;
  char ch;

  stream.get(ch);
  if (ch != '[') {
    stream.setstate(stream.rdstate() | std::ios_base::failbit);
    // TODO(abdulras) throw an exception
    return;
  }

  while (stream) {
    jsonxx::value element;

    if (stream.peek() == ',')
      stream.get(ch);

    stream >> whitespace;

    if (stream.peek() == ']')
      break;

    parse(stream, element);
    array.push_back(element);

    stream >> whitespace;

    stream.get(ch);
    if (ch != ',') {
      stream.unget();
      break;
    }
  }

  stream.get(ch);
  if (ch != ']') {
    stream.setstate(stream.rdstate() | std::ios_base::failbit);
    // TODO(abdulras) throw an exception
    return;
  }

  value = array;
}

void consume_object(std::istream& stream, object& value) {
  object object;
  char ch;

  stream.get(ch);
  if (ch != '{') {
    stream.setstate(stream.rdstate() | std::ios_base::failbit);
    // TODO(abdulras) throw an exception
    return;
  }

  while (stream) {
    std::string k;
    jsonxx::value v;

    if (stream.peek() == ',')
      stream.get(ch);

    stream >> whitespace;

    if (stream.peek() == '}')
      break;

    consume_string(stream, k);
    if (!stream) {
      stream.setstate(stream.rdstate() | std::ios_base::failbit);
      // TODO(abdulras) throw an exception
      return;
    }

    stream >> whitespace;

    stream.get(ch);
    if (ch != ':') {
      stream.setstate(stream.rdstate() | std::ios_base::failbit);
      // TODO(abdulras) throw an exception
      return;
    }

    parse(stream, v);
    object[k] = v;

    if (!stream) {
      stream.setstate(stream.rdstate() | std::ios_base::failbit);
      // TODO(abdulras) throw an exception
      return;
    }

    stream >> whitespace;

    stream.get(ch);
    if (ch != ',') {
      stream.unget();
      break;
    }
  }

  stream.get(ch);
  if (ch != '}')
    stream.setstate(stream.rdstate() | std::ios_base::failbit);

  value = object;
}

void consume_boolean(std::istream& stream, boolean& value) {
  char buffer[6] = { 0 };

  stream.read(buffer, 5);

  if (::strncmp(buffer, "true", 4) == 0) {
    stream.unget();
    value = true;
  } else if (::strcmp(buffer, "false") == 0) {
    value = false;
  } else {
    stream.setstate(stream.rdstate() | std::ios_base::failbit);
  }
}

void consume_null(std::istream& stream, null&) {
  char buffer[5] = { 0 };

  stream.read(buffer, 4);

  if (::strcmp(buffer, "null") != 0)
    stream.setstate(stream.rdstate() | std::ios_base::failbit);
}
}

void allow_comments(const bool value) {
  internals::whitespace.allow_comments = value;
}

bool allow_comments() {
  return internals::whitespace.allow_comments;
}

void parse(std::istream& stream, value& value) {
  char ch;

  stream >> internals::whitespace;

  stream.get(ch);
  switch (ch) {
  case '{':
    internals::consume_object(stream.unget(), value.object_);
    if (!stream.fail())
      value.type_ = value::type::object;
    break;
  case '[':
    internals::consume_array(stream.unget(), value.array_);
    if (!stream.fail())
      value.type_ = value::type::array;
    break;
  case '"':
    internals::consume_string(stream.unget(), value.string_);
    if (!stream.fail())
      value.type_ = value::type::string;
    break;
  case '-':
  case '0' ... '9':
    internals::consume_number(stream.unget(), value.number_);
    if (!stream.fail())
      value.type_ = value::type::number;
    break;
  case 't':
  case 'f':
    internals::consume_boolean(stream.unget(), value.boolean_);
    if (!stream.fail())
      value.type_ = value::type::boolean;
    break;
  case 'n':
    internals::consume_null(stream.unget(), value.null_);
    if (!stream.fail())
      value.type_ = value::type::null;
    break;
  default:
    // throw
    break;
  }
}
}
