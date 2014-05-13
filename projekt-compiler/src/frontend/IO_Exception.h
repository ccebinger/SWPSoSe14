#ifndef IO_EXCEPTION_H_INCLUDED
#define IO_EXCEPTION_H_INCLUDED

#include <exception>

class IO_Exception: public std::exception
{
private:
  std::string file;

public:
  IO_Exception(const std::string& fileName) {file=fileName;}
  virtual const char* what() const throw()
  {
    return ("Can't open the given file: " + file).c_str();
  }
};


#endif // IO_EXCEPTION_H_INCLUDED
