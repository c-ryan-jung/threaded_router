/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>



/// Time measurement.
class Timer
{
  /// Timeval snapshot.
  timeval last_timeval;


public:
  /// Constructor.
  Timer()
  {
    gettimeofday(&last_timeval, 0);
  }

  /// Reset timer.
  void restart()
  {
    gettimeofday(&last_timeval, 0);
  }

  /// Get elapsed timeval.
  double elapsed() const
  {
    timeval local_timeval;
    gettimeofday(&local_timeval, 0);
    double t = local_timeval.tv_usec - last_timeval.tv_usec;
    t /= 1e6;
    t += local_timeval.tv_sec - last_timeval.tv_sec;
    return t;
  }
};


#endif
