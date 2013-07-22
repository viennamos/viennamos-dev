#ifndef VIENNAFVM_TIMER_HPP
#define VIENNAFVM_TIMER_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Josef Weinbub                   weinbub@iue.tuwien.ac.at
               (add your name here)

   license:    see file LICENSE in the ViennaFVM base directory
======================================================================= */

#include <iostream>

namespace viennafvm
{

inline void printOps(double num_ops, double exec_time)
{
  std::cout << "GFLOPs: " << num_ops / (1000000 * exec_time * 1000) << std::endl;
}

} // viennafvm


#ifdef _WIN32

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max

namespace viennafvm
{

class Timer
{
public:

  Timer()
  {
    QueryPerformanceFrequency(&freq);
  }

  void start()
  {
    QueryPerformanceCounter((LARGE_INTEGER*) &start_time);
  }

  double get() const
  {
    LARGE_INTEGER  end_time;
    QueryPerformanceCounter((LARGE_INTEGER*) &end_time);
    return (static_cast<double>(end_time.QuadPart) - static_cast<double>(start_time.QuadPart)) / static_cast<double>(freq.QuadPart);
  }


private:
  LARGE_INTEGER freq;
    LARGE_INTEGER start_time;
};

} // viennafvm

#else

#include <sys/time.h>

namespace viennafvm
{

class Timer
{
public:

  Timer() : ts(0)
  {}

  void start()
  {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    ts = tval.tv_sec * 1000000 + tval.tv_usec;
  }

  double get() const
  {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    double end_time = tval.tv_sec * 1000000 + tval.tv_usec;

    return static_cast<double>(end_time-ts) / 1000000.0;
  }

private:
  double ts;
};

} // viennafvm

#endif

#endif
