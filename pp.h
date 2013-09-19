#define STRINGIFY(x) #x
#define EXPAND(x) x

#define CONCAT(x, y) CONCAT1(EXPAND(x), EXPAND(y))
#define CONCAT1(x, y) CONCAT2(x, y)
#define CONCAT2(x, y) x##y

#define UNIQUE_NAME(prefix) CONCAT(prefix, __LINE__)

#define GET_1ST(_1, ...) _1
#define GET_2ND(_1, _2, ...) _2
#define GET_3RD(_1, _2, _3, ...) _3
#define GET_4TH(_1, _2, _3, _4, ...) _4

#define GET_10TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define GET_11TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define GET_12TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12

#define GET_16TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16

#define FOREACH_0(what)
#define FOREACH_1(what, x, ...) what(x) FOREACH_0(what, ##__VA_ARGS__)
#define FOREACH_2(what, x, ...) what(x) FOREACH_1(what, ##__VA_ARGS__)
#define FOREACH_3(what, x, ...) what(x) FOREACH_2(what, ##__VA_ARGS__)
#define FOREACH_4(what, x, ...) what(x) FOREACH_3(what, ##__VA_ARGS__)
#define FOREACH_5(what, x, ...) what(x) FOREACH_4(what, ##__VA_ARGS__)
#define FOREACH_6(what, x, ...) what(x) FOREACH_5(what, ##__VA_ARGS__)
#define FOREACH_7(what, x, ...) what(x) FOREACH_6(what, ##__VA_ARGS__)
#define FOREACH_8(what, x, ...) what(x) FOREACH_7(what, ##__VA_ARGS__)
#define FOREACH_9(what, x, ...) what(x) FOREACH_8(what, ##__VA_ARGS__)
#define FOREACH_10(what, x, ...) what(x) FOREACH_9(what, ##__VA_ARGS__)
#define GET_FOREACH_MACRO(...) GET_12TH((), ##__VA_ARGS__, FOREACH_10, FOREACH_9, FOREACH_8, FOREACH_7, FOREACH_6, FOREACH_5, FOREACH_4, FOREACH_3, FOREACH_2, FOREACH_1, FOREACH_0)
#define FOREACH(what, ...) GET_FOREACH_MACRO(__VA_ARGS__)(what, ##__VA_ARGS__)

#define JOIN_0(sep)
#define JOIN_1(sep, x, ...) x
#define JOIN_2(sep, x, ...) x sep JOIN_1(sep, ##__VA_ARGS__)
#define JOIN_3(sep, x, ...) x sep JOIN_2(sep, ##__VA_ARGS__)
#define JOIN_4(sep, x, ...) x sep JOIN_3(sep, ##__VA_ARGS__)
#define JOIN_5(sep, x, ...) x sep JOIN_4(sep, ##__VA_ARGS__)
#define JOIN_6(sep, x, ...) x sep JOIN_5(sep, ##__VA_ARGS__)
#define JOIN_7(sep, x, ...) x sep JOIN_6(sep, ##__VA_ARGS__)
#define JOIN_8(sep, x, ...) x sep JOIN_7(sep, ##__VA_ARGS__)
#define JOIN_9(sep, x, ...) x sep JOIN_8(sep, ##__VA_ARGS__)
#define JOIN_10(sep, x, ...) x sep JOIN_9(sep, ##__VA_ARGS__)
#define GET_JOIN_MACRO(...) GET_12TH((), ##__VA_ARGS__, JOIN_10, JOIN_9, JOIN_8, JOIN_7, JOIN_6, JOIN_5, JOIN_4, JOIN_3, JOIN_2, JOIN_1, JOIN_0)
#define JOIN(sep, ...) GET_JOIN_MACRO(__VA_ARGS__)(sep, ##__VA_ARGS__)

#define CALL(func, ...) func(__VA_ARGS__)
