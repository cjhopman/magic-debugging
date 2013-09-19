#ifndef MAGIC_ENABLE_IF_H
#define MAGIC_ENABLE_IF_H

#include "mpl.h"
#include "type_list.h"

namespace magic {

template <bool T, typename U> struct enable_if {};
template <typename U> struct enable_if<true, U> {
  typedef U type;
};

template <typename T>
struct extract_type {};
template <typename T>
struct extract_type<void(T)> { typedef T type; };

#define EXTRACT_TYPE(t) extract_type<void(t)>::type

template <typename T> \
struct maker { static const T& make(); }; \
template <typename T> \
struct maker<T[]> { static T* make(); }; \
template <typename T, size_t n> \
struct maker<T[n]> { static T* make(); }; \

template <typename R, typename Args>
struct FunctionTypeImpl {
  typedef R ReturnType;
  typedef Args ArgsTypes;
};

template <typename>
struct FunctionType {};

template <typename R>
struct FunctionType<R(*)()> {
  typedef R ReturnType;
  typedef MAGIC_TYPE_LIST() ArgsTypes;
};

template <typename R, typename A0>
struct FunctionType<R(*)(A0)> {
  typedef R ReturnType;
  typedef MAGIC_TYPE_LIST(A0) ArgsTypes;
};

template <typename R, typename A0, typename A1>
struct FunctionType<R(*)(A0, A1)> {
  typedef R ReturnType;
  typedef MAGIC_TYPE_LIST(A0, A1) ArgsTypes;
};

template <typename R, typename C, typename Args>
struct MemberFunctionTypeImpl {
  typedef R ReturnType;
  typedef C ClassType;
  typedef Args ArgsTypes;
};

template <typename>
struct MemberFunctionType {};

template <typename R, typename C>
struct MemberFunctionType<R(C::*)()> {
  typedef R ReturnType;
  typedef C ClassType;
  typedef MAGIC_TYPE_LIST() ArgsTypes;
};

template <typename R, typename C, typename A0>
struct MemberFunctionType<R(C::*)(A0)> {
  typedef R ReturnType;
  typedef C ClassType;
  typedef MAGIC_TYPE_LIST(A0) ArgsTypes;
};

template <typename R, typename C, typename A0, typename A1>
struct MemberFunctionType<R(C::*)(A0, A1)> {
  typedef R ReturnType;
  typedef C ClassType;
  typedef MAGIC_TYPE_LIST(A0, A1) ArgsTypes;
};

template <typename R, typename C>
struct MemberFunctionType<R(C::*)() const> {
  typedef R ReturnType;
  typedef C ClassType;
  typedef MAGIC_TYPE_LIST() ArgsTypes;
};

template <typename R, typename C, typename A0>
struct MemberFunctionType<R(C::*)(A0) const> {
  typedef R ReturnType;
  typedef C ClassType;
  typedef MAGIC_TYPE_LIST(A0) ArgsTypes;
};

template <typename R, typename C, typename A0, typename A1>
struct MemberFunctionType<R(C::*)(A0, A1) const> {
  typedef R ReturnType;
  typedef C ClassType;
  typedef MAGIC_TYPE_LIST(A0, A1) ArgsTypes;
};


template <int i, typename F>
struct extract_argument {
  typedef typename TypeAt<typename FunctionType<F>::ArgsTypes, i>::type type;
};

struct any_conversion {
  template<class T>
  any_conversion(const T&);
};

#define DEFINE_HAS_MEM_FUNC(funcname, sig, name) \
template <typename T> \
struct name { \
  typedef char yes[1]; \
  typedef char no[2]; \
  template <typename Base> struct func_sig { typedef typename EXTRACT_TYPE(sig) type; }; \
  template <typename U, U> struct type_check; \
  template <typename _1> static yes &chk(type_check<typename func_sig<typename MemberFunctionType<__typeof__(&_1::funcname)>::ClassType>::type, &_1::funcname> *); \
  template <typename > static no &chk(...); \
  typedef bool_<sizeof(chk<T>(0)) == sizeof(yes)> type; \
};

#define DEFINE_HAS_FUNC(funcname, sig, name) \
namespace adl_barrier { \
  typedef char no; \
  struct yes { no dummy[2]; }; \
  struct no_overload {}; \
  static yes is_overloaded_impl(...) { return *(yes*)1; } \
  static no is_overloaded_impl(const no_overload&) { return *(no*)1; } \
  static no_overload funcname(any_conversion, any_conversion) { return no_overload(); } \
  template<class T> \
  struct name \
  { \
    typedef typename extract_argument<0, void(sig)>::type arg0_t; \
    typedef typename extract_argument<1, void(sig)>::type arg1_t; \
    enum B { value = \
        sizeof(is_overloaded_impl(funcname(maker<arg0_t>::make(), maker<arg1_t>::make()))) == \
        sizeof(yes) \
    }; \
    typedef name type; \
  }; \
} \
using adl_barrier::name;


#define DEFINE_HAS_TYPEDEF(typedef_, name) \
template <typename T> \
struct name { \
  template<class U> static char (&test(typename U::typedef_ const*))[1]; \
  template<class U> static char (&test(...))[2]; \
  enum B { value = (sizeof(test<T>(0)) == 1) }; \
  typedef name type; \
}; \
template <typename T> \
struct name##_c { \
  typedef bool_<name<T>::value> type; \
};


#define ENABLE_IF_TYPEDEF(value, name) \
template <typename T> \
struct name { typedef typename extract_type<void(value)>::type type; };

}

#endif
