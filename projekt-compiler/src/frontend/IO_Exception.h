#ifndef IO_EXCEPTION_H_INCLUDED
#define IO_EXCEPTION_H_INCLUDED

#include <exception>

/**
 * Represents an io exception,
 * which will be thrown if the read or write process of a given file fails.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>
 */
class IO_Exception: public std::exception
{
private:
 /**
  * The file on which the io exception appears.
  */
  std::string file;
public:

 /**
  * Sets the file on which the io exception appears.
  */
  void set_file(const std::string& fileName) {file=fileName;}
private:
  virtual const char* what() const throw()
  {
    return (file).c_str();
  }
};


#endif // IO_EXCEPTION_H_INCLUDED
