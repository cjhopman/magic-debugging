#ifndef MAGIC_TIMER_H
#define MAGIC_TIMER_H

#include <ctime>

#if __cplusplus > 199711L && 0
#define MAGIC_HAS_CHRONO
#include <ratio>
#include <chrono>
#endif

#include "pp.h"
#include "linux.h"

// Usage
//  Print time from now 'til exiting current scope:
//    TIMER();
//  Same but print with a tag:
//    TIMER("tag");

namespace {
  struct magic_timer {
#ifdef MAGIC_HAS_CHRONO
    typedef std::chrono::high_resolution_clock hrclock;
    typedef hrclock::time_point time_point;
    time_point now() { return hrclock::now(); }
    double value() { return std::duration_cast<std::duration<double>>(now() - start).count(); }
#else
    typedef time_t time_point;
    time_point now() { return clock(); }
    double value() { return now() - start; }
#endif
    std::string name;
    time_point start;
    magic_timer(std::string name = "") : name(name) { reset(); }
    void print() { DUMP() << name << " time: " << value(); }
    void reset() { start = now(); }
    ~magic_timer() { print(); }
  };
}

#define NAMED_TIMER(name) magic_timer timer_ ## __LINE__ = magic_timer(name);
#define ANON_TIMER() NAMED_TIMER("");
#define TIMER(...) GET_2ND(__VA_ARGS__, NAMED_TIMER, ANON_TIMER)(__VA_ARGS__)

#endif
