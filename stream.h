#ifndef MAGIC_STREAM_H
#define MAGIC_STREAM_H

#include "enable_if.h"
#include "type_list.h"
#include "linux.h"

namespace magic {

// This adds functions for output streaming of various things.
//
// iterable classes (that aren't already streamable)
//   e.g. std::vector, std::map, not std::string
// classes with function OutputToStream
//   e.g. base::debug::StackTrace
// classes with function ToString
//   e.g. ui::gfx::Size
// classes with free frunction PrintTo
//   i.e. things printable by gtest

DEFINE_HAS_MEM_FUNC(OutputToStream, void(Base::*)(std::ostream*)const, HasOutputToStream);
//DEFINE_HAS_MEM_FUNC(begin, has_begin);
//DEFINE_HAS_MEM_FUNC(end, has_end);
DEFINE_HAS_MEM_FUNC(ToString, std::string(Base::*)()const, HasToString);
DEFINE_HAS_MEM_FUNC(SerializeAsString, std::string(Base::*)()const, HasSerializeAsString);
DEFINE_HAS_TYPEDEF(const_iterator, HasStreamableConstIterator);
DEFINE_HAS_FUNC(PrintTo, (void(const T&, std::ostream*)), HasPrintTo);

namespace internal {
  namespace adl_barrier {
    typedef char no;
    struct yes { no dummy[2]; };

    struct no_overload {};
    struct any_conversion {
      template<class T>
      any_conversion(const T&);
    };

    yes is_overloaded_impl(...);
    no is_overloaded_impl(const no_overload&);
    no_overload operator<<(any_conversion, const any_conversion&);

    using namespace std;

    template<class T>
    struct is_streamable
    {
      enum B { value =
          sizeof(is_overloaded_impl(operator<<(maker<std::ostream&>::make(), maker<T>::make()))) ==
          sizeof(yes)
      };
      typedef is_streamable type;
    };
  }
}
using internal::adl_barrier::is_streamable;


template <typename T>
struct streamable_type {
  typedef MAGIC_TYPE_LIST(
      not_c<IsClass<T> >,
      is_streamable<T>,
      HasOutputToStream<T>,
      HasToString<T>,
      HasStreamableConstIterator<T>,
      HasSerializeAsString<T>,
      Nil
    )
    all_streamable_types;
  typedef typename Find<all_streamable_types, IsTrue>::type type;
};

#define DEFINE_STREAM_TO_FOR(stream_type, arg) \
template <typename T> \
typename magic::enable_if< \
  magic::AreSame< \
    typename magic::streamable_type<T>::type, \
    stream_type \
  >::type::value, \
  void>::type \
StreamTo(_magic_logger& os, arg)

static inline void StreamTo(_magic_logger& os, void*const t) {
  std::stringstream ss;
  ss << t;
  StreamTo(os, ss.str());
}

static inline void StreamTo(_magic_logger& os, char*const t) {
  StreamTo(os, std::string(t));
}

static inline void StreamTo(_magic_logger& os, const char*const t) {
  StreamTo(os, std::string(t));
}

template <typename T>
void StreamTo(_magic_logger& os, T*const t) {
  StreamTo(os, (void*)t);
}

template <typename L, typename R>
_magic_logger& StreamTo(_magic_logger& log, const std::pair<L, R>& p) {
  StreamTo(log, "pair<");
  StreamTo(log, p.first);
  StreamTo(log, ",");
  StreamTo(log, p.second);
  StreamTo(log, ">");
  return log;
}

DEFINE_STREAM_TO_FOR(magic::HasOutputToStream<T>, const T& t) {
  std::stringstream ss;
  ss << "\n";
  t.OutputToStream(&ss);
  ss << "\n";
  os << ss.str();
}

DEFINE_STREAM_TO_FOR(magic::HasToString<T>, const T& t) {
  os << t.ToString();
}

DEFINE_STREAM_TO_FOR(magic::HasStreamableConstIterator<T>, const T& t) {
  bool first = true;
  os << "<<";
  for (typename T::const_iterator it = t.begin(); it != t.end(); ++it) {
    if (!first)
      os << ", ";
    os << *it;
    first = false;
  }
  os << ">>";
}

DEFINE_STREAM_TO_FOR(magic::is_streamable<T>, const T& t) {
  std::stringstream ss;
  ss << t;
  StreamTo(os, ss.str());
}

DEFINE_STREAM_TO_FOR(magic::not_c<magic::IsClass<T> >, const T& t) {
  std::stringstream ss;
  ss << t;
  StreamTo(os, ss.str());
}

MAGIC_EXPORT std::string StripNonPrintable(std::string s);

DEFINE_STREAM_TO_FOR(magic::HasSerializeAsString<T>, const T& t) {
  os << StripNonPrintable(t.SerializeAsString());
}

}


#endif
