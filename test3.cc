#include <iostream>
#include <typeinfo>
#include "enable_if.h"
namespace magic {
namespace adl_barrier {
  typedef char no; struct yes { no dummy[2]; };
  struct no_overload {};
  struct any_conversion { template<class T> any_conversion(const
      T&); };
  static yes is_overloaded_impl(...) { return *(yes*)1; }
  static no is_overloaded_impl(const no_overload&) { return *(no*)1; }
  static no_overload PrintTo(any_conversion, any_conversion) { return no_overload(); }
  template<class T, typename Uniq>
    struct HasPrintTo_impl {
      typedef typename extract_argument<0, typename extract_type<void((void(*)(const T&, std::ostream*)))>::type>::type arg0_t;
      typedef typename extract_argument<1, typename extract_type<void((void(*)(const T&, std::ostream*)))>::type>::type arg1_t;
      /*
      enum B { value =
        sizeof(is_overloaded_impl(
              PrintTo(maker<arg0_t>::make(), maker<arg1_t>::make())
            ))
          == sizeof(yes) };
      typedef HasPrintTo_impl type;
      */
    };
}
using adl_barrier:: HasPrintTo_impl;
template <typename T> struct HasPrintTo { enum B { value = HasPrintTo_impl<T,
  void>::value }; typedef HasPrintTo type; }; template <typename T> struct
    HasPrintTo_c { typedef HasPrintTo_impl<T, void> type; };;;
}

struct HasPrintToC {
  int i;
};

void PrintTo(const HasPrintToC& hpt, std::ostream* os) {
  *os << hpt.i;
}

using namespace std;

int main() {
  cout << magic::demangle(typeid(magic::FunctionType<magic::extract_type<void(void(const HasPrintToC&, std::ostream*))>::type>).name()) << endl;
  cout << magic::demangle(typeid(magic::FunctionType<magic::extract_type<void(void(const HasPrintToC&, std::ostream*))>::type>::ReturnType).name()) << endl;
  cout << magic::demangle(typeid(magic::FunctionType<magic::extract_type<void(void(const HasPrintToC&, std::ostream*))>::type>::ArgsTypes).name()) << endl;
  //cout << magic::demangle(typeid(magic::FunctionType<void(void(const HasPrintToC&, std::ostream*))>::ReturnType).name()) << endl;
  //cout << magic::demangle(typeid(magic::FunctionType<void(void(const HasPrintToC&, std::ostream*))>::ArgsTypes).name()) << endl;
  cout << typeid(magic::HasPrintTo_impl<HasPrintToC, void>::arg0_t).name() << endl;
  cout << typeid(magic::HasPrintTo_impl<HasPrintToC, void>::arg1_t).name() << endl;
  cout << magic::demangle(typeid(magic::adl_barrier::is_overloaded_impl(PrintTo(magic::maker<HasPrintToC>::make(), magic::maker<std::ostream*>::make()))).name()) << endl;
}
