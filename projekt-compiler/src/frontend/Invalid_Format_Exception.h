#ifndef INVALID_FORMAT_EXCEPTION_H_INCLUDED
#define INVALID_FORMAT_EXCEPTION_H_INCLUDED

#include <exception>

/**
 * Represents a invalid format exception, which will be thrown if
 * the ast file is not in a valid format.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>
 */
class Invalid_Format_Exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "The ast file has an invalid format!";
  }
};

#endif // INVALID_FORMAT_EXCEPTION_H_INCLUDED
