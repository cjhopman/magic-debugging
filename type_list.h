#ifndef MAGIC_TYPE_LIST_H
#define MAGIC_TYPE_LIST_H

#include "pp.h"
#include "mpl.h"

#include <string>
#include <cstdlib>

namespace magic {
  struct Nil {
    typedef Nil type;
  };

  template <typename Head, typename Tail>
  struct TypeList {
    typedef Head head;
    typedef Tail tail;
    typedef TypeList type;
  };

  template <
    typename T1 = Nil,
    typename T2 = Nil,
    typename T3 = Nil,
    typename T4 = Nil,
    typename T5 = Nil,
    typename T6 = Nil,
    typename T7 = Nil,
    typename T8 = Nil,
    typename T9 = Nil,
    typename T10 = Nil,
    typename T11 = Nil,
    typename T12 = Nil,
    typename T13 = Nil,
    typename T14 = Nil,
    typename T15 = Nil,
    typename T16 = Nil
      >
  struct MakeTypeList {
    typedef T1 head;
    typedef typename MakeTypeList<
      T2, T3, T4, T5,
      T6, T7, T8, T9,
      T10, T11, T12, T13,
      T14, T15, T16
        >::type tail;
    typedef TypeList<head, tail> type;
  };
  template <>
  struct MakeTypeList<
    Nil, Nil, Nil, Nil,
    Nil, Nil, Nil, Nil,
    Nil, Nil, Nil, Nil,
    Nil, Nil, Nil, Nil
      > {
    typedef Nil head;
    typedef Nil tail;
    typedef TypeList<Nil, Nil> type;
  };

  template <typename List, int index>
  struct TypeAt {
    typedef typename TypeAt<typename List::tail, index - 1>::type type;
  };
  template <typename List>
  struct TypeAt<List, 0> {
    typedef typename List::head type;
  };

  template <bool B, typename If, typename Else = Nil>
  struct If_ {
    typedef If type;
  };
  template <typename If, typename Else>
  struct If_<false, If, Else> {
    typedef Else type;
  };

  template <typename Cond, typename If, typename Else = Nil>
  struct If_c {
    typedef typename If_<Cond::type::value, If, Else>::type type;
  };

  template <typename List, template<typename>class Pred> struct IndexOf;

  template <int v>
  struct Num { enum B { value = v }; };

  template <typename L, typename R>
  struct Plus { enum B { value = L::value + R::value }; };

  template <typename Head, typename Tail, template<typename T> class Pred>
  struct IndexOfImpl {
    typedef typename If_c<Pred<Head>, Num<0>, Plus<Num<1>, IndexOf<Tail, Pred> > >::type type;
    enum V { value = type::value };
  };

  template <typename Tail, template<typename>class Pred>
  struct IndexOfImpl<Nil, Tail, Pred> {
    enum V { value = 0 };
  };

  template <typename List, template<typename>class Pred>
  struct IndexOf {
    enum V { value = IndexOfImpl<typename List::head, typename List::tail, Pred>::value };
  };

  template <typename List, template<typename>class Pred>
  struct Find {
    typedef typename TypeAt<List, IndexOf<List, Pred>::value>::type type;
  };


#define MAGIC_STATIC_ASSERT(T) \
  { char b[magic::extract_type<void(T)>::type::type::value ? 1 : -1]; }

#define MAGIC_TYPE_LIST(...) \
  MakeTypeList< \
    __VA_ARGS__ \
  >

#if defined(HAVE_CXA_DEMANGLE)
}
#include <cxxabi.h>
namespace magic {
  static const std::string demangle(const char* name) {
    int status = -4;
    char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
    const char* const demangled_name = (status==0)?res:name;
    std::string ret_val(demangled_name);
    free(res);
    return ret_val;
  }
#else
  static const char* demangle(const char* name)
  {
    return name;
  }
#endif

}

#endif

