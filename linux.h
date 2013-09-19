#ifndef MAGIC_LINUX_H
#define MAGIC_LINUX_H

#include <sstream>
#include <iostream>
#include <cstdio>
#include <string>

#if defined(OS_ANDROID) || defined(ANDROID)
#include <android/log.h>
#endif

#include "pp.h"
#include "export.h"

// Usage
//   Log a message (with file:line+function tag):
//     TRACE << "this is the value of x:" << x;
//   Indent traces one level until leaving this scope:
//     TRACE_INDENT;
//   Log a message when exiting the current scope:
//     TRACE_EXIT << "exiting scope at time: " << getTime();
//   Print foo=<value of foo>:
//     TRACE << "these are the function args: " << TFORMAT(arg1) << TFORMAT(arg2);
//     TRACE << "or something more complicated: " << TFORMAT(this->getFoo().getBar(x, y));
//     TRACE << "or multiple arguments in one call: " << TFORMAT(arg1, arg2, arg3); <-- support ~10 arguments
//   Just dump some formatted variables:
//     DUMP(arg1, arg2, this->getTime());
//   Print a message now, another when exiting the current scope, and indent all
//   traces between those two points.
//     TRACE_SCOPE("entering the scope", "exiting the scope");
//     TRACE_SCOPE("custom message only when entering"); // prints a default message on exit
//     TRACE_SCOPE(); // default message on entering/exiting
//   Print a message now, and when the scope starting right after the call and
//   indent traces between (!IMPORTANT no semicolon after)
//     TRACE_NEXT_SCOPE()
//     for (int i = 0; i < 10; i++) { ... }
//     TRACE_NEXT_SCOPE("on entering", "on exiting")
//     if (...) { ... }
//
//   @DEPRECATED - use TRACE_SCOPE
//   Print entering/exiting of a function and indent all traces 1 level while in function (must be placed at start of function):
//     TRACE_FUNC;

namespace magic {

extern int MAGIC_EXPORT _magic_indent_level;

struct MAGIC_EXPORT _magic_logger {
  static int indent_level;
  static std::string indent_string();
  static std::string tag(std::string file, int line, std::string func);

  _magic_logger();
  _magic_logger(const _magic_logger& o);
  ~_magic_logger();

  // this is strange, adds indent while this is alive, but not for this loggers
  // message
  _magic_logger& add_scope_indent(int i) {
    scope_indent += i;
    _magic_indent_level += i;
    return *this;
  }
  _magic_logger& add_message(std::string msg);
  operator bool() {
    return true;
  }
  int scope_indent;
  std::stringstream buf;
  bool at_new_line;
};

MAGIC_EXPORT _magic_logger& StreamTo(_magic_logger& log, std::string msg);

template <typename T>
_magic_logger& operator<<(_magic_logger& log, const T& t) {
  StreamTo(log, t);
  return log;
}

template <typename T>
std::string streamableToString(const T& t) {
  std::stringstream buf;
  buf << t;
  return buf.str();
}

struct MAGIC_EXPORT _magic_indenter {
  _magic_indenter() {
    _magic_indent_level++;
  }
  ~_magic_indenter() {
    _magic_indent_level--;
  }
};

#define TRACE_TAG magic::_magic_logger::tag(__FILE__, __LINE__, std::string(__func__).substr(0, std::string(__func__).find('('))) << magic::_magic_logger::indent_string()

#define TRACE_IMPL(name) if (magic::_magic_logger name = magic::_magic_logger()) name << TRACE_TAG
#define TRACE TRACE_IMPL(UNIQUE_NAME(lg_trace_))

#define TRACE_EXIT_IMPL(name) magic::_magic_logger name; name << TRACE_TAG; name
#define TRACE_EXIT TRACE_EXIT_IMPL(UNIQUE_NAME(lg_exit_))

#define TRACE_INDENT magic::_magic_indenter UNIQUE_NAME(mi_indent_);

#define TRACE_SCOPE_IMPL(_, enter, exit, ...) TRACE << ">>>>> " << enter; TRACE_EXIT << "<<<<< " << exit; TRACE_INDENT;
#define TRACE_SCOPE(...) TRACE_SCOPE_IMPL((), ##__VA_ARGS__, "", "")
#define TRACE_FUNC TRACE_SCOPE("Entering function", "Exiting function")

#define TRACE_NEXT_SCOPE_IMPL(name, enter, exit, ...) TRACE << ">>>>> " << enter; if (magic::_magic_logger name = magic::_magic_logger()) if (name.add_scope_indent(1) << TRACE_TAG << "<<<<< " << exit)
#define TRACE_NEXT_SCOPE(...) TRACE_NEXT_SCOPE_IMPL(UNIQUE_NAME(lg_next_scope_), ##__VA_ARGS__, "", "")

#define TFORMAT1(X) STRINGIFY(X) << "=" << X << " "
#define TFORMATR(X) TFORMAT1(X),
#define TFORMAT(...) CALL(JOIN, <<, FOREACH(TFORMATR, __VA_ARGS__) "")

#define DUMP(...) TRACE << TFORMAT(__VA_ARGS__)

}  // namespace

#endif
