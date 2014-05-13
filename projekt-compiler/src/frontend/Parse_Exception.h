#ifndef PARSE_EXCEPTION_H_INCLUDED
#define PARSE_EXCEPTION_H_INCLUDED


#include <exception>
#include <sstream>
class Parse_Exception: public std::exception
{
private:
  std::string msg;
public:
  void set_msg(const std::string& m) {msg = m;}
private:
  virtual const char* what() const throw()
  {
    return msg.c_str();
  }
};

#endif // PARSE_EXCEPTION_H_INCLUDED
