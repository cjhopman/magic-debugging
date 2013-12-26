#include "pp.h"

#define TFORMAT1(X) M_STRINGIFY(X) << "=" << X << " "
#define TFORMATR(X) TFORMAT1(X),
#define TFORMAT(...) CALL(JOIN, <<, FOREACH(TFORMATR, __VA_ARGS__) "")

#define DUMP(...) TRACE << TFORMAT(__VA_ARGS__)

GET_JOIN_MACRO(a, b, c)
GET_JOIN_MACRO(a)
GET_JOIN_MACRO()
GET_FOREACH_MACRO(a, b, c)
GET_FOREACH_MACRO(a)
GET_FOREACH_MACRO()
FOREACH(foreach, x)
FOREACH(foreach, x, y, z)
JOIN(1, 2)

JOIN(xyz, a, b, c, d)

TFORMATR(1)
TFORMAT(1)
TFORMAT(x, y, z->this.what(x, y))
DUMP(x, y, this->(x, y))
DUMP(x)

