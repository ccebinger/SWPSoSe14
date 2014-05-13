#ifndef IO_EXCEPTION_H_INCLUDED
#define IO_EXCEPTION_H_INCLUDED

#include <exception>

class IO_Exception: public std::exception
{
private:
  std::string file;
public:
  void set_file(const std::string& fileName) {file=fileName;}
private:
  virtual const char* what() const throw()
  {
    return (file).c_str();
  }
};


#endif // IO_EXCEPTION_H_INCLUDED
