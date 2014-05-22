#ifndef PARSE_EXCEPTION_H_INCLUDED
#define PARSE_EXCEPTION_H_INCLUDED

#include <exception>
#include <sstream>

/**
 * Represents a parse exception,
 * which will be thrown if the parser ends in an error state.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>
 */
class Parse_Exception: public std::exception
{
private:
 /**
  * The error message or the error state of the parser.
  */
  std::string msg;
public:
 /**
  * Sets the error message or state of the parser.
  */
  void set_msg(const std::string& m) {msg = m;}
private:
  virtual const char* what() const throw()
  {
    return msg.c_str();
  }
};

#endif // PARSE_EXCEPTION_H_INCLUDED
