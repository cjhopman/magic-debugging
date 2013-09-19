#include "linux.h"

namespace magic {

int _magic_indent_level = 0;

namespace {
  std::string force_length(std::string str, size_t len) {
    if (str.size() > len)
      return str.substr(str.size() - len);
    return std::string(len - str.size(), ' ') + str;
  }
}

std::string _magic_logger::indent_string() {
  return std::string(_magic_indent_level, ' ');
}

std::string _magic_logger::tag(std::string filename, int line, std::string funcname) {
  filename = force_length(filename, 30);
  funcname = force_length(funcname, 40);
  char buf[350];
  sprintf(buf, "%s:% 5d %s: ", filename.c_str(), line, funcname.c_str());
  return std::string(buf);
}

_magic_logger& StreamTo(_magic_logger& log, std::string msg) {
  if (log.at_new_line)
    log.buf << std::string(_magic_logger::tag("", 0, "").size(), ' ') << _magic_logger::indent_string();
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
  at_new_line(false)
{
}
_magic_logger::_magic_logger(const _magic_logger& o) :
  scope_indent(0),
  at_new_line(false)
{
}
_magic_logger::~_magic_logger() {
  _magic_indent_level -= scope_indent;
#if defined(OS_ANDROID) || defined(ANDROID)
  __android_log_print(ANDROID_LOG_ERROR, "MAGIC-CPP", "%s", buf.str().c_str());
#else
  std::cerr << buf.str() << std::endl;
#endif
}

}  // namespace magic

