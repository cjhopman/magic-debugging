#ifndef MAGIC_TIMER_H
#define MAGIC_TIMER_H

#include <ctime>
#include <map>

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

namespace magic {
  struct magic_timer {
#ifdef MAGIC_HAS_CHRONO
    typedef std::chrono::high_resolution_clock hrclock;
    typedef hrclock::time_point time_point;
    static time_point now() { return hrclock::now(); }
    double value() { return std::duration_cast<std::chrono::milliseconds>(now() - start).count(); }
#else
    typedef time_t time_point;
    static time_point now() { return clock(); }
    double value() { return 1000.0 * (now() - start) / CLOCKS_PER_SEC; }
#endif

    std::string name;
    time_point start;
    bool suppress_destructor;

    magic_timer(std::string name = "", bool suppress_destructor = true)
        : name(name), suppress_destructor(suppress_destructor) {
      reset();
    }
    magic_timer(const magic_timer& o) { *this = o; }
    magic_timer& operator=(const magic_timer& o) {
      name = o.name;
      start = o.start;
      suppress_destructor = o.suppress_destructor;
      return *this;
    }

    void print();
    void reset() { start = now(); }
    ~magic_timer() { if (!suppress_destructor) print(); }

    static std::map<std::string, magic_timer>& global_timers() {
      static std::map<std::string, magic_timer> timers;
      return timers;
    }

    static magic_timer& LookupGlobal(std::string name);
  };

  double GlobalTimerDelta(std::string name);
}

#define GLOBAL_TIMER_RESET(name) magic::magic_timer::LookupGlobal(name).reset()
#define GLOBAL_TIMER_DELTA(name) magic::GlobalTimerDelta(name)

#define NAMED_TIMER(name) magic_timer timer_ ## __LINE__ = magic_timer(name, false);
#define ANON_TIMER() NAMED_TIMER("");
#define TIMER(...) GET_2ND(__VA_ARGS__, NAMED_TIMER, ANON_TIMER)(__VA_ARGS__)
#define DUMP_TIME(...) DUMP(__VA_ARGS__) << magic_timer::now()

#endif
