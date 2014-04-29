#include "magic.h"

#include <iostream>
#include <typeinfo>
#include <vector>

#include "math.h"

struct HasOutputToStream {
  int i;
  HasOutputToStream(int j) : i(j) { }
  void OutputToStream(std::ostream* os) const {
    *os << i;
  }
};

struct HasToString {
  int i;
  HasToString(int j) : i(j) { }
  std::string ToString() const {
    std::stringstream buf;
    buf << i;
    return buf.str();
  }
};

struct HasSerializeAsString {
  int i;
  HasSerializeAsString(int j) : i(j) { }
  std::string SerializeAsString() const {
    std::stringstream buf;
    buf << i;
    return buf.str();
  }
};

struct HasSerializeAsStringChild : public HasSerializeAsString {
  HasSerializeAsStringChild(int j) : HasSerializeAsString(j) { }
};

struct HasToStringAndOutputToStream {
  int i;
  HasToStringAndOutputToStream(int j) : i(j) { }
  void OutputToStream(std::ostream* os) const {
    *os << i;
  }
  std::string ToString() const {
    std::stringstream buf;
    buf << i;
    return buf.str();
  }
};

struct HasPrintToC {
  int i;
};

void PrintTo(const HasPrintToC& hpt, std::ostream* os) {
  *os << hpt.i;
}

using magic::bool_;
using magic::bool_c;
using magic::not_c;
using magic::all_c;
using magic::IsTrue;

magic::streamable_type<double>::type t;


template <typename T>
struct IsInt { typedef bool_<false> type; };
template <>
struct IsInt<int> { typedef bool_<true> type; };

int main() {
  GLOBAL_TIMER_RESET("test");
  DUMP();

  isfinite(0);
  DUMP_START << TFORMAT();
  DUMP_START << TFORMAT(1);
  using std::cout;
  using std::endl;
  cout << all_c<bool_<true>, bool_<true> >::type::value << endl;
  cout << all_c<bool_<false>, bool_<true> >::type::value << endl;
  cout << all_c<bool_<true>, bool_<false> >::type::value << endl;
  cout << all_c<bool_<false>, bool_<false> >::type::value << endl;

  // cout << magic::HasPrintTo<HasPrintToC>::type::value << endl;

  //MAGIC_STATIC_ASSERT((magic::AreSame<magic::bool_<1>, magic::HasPrintTo<HasPrintToC>::type>));

  HasSerializeAsString hsas(5);
  DUMP(hsas);
  DUMP((const HasSerializeAsString&)(hsas));
  DUMP((const HasSerializeAsString)(hsas));

  HasSerializeAsStringChild hsasc(8);
  DUMP(hsasc);

  {
    DUMP_SCOPE() << "with message";
  }

  DUMP_SCOPE();
  DUMP_SCOPE("hello", "world");
  int x = 10, y = 20;
  std::string z = "magic";
  DUMP(x, y, z, "test");
  {
    DUMP_SCOPE("inner", "scope");
  }

  DUMP() << "indent 0";
  {
    DUMP_SCOPE();
    DUMP() << "indent 1";
    {
      DUMP_SCOPE();
      DUMP() << "indent 2";
      {
        DUMP_SCOPE();
        DUMP() << "indent 3";
      }
    }
  }

  DUMP_NEXT_SCOPE("for", "loop")
  for (int i = 0; i < 3; i++) {
    DUMP_SCOPE();
    DUMP() << i;
    DUMP() << TFORMAT(i);
  }

  HasOutputToStream ots(3);
  DUMP(ots);

  HasToStringAndOutputToStream tsaots(5);
  DUMP(tsaots);

  std::vector<int> vi;
  vi.push_back(1);
  vi.push_back(2);
  vi.push_back(3);
  DUMP(vi);

  std::string cppstring("this is a c++ string");
  const char* cstring = "this is a c string";
  int i = 2;
  int* j = &i;

  int loops = 0;
  while (GLOBAL_TIMER_DELTA("test") < 100) {
    ++loops;
  }
  DUMP(loops);

  DUMP(cppstring);
  DUMP(cppstring.c_str());
  DUMP(cstring);
  DUMP(i);
  DUMP(j);

  DUMP(GLOBAL_TIMER_DELTA("test"));
}
