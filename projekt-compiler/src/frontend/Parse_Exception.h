#ifndef PARSE_EXCEPTION_H_INCLUDED
#define PARSE_EXCEPTION_H_INCLUDED


#include <exception>
class Parse_Exception: public std::exception
{
private:
  std::string message;

public:
  Parse_Exception(const std::string& msg) {message=msg;}
  virtual const char* what() const throw()
  {
    return ("Parser exception: " + message).c_str();
  }
};

#endif // PARSE_EXCEPTION_H_INCLUDED
