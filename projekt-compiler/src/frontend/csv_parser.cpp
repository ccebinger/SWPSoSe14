#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

int main()
{
  std::ifstream infile("file.csv");
  std::vector<std::string> result;
  std::string line;
  while(std::getline(infile, line))
  {
    std::stringstream lineStream(line);
    std::string cell;
    while(std::getline(lineStream, cell, ';'))
    {
      result.push_back(cell);
    }
  }
  for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
  {
    std::cout << ' ' << *it << std::endl;
  }
  return 0;
}
