#ifndef MAGIC_MPL_H
#define MAGIC_MPL_H

#include "pp.h"

namespace magic {

template <bool b> struct bool_ {
  enum B { value = b };
  typedef bool_ type;
};
template <typename T> struct bool_c {
  typedef typename T::type type;
};

template <bool L, bool R> struct all_ : bool_<false> {};
template <> struct all_<true, true> : bool_<true> {};

template <bool L, typename U> struct all_c_impl;
template <typename U> struct all_c_impl<false, U> {
  typedef bool_<false> type;
};
template <typename U> struct all_c_impl<true, U> {
  typedef typename bool_c<U>::type type;
};

// all_c shortcircuits evaluation of the "bool_" values of the types
template <typename T, typename U> struct all_c {
  typedef typename all_c_impl<bool_c<T>::type::value, U>::type type;
};

template <bool b> struct not_ { enum B { value = !b }; };
template <typename T> struct not_c {
  typedef not_<T::type::value> type;
};

template <typename T>
struct IsClass {
  template <typename X>
  static char (&i_class(void(X::*)()))[1];

  template <typename X>
  static char (&i_class(...))[2];

  enum B { value = sizeof(i_class<T>(0)) == 1 };
  typedef bool_<value> type;
};

template <typename T> struct IsTrue {
  typedef typename T::type type;
};

template <typename T, typename U>
struct AreSame {
  typedef bool_<false> type;
};

template <typename U>
struct AreSame<U, U> {
  typedef bool_<true> type;
};

}


#endif
