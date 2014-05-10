#ifndef INVALID_FORMAT_EXCEPTION_H_INCLUDED
#define INVALID_FORMAT_EXCEPTION_H_INCLUDED

#include <exception>

class Invalid_Format_Exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "The ast file has an invalid format!";
  }
};

#endif // INVALID_FORMAT_EXCEPTION_H_INCLUDED
