/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */


#ifndef BASICS_HPP
#define BASICS_HPP

#include <cmath>
#include <sstream>
#include <string>

#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>


using namespace std;
using namespace log4cplus;



// ----------------------------------------------------------------------------

/// Num-to-string conversion.
// template<typename T>
// string numtos(T i)
// {
//   ostringstream s;
//   s << i;
//   return s.str();
// }
string itos(long number)
{
  char buffer[50];
  sprintf(buffer, "%ld", number);
  return string(buffer);
}

string ftos(double number)
{
  char buffer[50];
  sprintf(buffer, "%G", number);
  return string(buffer);
}




/// Relative difference.
/// Used to check for loating-point equality.
template<typename T>
T reldiff(T number1, T number2)
{
  T absmax = max(fabs(number1), fabs(number2));
  return absmax==0.0 ? 0.0 : fabs(number1-number2)/absmax;
}



/// Euclidean distance between two points in 2D.
template<typename T>
T euclid_dist(T x1, T y1, T x2, T y2)
{
  return sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
}



/// Event type.
enum Event_Type { TN, TNB, VN, VNB, TE, TEB, VE, VEB };



// logger stuff

/// Simple pattern layout.
auto_ptr<Layout> myLayout =
  auto_ptr<Layout>(new PatternLayout("%-5p  %l:  %m%n"));

/// Shared console appender
SharedAppenderPtr myConsoleAppender(new ConsoleAppender());

#endif
