#include "linux.h"
#include "stream.h"
#include "timer.h"

#include <algorithm>
#include <iostream>
#include <cstdio>

#if defined(OS_ANDROID) || defined(ANDROID)
#include <android/log.h>
#endif

namespace magic {

int _magic_indent_level = 0;

namespace {
  std::string force_length(std::string str, size_t len) {
    if (str.size() > len)
      return str.substr(str.size() - len);
    return std::string(len - str.size(), ' ') + str;
  }
}

void magic_timer::print() {
  DUMP() << name << " time: " << value();
}

magic_timer& magic_timer::LookupGlobal(std::string name) {
  return global_timers()[name];
}

std::map<std::string, magic_timer>& magic_timer::global_timers() {
  static std::map<std::string, magic_timer>* timers = new std::map<std::string, magic_timer>();
  return *timers;
}

double GlobalTimerDelta(std::string name) {
  return magic_timer::LookupGlobal(name).value();
}

std::string _magic_logger::indent_string() {
  return std::string(_magic_indent_level + self_indent, ' ');
}

std::string _magic_logger::tag(std::string filename, int line, std::string funcname) {
  std::string func = funcname.substr(0, funcname.find('('));
  filename = force_length(filename, 30);
  funcname = force_length(funcname, 40);
  char buf[350];
  sprintf(buf, "%s:% 5d %s: ", filename.c_str(), line, funcname.c_str());
  return std::string(buf);
}

_magic_logger& StreamTo(_magic_logger& log, std::string msg) {
  if (log.at_new_line) {
    // 8 == length of time prefix (see ~_magic_logger)
    log.buf << std::string(8 + _magic_logger::tag("", 0, "").size(), ' ') << log.indent_string();
  }
  log.at_new_line = false;

  size_t end = msg.find('\n');
  if (end == std::string::npos) {
    log.buf << msg;
  } else if (end == msg.size() - 1) {
    log.buf << msg;
    log.at_new_line = true;
  } else {
    log.buf << msg.substr(0, end + 1);
    log.at_new_line = true;
    log << msg.substr(end + 1);
  }
  return log;
}

_magic_logger& _magic_logger::add_message(std::string msg) {
  *this << msg;
  return *this;
}

_magic_logger::_magic_logger() :
  scope_indent(0),
  self_indent(0),
  at_new_line(false)
{
}

_magic_logger::_magic_logger(int indent) :
  scope_indent(0),
  self_indent(indent),
  at_new_line(false)
{
}

_magic_logger::_magic_logger(const _magic_logger& o) :
  scope_indent(0),
  self_indent(o.self_indent),
  at_new_line(false)
{
}

_magic_logger::~_magic_logger() {
  _magic_indent_level -= scope_indent;
  char tbuf[1024];
  snprintf(
      tbuf, 400, "[% 5d] %s", static_cast<int>(GLOBAL_TIMER_DELTA("_magic_logger")), buf.str().c_str());

#if defined(OS_ANDROID) || defined(ANDROID)
  __android_log_print(ANDROID_LOG_ERROR, "MAGIC-CPP", tbuf);
#else
  std::cerr << tbuf << std::endl;
#endif
}

static bool IsNotPrintable(unsigned char c) {
  return !(c > 0x20 && c < 0x97);
}

std::string StripNonPrintable(std::string s) {
  std::replace_if(s.begin(), s.end(), IsNotPrintable, '^');
  s.erase(std::unique(s.begin(), s.end()), s.end());
  return s;
}

}  // namespace magic

